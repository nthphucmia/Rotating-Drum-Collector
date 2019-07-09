#ifndef _SERMOTOR_CONFIG_H
#define _SERMOTOR_CONFIG_H

#include "main.h"
/*Define------------------------------------------------------------------*/
#define PWM_PERIOD					60000

void ServoInit (uint16_t pulse);
void UpdateServoPWM(uint16_t pulseUpdate);
void RunServo (uint16_t pulseRun);
void StopServo_noBRA(void);
void StopServo_BRA(void);

#endif /* _SERMOTOR_CONFIG_H */
