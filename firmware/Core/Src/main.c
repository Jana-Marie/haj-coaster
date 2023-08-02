#include "main.h"
#include "usb_device.h"
#include "is31fl3733.h"

#define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define ABS(x) ((x) < 0 ? -(x) : (x))

ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_tx;
DMA_HandleTypeDef hdma_i2c1_rx;

TIM_HandleTypeDef htim1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC_Init(void);
static void MX_TIM1_Init(void);
void rng(void);

uint16_t adc[3];
uint8_t noise8;
uint16_t noise16;
uint32_t noise32 = 37;

// Map of all Stars
uint8_t mappingStars[18] = {0x30, 0x32, 0x33, 0x35, 0x36, 0x38,
                            0x40, 0x42, 0x43, 0x45, 0x46, 0x48,
                            0x50, 0x52, 0x53, 0x55, 0x56, 0x58};
// Index of mappingStars
uint8_t subMappingStarsRed = 0x08;
uint8_t subMappingStarsGreen = 0x09;
uint8_t subMappingStarsYellow[2] = {0x00, 0x10};
uint8_t subMappingStarsPink[3] = {0x0E, 0x07, 0x04};
uint8_t subMappingStarsCyan[3] = {0x0B, 0x11, 0x05};
uint8_t subMappingStarsWhite[8] = {0x01, 0x02, 0x03, 0x06, 0x0A, 0x0C, 0x0D, 0x0F};

// Map of all RGB LEDs and their adjacent Cyan and Pink ones
uint8_t mappingR[20] = {0x01, 0x04, 0x07, 0x0A, 0x0D,
                        0x31, 0x34, 0x37, 0x3A, 0x3D,
                        0x61, 0x64, 0x67, 0x6A, 0x6D,
                        0xB1, 0xB4, 0xB7, 0xBA, 0xBD};
uint8_t mappingG[20] = {0x11, 0x14, 0x17, 0x1A, 0x1D,
                        0x41, 0x44, 0x47, 0x4A, 0x4D,
                        0x71, 0x74, 0x77, 0x7A, 0x7D,
                        0xA1, 0xA4, 0xA7, 0xAA, 0xAD};
uint8_t mappingB[20] = {0x21, 0x24, 0x27, 0x2A, 0x2D,
                        0x51, 0x54, 0x57, 0x5A, 0x5D,
                        0x81, 0x84, 0x87, 0x8A, 0x8D,
                        0x91, 0x94, 0x97, 0x9A, 0x9D};

uint8_t mappingP[20] = {0x0C, 0x00, 0x09, 0x06, 0x03,
                        0x10, 0x13, 0x1C, 0x19, 0x16,
                        0x26, 0x20, 0x23, 0x29, 0x2C,
                        0x60, 0x63, 0x66, 0x69, 0x6C};
uint8_t mappingC[20] = {0x0E, 0x02, 0x0B, 0x08, 0x05,
                        0x12, 0x15, 0x1E, 0x1B, 0x18,
                        0x28, 0x22, 0x25, 0x2B, 0x2E,
                        0x62, 0x65, 0x68, 0x6B, 0x6E};

// Index of all RGB + adjacent LEDs
uint8_t subMappingGlass = 0x13;
uint8_t subMappingShark[4] = {0x01, 0x02, 0x05, 0x0C};
uint8_t subMappingHead[4] = {0x04, 0x03, 0x09, 0x0A};
uint8_t subMappingArms[3] = {0x00, 0x07, 0x06};
uint8_t subMappingLegs[8] = {0x08, 0x11, 0x0B, 0x0D, 0x0F, 0x0E, 0x10, 0x12};
uint8_t subMappingBody[4+3+8] = { 0 };

// Additional maps
uint8_t gammaMap[256] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 16, 18, 20, 22, 
                          24, 26, 29, 32, 35, 38, 41, 44, 47, 50, 53, 57, 61, 
                          65, 69, 73, 77, 81, 85, 89, 94, 99, 104, 109, 114, 119, 
                          124, 129, 134, 140, 146, 152, 158, 164, 170, 176, 
                          182, 188, 195, 202, 209, 216, 223, 230, 237, 244, 251, 255};

int8_t starSparkleMap[18];
uint8_t starSparkleNormalMap[18];
uint8_t starBaseBrightWhite = 56;
uint8_t starBaseBrightCyan = 20;
uint8_t starBaseBrightPink = 15;
uint8_t starBaseBrightYellow = 30;
uint8_t starBaseBrightRed = 10;
uint8_t starBaseBrightGreen = 30;

