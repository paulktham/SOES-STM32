///*
// * spi.c
// *
// *  Created on: Oct 23, 2025
// *      Author: apicoo_hx
// */
//
//
//#include "spi.h"
//
//uint32_t LAN9252_ReadReg32(uint16_t address)
//{
//    uint8_t cmd[4];
//    uint8_t recv[4];
//
//    // Fast Read command (0x03) + 16-bit address + dummy byte
//    cmd[0] = 0x03;
//    cmd[1] = (address >> 8) & 0xFF;
//    cmd[2] = address & 0xFF;
//    cmd[3] = 0x00;  // Dummy byte
//
//    LAN9252_CS_LOW();
//    HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY);
//    HAL_SPI_Receive(&hspi1, recv, 4, HAL_MAX_DELAY);
//    LAN9252_CS_HIGH();
//
//    // LAN9252 returns data in little-endian format
//    return (recv[2] << 24) | (recv[1] << 16) | (recv[0] << 8) | recv[3];
//}
//
//void LAN9252_WriteInternal(uint16_t addr, uint32_t data, uint8_t len) {
//    uint8_t tx_buffer[8];  // Max: 1 cmd + 2 addr + 4 data + 1 spare
//    uint8_t tx_len = 0;
//
//    // Build transmit buffer
//    tx_buffer[tx_len++] = SPI_WRITE_CMD;
//    tx_buffer[tx_len++] = (addr >> 8) & 0xFF;   // Address high byte
//    tx_buffer[tx_len++] = addr & 0xFF;           // Address low byte
//
//    // Add data bytes (little endian)
//    for (uint8_t i = 0; i < len; i++) {
//        tx_buffer[tx_len++] = (data >> (i * 8)) & 0xFF;
//    }
//
//    // Send via SPI
//    LAN9252_CS_LOW();
//    HAL_SPI_Transmit(&hspi1, tx_buffer, tx_len, HAL_MAX_DELAY);
//    LAN9252_CS_HIGH();
//}
//
//uint16_t LAN9252_ReadReg16(uint16_t address)
//{
//    uint8_t cmd[4];
//    uint8_t recv[2];
//
//    // Fast Read command (0x03) + 16-bit address + dummy byte
//    cmd[0] = 0x03;
//    cmd[1] = (address >> 8) & 0xFF;
//    cmd[2] = address & 0xFF;
//    cmd[3] = 0x00;  // Dummy byte
//
//    LAN9252_CS_LOW();
//    HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY);
//    HAL_SPI_Receive(&hspi1, recv, 4, HAL_MAX_DELAY);
//    LAN9252_CS_HIGH();
//
//    // LAN9252 returns data in little-endian format
//    return (recv[1] << 8) | (recv[0] << 8);
//}
//
//void LAN9252_Reset(void)
//{
//    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
//    HAL_Delay(100);
//    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
//    HAL_Delay(10);  // Initial delay after reset
//}
//
//uint8_t LAN9252_CSR_WaitReady(uint32_t timeout_us) {
//    while (timeout_us--) {
//        uint32_t status = LAN9252_ReadReg32(LAN9252_CSR_CMD_REG);
//        if (!(status & CSR_BUSY)) {
//            return 1;
//        }
//        HAL_Delay(1);
//    }
//    return 0;
//}
//
//// Read from ESC address space (this is what you need!)
//uint32_t LAN9252_ReadESC(uint16_t esc_addr, uint8_t len) {
//    uint32_t cmd;
//
//    // Wait if previous operation still busy
//    if (!LAN9252_CSR_WaitReady(10000)) {
//        return 0xFFFFFFFF; // Timeout error
//    }
//
//    // Build command: READ | SIZE | ADDRESS
//    switch(len) {
//        case 1: cmd = CSR_BUSY | CSR_READ | CSR_SIZE_1BYTE | esc_addr; break;
//        case 2: cmd = CSR_BUSY | CSR_READ | CSR_SIZE_2BYTE | esc_addr; break;
//        case 4: cmd = CSR_BUSY | CSR_READ | CSR_SIZE_4BYTE | esc_addr; break;
//        default: return 0xFFFFFFFF;
//    }
//
//    // Write command to CSR_CMD register
//    LAN9252_WriteInternal(LAN9252_CSR_CMD_REG, cmd, 4);
//
//    // Wait for read to complete
//    if (!LAN9252_CSR_WaitReady(10000)) {
//        return 0xFFFFFFFF;
//    }
////    uint32_t test = LAN9252_ReadReg32(LAN9252_CSR_CMD_REG);
//    // Read data from CSR_DATA register
//    uint32_t data = LAN9252_ReadReg32(LAN9252_CSR_DATA_REG);
//
//    return data;
//}
//
//// Write to ESC address space
//uint8_t LAN9252_WriteESC(uint16_t esc_addr, uint32_t data, uint8_t len) {
//    uint32_t cmd;
//
//    // Wait if previous operation still busy
//    if (!LAN9252_CSR_WaitReady(10000)) {
//        return 0;
//    }
//
//    // Write data first
//    LAN9252_WriteInternal(LAN9252_CSR_DATA_REG, data, 4);
//
//    // Build command: WRITE | SIZE | ADDRESS
//    switch(len) {
//        case 1: cmd = CSR_BUSY | CSR_WRITE | CSR_SIZE_1BYTE | esc_addr; break;
//        case 2: cmd = CSR_BUSY | CSR_WRITE | CSR_SIZE_2BYTE | esc_addr; break;
//        case 4: cmd = CSR_BUSY | CSR_WRITE | CSR_SIZE_4BYTE | esc_addr; break;
//        default: return 0;
//    }
//
//    // Write command
//    LAN9252_WriteInternal(LAN9252_CSR_CMD_REG, cmd, 4);
//
////    HAL_Delay(1);
//
//    // Wait for write to complete
////    return 1;
//    return LAN9252_CSR_WaitReady(10000);
//}
