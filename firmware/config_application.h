#include <avr/io.h>

/*
======================================
=== Application ===
======================================
*/

/*
 * DS1629 address on i2c
 * Last bit will be ignored in i2c
 */
#define DS1629_ADDR                 0x02

/*
 * Input and display pins definitions
 */
#define INPUT_PORT                  PORTC;
#define INPUT_PIN                   PINC;
#define INPUT_DDR                   DDRC;
#define VOLTAGE_PIN                 PC6;
#define CHARGE_PIN                  PC1;
#define BUTTON_PIN                  PC0;

/*
 * 7-seg indicator segment pins
 * Low level means active segment
 */
#define DISP_PORT                   PORTD;
#define DISP_PIN                    PIND;
#define DISP_DDR                    DDRD;
#define DISP_A                      PD0;
#define DISP_B                      PD2;
#define DISP_C                      PD3;
#define DISP_D                      PD4;
#define DISP_E                      PD5;
#define DISP_F                      PD6;
#define DISP_G                      PD7;
#define DISP_SEG_MASK \
	_BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_D)|_BV(DISP_E)|_BV(DISP_F)|_BV(DISP_G)

/*
 * Digits common pins
 * Low level means active digit
 */
#define DISP_COM_PORT               PORTB;
#define DISP_COM_PIN                PINB;
#define DISP_COM_DDR                DDRB;
#define DISP_COM1                   PB1;
#define DISP_COM2                   PB4;
#define DISP_COM3                   PB3;
#define DISP_COM4                   PB2;
#define DISP_COM5                   PB0;
#define DISP_COM_MASK  \
	_BV(DISP_COM1)|_BV(DISP_COM2)|_BV(DISP_COM3)|_BV(DISP_COM4)|_BV(DISP_COM5)

/*
 * Digits patterns
 */
#define SEG_PATTERN_0 \
     _BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_D)|_BV(DISP_E)|_BV(DISP_F)
     
#define SEG_PATTERN_1 \
    _BV(DISP_B)|_BV(DISP_C)
    
#define SEG_PATTERN_2 \
    _BV(DISP_A)|_BV(DISP_B)|_BV(DISP_G)|_BV(DISP_E)|_BV(DISP_D)
    
#define SEG_PATTERN_3 \
    _BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_D)|_BV(DISP_G)
    
#define SEG_PATTERN_4 \
    _BV(DISP_B)|_BV(DISP_C)|_BV(DISP_F)|_BV(DISP_G)
    
#define SEG_PATTERN_5 \
    _BV(DISP_A)|_BV(DISP_F)|_BV(DISP_G)|_BV(DISP_C)|_BV(DISP_D)
    
#define SEG_PATTERN_6 \
    _BV(DISP_C)|_BV(DISP_D)|_BV(DISP_E)|_BV(DISP_F)|_BV(DISP_G)
    
#define SEG_PATTERN_7 \
    _BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)
    
#define SEG_PATTERN_8 \
    _BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_D)|_BV(DISP_E)|_BV(DISP_F)|_BV(DISP_G)
    
#define SEG_PATTERN_9 \
    _BV(DISP_A)|_BV(DISP_B)|_BV(DISP_C)|_BV(DISP_F)|_BV(DISP_G)
    

/*
 * Another indication patterns
 */
#define SEG_MODE_1                  _BV(DISP_A)
#define SEG_MODE_2                  _BV(DISP_B)
#define SEG_MODE_3                  _BV(DISP_C)
#define SEG_COLON                   _BV(DISP_D)|_BV(DISP_E)
#define SEG_DOT                     _BV(DISP_F)
#define SEG_CHARGE                  _BV(DISP_G)
#define SEG_MINUS                   _BV(DISP_G) /* Minus in digit */


/*
======================================
=== Timings settings ===
======================================
*/

/*
 * 1 second duration in system ticks. Base value for other timeouts
 * 2 bytes
 */
#define SYSTEM_TICKS_1SEC \
	devClockFrequency / \
	(cfgSysClockDivider * cfgSysSubTickDivider * cfgSysSubTicksPerFullTick)

/*
 * Count of input ticks when pressed button goes to HOLD mode
 * 1 byte
 */
#define HOLD_THRESHOLD              200 // :)

/*
 * Minimal count of input ticks between clicks (when CLICK mode goes to IDLE)
 * 1 byte
 */
#define CLICK_GAP                   50

/*
 * Interval between consecutive HOLD events in input ticks
 * 1 byte
 */
#define HOLD_EVENT_INTERVAL         100

/*
 * Input tick duration in system ticks
 */
#define INPUT_TICK_DURATION         SYSTEM_TICKS_1SEC/80 //Input tick = ~5ms

/*
 * Time to show one digit while rendering in system ticks
 * 2 bytes
 */
#define DIGIT_RENDER_DELAY          SYSTEM_TICKS_1SEC/40 //Delay 2.5ms (whole display is 2.5*4=10ms)

/*
 * Duty cycle in system ticks when digit in shadow mode
 */
