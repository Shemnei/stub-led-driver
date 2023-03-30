//! \file led.h
//!
//! Led Register Layout (8bit):
//!
//! ```txt
//! +------------+-------+-------+
//! | Brightness | Color | State |
//! +------------+-------+-------+
//! <--- 4bit ---|<- 3bit|<- 1bit|
//!     7..4       3..1     0
//! ```

#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>
#include <stdbool.h>

/// Bit mask for the state section
#define LED_STATE_MASK 0x1

/// Bit offset of the color section
#define LED_COLOR_OFFSET     0x1 // 1
/// Bit mask of the color value (apply to constrain the value to valid values)
#define LED_COLOR_VALUE_MASK 0x7 // 7 / 0b111
/// Bit mask for the color section
#define LED_COLOR_MASK       LED_COLOR_VALUE_MASK << LED_COLOR_OFFSET // 0b0000_1110

/// Bit offset of the brightness section
#define LED_BRIGHTNESS_OFFSET     0x4 // 4
/// Bit mask of the brightness value (apply to constrain the value to valid values)
#define LED_BRIGHTNESS_VALUE_MASK 0xf // 15 / 0b1111
/// Bit mask for the brightness section
#define LED_BRIGHTNESS_MASK       LED_BRIGHTNESS_VALUE_MASK << LED_BRIGHTNESS_OFFSET // 0b1111_0000
/// Minimum allowed value for brightness
#define LED_BRIGHTNESS_MIN        0x0 // 0
/// Maximum allowed value for brightness
#define LED_BRIGHTNESS_MAX        0xf // 15

/// The allowed states for the led state.
typedef enum {
	/// Disables the led.
	LED_STATE_OFF = 0x0,
	/// Enables the led.
	LED_STATE_ON  = 0x1,
} LedState;

/// All allowed colors.
///
/// # NOTE
/// Values can be combined by `or`ing them:
///
/// ```c
/// int color = LED_COLOR_RED | LED_COLOR_GREEN;
/// ```
typedef enum {
	/// The color red.
	LED_COLOR_RED   = 0x1,
	/// The color green.
	LED_COLOR_GREEN = 0x2,
	/// The color blue.
	LED_COLOR_BLUE  = 0x4,
} LedColor;

/// Initializes the led.
/// This functions should be called before any other function from this unit.
void led_init(void);

/// Clears all relevant bits from the led.
void led_clear(void);

/// Sets the state of the led.
///
/// @param state The new state for the led (1bit).
void led_state_set(LedState state);

/// Enables the led.
///
/// # NOTE
/// This is the same as calling:
///
/// ```c
/// led_state_set(LED_STATE_ON);
/// ```
void led_state_on(void);

/// Disables the led.
///
/// # NOTE
/// This is the same as calling:
///
/// ```c
/// led_state_set(LED_STATE_OFF);
/// ```
void led_state_off(void);

/// Sets the color(s) for the led.
///
/// @param color The color(s) to set the led to (3bit).
///
/// # NOTE
/// Each call to this function will clear any existing set color bits.
///
/// # Example
/// ```c
/// led_color_set(LED_COLOR_RED | LED_COLOR_GREEN);
/// ```
void led_color_set(uint8_t color);

/// Sets the color of the led to red.
///
/// # NOTE
/// This is the same as calling:
///
/// ```c
/// led_color_set(LED_COLOR_RED);
/// ```
void led_color_red(void);

/// Sets the color of the led to green.
///
/// # NOTE
/// This is the same as calling:
///
/// ```c
/// led_color_set(LED_COLOR_GREEN);
/// ```
void led_color_green(void);

/// Sets the color of the led to blue.
///
/// # NOTE
/// This is the same as calling:
///
/// ```c
/// led_color_set(LED_COLOR_BLUE);
/// ```
void led_color_blue(void);

/// Sets the brightness of the led.
///
/// @param brightness The brightness to set the led to (4bit).
///
/// # NOTE
/// The value for brightness should be constraint to LED_BRIGHTNESS_MIN and LED_BRIGHTNESS_MAX.
void led_brightness_set(uint8_t brightness);

/// Sets the brightness of the led to the minimum value.
///
/// # NOTE
/// This is the same as calling:
///
/// ```c
/// led_brightness_set(LED_BRIGHTNESS_MIN);
/// ```
void led_brightness_min(void);

/// Sets the brightness of the led to the maximum value.
///
/// # NOTE
/// This is the same as calling:
///
/// ```c
/// led_brightness_set(LED_BRIGHTNESS_MAX);
/// ```
void led_brightness_max(void);

#endif
