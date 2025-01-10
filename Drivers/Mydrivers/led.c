#include "gpio.h"

void LED0_On(void)
{
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET);
}

void LED0_Off(void)
{
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
}

void LED1_On(void)
{
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);
}

void LED1_Off(void)
{
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);
}

void LED0_Turn(void)
{
  HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
}

void LED1_Turn(void)
{
  HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10);
}
