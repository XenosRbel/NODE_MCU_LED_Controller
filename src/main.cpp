#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
#include <../lib/LightDependentResistor/src/LightDependentResistor.h>

#define LOG_D(fmt, ...) printf_P(PSTR(fmt "\n"), ##__VA_ARGS__);

//==============================
// My sensors
//==============================

#define PIN_PHOTO_SENSOR A0

#define OTHER_RESISTOR 10000 //ohms
#define USED_PHOTOCELL LightDependentResistor::GL5528

LightDependentResistor photocell(PIN_PHOTO_SENSOR, OTHER_RESISTOR, USED_PHOTOCELL);

//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;

extern "C" homekit_characteristic_t cha_light;

extern "C" homekit_characteristic_t cha_lightbulb_on;
extern "C" homekit_characteristic_t cha_lightbulb_bright;

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

#define PIN_SWITCH 2
#define PIN_SWITCH_LEFT D1
#define PIN_SWITCH_RIGHT D2
#define PIN_LIGHTBULB D3

#define PWMRANGE_MAX_VALUE 1023

int calculate_pwm_bright(int bright_percent)
{
	return round((PWMRANGE_MAX_VALUE / 100) * bright_percent);
}

void cha_lightbulb_on_setter(const homekit_value_t value)
{
	bool on = value.bool_value;
	bool current_state = cha_lightbulb_on.value.bool_value;

	cha_lightbulb_on.value.bool_value = on;

	int bright = calculate_pwm_bright(cha_lightbulb_bright.value.int_value);

	LOG_D("Led is: %s", on ? "ON" : "OFF");
	LOG_D("Bright Led: %i", bright);

	if (!on)
	{
		analogWrite(PIN_LIGHTBULB, LOW);
	}
	if (!current_state && on)
	{
		analogWrite(PIN_LIGHTBULB, bright);
	}
}

void cha_lightbulb_bright_setter(const homekit_value_t value)
{
	int bright = value.int_value;
	cha_lightbulb_bright.value.int_value = bright;

	LOG_D("Bright: %i", bright);
	analogWrite(PIN_LIGHTBULB, calculate_pwm_bright(bright));
}

void my_homekit_report()
{
	double intensity_in_lux = photocell.getCurrentLux();

	cha_light.value.int_value = round(intensity_in_lux);
	homekit_characteristic_notify(&cha_light, cha_light.value);

	LOG_D("Current lux: %.5f", intensity_in_lux);
}

void my_homekit_setup()
{
	pinMode(PIN_SWITCH, OUTPUT);
	pinMode(PIN_LIGHTBULB, OUTPUT);

	digitalWrite(PIN_SWITCH, HIGH);

	cha_lightbulb_bright.setter = cha_lightbulb_bright_setter;
	cha_lightbulb_on.setter = cha_lightbulb_on_setter;

	arduino_homekit_setup(&config);

	homekit_characteristic_notify(&cha_lightbulb_bright, cha_lightbulb_bright.value);
	homekit_characteristic_notify(&cha_lightbulb_on, cha_lightbulb_on.value);
}

void my_homekit_loop()
{
	arduino_homekit_loop();

	const uint32_t t = millis();

	if (t > next_report_millis)
	{
		// report sensor values every 10 seconds
		next_report_millis = t + 10 * 1000;
		my_homekit_report();
	}

	if (t > next_heap_millis)
	{
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
			  ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
	}
}

void setup()
{
	Serial.begin(115200);

	photocell.setPhotocellPositionOnGround(false);

	wifi_connect();

	//homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example

	my_homekit_setup();
}

void loop()
{
	my_homekit_loop();
	delay(10);
}
