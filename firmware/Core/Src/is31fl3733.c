#include "is31fl3733.h"

void ISSI_Init(void){
	ISSI_Switch_Page(commandPage3);
	ISSI_Send_Command(regConfiguration, 0x01); // no sync, no Open or Short detection, PWM mode enable, Normal operation
	ISSI_Set_Current(250); // 25.5mA max IOUT, ILED = IOUT/12.75 → ILED = 25.5/12.75 = 2mA
	uint8_t ledOnMatrix[24] = { 0xFF, 0xFF,  // SW1
								0xFF, 0xFF,  // SW2
								0xFF, 0xFF,  // SW3
								0xFF, 0x25,  // SW4*
								0xFF, 0x25,  // SW5*
								0xFF, 0x25,  // SW6*
								0xFF, 0xFF,  // SW7
								0x92, 0x24,  // SW8*
								0x92, 0x24,  // SW9*
								0x92, 0x24,  // SW10*
								0x92, 0x24,  // SW11*
								0x92, 0x24}; // SW12*
	ISSI_Switch_Page(commandPage0);
	ISSI_Write_Register(0x00, ledOnMatrix, 24);
}

HAL_StatusTypeDef ISSI_Unlock_Command(void){
	uint8_t __tmp = commandRegUnlock;
	return ISSI_Write_Register(regCommandLock, &__tmp, 1);
}

HAL_StatusTypeDef ISSI_Switch_Page(uint8_t page){
	ISSI_Unlock_Command();
	return ISSI_Write_Register(regCommand, &page, 1);
}

HAL_StatusTypeDef ISSI_Send_Command(uint8_t reg, uint8_t command){
	return ISSI_Write_Register(reg, &command, 1);
}

// Max 42mA - current in µA
HAL_StatusTypeDef ISSI_Set_Current(uint8_t current){
	//uint8_t __tmp = (uint8_t)(256000/(42*current));
	//uint8_t __tmp = 200; //(uint8_t)(256000/(42*current));
	return ISSI_Write_Register(regGlobalCurrent, &current, 1);
}

HAL_StatusTypeDef ISSI_Read_Register(uint8_t reg, uint8_t *data, uint8_t len){
  return HAL_I2C_Mem_Read(ISSIHANDLE, (ISSIADDR << 1), (uint16_t) reg, I2C_MEMADD_SIZE_8BIT, data, len, 1000);
}

HAL_StatusTypeDef ISSI_Write_Register(uint8_t reg, uint8_t *data, uint8_t len){
  return HAL_I2C_Mem_Write(ISSIHANDLE, (ISSIADDR << 1), (uint16_t) reg, I2C_MEMADD_SIZE_8BIT, data, len, 1000);
}