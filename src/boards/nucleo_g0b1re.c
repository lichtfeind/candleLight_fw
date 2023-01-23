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

#include "board.h"
#include "config.h"
#include "device.h"
#include "gpio.h"
#include "usbd_gs_can.h"

void nucleo_g0b1re_setup(USBD_GS_CAN_HandleTypeDef *hGS_CAN) {
	// Clock setup
	// Systick setup
	device_sysclock_config();

	// Interrupt

	// gpio

	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

#ifdef LEDRX_Pin
	HAL_GPIO_WritePin(LEDRX_GPIO_Port, LEDRX_Pin,
					  GPIO_INIT_STATE(LEDRX_Active_High));
	GPIO_InitStruct.Pin = LEDRX_Pin;
	GPIO_InitStruct.Mode = LEDRX_Mode;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LEDRX_GPIO_Port, &GPIO_InitStruct);
#endif

#ifdef LEDTX_Pin
	HAL_GPIO_WritePin(LEDTX_GPIO_Port, LEDTX_Pin,
					  GPIO_INIT_STATE(LEDTX_Active_High));
	GPIO_InitStruct.Pin = LEDTX_Pin;
	GPIO_InitStruct.Mode = LEDTX_Mode;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LEDTX_GPIO_Port, &GPIO_InitStruct);
#endif

	// timer

	// LEDs
	led_init(&hGS_CAN->channels[0].leds, LEDRX_GPIO_Port, LEDRX_Pin, LEDRX_Active_High,
			 LEDTX_GPIO_Port, LEDTX_Pin, LEDTX_Active_High);
	led_init(&hGS_CAN->channels[1].leds, LEDRX_GPIO_Port, LEDRX_Pin, LEDRX_Active_High,
			 LEDTX_GPIO_Port, LEDTX_Pin, LEDTX_Active_High);

	/* nice wake-up pattern */
	for (uint8_t j = 0; j < 10; j++) {
		HAL_GPIO_TogglePin(LEDRX_GPIO_Port, LEDRX_Pin);
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LEDTX_GPIO_Port, LEDTX_Pin);
	}
#ifdef CAN_S_GPIO_Port
	HAL_GPIO_WritePin(CAN_S_GPIO_Port, CAN_S_Pin, GPIO_PIN_RESET);
#endif

}

void nucleo_g0b1re_phy_pwr(can_data_t *channel, bool state) {
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

enum gs_can_termination_state nucleo_g0b1re_set_term(
	can_data_t *channel,
	enum gs_can_termination_state state
) {
	UNUSED(channel);
	UNUSED(state);
	return GS_CAN_TERMINATION_UNSUPPORTED;
}

enum gs_can_termination_state nucleo_g0b1re_get_term(
	can_data_t *channel
) {
	UNUSED(channel);
	return GS_CAN_TERMINATION_UNSUPPORTED;
}

void nucleo_g0b1re_set_standby(can_data_t *channel, bool state) {
	UNUSED(channel);
	UNUSED(state);
}

const struct BoardConfig config = {
	.setup = nucleo_g0b1re_setup,
	.set_phy_pwr = nucleo_g0b1re_phy_pwr,
	.set_term = nucleo_g0b1re_set_term,
	.get_term = nucleo_g0b1re_get_term,
	.set_standby = nucleo_g0b1re_set_standby,

	.usbd_product_string = USBD_PRODUCT_STRING_FS,
	.usbd_manufacturer_string = USBD_MANUFACTURER_STRING,
	.dfu_interface_name = DFU_INTERFACE_STRING_FS, // TODO never used

	// TIM2 Clock speed
	.timer_clock_speed = 64000000,

	.can_clock_speed = 40000000,
	// Array of channel configs
	.channels = {
		[0] = {
			.interface = FDCAN1,
		},
		[1] = {
			.interface = FDCAN2,
		},
	},
};
