#include "local_inc/Output_Task.h"

#include <stdio.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <driverlib/gpio.h>
#include <driverlib/ssi.h>

/*Board Header files */
#include <Board.h>

#include <ctype.h>
#include <string.h>

#include "local_inc/EK_TM4C1294XL.h"
#include "local_inc/OLED_W.h"

#define DISPLAY_MEMORY_SIZE 576

static uint8_t display[DISPLAY_MEMORY_SIZE] = {0};  //Speicherbereich zum Vorbereiten der Anzeige
static Event_Handle myEvent;
static Clock_Handle clock;


/**
*
* \brief Senden eines Kommando an das OLED W Display
*
* \param character der als Kommando an das Display geschickt wird
*/
static void OLED_M_command(unsigned char temp) {
	GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_5, 0x00);
	GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_7, 0x00);
	SSIDataPut(SSI3_BASE, temp);
	while (SSIBusy(SSI3_BASE));
	GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_5, GPIO_PIN_5);
}

/**
*
* \brief Senden von Daten an das OLED W Display
*
* \param character der als Daten an das Display geschickt wird
*/
static void OLED_M_data(unsigned char temp) {
	GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_5, 0x00);
	GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_7, GPIO_PIN_7);
	SSIDataPut(SSI3_BASE, temp);
	while (SSIBusy(SSI3_BASE));
	GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_5, GPIO_PIN_5);
}

/**
*
* \brief Intialisierung des OLED Click
*
*/
static void OLED_M_Init() {
	GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_4, 0x00);
	Task_sleep(1000);
	GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_4, GPIO_PIN_4);
	Task_sleep(1000);

	OLED_M_command(SSD1306_DISPLAYOFF);             //0xAE  Set OLED Display Off
	OLED_M_command(SSD1306_SETDISPLAYCLOCKDIV);     //0xD5  Set Display Clock Divide Ratio/Oscillator Frequency
	OLED_M_command(0x80);
	OLED_M_command(SSD1306_SETMULTIPLEX);           //0xA8  Set Multiplex Ratio
	OLED_M_command(0x27);
	OLED_M_command(SSD1306_SETDISPLAYOFFSET);       //0xD3  Set Display Offset
	OLED_M_command(0x00);
	OLED_M_command(SSD1306_SETSTARTLINE);           //0x40  Set Display Start Line
	OLED_M_command(SSD1306_CHARGEPUMP);             //0x8D  Set Charge Pump
	OLED_M_command(0x14);                           //0x14  Enable Charge Pump
	OLED_M_command(SSD1306_COMSCANDEC);             //0xC8  Set COM Output Scan Direction
	OLED_M_command(SSD1306_SETCOMPINS);             //0xDA  Set COM Pins Hardware Configuration
	OLED_M_command(0x12);
	OLED_M_command(SSD1306_SETCONTRAST);            //0x81   Set Contrast Control
	OLED_M_command(0xAF);
	OLED_M_command(SSD1306_SETPRECHARGE);           //0xD9   Set Pre-Charge Period
	OLED_M_command(0x25);
	OLED_M_command(SSD1306_SETVCOMDETECT);          //0xDB   Set VCOMH Deselect Level
	OLED_M_command(0x20);
	OLED_M_command(SSD1306_DISPLAYALLON_RESUME);    //0xA4   Set Entire Display On/Off
	OLED_M_command(SSD1306_NORMALDISPLAY);          //0xA6   Set Normal/Inverse Display
	OLED_M_command(SSD1306_DISPLAYON);              //0xAF   Set OLED Display On
}


/**
*
* \brief Set page adress for Page Addressing Mode
*
* \param add: Gewünschte Addresse
*
*/
static void Set_Page_Address(unsigned char add) {
	unsigned char res = 0xb0 | add;
	OLED_M_command(res);
}

/**
*
* \brief Inhalt der globalen Variable display
*        an das OLED Click schicken
*
* \param mem: Speicherbereich der in den Speicher des Display geschickt werden soll
* \param direction: von links nach rechts oder umgekehrt in den Speichern des Display schreiben
*                   Verwendung: das Bild aus dem Beispielcode enthält die Bytes in einer anderen Reihenfolge
*                               als der Text-Speicherbuffer(Globale variable: display)
*/
static void Display_Memory(unsigned char mem[], uint8_t direction) {
	unsigned char i = 0;
	unsigned char j = 0;
	for (i = 0; i < 0x05; i++) {
		Set_Page_Address(i);
		OLED_M_command(0x10);
		OLED_M_command(0x40);
		if (direction == 0x00) { // von links nach rechts oder umgekehrt ins Display schreiben
			for (j = 0x60; j > 0x00; j--) {
				OLED_M_data(mem[i * 0x60 + j - 0x01]);
			}
		} else {
			for (j = 0x00; j < 0x60; j++) {
				OLED_M_data(mem[i * 0x60 + j]);
			}
		}
	}
}

