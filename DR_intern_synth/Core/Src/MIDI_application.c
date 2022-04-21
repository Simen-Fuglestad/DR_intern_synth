/*
 * MIDI_application.c
 *
 *  First created on: 6 dec. 2014
 *      Author: Xavier Halgand
 */

/* Includes ------------------------------------------------------------------*/

//#include "main.h"
#include "MIDI_application.h"
#include "usb_host.h"

/* Private define ------------------------------------------------------------*/

#define RX_BUFF_SIZE 64 /* USB MIDI buffer : max received data 64 bytes */

uint8_t MIDI_RX_Buffer[RX_BUFF_SIZE]; // MIDI reception buffer
uint8_t MIDI_key_pressed;

/* Private function prototypes -----------------------------------------------*/
void ProcessReceivedMidiData(void);

/*-----------------------------------------------------------------------------*/
/**
 * @brief  Main routine for MIDI application, looped in main.c
 * @param  None
 * @retval none
 */
void MIDI_Application(void)
{
	if (Appli_state == MIDI_APPLICATION_READY)
	{
		USBH_MIDI_Receive(&hUsbHostFS, MIDI_RX_Buffer, RX_BUFF_SIZE); // just once at the beginning, start the first reception
		Appli_state = APPLICATION_RUNNING;
	}
	if (Appli_state == APPLICATION_RUNNING)
	{
		//....pffff......grrrrr......
	}

	if (Appli_state == MIDI_APPLICATION_DISCONNECT)
	{
		Appli_state = MIDI_APPLICATION_IDLE;
		USBH_MIDI_Stop(&hUsbHostFS);
	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief  MIDI data receive callback.
 * @param  phost: Host handle
 * @retval None
 */
void USBH_MIDI_ReceiveCallback(USBH_HandleTypeDef *phost)
{
//	ProcessReceivedMidiData();
	if (MIDI_RX_Buffer[0] == 0x09) {
		MIDI_key_pressed = MIDI_RX_Buffer[2];
	} else if (MIDI_RX_Buffer[0] == 0x08) {
		MIDI_key_pressed = 0;
	}
	USBH_MIDI_Receive(&hUsbHostFS, MIDI_RX_Buffer, RX_BUFF_SIZE); // start a new reception
}

uint8_t MIDI_get_key_pressed() {
	return MIDI_key_pressed;
}

void ProcessReceivedMidiData() {

}
