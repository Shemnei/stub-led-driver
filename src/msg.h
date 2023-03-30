//! \file msg.h
//!
//! This file defines a callback function which can receive a message and
//! process it.
//!
//! Message definition:
//!
//! | Opcode          | Definition   | Parameter                       | Remarks                                                                                                                                                   |
//! | --------------- | ------------ | ------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
//! | `0x00`          | ON           | -                               | Turns the led on                                                                                                                                          |
//! | `0x01`          | OFF          | -                               | Turns the led off                                                                                                                                         |
//! | `0x02`          | Led Settings | Color - 8bit; Brightness - 8bit | **Color Bits**: <br/> `0` Red <br/> `1` Green <br/> `2` Blue <br/> `3:7` Reserved <br/><br/> **Brightness**: <br/> `0:3` Brightness level <br/> `4:7` Reserved |
//! | `0x03` - `0xff` | Reserved     |                                 |                                                                                                                                                           |

#ifndef _MSG_H_
#define _MSG_H_

#include <stdint.h>

/// Defines the valid op codes which can be used in the message.
typedef enum {
	/// Turns the led on.
	MSG_OP_CODE_ON           = 0x00,
	/// Turns the led off.
	MSG_OP_CODE_OFF          = 0x01,
	/// Sets the color and/or brightness of the led.
	MSG_OP_CODE_LED_SETTINGS = 0x02,
} MsgOpCode;

/// Defines the errors which might occur during the message processing.
typedef enum {
	/// The received message was empty.
	MSG_ERROR_CODE_EMPTY              =  1,
	/// The given op code is invalid / not known.
	MSG_ERROR_CODE_INVALID_OP_CODE    = 10,
	/// The message had trailing bytes which where not expected.
	MSG_ERROR_CODE_TRAILING_BYTES     = 20,
	/// The message was missing some parameters.
	MSG_ERROR_CODE_MISSING_PARAMETERS = 30,
} MsgErrorCode;

/// Processes the message in buffer and set the led accordingly.
///
/// @param bufferLength The length of the message in buffer.
/// @param buffer The buffer which holds the messages to be processed.
///
/// @return Returns `0` if the message was processed successfully, otherwise a variant of #MsgErrorCode will be returned.
int process_message(uint8_t bufferLength, uint8_t* buffer);

#endif