/**
*
* \brief Inhalt der globalen Variable display um angegebene
*        Anzahl von Pixel nach oben verschieben
*
* \param offset: Anzahl der Bits/Pixel um die verschoben werden soll
*/
static void shift_display_up(int offset) {
	unsigned char i = 0;
	int j = 0x1E0;
	uint64_t temp_register = 0;
	for (i = 0; i < 0x60; i++) {
		temp_register = display[i + 0x1E0];
		for (j = 0x180; j >= 0x00; j -= 0x60) {
			temp_register <<= 8;
			temp_register += display[i + j];
		}

		temp_register >>= offset;
		display[i + 0x00] = temp_register % 0x100;
		for (j = 0x60; j <= 0x1E0; j += 0x60) {
			temp_register >>= 8;
			display[i + j] = temp_register % 0x100;
		}
	}
}

/**
*
* \brief Inhalt der globalen Variable display um ein Pixel
*        nach links verschieben
*
*/
static void shift_display_left() {
	unsigned char i = 0;
	unsigned char j = 0;
	for (i = 0; i < 0x05; i++) {
		for (j = 0x01; j <= 0x60; j++) {
			display[j + (i * 0x60) - 0x01] = display[j + (i * 0x60)];
		}
		display[(i * 0x60)] = 0x00;
	}
}


/**
*
* \brief Umsetzungsfunktion um aus der ASCII-Zeichennummer
*        die richtige Stelle in der Pixelschriftbutmap zu kommen
*
* \param c: umzusetzendes ASCII Zeichen
*/
static int number_in_char_table(char c) {
	if (c >= ' ' && c <= '$') {
		return c - 32;
	} else if (c >= '(' && c <= 'Z') {
		return c - 40 + 5;
	} else if (c >= 'a' && c <= 'z') {
		return c - 96 + 55;
	}
	return 0;
}



/**
*
* \brief Eintragen des Bitmusters eines Textes in
*        die globale Variable display
*
* \param text: Text der in den Anzeige-Speicherbuffer eingetragen werden soll
* \param char_display_size: Anzahl der Zeichen die am Display dargestellt werden sollen.
* \param offset: Ab welchem Zeichen der Text dargestellt werden soll
*/
static void text_to_display(char* text, int char_display_size, int offset) {
	unsigned char i = 0; // Zeilen
	unsigned char j = 0; // Spalten
	unsigned char k = 0; // Bytes eines Buchstaben
	int char_in_text = offset;
	memset(&display, 0, sizeof(uint8_t) * DISPLAY_MEMORY_SIZE);

	for (i = 0; i < 0x06; i++) { // 6
		for (j = 0; j < 0x10; j++) {  // 16
			if (text[char_in_text] == '\n') {
				char_in_text++;
			}
			unsigned char *source = &chars[number_in_char_table(
					char_in_text <= strlen(text) ? text[char_in_text] : ' ')
					* 0x06];
			for (k = 0; k < 0x06; k++) {  //6
				display[i * 0x60 + j * 0x06 + k] = source[k];
			}

			char_in_text++;
			if ((char_in_text - offset) >= char_display_size) {
				return;
			}
		}
	}
}

/**
*
* \brief Wird bei Ablauf des Clock-Task aufgerufen und löst ein Event aus
*
*/
static void ClockFxn() {
	Event_post(myEvent, Event_Id_01);
}

/**
*
* \brief Initalisieren eines Clock-Task der als Taktgeber
*        für alle Display_Modes verwendet wird die nicht
*        mit den Sensoren arbeiten. (Bei den Display_Modes mit
*        Sensor gibt dieser den Takt vor)
*
*/
static void create_clock_event() {
	Clock_Params clockParams;
	Error_Block eb;
	Error_init(&eb);
	Clock_Params_init(&clockParams);
	clockParams.period = 300;
	clockParams.startFlag = TRUE;
	clock = Clock_create((Clock_FuncPtr) ClockFxn, 4, &clockParams, &eb);
	if (clock == NULL) {
		System_abort("Task create failed");
	}

	Error_init(&eb);
	myEvent = Event_create(NULL, &eb);
	if (myEvent == NULL) {
		System_abort("Event create failed");
	}
}

