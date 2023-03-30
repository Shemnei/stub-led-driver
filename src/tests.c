//! \file tests.c
//!
//! A small collection of tests to test the functionality of the led/msg libraries.

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "led.h"
#include "msg.h"
#include "registers.h"

#define UINT8_T_BITS sizeof(uint8_t) * 8

/// Calls assert_bits_eq() with some values prefilled.
#define assert_led_bits(bits, message) assert_bits_eq(LED, bits, message, __FILE__, __func__, __LINE__)
/// Calls assert_msg_process() with some values prefilled.
#define assert_msg_process(is, should, message) assert_msg_process_full(is, should, message, __FILE__, __func__, __LINE__)

/// Pretty prints the bits of a `uint8_t`.
///
/// @param value The value to pretty print the bits of.
///
/// # NOTE
/// No newline character will be printed by this function.
/// See println_bits() instead.
///
/// # Example
///
/// ```c
/// print_bits(0x11);
/// // => 0b0001_0001
/// ```
void print_bits(uint8_t value) {
	printf("0b");
	for (size_t i = 0; i < UINT8_T_BITS; i++) {
		if (i > 0 && i % 4 == 0) {
			printf("_");
		}

		printf("%d", (value >> (UINT8_T_BITS - 1 - i) & 0x1));
	}
}

/// Pretty prints the bits of value. Also appends a newline at then end.
///
/// Also see print_bits().
void println_bits(uint8_t value) {
	print_bits(value);
	printf("\n");
}

/// Checks if the bits of `is` are equal to the bits in decimal notation of `should`.
/// If not `message` will be printed and the program will be exited.
///
/// @param is The value to check.
/// @param should The value which is expected in binary format (e.g. `1110101`).
/// @param message Optional message to print if the assertion fails.
/// @param file The file in which the assertion is located (filled in by the assert_led_bits macro)
/// @param func The func in which the assertion is located (filled in by the assert_led_bits macro)
/// @param line The line which the assertion is located on (filled in by the assert_led_bits macro)
///
/// # NOTE
/// The value in should is the decimal representation of a binary number (e.g. `110111`).
/// This is done, because cstd11 does not binary number representations.
///
/// # Example
/// ```c
/// assert_bits_eq(5, 1001, "Not equal");
/// ```
void assert_bits_eq(uint8_t is, uint32_t should, const char* message, const char* file, const char* func, unsigned int line) {
	size_t number_div = 1;

	for (size_t i = 0; i < UINT8_T_BITS; i++) {
		uint8_t is_bit     = (is >> i) & 0x1;
		uint8_t should_bit;
		if (i > 0) {
			should_bit = ((should / number_div) % 10);
		} else {
			should_bit = (should % 10);
		}

		if (is_bit != should_bit) {
			printf("%s:%u <%s> Assert failed: ", file, line, func);

			if (message != NULL) {
				printf("%s", message);
			} else {
				printf("Bits do not match");
			}

			printf("\n\tIs    : ");
			print_bits(is);
			printf("\n\tShould: 0b%04d_%04d\n", (should / 10000), should % 10000);

			exit(1);
		}

		number_div *= 10;
	}
}

// NOTE: Do not change the order of the test cases, they build up on each other.
/// Unit/Integration tests for the led interface (led.h / led.c).
void test_led() {
	printf("Running led interface tests\n");

	// Prepare led
	led_init();
	assert_led_bits(0, "Init");

	led_state_set(LED_STATE_ON);
	assert_led_bits(1, "State on");

	led_state_set(LED_STATE_OFF);
	assert_led_bits(0, "State off");

	led_color_set(LED_COLOR_RED | LED_COLOR_GREEN);
	assert_led_bits(110, "Colors on red|green");

	led_color_set(LED_COLOR_RED | LED_COLOR_GREEN | LED_COLOR_BLUE);
	assert_led_bits(1110, "Colors on red|green|blue");

	// Clear color
	led_color_set(0x0);
	assert_led_bits(0, "Colors off");

	led_color_red();
	assert_led_bits(10, "Colors on red");

	led_color_green();
	assert_led_bits(100, "Colors on green");

	led_color_blue();
	assert_led_bits(1000, "Colors on blue");

	led_color_set(LED_COLOR_RED | LED_COLOR_GREEN);
	assert_led_bits(110, "Colors on red|green (2)");

	led_brightness_set(LED_BRIGHTNESS_MIN);
	assert_led_bits(110, "Min brightness");

	led_brightness_set(LED_BRIGHTNESS_MAX);
	assert_led_bits(11110110, "Max brightness");

	led_brightness_set(0xa);
	assert_led_bits(10100110, "10 brightness");

	led_brightness_min();
	assert_led_bits(110, "Min brightness 2");

	led_brightness_set(0x5);
	assert_led_bits(1010110, "5 brightness");

	led_brightness_max();
	assert_led_bits(11110110, "Max brightness 2");
}

