

#ifndef PROJEKT_LOCAL_INC_OUTPUT_TASK_H_
#define PROJEKT_LOCAL_INC_OUTPUT_TASK_H_

#include "Common.h"

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
int setup_Output_Task(Shared_Data *shared_Data);

#endif /* PROJEKT_LOCAL_INC_OUTPUT_TASK_H_ */
