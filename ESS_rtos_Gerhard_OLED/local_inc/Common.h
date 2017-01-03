

#ifndef PROJEKT_LOCAL_INC_COMMON_H_
#define PROJEKT_LOCAL_INC_COMMON_H_

#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Event.h>

#define OUTPUT_SIZE (16 * 5)  // Auf dem Display werden 5 Zeilen mit jeweils 16 Zeichen angezeigt

#define AMBI_MAX_VALUE                  16383
#define PROXI_MAX_VALUE                   120
#define PROXI_UNENDLICH                   999

/**
*
* \brief struct die als shared memory verwendet wird um Daten zwischen den Task auszutauschen
*
*/
typedef struct Shared_Data {
   Semaphore_Handle sem;              // Semaphore zum Sperren des Zugriffs auf den Speicher
   Event_Handle UART_Event;           // Event das vom Sensor ausgelöst wird wenn ein neuer Wert auf die UART geaschrieben werden soll
   Event_Handle Output_Event;         // Event das vom Sensor ausgelöst wird wenn ein neuer Wert auf das Display geaschrieben werden soll
   uint16_t brightness;               // Vom Sensor gelesene Helligkeit
   uint16_t distance_read;            // Vom Sensor gelesener Abstand
   uint16_t distance;                 // Berechneter Abstandswert
   uint16_t speed;                    // Geschwindigkeit mit der Lauftext über das Display scrollt
   UInt Display_Mode;                 // Wird gesteuert über Butten 1 oder UART: 5 verschiedene: 0..Anzeige aller Zeichen der Pixelschrift, 1..Lauftext, 2..Sensorwerte als Text, 3..Bild, 4..Sensorwerte als Diagramm
   UInt uart_out_OFF;                 // Ausschalten des Senden der Sensorwerte über die UART. Wird gesteuert über Button 2
} Shared_Data;


#endif /* PROJEKT_LOCAL_INC_COMMON_H_ */
