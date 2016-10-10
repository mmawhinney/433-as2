#ifndef _DELAY_CALCUALTOR_H_
#define _DELAY_CALCUALTOR_H_

void delayCalculator_enableCape();
void delayCalculator_getReading(int *reading);
void delayCalculator_determineDelay(int reading);
int delayCalculator_getDelay();
void* delayCalculator_launchThread(void* args);

#endif