/**
*
* \brief Funktion die anhand des gewählten Display_Mode
*   eines der folgenden Dinge anzeigt:
*     * Alle als Bitmuster gespeicherten Zeichen
*     * Lauftext der Star Wars Episode 1
*     * Sensorwerte als Text
*     * Bild das als Bitmuster vorliegt
*     * Sensorwerte als Diagramm
*
*     \param arg0 shared_Data um auch hier das shared memory verwenden zu können
*
*/
static void OutputFxn(UArg arg0) {
	Shared_Data* _shared_Data = (Shared_Data*) arg0;
	uint32_t offset_text = 0x00;
	uint32_t shift_count = 0x00;
	OLED_M_Init();

	while (TRUE) {
		if (Clock_getPeriod(clock) != _shared_Data->speed) {
			Clock_setPeriod(clock,_shared_Data->speed);
		}


		if (_shared_Data->Display_Mode == 0x00) { // zeige alle möglichen Zeichen
			Event_pend(myEvent, Event_Id_01, Event_Id_01, BIOS_WAIT_FOREVER);
			Display_Memory(chars, 0x00);
			shift_count = 0x00;
		}
		else if (_shared_Data->Display_Mode == 0x01) { // Lauftext
			Event_pend(myEvent, Event_Id_01, Event_Id_01, BIOS_WAIT_FOREVER);
			if (shift_count == 0x00) {
				text_to_display(LAUFTEXT, 16 * 7, offset_text);
			}

			shift_count = (shift_count + 0x01);
			if (shift_count >= 0x08) {
				shift_count = 0x00;
				offset_text = (offset_text + 0x10) > strlen(LAUFTEXT) ? 0x00 : offset_text + 0x10;
			}
			shift_display_up(0x01);
			Display_Memory(display, 0x00);
		} else if (_shared_Data->Display_Mode == 0x02) { //Sensorwerte
			Event_pend(_shared_Data->Output_Event, Event_Id_03, Event_Id_03, 5000);
		    Semaphore_pend(_shared_Data->sem, BIOS_WAIT_FOREVER);
		    char output[OUTPUT_SIZE];
			sprintf(output, "Helligkeit:     %6d lx       Entfernung:     %6d mm       Sensor:%6d", _shared_Data->brightness, _shared_Data->distance, _shared_Data->distance_read);
		    Semaphore_post(_shared_Data->sem);
			text_to_display(output, 16 * 7, 0x00);
		    Display_Memory(display, 0x00);
		}
		else if (_shared_Data->Display_Mode == 0x03) { // Bild anzeigen
			Event_pend(myEvent, Event_Id_01, Event_Id_01, BIOS_WAIT_FOREVER);
			Display_Memory(pic, 0x01);
			memset(&display, 0, sizeof(uint8_t) * DISPLAY_MEMORY_SIZE);
		} else { //Diagramm
			Event_pend(_shared_Data->Output_Event, Event_Id_03, Event_Id_03, 2000);
			Semaphore_pend(_shared_Data->sem, BIOS_WAIT_FOREVER);

			int pos1 = SSD1306_LCDHEIGHT;
            if(_shared_Data->distance != PROXI_UNENDLICH) {
			  pos1 = (_shared_Data->distance * SSD1306_LCDHEIGHT) / PROXI_MAX_VALUE;
            }
			uint64_t temp_register1 = 0x01;
			temp_register1 <<= 40;
			temp_register1 >>= pos1;

			int pos2 = (_shared_Data->brightness * SSD1306_LCDHEIGHT) / AMBI_MAX_VALUE;
			uint64_t temp_register2 = 0x01;
			temp_register2 <<= 40;
			temp_register2 >>= pos2;

			temp_register1 |= temp_register2;

			display[0x60] = temp_register1 % 0x100;
			temp_register1 >>= 8;
			display[0xC0] = temp_register1 % 0x100;
			temp_register1 >>= 8;
			display[0x120] = temp_register1 % 0x100;
			temp_register1 >>= 8;
			display[0x180] = temp_register1 % 0x100;
			temp_register1 >>= 8;
			display[0x1E0] = temp_register1 % 0x100;

			shift_display_left();
			Semaphore_post(_shared_Data->sem);
			Display_Memory(display, 0x00);
		}
		Task_sleep(2);
	}
}

/**
*
* \brief Initalisieren der SPI-Schnittstelle
*
*/
static void initSPI() {
	/* SSI3 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);

	GPIOPinConfigure(GPIO_PQ0_SSI3CLK);
	//GPIOPinConfigure(GPIO_PQ1_SSI3FSS);
	GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0);
	GPIOPinConfigure(GPIO_PQ3_SSI3XDAT1);
	//SSIAdvModeSet(SSI3_BASE,SSI_ADV_MODE_WRITE);

	GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_0 | // GPIO_PIN_1 |
			GPIO_PIN_2 | GPIO_PIN_3);

	// D/C
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_7);

	// CS / RST
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
	GPIOPinTypeGPIOOutput(GPIO_PORTP_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_5, GPIO_PIN_5);

	SSIClockSourceSet(SSI3_BASE, SSI_CLOCK_SYSTEM);
	SSIConfigSetExpClk(SSI3_BASE, 120 * 1000 * 1000, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 8);
	SSIEnable(SSI3_BASE);
}

/**
*
* \brief Initalisieren eines Task und aufrufen
*        der anderen Initialisierungsfunktionen
*        in diesem File. Um ein OLED W Display anzusprechen.
*
* \param shared_Data um auch hier das shared memory verwenden zu können
*
* \return 0 wenn alles erfolgreich war
*/
int setup_Output_Task(Shared_Data *shared_Data) {
	Task_Params taskParams;
	Task_Handle task;
	Error_Block eb;

	create_clock_event();

	Error_init(&eb);
	Task_Params_init(&taskParams);
	taskParams.stackSize = 1024; /*stack in bytes*/
	taskParams.priority = 15; /*15 is default 15 is highest priority -> see RTOS configuration*/
	taskParams.arg0 = (UArg) shared_Data;
	task = Task_create((Task_FuncPtr) OutputFxn, &taskParams, &eb);
	if (task == NULL) {
		System_abort("Output Task create failed");
	}
	initSPI();

	return (0);
}
