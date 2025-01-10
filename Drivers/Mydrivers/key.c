#include "gpio.h"
#include "key.h"

uint8_t Key0_GetNum(void)
{
  uint8_t Keynum = 0;
  if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == 0)
  {
    HAL_Delay(20);
		while( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == 0);
		HAL_Delay(20);
		Keynum=1;
	}
  return Keynum;
}

uint8_t Key1_GetNum(void)
{
  uint8_t Keynum = 0;
  if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == 0)
  {
    HAL_Delay(20);
		while( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == 0);
		HAL_Delay(20);
		Keynum=1;
	}
  return Keynum;
}

uint8_t Key2_GetNum(void)
{
  uint8_t Keynum = 0;
  if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == 0)
  {
    HAL_Delay(20);
		while( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == 0);
		HAL_Delay(20);
		Keynum=1;
	}
  return Keynum;
}

uint8_t Key_up_GetNum(void)
{
  uint8_t Keynum = 0;
  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1)
  {
    HAL_Delay(20);
		while( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1);
		HAL_Delay(20);
		Keynum=1;
	}
  return Keynum;
}
