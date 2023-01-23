#pragma once

/*

The MIT License (MIT)

Copyright (c) 2022 fenugrec

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

#include "usbd_gs_can.h"

struct BoardChannelConfig {
	void (* set_phy_pwr)(can_data_t *channel, bool state);
	enum gs_can_termination_state (* set_term)(can_data_t *channel, enum gs_can_termination_state state);
	enum gs_can_termination_state (* get_term)(can_data_t *channel, enum gs_can_termination_state state);
#if defined(STM32G0)
	FDCAN_GlobalTypeDef *interface;
#else
	CAN_TypeDef *interface;
#endif
};

struct BoardConfig {
	// Callback setup: clock, LEDs, GPIO, CAN Pinmux
	void (* setup)(USBD_GS_CAN_HandleTypeDef * hGS_CAN);
	// USBD strings
	const uint8_t *usbd_product_string;
	const uint8_t *usbd_manufacturer_string;
	const uint8_t *dfu_interface_name;
	// TIM2 Clock speed
	uint32_t timer_clock_speed;

	// CAN config
	uint32_t can_clock_speed;
	// Array of channel configs
	struct BoardChannelConfig channels[NUM_CAN_CHANNEL];
};

extern const struct BoardConfig config;
