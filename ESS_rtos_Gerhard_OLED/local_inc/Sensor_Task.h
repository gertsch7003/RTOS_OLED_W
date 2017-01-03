

#ifndef PROJEKT_LOCAL_INC_SENSOR_TASK_H_
#define PROJEKT_LOCAL_INC_SENSOR_TASK_H_

#include "Common.h"

#define SENSOR_PERIOD 100

/* Device addresses */
#define SENSOR                         (0x13)
#define SENSOR_WRITE                   (0x26)
#define SENSOR_READ                    (0x27)

/* Register addresses */
#define REGISTER_COMMAND 			   (0x80)
#define REGISTER_ID                    (0x81)
#define REGISTER_PROX_RATE             (0x82) // 111(= 250 measurements/s) to 000(= 2 measurements/s)
#define REGISTER_PROX_CURRENT          (0x83)
#define REGISTER_AMBI_PARAMETER        (0x84)
#define REGISTER_AMBI_VALUE_HI         (0x85)
#define REGISTER_AMBI_VALUE_LO         (0x86)
#define REGISTER_PROX_VALUE_HI         (0x87)
#define REGISTER_PROX_VALUE_LO         (0x88)
#define REGISTER_INTERRUPT_CONTROL     (0x89)
#define REGISTER_INTERRUPT_LOW_THRES   (0x8a)
#define REGISTER_INTERRUPT_HIGH_THRES  (0x8c)
#define REGISTER_INTERRUPT_STATUS      (0x8e)
#define REGISTER_PROX_TIMING           (0x8f)
#define REGISTER_AMBI_IR_LIGHT_LEVEL   (0x90)

/* Bits in Command register (0x80) */
#define COMMAND_ALL_DISABLE            (0x00)
#define COMMAND_SELFTIMED_MODE_ENABLE  (0x01)
#define COMMAND_PROX_ENABLE            (0x02)
#define COMMAND_AMBI_ENABLE            (0x04)
#define COMMAND_PROX_ON_DEMAND         (0x08)
#define COMMAND_AMBI_ON_DEMAND         (0x10)
#define COMMAND_MASK_PROX_DATA_READY   (0x20)
#define COMMAND_MASK_AMBI_DATA_READY   (0x40)
#define COMMAND_MASK_LOCK (0x80)

/* Bits in Prox Measurement Rate register (0x82) */
#define PROX_MEASUREMENT_RATE_2        (0x00)   // DEFAULT
#define PROX_MEASUREMENT_RATE_4        (0x01)
#define PROX_MEASUREMENT_RATE_8        (0x02)
#define PROX_MEASUREMENT_RATE_16       (0x03)
#define PROX_MEASUREMENT_RATE_31       (0x04)
#define PROX_MEASUREMENT_RATE_62       (0x05)
#define PROX_MEASUREMENT_RATE_125      (0x06)
#define PROX_MEASUREMENT_RATE_250      (0x07)
#define PROX_MASK_MEASUREMENT_RATE     (0x07)


/**
*
* \brief Initalisieren eines Task und aufrufen
*        der anderen Initialisierungsfunktionen
*        in diesem File. Um einen Helligkeits- und Entfernungssenor auszulesen.
*
* \param shared_Data um auch hier das shared memory verwenden zu können
*
* \return 0 wenn alles erfolgreich war
*/
int setup_Sensor_Task(Shared_Data *shared_Data);


#endif /* PROJEKT_LOCAL_INC_SENSOR_TASK_H_ */
