#include "timer.h"




/**
 *
 */
void setTimmer(uint32_t time, char multipleShot)
{
cortex_a9_gid_enable_irq(29);
cortex_a9_gid_enable_irq(30);
uint32_t watchdogCtrlVal = arm_mmio_read32(cortex_a9_peripheral_base() + ARM_PWT_BASE_OFFSET, TIMER_OFF_WATCHDOG_REGISTER_CONTROL);
	uint32_t watchdogCtrlValSave = watchdogCtrlVal;
	uint32_t offset;
   
	char isWatchdogMode  = getBit32(watchdogCtrlVal, TIMER_BIT_WATCHDOG_REGISTER_CONTROL_WATCHDOG_MODE);
	char isInterupEnable = getBit32(watchdogCtrlVal, TIMER_BIT_WATCHDOG_REGISTER_CONTROL_INTERUPT_ENABLE);
	char isAutoReload    = getBit32(watchdogCtrlVal, TIMER_BIT_WATCHDOG_REGISTER_CONTROL_AUTO_RELOAD);
	char isWatchdogEnable= getBit32(watchdogCtrlVal, TIMER_BIT_WATCHDOG_REGISTER_CONTROL_WATCHDOG_ENABLE);

	// Case timer in Watchdog mode
	if (isWatchdogMode)	offset = TIMER_OFF_WATCHDOG_REGISTER_LOAD;
	// Case timer in timer mode (default)
	else			offset = TIMER_OFF_WATCHDOG_REGISTER_COUNTER;

	// Update the timer value
arm_mmio_write32(cortex_a9_peripheral_base() + ARM_PWT_BASE_OFFSET, offset, time);

	if (!isInterupEnable)
	     watchdogCtrlVal = setBit32(watchdogCtrlVal, TIMER_BIT_WATCHDOG_REGISTER_CONTROL_INTERUPT_ENABLE);
	if (!isWatchdogEnable)
	     watchdogCtrlVal = setBit32(watchdogCtrlVal, TIMER_BIT_WATCHDOG_REGISTER_CONTROL_WATCHDOG_ENABLE);
	if (!isAutoReload && multipleShot)
	{
	     watchdogCtrlVal = setBit32(watchdogCtrlVal, TIMER_BIT_WATCHDOG_REGISTER_CONTROL_AUTO_RELOAD);
	     if (! isWatchdogMode)
	     {
arm_mmio_write32(cortex_a9_peripheral_base() + ARM_PWT_BASE_OFFSET, TIMER_OFF_WATCHDOG_REGISTER_LOAD, time);
	     }
	}

	// Set the watchdog control value (if has changed)
	if (watchdogCtrlVal != watchdogCtrlValSave)
		arm_mmio_write32(ARM_PWT_BASE_OFFSET, TIMER_OFF_WATCHDOG_REGISTER_CONTROL, watchdogCtrlVal);

}


/**
 *
 */
void unsetTimmer()
{
// TODO
}
