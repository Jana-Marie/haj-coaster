#ifndef __IS31FL3733_H
#define __IS31FL3733_H

#include "stm32f0xx_hal.h"

extern I2C_HandleTypeDef hi2c1;
#define ISSIHANDLE &hi2c1
#define ISSIADDR 0x50

/*
  Command registermap
*/
#define regCommand 0xFD
#define commandPage0 0x00
#define commandPage1 0x01
#define commandPage2 0x02
#define commandPage3 0x03

#define regCommandLock 0xFE
#define commandRegUnlock 0xC5

#define regInterruptMask 0xF0
#define maskInterruptAutoClear 0x08
#define maskInterruptAutoBreath 0x04
#define maskInterruptDotShort 0x02
#define maskInterruptDotOpen 0x01

#define regInterruptStatus 0xF1
#define maskInterruptAbm3 0x10
#define maskInterruptAbm2 0x08
#define maskInterruptAbm1 0x04
#define maskInterruptShort 0x02
#define maskInterruptOpen 0x01

/*
  Page0 - LED Control
*/
#define regLedOnOff 0x00 // - 0x17
#define ledOnOffLen 24

#define regLedOpen 0x18 // - 0x2F
#define LedOpenLen 24

#define regLedShort 0x30 // - 0x47
#define LedShortLen 24

/*
  Page1 - PWM
*/
#define regPwm 0x00 // - 0xBF
#define pwmLen 192

/*
  Page2 - Auto Breath Mode (ABM)
*/
#define regAbm 0x00 // - 0xBF
#define abmLen 192

/*
  Page3 - Function, Configuration & Reset
*/
#define regConfiguration 0x00
#define regGlobalCurrent 0x01

#define regAbm1FadeIn 0x02
#define regAbm1FadeOut 0x03
#define regAbm1loop0 0x04
#define regAbm1loop1 0x05

#define regAbm2FadeIn 0x06
#define regAbm2FadeOut 0x07
#define regAbm2loop0 0x08
#define regAbm2loop1 0x09

#define regAbm3FadeIn 0x0A
#define regAbm3FadeOut 0x0B
#define regAbm3loop0 0x0C
#define regAbm3loop1 0x0D

#define regTimeUpdate 0x0E

#define regSWy 0x0F
#define regCSx 0x10

#define regReset 0x11

enum page_e {PAGE0, PAGE1, PAGE2, PAGE3};

void ISSI_Init(void);
HAL_StatusTypeDef ISSI_Unlock_Command(void);
HAL_StatusTypeDef ISSI_Switch_Page(uint8_t page);
HAL_StatusTypeDef ISSI_Send_Command(uint8_t reg, uint8_t command);
HAL_StatusTypeDef ISSI_Set_Current(uint8_t current);
HAL_StatusTypeDef ISSI_Read_Register(uint8_t reg, uint8_t *data, uint8_t len);
HAL_StatusTypeDef ISSI_Write_Register(uint8_t reg, uint8_t *data, uint8_t len);

#endif