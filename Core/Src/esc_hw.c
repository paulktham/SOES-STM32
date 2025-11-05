#include "esc_hw.h"
#include "ecat_slv.h"
#include <string.h>

uint32_t LAN9252_ReadReg32(uint16_t address)
{
    uint8_t cmd[4];
    uint8_t recv[4];

    // Fast Read command (0x03) + 16-bit address + dummy byte
    cmd[0] = 0x03;
    cmd[1] = (address >> 8) & 0xFF;
    cmd[2] = address & 0xFF;
    cmd[3] = 0x00;  // Dummy byte

    LAN9252_CS_LOW();
    HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, recv, 4, HAL_MAX_DELAY);
    LAN9252_CS_HIGH();

    // LAN9252 returns data in weird formate, do check if you have the same behaviour as mine
    return (recv[2] << 24) | (recv[1] << 16) | (recv[0] << 8) | recv[3];
}

void LAN9252_WriteInternal(uint16_t addr, uint32_t data, uint8_t len) {
    uint8_t tx_buffer[8];  // Max: 1 cmd + 2 addr + 4 data + 1 spare
    uint8_t tx_len = 0;

    // Build transmit buffer
    tx_buffer[tx_len++] = SPI_WRITE_CMD;
    tx_buffer[tx_len++] = (addr >> 8) & 0xFF;   // Address high byte
    tx_buffer[tx_len++] = addr & 0xFF;           // Address low byte

    // Add data bytes (little endian)
    for (uint8_t i = 0; i < len; i++) {
        tx_buffer[tx_len++] = (data >> (i * 8)) & 0xFF;
    }

    // Send via SPI
    LAN9252_CS_LOW();
    HAL_SPI_Transmit(&hspi1, tx_buffer, tx_len, HAL_MAX_DELAY);
    LAN9252_CS_HIGH();
}

void LAN9252_Reset(void)
{
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
    HAL_Delay(10);  // Initial delay after reset
}

uint8_t LAN9252_CSR_WaitReady(uint32_t timeout_us) {
    while (timeout_us--) {
        uint32_t status = LAN9252_ReadReg32(LAN9252_CSR_CMD_REG);
        if (!(status & CSR_BUSY)) {
            return 1;
        }
        HAL_Delay(1);
    }
    return 0;
}

uint32_t LAN9252_ReadESC(uint16_t esc_addr, uint8_t len) {
    uint32_t cmd;

    // Wait if previous operation still busy
    if (!LAN9252_CSR_WaitReady(10000)) {
        return 0xFFFFFFFF; // Timeout error
    }

    // Build command: READ | SIZE | ADDRESS
    switch(len) {
        case 1: cmd = CSR_BUSY | CSR_READ | CSR_SIZE_1BYTE | esc_addr; break;
        case 2: cmd = CSR_BUSY | CSR_READ | CSR_SIZE_2BYTE | esc_addr; break;
        case 4: cmd = CSR_BUSY | CSR_READ | CSR_SIZE_4BYTE | esc_addr; break;
        default: return 0xFFFFFFFF;
    }

    // Write command to CSR_CMD register
    LAN9252_WriteInternal(LAN9252_CSR_CMD_REG, cmd, 4);

    // Wait for read to complete
    if (!LAN9252_CSR_WaitReady(10000)) {
        return 0xFFFFFFFF;
    }

    // Read data from CSR_DATA register
    uint32_t data = LAN9252_ReadReg32(LAN9252_CSR_DATA_REG);

    return data;
}

