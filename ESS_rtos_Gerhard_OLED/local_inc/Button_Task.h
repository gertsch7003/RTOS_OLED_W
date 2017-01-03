#ifndef LOCAL_INC_BUTTON_TASK_H_
#define LOCAL_INC_BUTTON_TASK_H_

#include "Common.h"

/**
*
* \brief Initalisierung für die beiden Button
*
* \param Shared_Data: shared memory in dem in Abhängigkeit der gedruckten Buttons
*        shared_Data->Display_Mode und shared_Data->Sensor_OFF gesetzt werden
*
* \return 0 wenn alles erfolgreich war
*/
int setup_Button_Task(Shared_Data *shared_Data);

#endif /* LOCAL_INC_BUTTON_TASK_H_ */
