#include "local_inc/Button_Task.h"

#include <Board.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/knl/Clock.h>

#include <ti/drivers/GPIO.h>

static Shared_Data *shared_Data;

static int button_enabled = 0x00;

/**
*
* \brief ISR für Button 1: Umschalten des Display_Mode
*
*/
static void Button1_ISR(void) {
	if (button_enabled == 0x01) {
		shared_Data->Display_Mode = (shared_Data->Display_Mode + 0x01) % 0x05;
		button_enabled = 0x00;
	}
}

/**
*
* \brief ISR für Button 2: Ausschalten der Sensor-Messungen
*
*/
static void Button2_ISR(void) {
	if (button_enabled == 0x01) {
		shared_Data->uart_out_OFF = shared_Data->uart_out_OFF ^ 0x01;
		button_enabled = 0x00;
	}
}

/**
*
* \brief nach Ablauf des ClockTask die Buttons wieder einschalten
*
*/
static void ClockFxn1() {
	button_enabled = 0x01;
}

/**
*
* \brief Clock-Task für Tasten entprellen,
*        nur einmal innerhalb einer Sekunde
*        auf eine Taste reagieren
*
*/
static void create_clock() {
	Clock_Params clockParams;
	Clock_Handle clock;
	Error_Block eb;
	Error_init(&eb);
	Clock_Params_init(&clockParams);
	clockParams.period = 1000;
	clockParams.startFlag = TRUE;
	clock = Clock_create((Clock_FuncPtr) ClockFxn1, 4, &clockParams, &eb);
	if (clock == NULL) {
		System_abort("Task create failed");
	}
}

/**
*
* \brief Initalisierung für die beiden Button
*
* \param Shared_Data: shared memory in dem in Abhängigkeit der gedruckten Buttons
*        shared_Data->Display_Mode und shared_Data->Sensor_OFF gesetzt werden
*
* \return 0 wenn alles erfolgreich war
*/
int setup_Button_Task(Shared_Data *_shared_Data) {
	shared_Data = _shared_Data;
	create_clock();
	GPIO_setCallback(Board_BUTTON0, Button1_ISR);
	GPIO_setCallback(Board_BUTTON1, Button2_ISR);
	GPIO_enableInt(Board_BUTTON0);
	GPIO_enableInt(Board_BUTTON1);
	return (0);
}
