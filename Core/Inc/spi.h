//#ifndef INC_SPI_H_
//#define INC_SPI_H_
//
//#include "main.h"
//
//extern SPI_HandleTypeDef hspi1;
//
//#define LAN9252_CS_LOW()   HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)
//#define LAN9252_CS_HIGH()  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)
//
//// LAN9252 SPI Command Structure
//#define SPI_READ_CMD    0x03
//#define SPI_WRITE_CMD   0x02
//#define SPI_FAST_READ   0x0B
//
//// LAN9252 Internal Registers for ESC Access
//#define LAN9252_BYTE_ORDER_REG      0x64
//#define LAN9252_CSR_DATA_REG        0x300
//#define LAN9252_CSR_CMD_REG         0x304
//#define LAN9252_CSR_STATUS_REG      0x6C
//
//// CSR Command Register bits
//#define CSR_BUSY        (1 << 31)
//#define CSR_READ        (1 << 30)
//#define CSR_WRITE       (0 << 30)
//#define CSR_SIZE_1BYTE  (1 << 16)
//#define CSR_SIZE_2BYTE  (2 << 16)
//#define CSR_SIZE_4BYTE  (4 << 16)
//
//#define BITS_8  1
//#define BITS_16 2
//#define BITS_32 4
//
//// Function declarations only (no implementation here!)
//uint32_t LAN9252_ReadReg32(uint16_t address);
//uint16_t LAN9252_ReadReg16(uint16_t address);
//void LAN9252_WriteInternal(uint16_t addr, uint32_t data, uint8_t len);
//void LAN9252_Reset(void);
//uint8_t LAN9252_CSR_WaitReady(uint32_t timeout_us);
//uint32_t LAN9252_ReadESC(uint16_t esc_addr, uint8_t len);
//uint8_t LAN9252_WriteESC(uint16_t esc_addr, uint32_t data, uint8_t len);
//
//#endif /* INC_SPI_H_ */
