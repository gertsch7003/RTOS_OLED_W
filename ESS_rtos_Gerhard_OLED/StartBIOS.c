/*
 * CCSv6 project using TI-RTOS
 *
 */

/*
 *  ======== StartBIOS.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/hal/Timer.h>
#include <ti/sysbios/knl/Event.h>

/* TI-RTOS Header files */
#include <ti/drivers/UART.h>


/*Board Header files */
#include <Board.h>
#include <EK_TM4C1294XL.h>

/*application header files*/
#include <ctype.h>
#include <string.h>


/*SPI - HEADER -- FOR GERHARD - DISPLAY - Boosterpack 1*/
#include <ti/drivers/SPI.h>
//#include "local_inc/OLED_W.h"

#include "local_inc/UART_Task.h"
#include "local_inc/Common.h"
#include "local_inc/Output_Task.h"
#include "local_inc/Sensor_Task.h"
#include "local_inc/Button_Task.h"



static Shared_Data shared_Data;

/**
*
* \brief Die Werte im gemeinsam genutzen Speicher
*        sinnvoll initialisiern
*
*/
static void init_sharedData() {
	Error_Block eb;

	Error_init(&eb);
	shared_Data.sem = Semaphore_create(1, NULL, &eb);
	if (shared_Data.sem == NULL) {
		System_abort("Semaphore create failed");
	}
	Error_init(&eb);
	shared_Data.UART_Event = Event_create(NULL, &eb);
	if (shared_Data.UART_Event == NULL) {
		System_abort("Event create failed");
	}
	Error_init(&eb);
	shared_Data.Output_Event = Event_create(NULL, &eb);
	if (shared_Data.UART_Event == NULL) {
		System_abort("Event create failed");
	}
	shared_Data.Display_Mode = 0x02;
	shared_Data.uart_out_OFF = 0x01;
	shared_Data.speed = 300;
}


/**
*
* \brief main Funktion des Projekts. Aufrufen der Intialisierungsfunktionen der verschiedenen Files.
*
*/
int main(void) {
	uint32_t ui32SysClock = Board_initGeneral(120 * 1000 * 1000);
	(void) ui32SysClock;
	Board_initGPIO();

	init_sharedData();
	setup_Button_Task(&shared_Data);
	setup_Sensor_Task(&shared_Data);
	setup_Output_Task(&shared_Data);
	setup_UART_Task(&shared_Data);

	System_printf("Start BIOS\n");
	System_flush();

	/* Start BIOS */
	BIOS_start();
}