#define SHADOW_DIGIT_RENDER_DELAY   SYSTEM_TICKS_1SEC/80 //Delay 1.25ms

/*
 * Blink interval in system ticks when something is blinking on display
 */
#define DISPLAY_BLINK_INTERVAL      SYSTEM_TICKS_1SEC/2


/*
======================================
=== FemtoOS settings ===
======================================
*/

/*
 * Tasks
 */
#define CN_00                       Display
#define CN_01                       DisplayBlink
#define CN_02                       Button
#define CN_03                       Sources

#define TaskIncludeOverride         cfgOverrideNon
#define TaskInclude_Display         cfgStartRunning
#define TaskInclude_DisplayBlink    cfgStartSuspended
#define TaskInclude_Button          cfgStartRunning
#define TaskInclude_Sources         cfgStartRunning

#define StackSize_Display           128
#define StackSize_DisplayBlink      32
#define StackSize_Button            320
#define StackSize_Sources           320

#define CapabilitiesOverride        cfgCapAll

/*
 * Settings
 * 4Mhz clock freq
 */
#define cfgSysSubTicksPerFullTick   16
#define cfgSysSubTickDivider        256

#define includeTaskYield            cfgFalse
#define includeTaskDelayFromNow     cfgTrue
#define includeTaskDelayFromWake    cfgTrue
#define includeGenSuspend           cfgFalse
#define includeTaskSuspend          cfgTrue
#define includeTaskMutex            cfgTrue
#define includeGenResume            cfgTrue

#define callAppBoot                 cfgTrue

#define StackSafety                 4
#define StackSizeOS                 24
#define StackSizeISR                0
#define StackSizeShared             0
#define StackSizeOverride           46

#define cfgIntUserDefined           cfgFalse