// Framebuffer
uint8_t pwmBuf[pwmLen];
uint8_t heartbeat;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_ADC_Init();
  MX_TIM1_Init();
  MX_USB_DEVICE_Init();
  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, 1);
  HAL_ADC_Start_DMA(&hadc, (uint32_t *) &adc[0], 3);
  HAL_TIM_Base_Start_IT(&htim1);

  ISSI_Init();
  HAL_Delay(20);

  memcpy(subMappingBody, subMappingHead, sizeof(subMappingHead));
  memcpy(subMappingBody + 4, subMappingArms, sizeof(subMappingArms));
  memcpy(subMappingBody + 4 + 3, subMappingLegs, sizeof(subMappingLegs));

  // Empty PWM buffer
  uint8_t pwmBuf[pwmLen];
  for(uint8_t i = 0; i < pwmLen; i++){
    pwmBuf[i] = 0;
  }

  // Switch to PWM Page
  ISSI_Switch_Page(commandPage1);

  // Clear LEDs
  ISSI_Write_Register(0x00, pwmBuf, pwmLen);

  // Fill Normals
  starSparkleNormalMap[subMappingStarsRed] = starBaseBrightRed;
  starSparkleNormalMap[subMappingStarsGreen] = starBaseBrightGreen;
  for(uint8_t i = 0; i < 2; i++){
      starSparkleNormalMap[subMappingStarsYellow[i]] = starBaseBrightYellow;
  }
  for(uint8_t i = 0; i < 3; i++){
      starSparkleNormalMap[subMappingStarsPink[i]] = starBaseBrightPink;
      starSparkleNormalMap[subMappingStarsCyan[i]] = starBaseBrightCyan;
  }
  for(uint8_t i = 0; i < 8; i++){
      starSparkleNormalMap[subMappingStarsWhite[i]] = starBaseBrightWhite;
  }

  while (1)
  {
    HAL_Delay(30);

    /* --------------------------- */
    /*         Star Sparkle        */
    if((noise8 >> 1) % 12 == 0){
      starSparkleMap[(noise8 / 14) % 18] = ((int8_t)(noise8 * noise16))/4;
    }
    for(uint8_t i = 0; i < 18; i++){
      starSparkleMap[i] -= starSparkleMap[i] > 0 ? 1 : 0;
      starSparkleMap[i] += starSparkleMap[i] < 0 ? 1 : 0;
    }
    for(uint8_t i = 0; i < 18; i++){
      pwmBuf[mappingStars[i]] = gammaMap[CLAMP(starSparkleNormalMap[i] + starSparkleMap[i], 0, 63)];
    }
    /* --------------------------- */

    /* --------------------------- */
    /*            Shork            */
    for(uint8_t i = 0; i < 4; i++){
      pwmBuf[mappingC[subMappingShark[i]]] = gammaMap[50];
      pwmBuf[mappingB[subMappingShark[i]]] = gammaMap[40];
    }
    pwmBuf[mappingC[subMappingShark[0]]] = gammaMap[20];
    pwmBuf[mappingP[subMappingShark[0]]] = gammaMap[10];
    pwmBuf[mappingR[subMappingShark[0]]] = gammaMap[30];
    pwmBuf[mappingG[subMappingShark[0]]] = gammaMap[40];
    /* --------------------------- */

    /* --------------------------- */
    /*            Body             */
    for(uint8_t i = 0; i < 15; i++){
      pwmBuf[mappingP[subMappingBody[i]]] = gammaMap[20];
      pwmBuf[mappingR[subMappingBody[i]]] = gammaMap[30];
      pwmBuf[mappingG[subMappingBody[i]]] = gammaMap[40];
      pwmBuf[mappingB[subMappingBody[i]]] = gammaMap[23];
    }
    for(uint8_t i = 0; i < 4; i++){
      pwmBuf[mappingP[subMappingHead[i]]] = gammaMap[0];
      pwmBuf[mappingR[subMappingHead[i]]] = gammaMap[20];
      pwmBuf[mappingG[subMappingHead[i]]] = gammaMap[20];
      pwmBuf[mappingB[subMappingHead[i]]] = gammaMap[5];
    }
    pwmBuf[mappingR[subMappingHead[2]]] = gammaMap[43];
    pwmBuf[mappingG[subMappingHead[2]]] = gammaMap[50];
    pwmBuf[mappingB[subMappingHead[2]]] = gammaMap[40];
    pwmBuf[mappingP[subMappingHead[2]]] = gammaMap[0];
    pwmBuf[mappingC[subMappingHead[2]]] = gammaMap[0];

    /* --------------------------- */

    /* --------------------------- */
    /*            Drink            */
    heartbeat = (uint8_t)ABS((int8_t)((HAL_GetTick() % 2560) / 10)) / 4;
    pwmBuf[mappingR[subMappingGlass]] = gammaMap[10];
    pwmBuf[mappingG[subMappingGlass]] = gammaMap[10];
    pwmBuf[mappingB[subMappingGlass]] = gammaMap[10];
    pwmBuf[mappingP[subMappingGlass]] = gammaMap[heartbeat+31];
    pwmBuf[mappingC[subMappingGlass]] = gammaMap[(32-heartbeat)+31];
    /* --------------------------- */

    /* --------------------------- */
    /*           Write FB          */
    ISSI_Write_Register(0x00, pwmBuf, pwmLen);
    /* --------------------------- */

  }
}

void rng(){
  noise32 += ((noise32 + 57) * (adc[2] + 123)) * 17 / 19;
  noise16 = (uint16_t)(noise32 * 19 / 7);
  noise16 ^= 0xAAAA;
  noise8 = (uint8_t)(noise32 * 23 / 11);
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

}

static void MX_ADC_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_TRGO;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc.Init.DMAContinuousRequests = ENABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  HAL_ADC_Init(&hadc);

  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);

  sConfig.Channel = ADC_CHANNEL_1;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);

  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);
}

static void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2010091A;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c1);

  HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE);

  HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0);

  __HAL_SYSCFG_FASTMODEPLUS_ENABLE(I2C_FASTMODEPLUS_I2C1);
}

static void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 47;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_Base_Init(&htim1);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);
}

static void MX_DMA_Init(void)
{
  __HAL_RCC_DMA1_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EN_GPIO_Port, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif
