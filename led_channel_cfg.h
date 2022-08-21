#pragma once

#include "daytime.h"

#include <stdint.h>

/*
 * The structure to keep led channel config.
*/
typedef struct {
	uint8_t max_day_power;		/// Max LED power at day
	uint8_t max_night_power;	/// Max LED power at night
	time_t sunrise;				/// Sunrise time
	time_t sunset;				/// Susnset time
} led_channel_cfg_t;
