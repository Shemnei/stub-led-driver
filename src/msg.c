//! \file msg.c
//! Implementation for the message process interface.
//!
//! See msg.h for the available functions and documentation.

#include <stdint.h>

#include "led.h"
#include "msg.h"

/// Processes a `0x00` / `ON` op code message.
///
/// @param len The total length of the received message.
///
/// @return Returns `0` if the message was processed successfully, otherwise a variant of MsgErrorCode will be returned.
int process_op_on(uint8_t len) {
	if (len != 1) {
		return MSG_ERROR_CODE_TRAILING_BYTES;
	}

	led_state_on();

	return 0;
}

/// Processes a `0x01` / `OFF` op code message.
///
/// @param len The total length of the received message.
///
/// @return Returns `0` if the message was processed successfully, otherwise a variant of MsgErrorCode will be returned.
int process_op_off(uint8_t len) {
	if (len != 1) {
		return MSG_ERROR_CODE_TRAILING_BYTES;
	}

	led_state_off();

	return 0;
}

/// Processes a `0x02` / `Led Settings` op code message.
///
/// @param len The total length of the received message.
/// @param buffer The complete message
///
/// @return Returns `0` if the message was processed successfully, otherwise a variant of MsgErrorCode will be returned.
int process_op_led_settings(uint8_t len, uint8_t* buffer) {
	if (len < 3) {
		return MSG_ERROR_CODE_MISSING_PARAMETERS;
	} else if (len > 3) {
		return MSG_ERROR_CODE_TRAILING_BYTES;
	}

	uint8_t color = buffer[1] & 0x7;
	uint8_t brightness = buffer[2] & 0xf;

	led_color_set(color);
	led_brightness_set(brightness);

	return 0;
}

int process_message(uint8_t bufferLength, uint8_t* buffer) {
	if (bufferLength < 1) {
		return MSG_ERROR_CODE_EMPTY;
	}

	uint8_t op_code = buffer[0];

	switch (op_code) {
		case MSG_OP_CODE_ON:
			return process_op_on(bufferLength);
		case MSG_OP_CODE_OFF:
			return process_op_off(bufferLength);
		case MSG_OP_CODE_LED_SETTINGS:
			return process_op_led_settings(bufferLength, buffer);
		default:
			return MSG_ERROR_CODE_INVALID_OP_CODE;
	}
}
