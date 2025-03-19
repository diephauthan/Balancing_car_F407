#include "iwdg.h"

void IWDG_Start(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    IWDG_SetReload(155); //(1/40000)*256*(155+1) Watchdog feeding time is set within 1s
    IWDG_Enable();
    IWDG_ReloadCounter(); 
}
