/// @file led.c

#include <stdint.h>

#include "registers.h"
#include "led.h"

/*
 * Utilities
 */

// Mask with 1 at the locations which to copy
uint8_t set_bits(uint8_t lhs, uint8_t rhs, uint8_t mask) {
	// First clear needed bits, then set them according to state.
	return (lhs & ~mask) | (rhs & mask);
}

void set_led_bits(uint8_t rhs, uint8_t mask) {
	LED = set_bits(LED, rhs, mask);
}

void led_clear(void) {
	LED = 0x00;
}

void led_init(void) {
	led_clear();
}

/*
 * STATE
 */

/// Set the state of the led.
/// @param state The new state.
void led_state_set(LedState state) {
	// Keep in valid/expected bounds
	uint8_t safe_state = state & LED_STATE_MASK;

	set_led_bits(safe_state, LED_STATE_MASK);
}

void led_state_on() {
	led_state_set(LED_STATE_ON);
}

void led_state_off() {
	led_state_set(LED_STATE_OFF);
}

/*
 * COLOR
 */

void led_color_set(uint8_t color) {
	uint8_t safe_brightness = (color & LED_COLOR_VALUE_MASK) << LED_COLOR_OFFSET;

	set_led_bits(safe_brightness, LED_COLOR_MASK);
}

void led_color_red(void) {
	led_color_set(LED_COLOR_RED);
}

void led_color_green(void) {
	led_color_set(LED_COLOR_GREEN);
}

void led_color_blue(void) {
	led_color_set(LED_COLOR_BLUE);
}

/*
 * BRIGHTNESS
 */
void led_brightness_set(uint8_t brightness) {
	uint8_t safe_brightness = (brightness & LED_BRIGHTNESS_VALUE_MASK) << LED_BRIGHTNESS_OFFSET;

	set_led_bits(safe_brightness, LED_BRIGHTNESS_MASK);
}

void led_brightness_min() {
	led_brightness_set(LED_BRIGHTNESS_MIN);
}

void led_brightness_max() {
	led_brightness_set(LED_BRIGHTNESS_MAX);
}
