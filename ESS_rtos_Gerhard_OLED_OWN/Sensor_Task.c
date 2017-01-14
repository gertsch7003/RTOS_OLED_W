#include "local_inc/Sensor_Task.h"

#include <stdbool.h>
#include <inc/hw_memmap.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

#include <inc/hw_ints.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/I2C.h>
#include <driverlib/i2c.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <ti/sysbios/hal/Hwi.h>

/*Board Header files */
#include <Board.h>
#include <EK_TM4C1294XL.h>

#include <ctype.h>
#include <string.h>

static Event_Handle myEvent;
static uint8_t readBuffer[4];
static uint8_t writeBuffer[4];
static I2C_Transaction i2c;
static I2C_Handle handle;


/**
*
* \brief Schreiben eines Byte auf den I2C Bus
*
* \param[in] registerAdress Adresse des Registers in das geschrieben wird
* \param[in] data Daten, die in das Register geschrieben werden
*/
static void I2C_WriteByte(unsigned char registerAdress, unsigned char data) {
	i2c.readCount = 0;
	i2c.writeCount = 2;
	writeBuffer[0] = registerAdress;
	writeBuffer[1] = data;

	if (!I2C_transfer(handle, &i2c)) {
		System_abort("Bad I2C transfer!");
	}
}

/**
*
* \brief Lesen eines Byte vom I2C Bus
*
* \param[in] registerAdress Adresse des Registers aus dem gelesen wird
*/
static void I2C_ReadByte(unsigned char registerAdress) {
	i2c.readCount = 1;
	i2c.writeCount = 1;
	writeBuffer[0] = registerAdress;

	if (!I2C_transfer(handle, &i2c)) {
		System_abort("Bad I2C transfer!");
	}
}


// Kalibirierungstabelle fuer Umrechnung auf mm
static uint16_t calibration[12][3] = {{1960,1970,110},
	     							  {1970,1980,100},
									  {1980,1990,90},
									  {1990,2014,80},
									  {2014,2040,70},
									  {2040,2070,60},
									  {2070,2200,50},
									  {2200,2320,40},
									  {2320,2600,30},
									  {2600,3200,20},
									  {3200,7500,10},
									  {7500,65535,0}};


/**
*
* \brief Konvertieren des Wertes vom Sensor nach mm
*
* \param[in] value vom Sensor generierter Wert
*
* \return Wert in mm
*/
static uint16_t convert_to_mm(uint16_t value) {
	uint16_t res = PROXI_UNENDLICH;
	uint8_t i = 0;
	for (i = 0; i < 12; i++) {
		if (value > calibration[i][0] && value <= calibration[i][1]) {
			res = (((calibration[i][1] - value) *10)  / (calibration[i][1] - calibration[i][0])) + calibration[i][2];
			break;
		}
	}
	return res;
}


/**
*
* \brief Auslesen des Sensors
*
* \param[in] arg0 shared memory, in dem die Sensorwerte abgelegt werden
*/
static void SensorFxn(UArg arg0) {
	Shared_Data* _shared_Data = (Shared_Data*) arg0;

	i2c.slaveAddress = SENSOR; /* Adresse des Sensors */
	i2c.readCount = 0;
	memset(readBuffer, 0, 4);
	i2c.readBuf = (uint8_t*) &readBuffer[0];
	i2c.writeBuf = writeBuffer;

	I2C_WriteByte(REGISTER_COMMAND, 0x07); /* Command-Register zum Aktivieren, 0x07 startet
											  Ambilight, Proximity und Self timed measurement */
	I2C_WriteByte(REGISTER_PROX_CURRENT,0x0A); /* setzt die Spannung des Sensors  */
	I2C_WriteByte(REGISTER_INTERRUPT_CONTROL, 0x0C); /* Aktiviert Interrupterzeugung fuer Proximity und Ambilight */

	uint16_t ambi_value = 0x00;
	uint16_t proxi_value = 0x00;
	while (TRUE) {
		Event_pend(myEvent, Event_Id_00, Event_Id_00, BIOS_WAIT_FOREVER);

		I2C_ReadByte(REGISTER_INTERRUPT_STATUS); /* auslesen ob Interrupt erzeugt wurde */
		uint8_t int_status_reg = readBuffer[0];
		// falls ein Ambilight Interrupt erzeugt wurde...
		if((int_status_reg & 0x04) == 0x04) {
			I2C_ReadByte(REGISTER_AMBI_VALUE_LO); /* Auslesen der Low Bits des Ambilight*/
			ambi_value = readBuffer[0];
			I2C_ReadByte(REGISTER_AMBI_VALUE_HI); /* Auslesen der High Bits des Ambilight*/
			ambi_value += (readBuffer[0] << 8); /* Shift der High Bits und Addition der Low Bits */
			ambi_value /=4; /* Division durch 4, da eine Aufloesung von 0.25 */
		}
		// falls ein Proximity Interrupt erzeugt wurde...
		if ((int_status_reg & 0x08) == 0x08) {
			I2C_ReadByte(REGISTER_PROX_VALUE_LO); /* Auslesen der Low Bits des Proximity*/
			proxi_value = readBuffer[0];
			I2C_ReadByte(REGISTER_PROX_VALUE_HI); /* Auslesen der High Bits des Proximity*/
			proxi_value += (readBuffer[0] << 8); /* Shift der High Bits und Addition der Low Bits */
		}
		I2C_WriteByte(REGISTER_INTERRUPT_STATUS, 0x0C); /* Status Bits zur�cksetzen indem eine 1 hineingeschrieben wird */

		Semaphore_pend(_shared_Data->sem, BIOS_WAIT_FOREVER);
		_shared_Data->brightness    = ambi_value;
		_shared_Data->distance_read = proxi_value;
		_shared_Data->distance      = convert_to_mm(proxi_value);
		Semaphore_post(_shared_Data->sem);

		// Events erzeugen damit die Lesenden nicht st�ndig lesen m�ssen sondern nur wenn sich etwas ge�ndert hat
		Event_post(_shared_Data->UART_Event, Event_Id_02);
		Event_post(_shared_Data->Output_Event, Event_Id_03);

		Task_sleep(1);
	}
}

