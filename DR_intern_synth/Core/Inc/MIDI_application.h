/*
 * MIDI_application.h
 *
 *  First created on: 6 dec. 2014
 *      Author: Xavier Halgand
 */

#ifndef MIDI_APPLICATION_H_
#define MIDI_APPLICATION_H_

/* Includes ------------------------------------------------------------------*/

#include "stdio.h"
#include "usbh_core.h"
#include "usbh_MIDI.h"

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/*------------------------------------------------------------------------------*/
typedef enum
{
	MIDI_APPLICATION_IDLE = 0,
	MIDI_APPLICATION_START,
	MIDI_APPLICATION_READY,
	APPLICATION_RUNNING,
	MIDI_APPLICATION_DISCONNECT
}
MIDI_ApplicationTypeDef;

/*------------------------------------------------------------------------------*/

extern USBH_HandleTypeDef hUsbHostFS;
extern MIDI_ApplicationTypeDef Appli_state;
extern int8_t currentNote;
extern int8_t velocity;

/* Exported functions ------------------------------------------------------- */
void MIDI_Application(void);

uint8_t MIDI_get_key_pressed(void);

/*------------------------------------------------------------------------------*/
#endif /* MIDI_APPLICATION_H_ */
