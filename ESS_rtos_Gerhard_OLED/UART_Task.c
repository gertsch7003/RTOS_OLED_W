/*
 *  ======== UART_Task.c ========
 *  Author: Michael Kramer / Matthias Wenzl / Markus Eggenberger
 */
#include <stdbool.h>
#include <stdio.h>
#include <inc/hw_memmap.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/*Board Header files */
#include <Board.h>
#include <EK_TM4C1294XL.h>

#include <ctype.h>
#include <string.h>

#include "local_inc/UART_Task.h"

static Shared_Data* _shared_Data;

/**
*
* \brief In Abhaengigkeit des von der UART
*        empfangen Zeichen den Display_Mode oder
*        die Geschwindigkeit des Lauftexts steuern.
*        Hier zugriff ohne die Semaphore, weil das hier
*        nur fuer den Testmodus ist und auch keine korrupten Daten
*        produzieren sollte.
*
* \param char der von der UART gelesen wurde
* \param shared_Data um auch hier das shared memory verwenden zu koennen
*/
static void interpret_uart_input(char input, Shared_Data* _shared_Data) {
	switch (input) {
	case '1':
		_shared_Data->speed = 2000;
		break;
	case '2':
		_shared_Data->speed = 1000;
		break;
	case '3':
		_shared_Data->speed = 300;
		break;
	case '4':
		_shared_Data->speed = 100;
		break;
	case '5':
		_shared_Data->speed = 1;
		break;
	case 'q':
		_shared_Data->Display_Mode = 0x00;
		break;
	case 'w':
		_shared_Data->Display_Mode = 0x01;
		break;
	case 'e':
		_shared_Data->Display_Mode = 0x02;
		break;
	case 'r':
		_shared_Data->Display_Mode = 0x03;
		break;
	case 't':
		_shared_Data->Display_Mode = 0x04;
		break;
	case 'a':
		_shared_Data->brightness = _shared_Data->brightness + 500 < AMBI_MAX_VALUE?_shared_Data->brightness + 500 : AMBI_MAX_VALUE;
		break;
	case 'y':
		_shared_Data->brightness = _shared_Data->brightness - 500 > 0 ?_shared_Data->brightness - 500 : 0;
		break;
	case 's':
		_shared_Data->distance = _shared_Data->distance + 10 < PROXI_MAX_VALUE?_shared_Data->distance + 10 : PROXI_MAX_VALUE;
		break;
	case 'x':
		_shared_Data->distance = _shared_Data->distance - 10 > 0 ?_shared_Data->distance - 10 : 0;
		break;
	default:
		break;
	}
}


/**
*
* \brief callback Funktion fuer das Lesen von der UART
*
* \param UART_Handle
* \param char* der von der UART gelesen wurde
* \param count: Anzahl der gelesenen Zeichen
*/
static void doRead(UART_Handle handle, char *buf, size_t count) {
	char c = buf[0];
	interpret_uart_input(c, _shared_Data);
}

/**
*
* \brief Task-Funktion: Schreiben der Sensorwerte
*        auf die UART. Empfangen einzelner Zeichen
*        zum steuern von Display_Mode und Lauftextgeschwindigkeit
*
* \param arg0 shared_Data um auch hier das shared memory verwenden zu k�nnen
* \param arg1 wird nicht verwendet
*
*/

static void UARTFxn(UArg arg0, UArg arg1) {
	char input;
	UART_Handle uart;
	UART_Params uartParams;
	_shared_Data = (Shared_Data*) arg0;
	const char echoPrompt[] = "\f1 bis 5 ... Geschwindigkeit der Laufschrift\r\nq,w,e,r,t... Display_Mode\r\na,y ... Helligkeit\r\ns,x ... Entfernung\r\n";

	/* Create a UART with data processing off. */
	UART_Params_init(&uartParams);
	uartParams.writeDataMode = UART_DATA_BINARY;
	uartParams.readDataMode = UART_DATA_BINARY;
	uartParams.readReturnMode = UART_RETURN_FULL;
	uartParams.readEcho = UART_ECHO_OFF;
	uartParams.baudRate = 9600;
	uartParams.readMode = UART_MODE_CALLBACK;
	uartParams.readCallback = (UART_Callback) doRead;
	uart = UART_open(Board_UART0, &uartParams);

	if (uart == NULL) {
		System_abort("Error opening the UART");
	}

	UART_write(uart, echoPrompt, sizeof(echoPrompt));

	while (1) {
		// write
		Event_pend(_shared_Data->UART_Event, Event_Id_02, Event_Id_02, 2);
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);

		Semaphore_pend(_shared_Data->sem, BIOS_WAIT_FOREVER);
		char output[OUTPUT_SIZE];
		snprintf(output, OUTPUT_SIZE, "Helligkeit: %5d Entf. Sensor: %5d Entf.ber: %4d\r\n", _shared_Data->brightness, _shared_Data->distance_read, _shared_Data->distance);
		Semaphore_post(_shared_Data->sem);

		if (_shared_Data->uart_out_OFF == 0x00) {
		UART_write(uart, output, strlen(output));
		}
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x00);

		// read
		UART_read(uart, &input, 1);
	    input = 0;
		Task_sleep(1);
	}
}

/**
*
* \brief UART und Task initialisieren
*
* \param shared_Data um auch hier das shared memory verwenden zu k�nnen
*
* \return 0 wenn alles erfolgreich war
*/
int setup_UART_Task(Shared_Data *shared_Data) {
	Task_Params taskUARTParams;
	Task_Handle taskUART;
	Error_Block eb;

	/* Enable and configure the peripherals used by the UART0 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UART_init();

	//Setup PortN LED1 activity signaling
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

	Error_init(&eb);
	Task_Params_init(&taskUARTParams);
	taskUARTParams.stackSize = 2048;/*stack in bytes*/
	taskUARTParams.priority = 15;/*15 is default 15 is highest priority -> see RTOS configuration*/
	taskUARTParams.arg0 = (UArg) shared_Data;
	taskUART = Task_create((Task_FuncPtr) UARTFxn, &taskUARTParams, &eb);
	if (taskUART == NULL) {
		System_abort("TaskUART create failed");
	}

	return (0);
}
