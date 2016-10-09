#ifndef _DELAY_CALCUALTOR_H_
#define _DELAY_CALCUALTOR_H_

void delayCalculator_enableCape();
void delayCalculator_getReading(int *reading);
int delayCalculator_determineDelay(int reading);

#endif