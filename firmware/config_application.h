#include <avr/io.h>
#include "femtoos_config.h"

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

/*
 * Settings
 * 4Mhz clock freq
 */
#define cfgSysSubTicksPerFullTick   16
#define cfgSysSubTickDivider        256

#define includeTaskYield            cfgFalse
#define includeTaskDelayFromNow     cfgTrue
#define includeTaskDelayFromWake    cfgFalse
#define includeGenSuspend           cfgFalse

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
#define  cfgSysSubTicksPerFullTick               32
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
#define  callAppBoot                             cfgFalse
#define  callAppInit                             cfgFalse
#define  callAppBark                             cfgFalse
#define  includeTaskYield                        cfgFalse
#define  includeTaskDelayFromNow                 cfgFalse
#define  includeTaskDelayFromWake                cfgFalse
#define  includeGenSuspend                       cfgFalse
#define  includeTaskSuspend                      cfgFalse
#define  includeGenResume                        cfgFalse
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
#define  includeTaskMutex                        cfgFalse
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
#define  CN_00                                   task0
#define  CN_01                                   task1
#define  CN_02                                   task2
#define  CN_03                                   task3
#define  CN_04                                   task4
#define  CN_05                                   task5
#define  CN_06                                   task6
#define  CN_07                                   task7
#define  CN_08                                   task8
#define  CN_09                                   task9
#define  CN_10                                   taskA
#define  CN_11                                   taskB
#define  CN_12                                   taskC
#define  CN_13                                   taskD
#define  CN_14                                   taskE
#define  CN_15                                   taskF
#define  TaskIncludeOverride                     cfgOverrideNon
#define  TaskInclude_task0                       cfgExclude
#define  TaskInclude_task1                       cfgExclude
#define  TaskInclude_task2                       cfgExclude
#define  TaskInclude_task3                       cfgExclude
#define  TaskInclude_task4                       cfgExclude
#define  TaskInclude_task5                       cfgExclude
#define  TaskInclude_task6                       cfgExclude
#define  TaskInclude_task7                       cfgExclude
#define  TaskInclude_task8                       cfgExclude
#define  TaskInclude_task9                       cfgExclude
#define  TaskInclude_taskA                       cfgExclude
#define  TaskInclude_taskB                       cfgExclude
#define  TaskInclude_taskC                       cfgExclude
#define  TaskInclude_taskD                       cfgExclude
#define  TaskInclude_taskE                       cfgExclude
#define  TaskInclude_taskF                       cfgExclude
#define  CapabilitiesOverride                    cfgOverrideNon
#define  Capabilities_task0                      cfgCapAll
#define  Capabilities_task1                      cfgCapAll
#define  Capabilities_task2                      cfgCapAll
#define  Capabilities_task3                      cfgCapAll
#define  Capabilities_task4                      cfgCapAll
#define  Capabilities_task5                      cfgCapAll
#define  Capabilities_task6                      cfgCapAll
#define  Capabilities_task7                      cfgCapAll
#define  Capabilities_task8                      cfgCapAll
#define  Capabilities_task9                      cfgCapAll
#define  Capabilities_taskA                      cfgCapAll
#define  Capabilities_taskB                      cfgCapAll
#define  Capabilities_taskC                      cfgCapAll
#define  Capabilities_taskD                      cfgCapAll
#define  Capabilities_taskE                      cfgCapAll
#define  Capabilities_taskF                      cfgCapAll
#define  StackSafety                             0
#define  StackSizeOS                             8
#define  StackSizeISR                            8
#define  StackSizeShared                         0
#define  StackSizeOverride                       cfgOverrideNon
#define  StackSize_task0                         4
#define  StackSize_task1                         4
#define  StackSize_task2                         4
#define  StackSize_task3                         4
#define  StackSize_task4                         4
#define  StackSize_task5                         4
#define  StackSize_task6                         4
#define  StackSize_task7                         4
#define  StackSize_task8                         4
#define  StackSize_task9                         4
#define  StackSize_taskA                         4
#define  StackSize_taskB                         4
#define  StackSize_taskC                         4
#define  StackSize_taskD                         4
#define  StackSize_taskE                         4
#define  StackSize_taskF                         4
#define  TimeSliceIdleTime                       cfgSysSubTicksPerFullTick
#define  TimeSliceOverride                       cfgOverrideNon
#define  TimeSlice_task0                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task1                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task2                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task3                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task4                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task5                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task6                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task7                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task8                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task9                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskA                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskB                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskC                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskD                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskE                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskF                         cfgSysSubTicksPerFullTick
#define  TaskName_task0                          "taak nul"
#define  TaskName_task1                          "taak een"
#define  TaskName_task2                          "taak twee"
#define  TaskName_task3                          "taak drie"
#define  TaskName_task4                          "taak vier"
#define  TaskName_task5                          "taak vijf"
#define  TaskName_task6                          "taak_zes"
#define  TaskName_task7                          "taak_zeven"
#define  TaskName_task8                          "taak_acht"
#define  TaskName_task9                          "taak_negen"
#define  TaskName_taskA                          "taak_tien"
#define  TaskName_taskB                          "taak_elf"
#define  TaskName_taskC                          "taak_twaalf"
#define  TaskName_taskD                          "taak_dertien"
#define  TaskName_taskE                          "taak_veertien"
#define  TaskName_taskF                          "taak_vijftien"
#define  PriorityOverride                        cfgOverrideNon
#define  Priority_task0                          0
#define  Priority_task1                          3
#define  Priority_task2                          2
#define  Priority_task3                          3
#define  Priority_task4                          2
#define  Priority_task5                          1
#define  Priority_task6                          1
#define  Priority_task7                          1
#define  Priority_task8                          0
#define  Priority_task9                          0
#define  Priority_taskA                          0
#define  Priority_taskB                          0
#define  Priority_taskC                          0
#define  Priority_taskD                          0
#define  Priority_taskE                          0
#define  Priority_taskF                          0
#define  RegisterUseOverride                     cfgOverrideNon
#define  RegisterUse_task0                       registersNon
#define  RegisterUse_task1                       r00r01r02r03 | r04r05r06r07
#define  RegisterUse_task2                       r08r09r10r11 | r12r13r14r15
#define  RegisterUse_task3                       r16r17r18r19 | r20r21r22r23
#define  RegisterUse_task4                       r24r25r26r27 | r28r29r30r31
#define  RegisterUse_task5                       r00r01r02r03
#define  RegisterUse_task6                       r04r05r06r07
#define  RegisterUse_task7                       r08r09r10r11
#define  RegisterUse_task8                       r12r13r14r15
#define  RegisterUse_task9                       r16r17r18r19
#define  RegisterUse_taskA                       r20r21r22r23
#define  RegisterUse_taskB                       r24r25r26r27
#define  RegisterUse_taskC                       r28r29r30r31
#define  RegisterUse_taskD                       registersAll
#define  RegisterUse_taskE                       registersAll
#define  RegisterUse_taskF                       registersAll
#define  RegisterCheckOverride                   cfgOverrideNon
#define  RegisterCheck_task0                     registersAll
#define  RegisterCheck_task1                     registersAll
#define  RegisterCheck_task2                     registersAll
#define  RegisterCheck_task3                     registersAll
#define  RegisterCheck_task4                     registersAll
#define  RegisterCheck_task5                     registersAll
#define  RegisterCheck_task6                     registersAll
#define  RegisterCheck_task7                     registersAll
#define  RegisterCheck_task8                     registersAll
#define  RegisterCheck_task9                     registersAll
#define  RegisterCheck_taskA                     registersAll
#define  RegisterCheck_taskB                     registersAll
#define  RegisterCheck_taskC                     registersAll
#define  RegisterCheck_taskD                     registersAll
#define  RegisterCheck_taskE                     registersAll
#define  RegisterCheck_taskF                     registersAll
#define  InterruptStartOverride                  cfgOverrideNon
#define  InterruptStart_task0                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task1                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task2                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task3                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task4                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task5                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task6                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task7                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task8                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task9                    cfgGlobSet | cfgTickSet
#define  InterruptStart_taskA                    cfgGlobSet | cfgTickSet
#define  InterruptStart_taskB                    cfgGlobSet | cfgTickSet
#define  InterruptStart_taskC                    cfgGlobSet | cfgTickSet
#define  InterruptStart_taskD                    cfgGlobSet | cfgTickSet
#define  InterruptStart_taskE                    cfgGlobSet | cfgTickSet
#define  InterruptStart_taskF                    cfgGlobSet | cfgTickSet
#define  EN_00                                   event0
#define  EN_01                                   event1
#define  EN_02                                   event2
#define  EN_03                                   event3
#define  EN_04                                   event4
#define  EN_05                                   event5
#define  EN_06                                   event6
#define  EN_07                                   event7
#define  SN_01                                   slot1
#define  SN_02                                   slot2
#define  SN_03                                   slot3
#define  SN_04                                   slot4
#define  SN_05                                   slot5
#define  SN_06                                   slot6
#define  SN_07                                   slot7
#define  SN_08                                   slot8
#define  SN_09                                   slot9
#define  SN_10                                   slotA
#define  SN_11                                   slotB
#define  SN_12                                   slotC
#define  SN_13                                   slotD
#define  SN_14                                   slotE
#define  SN_15                                   slotF
#define  SlotSizeOverride                        cfgOverrideNon
#define  SlotSize_task0                          0
#define  SlotSize_task1                          0
#define  SlotSize_task2                          0
#define  SlotSize_task3                          0
#define  SlotSize_task4                          0
#define  SlotSize_task5                          0
#define  SlotSize_task6                          0
#define  SlotSize_task7                          0
#define  SlotSize_task8                          0
#define  SlotSize_task9                          0
#define  SlotSize_taskA                          0
#define  SlotSize_taskB                          0
#define  SlotSize_taskC                          0
#define  SlotSize_taskD                          0
#define  SlotSize_taskE                          0
#define  SlotSize_taskF                          0
#define  SlotUseOverride                         cfgOverrideNon
#define  SlotUse_slot1                           cfgUseAsNon
#define  SlotUse_slot2                           cfgUseAsNon
#define  SlotUse_slot3                           cfgUseAsNon
#define  SlotUse_slot4                           cfgUseAsNon
#define  SlotUse_slot5                           cfgUseAsNon
#define  SlotUse_slot6                           cfgUseAsNon
#define  SlotUse_slot7                           cfgUseAsNon
#define  SlotUse_slot8                           cfgUseAsNon
#define  SlotUse_slot9                           cfgUseAsNon
#define  SlotUse_slotB                           cfgUseAsNon
#define  SlotUse_slotC                           cfgUseAsNon
#define  SlotUse_slotD                           cfgUseAsNon
#define  SlotUse_slotE                           cfgUseAsNon
#define  SlotUse_slotF                           cfgUseAsNon
#define  QueuSizeOverride                        cfgOverrideNon
#define  QueuSize_slot1                          0
#define  QueuSize_slot2                          10
#define  QueuSize_slot3                          10
#define  QueuSize_slot4                          10
#define  QueuSize_slot5                          10
#define  QueuSize_slot6                          10
#define  QueuSize_slot7                          10
#define  QueuSize_slot8                          10
#define  QueuSize_slot9                          10
#define  QueuSize_slotA                          10
#define  QueuSize_slotB                          10
#define  QueuSize_slotC                          10
#define  QueuSize_slotD                          10
#define  QueuSize_slotE                          10
#define  QueuSize_slotF                          10
#define  FN_00                                   file0
#define  FN_01                                   file1
#define  FN_02                                   file2
#define  FN_03                                   file3
#define  FN_04                                   file4
#define  FN_05                                   file5
#define  FN_06                                   file6
#define  FN_07                                   file7
#define  FN_08                                   file8
#define  FN_09                                   file9
#define  FN_10                                   fileA
#define  FN_11                                   fileB
#define  FN_12                                   fileC
#define  FN_13                                   fileD
#define  FN_14                                   fileE
#define  FN_15                                   fileF
#define  FileSpaceStandard                       0
#define  FileSpaceOverride                       cfgOverrideNon
#define  FileSpace_file0                         0
#define  FileSpace_file1                         0
#define  FileSpace_file2                         0
#define  FileSpace_file3                         0
#define  FileSpace_file4                         0
#define  FileSpace_file5                         0
#define  FileSpace_file6                         0
#define  FileSpace_file7                         0
#define  FileSpace_file8                         0
#define  FileSpace_file9                         0
#define  FileSpace_fileA                         0
#define  FileSpace_fileB                         0
#define  FileSpace_fileC                         0
#define  FileSpace_fileD                         0
#define  FileSpace_fileE                         0
#define  FileSpace_fileF                         0
