#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
}

// Switch (HAP section 8.38)
// required: ON
// optional: NAME

// format: bool; HAP section 9.70; write the .setter function to get the switch-event sent from iOS Home APP.
homekit_characteristic_t cha_switch_left_light = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch_right_light = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_lightbulb_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_light = HOMEKIT_CHARACTERISTIC_(CURRENT_AMBIENT_LIGHT_LEVEL, 2);

homekit_characteristic_t cha_name_lightbulb = HOMEKIT_CHARACTERISTIC_(NAME, "LED strip with rainbow switch");
homekit_characteristic_t cha_lightbulb_bright = HOMEKIT_CHARACTERISTIC_(BRIGHTNESS, 50);
homekit_characteristic_t cha_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch");

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Left Switch"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "NodeMCU v3 HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "21022021"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
		HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
			&cha_switch_left_light,
			&cha_name,
			NULL
		}),
        NULL
    }),
    HOMEKIT_ACCESSORY(.id=2, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Right Switch"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "NodeMCU v3 HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "21022021"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
		HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
			&cha_switch_right_light,
			&cha_name,
			NULL
		}),
        NULL
    }),
    HOMEKIT_ACCESSORY(.id=3, .category=homekit_accessory_category_sensor, .services=(homekit_service_t*[]) {
    	HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
			HOMEKIT_CHARACTERISTIC(NAME, "Light Sensor"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "NodeMCU v3 HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "21022021"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
			HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
			NULL
		}),
    	HOMEKIT_SERVICE(LIGHT_SENSOR, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
			HOMEKIT_CHARACTERISTIC(NAME, "Light"),
			&cha_light,
			NULL
		}),
		NULL
	}),
    HOMEKIT_ACCESSORY(.id=4, .category=homekit_accessory_category_lightbulb, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Monitor LED Controller"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "NodeMCU v3 HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "21022021"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
        HOMEKIT_SERVICE(LIGHTBULB, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
          HOMEKIT_CHARACTERISTIC(NAME, "Monitor LED"),
            &cha_lightbulb_on,
            &cha_name_lightbulb,
            &cha_lightbulb_bright,
            NULL
        }),
        NULL
    }),
    NULL
};

homekit_server_config_t config = {
		.accessories = accessories,
		.password = "111-11-111",
        .setupId = "1BY8",
};