/**
*
* \brief Initialisieren eines Event das f�r
*        das Anstossen des Sensor auslesen verwendet wird
*
*/
static void create_event() {
	Error_Block eb;
	Error_init(&eb);
	myEvent = Event_create(NULL, &eb);
	if (myEvent == NULL) {
		System_abort("Event create failed");
	}
}

/**
*
* \brief Funktion die beim Ausl�sen des Interrupt
*        aufgerufen wird
*
*/
static void hwiFunc(xdc_UArg x) {
	Event_post(myEvent, Event_Id_00);
	GPIOIntClear(GPIO_PORTC_BASE,GPIO_PIN_6);
	Hwi_clearInterrupt(INT_GPIOC);
}

/**
*
* \brief Initalisieren eines Hardware Interrupt
*        f�r die Interruptsteuerung durch den Sensor
*
*/
static void create_interrupt() {
	Hwi_Handle hwi0;
	Hwi_Params hwiParams;
	Error_Block eb;
	Error_init(&eb);
	Hwi_Params_init(&hwiParams);
	hwiParams.arg = 2;
	hwiParams.enableInt = TRUE;
	hwi0 = Hwi_create(INT_GPIOC, hwiFunc, &hwiParams, &eb);
	if (hwi0 == NULL) {
		System_abort("Hwi create failed");
	}
}

/**
*
* \brief Initalisieren der I2C Schnittstelle
*        und Konfigurieren eines IO-Pin f�r die Interruptsteuerung
*
*/
static void init_i2c() {
	// I2C
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C7);

	GPIOPinConfigure(GPIO_PD0_I2C7SCL);
    GPIOPinConfigure(GPIO_PD1_I2C7SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_1);

    I2CMasterInitExpClk(I2C7_BASE, 120 * 1000 * 1000, false); /* 120 MHz */
    I2CMasterEnable(I2C7_BASE);

	I2C_Params      i2cparams;
	I2C_Params_init(&i2cparams);
	i2cparams.bitRate = I2C_400kHz; /*in case this is too fast use I2C_400kHz*/
	i2cparams.transferMode = I2C_MODE_BLOCKING; /*important if you call I2C_transfer in Task context*/

	//handle = I2C_open(EK_TM4C1294XL_I2C7, &i2cparams);
	handle = I2C_open(EK_TM4C1294XL_I2C8, &i2cparams);

	if (handle == NULL) {
		System_abort("I2C was not opened");
	}

	// Interrupt GPIO Pin
    uint32_t ui32Strength;
    uint32_t ui32PinType;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_6);
	GPIOPadConfigGet(GPIO_PORTC_BASE, GPIO_PIN_6, &ui32Strength, &ui32PinType);
	GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_6,ui32Strength,GPIO_PIN_TYPE_STD_WPU);

	GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_FALLING_EDGE);
	GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_6);
}


/**
*
* \brief Initalisieren eines Task und aufrufen
*        der anderen Initialisierungsfunktionen
*        in diesem File. Um einen Helligkeits- und Entfernungssenor auszulesen.
*
* \param[in] shared_Data Zeiger auf das shared memory (struct)
*
* \return 0 wenn alles erfolgreich war
*/
int setup_Sensor_Task(Shared_Data *shared_Data) {
	Task_Params taskParams;
	Task_Handle task;
	Error_Block eb;

	create_event();
	create_interrupt();
	init_i2c();

	/* Create sensor task with priority 15*/
	Error_init(&eb);
	Task_Params_init(&taskParams);
	taskParams.stackSize = 1024; /*stack in bytes*/
	taskParams.priority = 15; /*15 is default 15 is highest priority -> see RTOS configuration*/
	taskParams.arg0 = (UArg) shared_Data;
	task = Task_create((Task_FuncPtr) SensorFxn, &taskParams, &eb);
	if (task == NULL) {
		System_abort("TaskLed create failed");
	}

	return (0);
}