/// Converts the numeric error representation of #MsgErrorCode to a string for printing.
///
/// @param process_result The value returned by process_message().
void print_process_error(int process_result) {
	switch (process_result) {
		case 0:
			printf("Ok");
			break;
		case MSG_ERROR_CODE_EMPTY:
			printf("Empty");
			break;
		case MSG_ERROR_CODE_INVALID_OP_CODE:
			printf("Invalid op code");
			break;
		case MSG_ERROR_CODE_TRAILING_BYTES:
			printf("Trailing bytes");
			break;
		case MSG_ERROR_CODE_MISSING_PARAMETERS:
			printf("Missing parameters");
			break;
	}
}

/// Checks that the result of process_message() is as expected.
/// If not the program exists and the given `message` is printed.
///
/// @param process_result The result of the function call process_message().
/// @param expected The expected result of the function call.
/// @param message The message to be printed on failure.
/// @param file The file in which the assertion is located (filled in by the assert_msg_process macro)
/// @param func The func in which the assertion is located (filled in by the assert_msg_process macro)
/// @param line The line which the assertion is located on (filled in by the assert_msg_process macro)
void assert_msg_process_full(int process_result, int expected, const char* message, const char* file, const char* func, unsigned int line) {
	if (process_result != expected) {
		printf("%s:%u <%s> Assert failed: ", file, line, func);

		if (message != NULL) {
			printf("%s", message);
		} else {
			printf("Bits do not match");
		}

		printf("\n\tIs    : ");
		print_process_error(process_result);
		printf("\n\tShould: ");
		print_process_error(expected);
		printf("\n");
		exit(1);
	}
}

// NOTE: Do not change the order of the test cases, they build up on each other.
/// Unit/Integration tests for the message processing interface (msg.h / msg.c).
void test_msg() {
	printf("Running msg process tests\n");

	// Reset to have a known start state
	led_clear();
	assert_led_bits(0, "msg: Clear");

	// Create buffer with enough space to test everything
	uint8_t msg_buf[10];

	// Sends on
	msg_buf[0] = 0x00;
	assert_msg_process(process_message(1, msg_buf), 0, "Send on");
	assert_led_bits(1, "msg: On");

	// Sends off
	msg_buf[0] = 0x01;
	assert_msg_process(process_message(1, msg_buf), 0, "Send off");
	assert_led_bits(0, "msg: On");

	// Sends configuration
	msg_buf[0] = 0x02;
	msg_buf[1] = LED_COLOR_GREEN; // color
	msg_buf[2] = 0x5; // brightness
	assert_msg_process(process_message(3, msg_buf), 0, "Send config");
	assert_led_bits(1010100, "msg: Config");

	// Turn on & send configuration
	led_clear();
	msg_buf[0] = 0x00;
	assert_msg_process(process_message(1, msg_buf), 0, "Send on (2)");
	assert_led_bits(1, "msg: On (2)");
	msg_buf[0] = 0x02;
	msg_buf[1] = LED_COLOR_RED; // color
	msg_buf[2] = LED_BRIGHTNESS_MAX; // brightness
	assert_msg_process(process_message(3, msg_buf), 0, "Send config (2)");
	assert_led_bits(11110011, "msg: Config (2)");

	// ERROR - Invalid op code
	msg_buf[0] = 0x05;
	assert_msg_process(process_message(1, msg_buf), MSG_ERROR_CODE_INVALID_OP_CODE, "Invalid op code");

	// ERROR - Invalid op code
	msg_buf[0] = 0xff;
	assert_msg_process(process_message(1, msg_buf), MSG_ERROR_CODE_INVALID_OP_CODE, "Invalid op code (2)");

	// ERROR - Empty
	assert_msg_process(process_message(0, msg_buf), MSG_ERROR_CODE_EMPTY, "Empty");

	// ERROR - Trailing bytes (on)
	msg_buf[0] = 0x00;
	msg_buf[1] = 0x00;
	assert_msg_process(process_message(2, msg_buf), MSG_ERROR_CODE_TRAILING_BYTES, "Trailing bytes (on)");

	// ERROR - Trailing bytes (off)
	msg_buf[0] = 0x01;
	msg_buf[1] = 0x00;
	assert_msg_process(process_message(2, msg_buf), MSG_ERROR_CODE_TRAILING_BYTES, "Trailing bytes (off)");

	// ERROR - Trailing bytes (settings)
	msg_buf[0] = 0x02;
	msg_buf[1] = 0x00;
	msg_buf[2] = 0x00;
	msg_buf[3] = 0x00;
	assert_msg_process(process_message(4, msg_buf), MSG_ERROR_CODE_TRAILING_BYTES, "Trailing bytes (settings)");

	// ERROR - Missing parameters (1)
	msg_buf[0] = 0x02;
	assert_msg_process(process_message(1, msg_buf), MSG_ERROR_CODE_MISSING_PARAMETERS, "Missing parameters (settings/1)");

	// ERROR - Missing parameters (2)
	msg_buf[0] = 0x02;
	msg_buf[1] = 0x00;
	assert_msg_process(process_message(2, msg_buf), MSG_ERROR_CODE_MISSING_PARAMETERS, "Missing parameters (settings/2)");
}

#if defined STM32F767ZI
#define _LED_COUNT 3
#elif defined STM32L162
#define _LED_COUNT 1
#else
#error No valid target mc defined
#endif

int main(void) {
	printf("Led count for target mc: %d\n", _LED_COUNT);
	test_led();
	test_msg();
}
