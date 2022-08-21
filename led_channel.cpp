#include "led_channel.h"

#include <Arduino.h>

void led_channel::set_power(const daytime_t daytime)
{
	bool update = true;

	if (daytime == DAY) {
		if (this->current_power < this->cfg.max_day_power) {
			this->current_power++;
		}
		else if (this->current_power > this->cfg.max_day_power) {
			this->current_power--;
		}
		else {
			update = false;
		}
	}
	else if (daytime == NIGHT) {
		if (this->current_power < this->cfg.max_night_power) {
			this->current_power++;
		}
		else if (this->current_power > this->cfg.max_night_power) {
			this->current_power--;
		}
		else {
			update = false;
		}
	}
	else {
		update = false;
	}

	if (update) {
		analogWrite(this->pin, this->current_power);
	}
}


led_channel::led_channel(const uint8_t pin, const char name, const uint8_t max_day_power, const uint8_t max_night_power, const time_t sunrise, const time_t sunset, const uint8_t sleep):pin(pin), name(name)
{
	this->current_power = 0;
	this->cfg.max_day_power = max_day_power;
	this->cfg.max_night_power = max_night_power;
	
	this->cfg.sunrise = sunrise;
	this->cfg.sunset = sunset;

	this->sleep = sleep;
}

led_channel::~led_channel()
{
}

void led_channel::init(void)
{
	pinMode(this->pin, OUTPUT);
	analogWrite(this->pin, LOW);
}

void led_channel::set_max_power(const uint8_t max_power, daytime_t daytime)
{
	if (daytime == DAY) {
		this->cfg.max_day_power = max_power;
	}
	else if (daytime == NIGHT) {
		this->cfg.max_night_power = max_power;
	}
}

void led_channel::set_current_power_on_value(const uint8_t power)
{
	if ((power < min_power) || (power > max_power)) {
		return;
	}

	this->current_power = power;
	analogWrite(this->pin, this->current_power);
}

void led_channel::set_sunset(const time_t sunset)
{
	this->cfg.sunset = sunset;
}

void led_channel::set_sunrise(const time_t sunrise)
{
	this->cfg.sunrise = sunrise;
}

uint8_t led_channel::current_power_get(void)
{

	return this->current_power;
}

led_channel_cfg_t led_channel::config_get(void)
{
	return this->cfg;
}

void led_channel::config_set(led_channel_cfg_t new_cfg)
{

	this->cfg = new_cfg;
}

char led_channel::name_get(void)
{

	return this->name;
}

void led_channel::handle(const time_extendet_t current_time)
{
	uint16_t current = current_time.hour * 60 + current_time.minute;
	uint16_t sunrise = this->cfg.sunrise.hour * 60 + this->cfg.sunrise.minute;
	uint16_t sunset = this->cfg.sunset.hour * 60 + this->cfg.sunset.minute;
	daytime_t daytime = NIGHT;
	static uint32_t last = 0;

	if (current > sunrise && current < sunset) {
		daytime = DAY;
	}

	if ((current_time.hour * 60 * 60 + current_time.minute * 60 + current_time.second) - last > this->sleep) {
		this->set_power(daytime);
	}

	last = current_time.hour * 60 * 60 + current_time.minute * 60 + current_time.second;
}
