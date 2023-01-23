/*

The MIT License (MIT)

Copyright (c) 2016 Hubert Denkmair

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "config.h"
#include "board.h"
#include "device.h"
#include "usbd_gs_can.h"
#include "gpio.h"

void legacy_setup(USBD_GS_CAN_HandleTypeDef *hGS_CAN) {
	// Clock setup
	// Systick setup
	device_sysclock_config();

	// Interrupt

	// gpio

	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
#if defined(STM32F4)
	__HAL_RCC_GPIOD_CLK_ENABLE();
#endif

#ifdef LEDRX_Pin
	HAL_GPIO_WritePin(LEDRX_GPIO_Port, LEDRX_Pin, GPIO_INIT_STATE(LEDRX_Active_High));
	GPIO_InitStruct.Pin = LEDRX_Pin;
	GPIO_InitStruct.Mode = LEDRX_Mode;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LEDRX_GPIO_Port, &GPIO_InitStruct);
#endif

#ifdef LEDTX_Pin
	HAL_GPIO_WritePin(LEDTX_GPIO_Port, LEDTX_Pin, GPIO_INIT_STATE(LEDTX_Active_High));
	GPIO_InitStruct.Pin = LEDTX_Pin;
	GPIO_InitStruct.Mode = LEDTX_Mode;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LEDTX_GPIO_Port, &GPIO_InitStruct);
#endif

#ifdef CAN_S_Pin
	HAL_GPIO_WritePin(CAN_S_GPIO_Port, CAN_S_Pin, GPIO_PIN_SET);
	GPIO_InitStruct.Pin = CAN_S_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(CAN_S_GPIO_Port, &GPIO_InitStruct);
#endif

#ifdef LEDRX_Pin
	HAL_GPIO_WritePin(LEDRX_GPIO_Port, LEDRX_Pin, GPIO_INIT_STATE(LEDRX_Active_High));
	GPIO_InitStruct.Pin = LEDRX_Pin;
	GPIO_InitStruct.Mode = LEDRX_Mode;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LEDRX_GPIO_Port, &GPIO_InitStruct);
#endif

#ifdef LEDTX_Pin
	HAL_GPIO_WritePin(LEDTX_GPIO_Port, LEDTX_Pin, GPIO_INIT_STATE(LEDTX_Active_High));
	GPIO_InitStruct.Pin = LEDTX_Pin;
	GPIO_InitStruct.Mode = LEDTX_Mode;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LEDTX_GPIO_Port, &GPIO_InitStruct);
#endif

#ifdef nCANSTBY_Pin
	HAL_GPIO_WritePin(nCANSTBY_Port, nCANSTBY_Pin, GPIO_INIT_STATE(nCANSTBY_Active_High));
	GPIO_InitStruct.Pin = nCANSTBY_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(nCANSTBY_Port, &GPIO_InitStruct); //xceiver standby.
#endif

#ifdef DCDCEN_Pin
	HAL_GPIO_WritePin(DCDCEN_Port, DCDCEN_Pin, GPIO_PIN_SET);
	GPIO_InitStruct.Pin = DCDCEN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DCDCEN_Port, &GPIO_InitStruct);   //start DCDC (TODO : wait until enumerated)
#endif

#ifdef nSI86EN_Pin
	HAL_GPIO_WritePin(nSI86EN_Port, nSI86EN_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = nSI86EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(nSI86EN_Port, &GPIO_InitStruct);  //enable si86
#endif


#if defined(BOARD_STM32F4_DevBoard)
	// initialize USB pins
	GPIO_InitStruct.Pin = USB_Pin_DM | USB_Pin_DP;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
	HAL_GPIO_Init(USB_GPIO_Port, &GPIO_InitStruct);
#endif

	UNUSED(GPIO_InitStruct);
	gpio_init_term();

	// timer

	// LEDs

	for (unsigned int i = 0; i < ARRAY_SIZE(hGS_CAN->channels); i++) {
		can_data_t *channel = &hGS_CAN->channels[i];

		led_init(&channel->leds,
				 LEDRX_GPIO_Port, LEDRX_Pin, LEDRX_Active_High,
				 LEDTX_GPIO_Port, LEDTX_Pin, LEDTX_Active_High);
	}

	/* nice wake-up pattern */
	for (uint8_t j = 0; j < 10; j++) {
		HAL_GPIO_TogglePin(LEDRX_GPIO_Port, LEDRX_Pin);
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LEDTX_GPIO_Port, LEDTX_Pin);
	}
}

void legacy_phy_pwr(can_data_t *channel, bool state) {
	UNUSED(channel);
	UNUSED(state);
	if(state) {
#ifdef nCANSTBY_Pin
		HAL_GPIO_WritePin(nCANSTBY_Port, nCANSTBY_Pin, !GPIO_INIT_STATE(nCANSTBY_Active_High));
#endif
	} else {
#ifdef nCANSTBY_Pin
		HAL_GPIO_WritePin(nCANSTBY_Port, nCANSTBY_Pin, GPIO_INIT_STATE(nCANSTBY_Active_High));
#endif
	}
}

enum gs_can_termination_state legacy_set_term(
	can_data_t *channel,
	enum gs_can_termination_state state
) {
	UNUSED(channel);
	UNUSED(state);
	return GS_CAN_TERMINATION_UNSUPPORTED;
}

enum gs_can_termination_state legacy_get_term(
	can_data_t *channel
) {
	UNUSED(channel);
	return GS_CAN_TERMINATION_UNSUPPORTED;
}

void legacy_set_standby(can_data_t *channel, bool state) {
	UNUSED(channel);
	UNUSED(state);
}

const struct BoardConfig config = {
	.setup = legacy_setup,
	.set_phy_pwr = legacy_phy_pwr,
	.set_term = legacy_set_term,
	.get_term = legacy_get_term,

	.usbd_product_string = USBD_PRODUCT_STRING_FS,
	.usbd_manufacturer_string = USBD_MANUFACTURER_STRING,
	.dfu_interface_name = DFU_INTERFACE_STRING_FS, //TODO never used

	// TIM2 Clock speed
	.timer_clock_speed = TIM2_CLOCK_SPEED,

	.can_clock_speed = CAN_CLOCK_SPEED,
	// Array of channel configs
	.channels = {{
					 .set_phy_pwr = legacy_phy_pwr,
					 .interface = CAN_INTERFACE,
				 }},
};