#define  cfgSysOptimized                         cfgFalse
#define  cfgSysDebug                             cfgFalse
#define  cfgSysInline                            cfgFalse
#define  cfgSysFramePointerCounterMeasures       cfgFalse
#define  cfgSysReduceProEpilogue                 cfgBikini
#define  cfgSysRegisterCheckByte                 0xA5
#define  cfgSysStackGrowthUp                     cfgFalse
#define  cfgSysZeroPageStack                     cfgFalse
#define  cfgSysClearUnusedR1                     cfgTrue
#define  cfgSysSqueezeState                      defSysOptimized
#define  cfgSysReuseOsStack                      cfgTrue
#define  cfgSysClockDivider                      1
#define  cfgSysSubTickDivider                    256
#define  cfgSysGCCstartup                        cfgKeep
#define  cfgSysFsStorageSpace                    512
#define  cfgIntGlobalOnly                        cfgTrue
#define  cfgIntUserDefined                       cfgFalse
#define  cfgIntSwitchUsesOSstack                 cfgFalse
#define  cfgIntOsProtected                       cfgTrue
#define  cfgIntTickTrack                         cfgFalse
#define  cfgIntManualTicks                       cfgFalse
#define  cfgCheckSystem                          cfgFalse
#define  cfgCheckReset                           cfgFalse
#define  cfgCheckTrace                           cfgFalse
#define  cfgCheckAlwaysFatal                     cfgTrue
#define  cfgCheckWatermarks                      cfgFalse
#define  cfgCheckOsStack                         cfgFalse
#define  cfgCheckTaskStack                       cfgFalse
#define  cfgCheckIsrStack                        cfgFalse
#define  cfgCheckRegisters                       cfgFalse
#define  cfgCheckTiming                          cfgFalse
#define  cfgCheckQueuFilling                     cfgFalse
#define  cfgCheckMethodUse                       cfgFalse
#define  cfgCheckApplication                     cfgFalse
#define  cfgUseEquidistantTicks                  cfgTrue
#define  cfgUseDelay                             cfgFalse
#define  cfgUseSuspendOnDelay                    cfgFalse
#define  cfgUseSynchronization                   cfgSyncNon
#define  cfgUseHierarchicalRoundRobin            cfgFalse
#define  cfgUseNestedCriticals                   cfgFalse
#define  cfgUsePrioritizedRelease                cfgFalse
#define  cfgUsePriorityLifting                   cfgFalse
#define  cfgUseTaskWatchdog                      cfgFalse
#define  cfgUseCorrectWakeupTimes                cfgFalse
#define  cfgUseTimeout                           cfgFalse
#define  cfgUseTasknames                         cfgFalse
#define  cfgUseLowPowerSleep                     cfgFalse
#define  cfgUseLowPowerOnDelay                   cfgFalse
#define  cfgUseLowPowerDelayRelease              cfgFalse
#define  cfgUseLoadMonitor                       cfgFalse
#define  cfgUseFileSystem                        cfgFalse
#define  cfgUseFileSystemConcurrentRead          cfgFalse
#define  cfgUseFileSystemEconomyMode             cfgFalse
#define  cfgUseFileSystemMaintainFAT             cfgFalse
#define  cfgUseEvents                            cfgFalse
#define  cfgUseEventsOnVariables                 cfgFalse
#define  cfgNumWatchdogDiv                       0
#define  cfgNumMonitorDiv                        0
#define  cfgNumSleepPeriod                       0
#define  cfgNumSleepThreshold                    0
#define  cfgNumGlobWidth                         0
#define  cfgNumSwitchWidth                       0
#define  cfgNumTickWidth                         0
#define  callAppTick00                           cfgFalse
#define  callAppTick08                           cfgFalse
#define  callAppTick16                           cfgFalse
#define  callAppTickSleep                        cfgFalse
#define  callAppEnterIdle                        cfgFalse
#define  callAppEnterSleep                       cfgFalse
#define  callAppExitSleep                        cfgFalse
#define  callAppInit                             cfgFalse
#define  callAppBark                             cfgFalse
#define  includeTaskYield                        cfgFalse
#define  includeGenSuspend                       cfgFalse
#define  includeTaskSleep                        cfgFalse
#define  includeTaskSleepAll                     cfgFalse
#define  includeGenSetPriority                   cfgFalse
#define  includeGenGetPriority                   cfgFalse
#define  includeGenGetTickCount                  cfgFalse
#define  includeGenAddtoTickCount                cfgFalse
#define  includeGenGetLastWakeTime               cfgFalse
#define  includeTaskFeedWatchdog                 cfgFalse
#define  includeTaskKillWatchdog                 cfgFalse
#define  includeGenGetTaskname                   cfgFalse
#define  includeTaskTerminate                    cfgFalse
#define  includeGenReboot                        cfgFalse
#define  includeTaskRecreate                     cfgFalse
#define  includeTaskRestart                      cfgFalse
#define  includeGenLogTask                       cfgFalse
#define  includeGenLogOs                         cfgFalse
#define  includeGenTrace                         cfgFalse
#define  includeGenPipeInt16                     cfgFalse
#define  includeGenPassFlashString               cfgFalse
#define  includeTaskWaitForEvents                cfgFalse
#define  includeIsrFireEvent                     cfgFalse
#define  includeGenFireEvent                     cfgFalse
#define  includeGenFireEventSet                  cfgFalse
#define  includeGenCountEventBlocks              cfgFalse
#define  includeTaskWaitForTasks                 cfgFalse
#define  includeGenWaitRelease                   cfgFalse
#define  includeTaskQueu                         cfgFalse
#define  includeGenQueuWrite                     cfgFalse
#define  includeGenQueuRead                      cfgFalse
#define  includeGenQueuUnwrite                   cfgFalse
#define  includeGenQueuUnread                    cfgFalse
#define  includeGenQueuClear                     cfgFalse
#define  includeGenQueuPeek                      cfgFalse
#define  includeGenQueuReadable                  cfgFalse
#define  includeGenQueuWriteable                 cfgFalse
#define  includeGenQueuFull                      cfgFalse
#define  includeGenQueuEmpty                     cfgFalse
#define  includeTaskFileAccess                   cfgFalse
#define  includeTaskFileFormat                   cfgFalse
#define  includeTaskFileGetSize                  cfgFalse
#define  includeTaskFileSetSize                  cfgFalse
#define  includeTaskFileReadByte                 cfgFalse
#define  includeTaskFileWriteByte                cfgFalse
#define  includeTaskFileAppendByte               cfgFalse
#define  includeTaskFileReadPipe                 cfgFalse
#define  includeTaskFileWritePipe                cfgFalse
#define  includeTaskFileReadBuffer               cfgFalse
#define  includeTaskFileWriteBuffer              cfgFalse
#define  includeTaskProtectGlobalInterrupts      cfgFalse
#define  includeTaskProtectSwitchTasks           cfgFalse
#define  includeTaskProtectTickInterrupts        cfgFalse
#define  includeTaskProtectGlobalCritical        cfgFalse
#define  includeTaskProtectSwitchCritical        cfgFalse
#define  includeTaskProtectTickCritical          cfgFalse
#define  includeIsrEnter                         cfgFalse
#define  includeIsrExit                          cfgFalse
#define  includeIsrBegin                         cfgFalse
#define  includeIsrEndReturn                     cfgFalse
#define  includeIsrEndYield                      cfgFalse
#define  includeIsrStartLoad                     cfgFalse
#define  includeIsrStopLoad                      cfgFalse
#define  includeTaskStackCheck                   cfgFalse
#define  includeIsrStackCheck                    cfgFalse
#define  StackSizeShared                         0
#define  TimeSliceIdleTime                       cfgSysSubTicksPerFullTick
#define  TimeSliceOverride                       cfgSysSubTicksPerFullTick
#define  PriorityOverride                        0
#define  RegisterUseOverride                     cfgOverrideNon
#define  RegisterCheckOverride                   registersAll
#define  SlotSizeOverride                        cfgOverrideNon
#define  SlotUseOverride                         cfgOverrideNon
#define  QueuSizeOverride                        cfgOverrideNon
#define  FileSpaceStandard                       0
#define  FileSpaceOverride                       cfgOverrideNon
