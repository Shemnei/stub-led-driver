#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>
#include <stdbool.h>

/*
 * Led Register Layout (8bit):
 *
 * +------------+-------+-------+
 * | Brightness | Color | State |
 * +------------+-------+-------+
 * <--- 4bit ---|<- 3bit|<- 1bit|
 *     7..4       3..1     1
 */

// STATE (1bit)
#define LED_STATE_MASK 0x1

// COLOR (3bit)
#define LED_COLOR_OFFSET     0x1 // 1
#define LED_COLOR_VALUE_MASK 0x7 // 7 / 0b111
#define LED_COLOR_MASK       LED_COLOR_VALUE_MASK << LED_COLOR_OFFSET // 0b0000_1110

// BRIGHTNESS (4bit)
#define LED_BRIGHTNESS_OFFSET     0x4 // 4
#define LED_BRIGHTNESS_VALUE_MASK 0xf // 15 / 0b1111
#define LED_BRIGHTNESS_MASK       LED_BRIGHTNESS_VALUE_MASK << LED_BRIGHTNESS_OFFSET // 0b1111_0000
#define LED_BRIGHTNESS_MIN        0x0 // 0
#define LED_BRIGHTNESS_MAX        0xf // 15

// Bit width: 1
typedef enum {
	LED_STATE_OFF = 0x0,
	LED_STATE_ON  = 0x1,
} LedState;

// Bit width: 3
typedef enum {
	LED_COLOR_RED   = 0x1,
	LED_COLOR_GREEN = 0x2,
	LED_COLOR_BLUE  = 0x4,
} LedColor;

void led_init(void);
void led_clear(void);

void led_state_set(LedState state);
void led_state_on(void);
void led_state_off(void);

void led_color_set(uint8_t color);
void led_color_red(void);
void led_color_green(void);
void led_color_blue(void);

void led_brightness_set(uint8_t brightness);
void led_brightness_min(void);
void led_brightness_max(void);

#endif