// Write to ESC address space
uint8_t LAN9252_WriteESC(uint16_t esc_addr, uint32_t data, uint8_t len) {
    uint32_t cmd;

    // Wait if previous operation still busy
    if (!LAN9252_CSR_WaitReady(10000)) {
        return 0;
    }

    // Write data first
    LAN9252_WriteInternal(LAN9252_CSR_DATA_REG, data, 4);

    // Build command: WRITE | SIZE | ADDRESS
    switch(len) {
        case 1: cmd = CSR_BUSY | CSR_WRITE | CSR_SIZE_1BYTE | esc_addr; break;
        case 2: cmd = CSR_BUSY | CSR_WRITE | CSR_SIZE_2BYTE | esc_addr; break;
        case 4: cmd = CSR_BUSY | CSR_WRITE | CSR_SIZE_4BYTE | esc_addr; break;
        default: return 0;
    }

    // Write command
    LAN9252_WriteInternal(LAN9252_CSR_CMD_REG, cmd, 4);

    // Wait for write to complete
    return LAN9252_CSR_WaitReady(10000);
}
// Initialize the ESC hardware
void ESC_init(const esc_cfg_t *config)
{
    // Reset LAN9252
    LAN9252_Reset();

    // Wait for chip to be ready
    HAL_Delay(100);

    // Verify chip ID (optional but recommended)
    uint32_t chip_id = LAN9252_ReadReg32(LAN9252_CHIP_ID_REG);
    if ((chip_id & 0xFFFF0000) != 0x92520000) {
        // Chip ID mismatch - handle error
    }
    LAN9252_WriteESC(LAN9252_AL_EVENT_REQ_REG, (uint32_t)0xFFFF, BITS_16);

    // Enable AL Control event (bit 0)
    LAN9252_WriteESC(LAN9252_AL_EVENT_MASK_REG, (uint32_t) 0x00000001, BITS_32);
    HAL_Delay(10);
}

// Read data from ESC address space
void ESC_read(uint16_t address, void *buf, uint16_t len)
{
    uint8_t *data = (uint8_t *)buf;
    uint16_t read_addr = address;
    uint16_t remaining = len;

    while (remaining > 0) {
        if (remaining >= 4 && (read_addr & 0x03) == 0) {
            // 4-byte aligned read
            uint32_t val = LAN9252_ReadESC(read_addr, 4);
            memcpy(data, &val, 4);
            data += 4;
            read_addr += 4;
            remaining -= 4;
        }
        else if (remaining >= 2 && (read_addr & 0x01) == 0) {
            // 2-byte aligned read
            uint32_t val = LAN9252_ReadESC(read_addr, 2);
            uint16_t val16 = (uint16_t)val;
            memcpy(data, &val16, 2);
            data += 2;
            read_addr += 2;
            remaining -= 2;
        }
        else {
            // Byte read
            uint32_t val = LAN9252_ReadESC(read_addr, 1);
            *data = (uint8_t)val;
            data++;
            read_addr++;
            remaining--;
        }
    }
}

// Write data to ESC address space
void ESC_write(uint16_t address, void *buf, uint16_t len)
{
    uint8_t *data = (uint8_t *)buf;
    uint16_t write_addr = address;
    uint16_t remaining = len;

    while (remaining > 0) {
        if (remaining >= 4 && (write_addr & 0x03) == 0) {
            // 4-byte aligned write
            uint32_t val;
            memcpy(&val, data, 4);
            LAN9252_WriteESC(write_addr, val, 4);
            data += 4;
            write_addr += 4;
            remaining -= 4;
        }
        else if (remaining >= 2 && (write_addr & 0x01) == 0) {
            // 2-byte aligned write
            uint16_t val16;
            memcpy(&val16, data, 2);
            LAN9252_WriteESC(write_addr, (uint32_t)val16, 2);
            data += 2;
            write_addr += 2;
            remaining -= 2;
        }
        else {
            // Byte write
            LAN9252_WriteESC(write_addr, (uint32_t)*data, 1);
            data++;
            write_addr++;
            remaining--;
        }
    }
}

// Enable ESC interrupts
void ESC_interrupt_enable(uint32_t mask)
{
    // Read current interrupt mask
    uint32_t int_mask = LAN9252_ReadReg32(LAN9252_INT_EN_REG);  // AL Event Mask register

    // Set the bits
    int_mask |= mask;

    // Write back
    LAN9252_WriteInternal(LAN9252_INT_EN_REG, int_mask, BITS_32);
}

// Disable ESC interrupts
void ESC_interrupt_disable(uint32_t mask)
{
    // Read current interrupt mask
    uint32_t int_mask = LAN9252_ReadReg32(LAN9252_INT_EN_REG);  // AL Event Mask register

    // Clear the bits
    int_mask &= ~mask;

    // Write back
    LAN9252_WriteInternal(LAN9252_INT_EN_REG, int_mask, BITS_32);
}

