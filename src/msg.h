#ifndef _MSG_H_
#define _MSG_H_

#include <stdint.h>

typedef enum {
	MSG_OP_CODE_ON           = 0x00,
	MSG_OP_CODE_OFF          = 0x01,
	MSG_OP_CODE_LED_SETTINGS = 0x02,
} MsgOpCode;

typedef enum {
	MSG_ERROR_CODE_EMPTY              =  1,
	MSG_ERROR_CODE_INVALID_OP_CODE    = 10,
	MSG_ERROR_CODE_TRAILING_BYTES     = 20,
	MSG_ERROR_CODE_MISSING_PARAMETERS = 30,
} MsgErrorCode;

int process_message(uint8_t bufferLength, uint8_t* buffer);

#endif
