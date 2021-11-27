#include "stdafx.h"
#include "SolaMonitorSvc.h"
#include "SolaMBMap.h"
#include "SolaMBMaps.h"
#include "SolaMultiValue.h"
#include "SolaChangeCode.h"
#include "SolaPage.h"

using namespace std;

CSolaMBMap* pcODResetConfig;
CSolaMBMap* pcDHWConfiguration;
CSolaMBMap* pcTrendStatus;
CSolaMBMap* pcSensorStatus;
CSolaMBMap* pcExtendedSensorStatus;
CSolaMBMap* pcSystemStatus;
CSolaMBMap* pcLLStatus;
CSolaMBMap* pcXLLStatus;
CSolaMBMap* pcLLConfig;
CSolaMBMap* pcXLLConfig;
CSolaMBMap* pcXSystemConfig;
CSolaMBMap* pcBurnerControlStatus;
CSolaMBMap* pcBurnerControlConfig;
CSolaMBMap* pcCHConfiguration;
CSolaMBMap* pcXCHConfig;
CSolaMBMap* pcX2CHConfig;
CSolaMBMap* pcModConfiguration;
CSolaMBMap* pcXModConfig;
CSolaMBMap* pcDemandModulationStatus;
CSolaMBMap* pcCHStatus;
CSolaMBMap* pcDHWStatus;
CSolaMBMap* pcPumpStatus;
CSolaMBMap* pcSystemConfiguration;
CSolaMBMap* pcSystemIDProductType;
CSolaMBMap* pcSystemIDBurnerName;
CSolaMBMap* pcSystemIDInstallationData;
CSolaMBMap* pcSystemIDOEMID;
CSolaMBMap* pcSystemIDOSNumber;
CSolaMBMap* pcSystemIDDateCode;
CSolaMBMap* pcSystemIDSafetyProcesorBuild;
CSolaMBMap* pcSystemIDAppProcessorBuild;
CSolaMBMap* pcAlarmCode;
CSolaMBMap* pcStatistics;
CSolaMBMap* pcAnnuncConfigGen;
CSolaMBMap* pcAnnuncConfig1;
CSolaMBMap* pcAnnuncConfig2;
CSolaMBMap* pcAnnuncConfig3;
CSolaMBMap* pcAnnuncConfig4;
CSolaMBMap* pcAnnuncConfig5;
CSolaMBMap* pcAnnuncConfig6;
CSolaMBMap* pcAnnuncConfig7;
CSolaMBMap* pcAnnuncConfig8;
CSolaMBMap* pcPIIAnnuncConfig;
CSolaMBMap* pcLCIAnnuncConfig;
CSolaMBMap* pcILKAnnuncConfig;
CSolaMBMap* pcFrostProtConfig;
CSolaMBMap* pcFanConfig;
CSolaMBMap* pcLimitsConfig;
CSolaMBMap* pcPumpConfig;
CSolaMultiValue* pcDHWEnableList;
CSolaMultiValue* pcBurnerControlStateValues;
CSolaMultiValue* pcBurnerControlStatusValues;
CSolaMultiValue* pcSolaLockoutDesc;
CSolaMultiValue* pcBurnerAlarmReasonCodes;
CSolaMultiValue* pcBurnerControlFlags;
CSolaMultiValue* pcBurnerRemoteStatCodes;
CSolaMultiValue* pcBurnerAnnunFirstOutCodes;
CSolaMultiValue* pcBurnerAnnunHoldCodes;
CSolaMultiValue* pcDigitalIOCodes;
CSolaMultiValue* pcRegisterAccess;
CSolaMBMaps* pcTrendMaps;
CSolaMBMaps* pcAllSolaMaps;
CSolaMBMaps* pcSystemIDMaps;
CSolaMBMaps* pcAnnuncMaps;
CSolaChangeCode* pcConfigChangeCodes;
CSolaChangeCode* pcStatusChangeCodes;
CSolaLockout* pcLockoutLog;
CSolaAlert* pcAlertLog;
CSolaPage* pcSummaryPage;

BOOL InitSolaDatabase()
{
	BOOL bSuccess = true;
	
	static SOLAMULTIVALUE BooleanCodes[] = {	0, _T("Off"),
									1, _T("On")};
	
	static SOLAMULTIVALUE EnableCodes[] = {	0, _T("Disabled"),
									1, _T("Enabled")};

	static SOLAMULTIVALUE ResetAndRestartCodes[] = {	0, _T("None"),
											1, _T("Burner control"),
											2, _T("Application"),
											3, _T("Burner control&application"),
											4, _T("Clear alert log")};

	static SOLAMULTIVALUE AnnunIOCodes[] = {	0, _T("Annunciator 1/IAS"),
									1, _T("Annunciator 2"),
									2, _T("Annunciator 3"),
									3, _T("Annunciator 4"),
									4, _T("Annunciator 5"),
									5, _T("Annunciator 6"),
									6, _T("Annunciator 7/HFS"),
									7, _T("Annunciator 8/LFS")};

	static SOLAMULTIVALUE PIMStatusCodes[] = {	0, _T("PM installed"),
											1, _T("Lead/Lag enabled"),
											2, _T("Valid copyright"),
											3, _T("Reserved"),
											4, _T("Reserved"),
											5, _T("Reserved"),
											6, _T("Reserved"),
											7, _T("Reserved"),
											8, _T("OEM parameter PCB stored in PM"),
											9, _T("OEM range PCB stored in PM"),
											10, _T("OEM alert PCB stored in PM"),
											11, _T("Reserved"),
											12, _T("Reserved"),
											13, _T("Reserved"),
											14, _T("Reserved"),
											15, _T("Reserved")};

	static SOLAMBMAP ODResetConfig[] = {_T("CH ODR max. OD temperature"), 0x01, 0x03, 0x0200, 0x0001, 0, true, true, true, NULL, 0, Temperature, NULL, 0,\
										_T("CH ODR min. OD temperature"), 0x01, 0x03, 0x0201, 0x0001, 0, true, true, true, NULL, 0,  Temperature, NULL, 0,\
										_T("CH ODR low water temperature"), 0x01, 0x03, 0x0202, 0x0001, 0, true, true, true, NULL, 0,  Temperature, NULL, 0,\
										_T("CH ODR boost time"), 0x01, 0x03, 0x0203, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
										_T("CH ODR maximum off point"), 0x01, 0x03, 0x00204, 0x0001, true, true, true, 0, NULL, 0,  Temperature, NULL, 0,\
										_T("LL CH ODR max. OD temperature"), 0x01, 0x03, 0x0205, 0x0001, 0, true, true, true, NULL, 0,  Temperature, NULL, 0,\
										_T("LL CH ODR min. OD temperature"), 0x01, 0x03, 0x0206, 0x0001, 0, true, true, true, NULL, 0,  Temperature, NULL, 0,\
										_T("LL CH ODR low water temperature"), 0x01, 0x03, 0x0207, 0x0001, 0, true, true, true, NULL, 0,  Temperature, NULL, 0,\
										_T("LL CH ODR boost time"), 0x01, 0x03, 0x0208, 0x0001, 0, true, true, true, NULL, 0,  Timevalue, NULL, 0,\
										_T("LL CH ODR max. off point"), 0x01, 0x03, 0x0209, 0x0001, 0, true, true, true, NULL, 0,  Temperature, NULL, 0,\
										_T("CH ODR boost step"), 0x01, 0x03, 0x020A, 0x0001, 0, true, true, true, NULL, 0,  Hysteresis, NULL, 0,\
										_T("CH ODR boost recovery step time"), 0x01, 0x03, 0x020B, 0x0001, 0, true, true, true, NULL, 0,  Timevalue, NULL , 0,\
										_T("LL ODR boost step"), 0x01, 0x03, 0x020C, 0x0001, 0, true, true, true, NULL, 0,  Hysteresis, NULL, 0,\
										_T("LL ODR boost recovery step time"), 0x01, 0x03, 0x020D, 0x0001, 0, true, true, true, NULL, 0,  Timevalue, NULL , 0,\
										_T("Min. boiler water temperature"), 0x01, 0x03, 0x020E, 0x0001, 0, true, true, true, NULL, 0,  Temperature, NULL, 0,\
										_T("LL CH ODR min. boiler water temperature"), 0x01, 0x03, 0x020F, 0x0001, 0, true, true, true, NULL, 0,  Temperature, NULL, 0};

	pcODResetConfig = (CSolaMBMap*)new CSolaMBMap(ODResetConfig, sizeof(ODResetConfig)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE AnnuncModesList[] = { 0, _T("Fixed"),\
												1, _T("Programmable")};

	static SOLAMBMAP AnnuncConfigGen[] = {_T("Annunc. enable"), 0x01, 0x03, 0x0130, 0x0001, 0, true, true, true, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE),\
_T("Annunc. mode"), 0x01, 0x03, 0x0131, 0x0001, 0, true, true, true, NULL, 0,  Multivalue, AnnuncModesList, sizeof(AnnuncModesList)/sizeof(SOLAMULTIVALUE)};

	pcAnnuncConfigGen = (CSolaMBMap*)new CSolaMBMap(AnnuncConfigGen,sizeof(AnnuncConfigGen)/sizeof(SOLAMBMAP));

	int cbStrlen = sizeof(SOLAANNUNCRECORD);
	static SOLAMBMAP AnnuncConfig1[] = {_T("Annunc. 1"), 0x01, 0x03, 0x0132, 0x000d, 0, true, true, true, new char[cbStrlen], cbStrlen, Stringvalue, NULL, 0};
	pcAnnuncConfig1 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig1,sizeof(AnnuncConfig1)/sizeof(SOLAMBMAP));
//	pcAnnuncConfig1 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig1,13);
	static SOLAMBMAP AnnuncConfig2[] = {_T("Annunc. 2"), 0x01, 0x03, 0x013f, 0x000d, 0, true, true, true, new char[cbStrlen], cbStrlen, Stringvalue, NULL, 0};
	pcAnnuncConfig2 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig2,sizeof(AnnuncConfig2)/sizeof(SOLAMBMAP));
//	pcAnnuncConfig2 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig2,13);
	static SOLAMBMAP AnnuncConfig3[] = {_T("Annunc. 3"), 0x01, 0x03, 0x014c, 0x000d, 0, true, true, true, new char[cbStrlen], cbStrlen, Stringvalue, NULL, 0};
	pcAnnuncConfig3 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig3,sizeof(AnnuncConfig3)/sizeof(SOLAMBMAP));
//	pcAnnuncConfig3 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig3,13);
	static SOLAMBMAP AnnuncConfig4[] = {_T("Annunc. 4"), 0x01, 0x03, 0x0159, 0x000d, 0, true, true, true, new char[cbStrlen], cbStrlen, Stringvalue, NULL, 0};
	pcAnnuncConfig4 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig4,sizeof(AnnuncConfig4)/sizeof(SOLAMBMAP));
//	pcAnnuncConfig4 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig4,13);
	static SOLAMBMAP AnnuncConfig5[] = {_T("Annunc. 5"), 0x01, 0x03, 0x0166, 0x000d, 0, true, true, true, new char[cbStrlen], cbStrlen, Stringvalue, NULL, 0};
	pcAnnuncConfig5 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig5,sizeof(AnnuncConfig5)/sizeof(SOLAMBMAP));
//	pcAnnuncConfig5 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig5,13);
	static SOLAMBMAP AnnuncConfig6[] = {_T("Annunc. 6"), 0x01, 0x03, 0x0173, 0x000d, 0, true, true, true, new char[cbStrlen], cbStrlen, Stringvalue, NULL, 0};
	pcAnnuncConfig6 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig6,sizeof(AnnuncConfig6)/sizeof(SOLAMBMAP));
//	pcAnnuncConfig6 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig6,13);
	static SOLAMBMAP AnnuncConfig7[] = {_T("Annunc. 7"), 0x01, 0x03, 0x0180, 0x000d, 0, true, true, true, new char[cbStrlen], cbStrlen, Stringvalue, NULL, 0};
	pcAnnuncConfig7 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig7,sizeof(AnnuncConfig7)/sizeof(SOLAMBMAP));
//	pcAnnuncConfig7 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig7,13);
	static SOLAMBMAP AnnuncConfig8[] = {_T("Annunc. 8"), 0x01, 0x03, 0x018d, 0x000d, 0, true, true, true, new char[cbStrlen], cbStrlen, Stringvalue, NULL, 0};
	pcAnnuncConfig8 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig8,sizeof(AnnuncConfig8)/sizeof(SOLAMBMAP));
//	pcAnnuncConfig8 = (CSolaMBMap*) new CSolaMBMap(AnnuncConfig8,13);
	cbStrlen = sizeof(SOLAANNUNCSHORT);
	static SOLAMBMAP PIIAnnuncConfig[] = {_T("PII"), 0x01, 0x03, 0x019a, 0x000c, 0, true, true, true, new char[cbStrlen], cbStrlen, Stringvalue, NULL, 0};
	pcPIIAnnuncConfig = (CSolaMBMap*) new CSolaMBMap(PIIAnnuncConfig,sizeof(PIIAnnuncConfig)/sizeof(SOLAMBMAP));
//	pcPIIAnnuncConfig = (CSolaMBMap*) new CSolaMBMap(PIIAnnuncConfig,12);
	static SOLAMBMAP LCIAnnuncConfig[] = {_T("LCI"), 0x01, 0x03, 0x01a6, 0x000c, 0, true, true, true, new char[cbStrlen], cbStrlen, Stringvalue, NULL, 0};
	pcLCIAnnuncConfig = (CSolaMBMap*) new CSolaMBMap(LCIAnnuncConfig,sizeof(LCIAnnuncConfig)/sizeof(SOLAMBMAP));
//	pcLCIAnnuncConfig = (CSolaMBMap*) new CSolaMBMap(LCIAnnuncConfig,12);
	static SOLAMBMAP ILKAnnuncConfig[] = {_T("ILK"), 0x01, 0x03, 0x01b2, 0x000c, 0, true, true, true, new char[cbStrlen], cbStrlen, Stringvalue, NULL, 0};
	pcILKAnnuncConfig = (CSolaMBMap*) new CSolaMBMap(ILKAnnuncConfig,sizeof(ILKAnnuncConfig)/sizeof(SOLAMBMAP));
//	pcILKAnnuncConfig = (CSolaMBMap*) new CSolaMBMap(ILKAnnuncConfig,12);

	static SOLAMBMAPS AnnuncMaps[] = {	pcAnnuncConfigGen,
										pcAnnuncConfig1,
										pcAnnuncConfig2,
										pcAnnuncConfig3,
										pcAnnuncConfig4,
										pcAnnuncConfig5,
										pcAnnuncConfig6,
										pcAnnuncConfig7,
										pcAnnuncConfig8,
										pcPIIAnnuncConfig,
										pcLCIAnnuncConfig,
										pcILKAnnuncConfig};

	pcAnnuncMaps = (CSolaMBMaps*) new CSolaMBMaps(AnnuncMaps,sizeof(AnnuncMaps)/sizeof(SOLAMBMAPS));

	static SOLAMULTIVALUE DHWEnableList[] = {	0, _T("Disabled"), \
												1, _T("Enabled") };
	
//	pcDHWEnableList = new CSolaMultiValue(DHWEnableList,sizeof(DHWEnableList)/sizeof(SOLAMULTIVALUE));

	static SOLAMULTIVALUE DHWDemandSwitchList[] = {	0, _T("Mod. sensor only"), \
													1, _T("ECOM DHW request"), \
													2, _T("DHW switch"), \
													3, _T("Unused"), \
													4, _T("Stat terminal"), \
													5, _T("Reserved"), \
													6, _T("Modbus STAT"), \
													7, _T("AUTO: S6 or ECOM DHW req"), \
													8, _T("Auto: S6 or snsr only"), \
													9, _T("Plate heat exchanger"), \
													10, _T("Reserved"), \
													11, _T("STAT2 or ECOM"), \
													12, _T("DHW snsr+on/off")};

	static SOLAMULTIVALUE DHWPriorityVsCH[]	= {	0, _T("CH > DHW"), \
												1, _T("DHW > CH") };

	static SOLAMULTIVALUE DHWPriorityVsLL[]	= {	0, _T("LL > DHW"), \
												1, _T("DHW > LL") };

	static SOLAMULTIVALUE DHWModSensorList[] = {	0, _T("DHW sensor"), \
												1, _T("Outlet sensor"), \
												2, _T("Inlet Sensor"), \
												3, _T("Modbus"), \
												4, _T("Auto: DHW or Inlet sensor"), \
												5, _T("Auto: DHW or Outlet sensor") };

	static SOLAMULTIVALUE DHWPrioritySource[]	= {	0, _T("Disable DHW priority"), \
													1, _T("DHW heat demand") };

	static SOLAMBMAP DHWConfiguration[] = {	_T("DHW enable"), 0x01, 0x03, 0x01C0, 0x0001, 0, true, true, true, NULL, 0, Multivalue, DHWEnableList, sizeof(DHWEnableList)/sizeof(SOLAMULTIVALUE),\
											_T("DHW demand switch"), 0x01, 0x03, 0x01C1, 0x0001, 0, true, true, true, NULL, 0,  Multivalue, DHWDemandSwitchList, sizeof(DHWDemandSwitchList)/sizeof(SOLAMULTIVALUE),\
											_T("DHW priority vs CH"), 0x01, 0x03, 0x01C2, 0x0001, 0, true, true, true, NULL, 0,  Multivalue, DHWPriorityVsCH, sizeof(DHWPriorityVsCH)/sizeof(SOLAMULTIVALUE),\
											_T("DHW priority vs LL"), 0x01, 0x03, 0x01C3, 0x0001, 0, true, true, true, NULL, 0, Multivalue, DHWPriorityVsLL, sizeof(DHWPriorityVsLL)/sizeof(SOLAMULTIVALUE),\
											_T("DHW priority time"), 0x01, 0x03, 0x01C4, 0x0001, 0, true, true, true, NULL, 0,  Timevalue, NULL, 0,\
											_T("DHW setpoint"), 0x01, 0x03, 0x01C5, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
											_T("DHW time of day setpoint"), 0x01, 0x03, 0x01C6, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
											_T("DHW on hysteresis"), 0x01, 0x03, 0x01C7, 0x0001, 0, true, true, true, NULL, 0,  Hysteresis, NULL, 0,\
											_T("DHW off hysteresis"), 0x01, 0x03, 0x01C8, 0x0001, 0, true, true, true, NULL, 0,  Hysteresis, NULL, 0,\
											_T("DHW P-gain"), 0x01, 0x03, 0x01C9, 0x0001, 0, true, true, true, NULL, 0,  Numericvalue, NULL, 0,\
											_T("DHW I-gain"), 0x01, 0x03, 0x01CA, 0x0001, 0, true, true, true, NULL, 0,  Numericvalue, NULL, 0,\
											_T("DHW D-gain"), 0x01, 0x03, 0x01CB, 0x0001, 0, true, true, true, NULL, 0,  Numericvalue, NULL, 0,\
											_T("DHW hysteresis step time"), 0x01, 0x03, 0x01CC, 0x0001, 0, true, true, true, NULL, 0,  Timevalue, NULL , 0,\
											_T("DHW modulation sensor"), 0x01, 0x03, 0x01CD, 0x0001, 0, true, true, true, NULL, 0,  Multivalue, DHWModSensorList, sizeof(DHWPriorityVsLL)/sizeof(SOLAMULTIVALUE),\
											_T("Reserved"), 0x01, 0x03, 0x01CE, 0x0001, 0, true, true, true, NULL, 0,  Novalue, NULL, 0,\
											_T("DHW priority source"), 0x01, 0x03, 0x01CF, 0x0001, 0, true, true, true, NULL, 0, Multivalue, DHWPrioritySource, sizeof(DHWPrioritySource)/sizeof(SOLAMULTIVALUE)};

	pcDHWConfiguration = (CSolaMBMap*)new CSolaMBMap(DHWConfiguration, sizeof(DHWConfiguration)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE ModOutputList[]	= {	0, _T("Fan PWM"),\
												1, _T("0-10V"),\
												2, _T("4-20mA")};

	static SOLAMULTIVALUE FiringRateControlList[]	= {	0, _T("Auto"),\
													1, _T("Manual in Run"),\
													2, _T("Manual in Run&Stdby")};

	static SOLAMBMAP ModConfiguration[] = {	_T("Mod. output"), 0x01, 0x03, 0x00C0, 0x0001, 0, true, true, false, NULL, 0, Multivalue, ModOutputList, sizeof(ModOutputList)/sizeof(SOLAMULTIVALUE),\
											_T("CH max. mod. rate"), 0x01, 0x03, 0x00C1, 0x0001, 0, true, true, true, NULL, 0,  Numericvalue, NULL, 0,\
											_T("DHW max. mod. rate"), 0x01, 0x03, 0x00C2, 0x0001, 0, true, true, true, NULL, 0,  Numericvalue, NULL, 0,\
											_T("Min. mod. rate"), 0x01, 0x03, 0x00C3, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
											_T("Prepurge rate"), 0x01, 0x03, 0x00C4, 0x0001, 0, true, true, false, NULL, 0,  Numericvalue, NULL, 0,\
											_T("Lightoff rate"), 0x01, 0x03, 0x00C5, 0x0001, 0, true, true, false, NULL, 0, Numericvalue, NULL, 0,\
											_T("Postpurge rate"), 0x01, 0x03, 0x00C6, 0x0001, 0, true, true, false, NULL, 0, Numericvalue, NULL, 0,\
											_T("CH forced rate"), 0x01, 0x03, 0x00C7, 0x0001, 0, true, true, true, NULL, 0,  Numericvalue, NULL, 0,\
											_T("CH forced rate time"), 0x01, 0x03, 0x00C8, 0x0001, 0, true, true, true, NULL, 0,  Timevalue, NULL, 0,\
											_T("DHW forced rate"), 0x01, 0x03, 0x00C9, 0x0001, 0, true, true, true, NULL, 0,  Numericvalue, NULL, 0,\
											_T("DHW forced rate time"), 0x01, 0x03, 0x00CA, 0x0001, 0, true, true, true, NULL, 0,  Timevalue, NULL, 0,\
											_T("Burner switch"), 0x01, 0x03, 0x00CB, 0x0001, 0, true, true, true, NULL, 0,  Multivalue, BooleanCodes, sizeof(BooleanCodes)/sizeof(SOLAMULTIVALUE),\
											_T("Firing rate control"), 0x01, 0x03, 0x00CC, 0x0001, 0, true, true, true, NULL, 0,  Multivalue, FiringRateControlList , sizeof(FiringRateControlList)/sizeof(SOLAMULTIVALUE),\
											_T("Manual firing rate"), 0x01, 0x03, 0x00CD, 0x0001, 0, true, true, true, NULL, 0,  Numericvalue, NULL, 0,\
											_T("Analog output hyst."), 0x01, 0x03, 0x00CE, 0x0001, 0, true, true, true, NULL, 0,  Percentvalue, NULL, 0,\
											_T("Standby rate"), 0x01, 0x03, 0x00CF, 0x0001, 0, true, true, false, NULL, 0,  Numericvalue, NULL, 0};

	pcModConfiguration = (CSolaMBMap*)new CSolaMBMap(ModConfiguration, sizeof(ModConfiguration)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE IASOpenResponse[] = { 0, _T("Lockout"),\
		1, _T("Recycle")};

	static SOLAMBMAP XModConfig[] = {_T("IAS open mod. enable"), 0x01, 0x03, 0x0218, 0x0001, 0, false, true, true, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE),\
								_T("IAS open rate diff."), 0x01, 0x03, 0x0219, 0x0001, 0, false, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("IAS open mod. step rate"), 0x01, 0x03, 0x021a, 0x0001, 0, false, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("IAS open mod. step time"), 0x01, 0x03, 0x021b, 0x0001, 0, false, true, true, NULL, 0, Timevalue, NULL, 0,\
								_T("IAS open response"), 0x01, 0x03, 0x0021c, 0x0001, 0, false, true, false, NULL, 0,  Multivalue, IASOpenResponse, sizeof(IASOpenResponse)/sizeof(SOLAMULTIVALUE)};

	pcXModConfig = (CSolaMBMap*)new CSolaMBMap(XModConfig, sizeof(XModConfig)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE PumpsList[] = {0,_T("None"),
		1,_T("Pump A"),
		2,_T("Pump B"),
		3,_T("Pump C")};

	static SOLAMULTIVALUE PumpControl[] = {0,_T("Auto"),
		1,_T("On")};

	static SOLAMULTIVALUE PumpOptions1[] = {0,_T("Lcl dmnd inhbtd faults"),
		1,_T("Norm dmnd if brnr"),
		2,_T("Norm dmnd if CH dmnd"),
		3,_T("Norm dmnd if DHW dmnd"),
		4,_T("Reserved"),
		5,_T("Norm dmnd if CH svc"),
		6,_T("Norm dmnd if DHW svc"),
		7,_T("Reserved"),
		8,_T("Norm dmnd if LL svc"),
		9,_T("Norm dmnd if LL dmnd"),
		10,_T("Reserved"),
		11,_T("Reserved"),
		12,_T("Reserved"),
		13,_T("Norm dmnd if aux X"),
		14,_T("Norm dmnd if aux Y"),
		15,_T("Norm dmnd if aux Z")};

	static SOLAMULTIVALUE PumpOptions2[] = {0,_T("Frc on outlet hi lim"),
		1,_T("Frc on brnr dmnd"),
		2,_T("Frc on LL slv dmnd"),
		3,_T("Frc on CH FP"),
		4,_T("Frc on DHW FP"),
		5,_T("Frc off DHW pri actv"),
		6,_T("Frc off CH anti-cond"),
		7,_T("Frc off DHW anti-cond"),
		8,_T("Frc off DHW hi lim"),
		9,_T("Reserved"),
		10,_T("Reserved"),
		11,_T("Reserved"),
		12,_T("Reserved"),
		13,_T("Reserved"),
		14,_T("Used for lcl dmnd"),
		15,_T("Used for LL")};

	static SOLAMBMAP PumpConfig[] = {_T("CH pump"), 0x01, 0x03, 0x0110, 0x0001, 0, false, true, false, NULL, 0, Multivalue, PumpsList, sizeof(PumpsList)/sizeof(SOLAMULTIVALUE),\
		_T("CH pump ctrl"), 0x01, 0x03, 0x0111, 0x0001, 0, false, true, false, NULL, 0, Multivalue, PumpControl, sizeof(PumpControl)/sizeof(SOLAMULTIVALUE),\
		_T("CH pump ovrn time"), 0x01, 0x03, 0x0112, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("CH FP pump ovrn time"), 0x01, 0x03, 0x0113, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("DHW pump"), 0x01, 0x03, 0x0114, 0x0001, 0, false, true, false, NULL, 0, Multivalue, PumpsList, sizeof(PumpsList)/sizeof(SOLAMULTIVALUE),\
		_T("DHW pump ctrl"), 0x01, 0x03, 0x0115, 0x0001, 0, false, true, false, NULL, 0, Multivalue, PumpControl, sizeof(PumpControl)/sizeof(SOLAMULTIVALUE),\
		_T("DHW pump ovrn time"), 0x01, 0x03, 0x0116, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("DHW FP pump ovrn time"), 0x01, 0x03, 0x0117, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("DHW pump strt dly"), 0x01, 0x03, 0x0118, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("Blr pump"), 0x01, 0x03, 0x0119, 0x0001, 0, false, true, false, NULL, 0, Multivalue, PumpsList, sizeof(PumpsList)/sizeof(SOLAMULTIVALUE),\
		_T("Blr pump ctrl"), 0x01, 0x03, 0x011a, 0x0001, 0, false, true, false, NULL, 0, Multivalue, PumpControl, sizeof(PumpControl)/sizeof(SOLAMULTIVALUE),\
		_T("Blr pump ovrn time"), 0x01, 0x03, 0x011b, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("Aux1 pump"), 0x01, 0x03, 0x011c, 0x0001, 0, false, true, false, NULL, 0, Multivalue, PumpsList, sizeof(PumpsList)/sizeof(SOLAMULTIVALUE),\
		_T("Aux1 pump ctrl"), 0x01, 0x03, 0x011d, 0x0001, 0, false, true, false, NULL, 0, Multivalue, PumpControl, sizeof(PumpControl)/sizeof(SOLAMULTIVALUE),\
		_T("RESERVED"), 0x01, 0x03, 0x011e, 0x0001, 0, false, true, false, NULL, 0, Novalue, NULL, 0,\
		_T("Sys pump"), 0x01, 0x03, 0x011f, 0x0001, 0, false, true, false, NULL, 0, Multivalue, PumpsList, sizeof(PumpsList)/sizeof(SOLAMULTIVALUE),\
		_T("Sys pump ctrl"), 0x01, 0x03, 0x0120, 0x0001, 0, false, true, false, NULL, 0, Multivalue, PumpControl, sizeof(PumpControl)/sizeof(SOLAMULTIVALUE),\
		_T("Sys pump ovrn time"), 0x01, 0x03, 0x0121, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("Pump xrcs intrvl"), 0x01, 0x03, 0x0122, 0x0001, 0, false, true, false, NULL, 0, Numericvalue, NULL, 0,\
		_T("Pump xrcs time"), 0x01, 0x03, 0x0123, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("CH pump strt dly"), 0x01, 0x03, 0x0124, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("Blr pump strt dly"), 0x01, 0x03, 0x0125, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("Sys pump strt dly"), 0x01, 0x03, 0x0126, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("Aux1 pump strt dly"), 0x01, 0x03, 0x0127, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("CH pump opts 1"), 0x01, 0x03, 0x0128, 0x0001, 0, false, true, false, NULL, 0, Bitmask, PumpOptions1, sizeof(PumpOptions1)/sizeof(SOLAMULTIVALUE),\
		_T("CH pump opts 2"), 0x01, 0x03, 0x0129, 0x0001, 0, true, true, true, NULL, 0, Bitmask, PumpOptions2, sizeof(PumpOptions2)/sizeof(SOLAMULTIVALUE),\
		_T("DHW pump opts 1"), 0x01, 0x03, 0x012a, 0x0001, 0, false, true, false, NULL, 0, Bitmask, NULL, 0,\
		_T("DHW pump opts 2"), 0x01, 0x03, 0x012b, 0x0001, 0, false, true, false, NULL, 0, Bitmask, NULL, 0,\
		_T("Blr pump opts 1"), 0x01, 0x03, 0x012c, 0x0001, 0, false, true, false, NULL, 0, Bitmask, NULL, 0,\
		_T("Blr pump opts 2"), 0x01, 0x03, 0x012d, 0x0001, 0, false, true, false, NULL, 0, Bitmask, NULL, 0,\
		_T("Sys pump opts 1"), 0x01, 0x03, 0x012e, 0x0001, 0, false, true, false, NULL, 0, Bitmask, NULL, 0,\
		_T("Sys pump opts 2"), 0x01, 0x03, 0x012f, 0x0001, 0, false, true, false, NULL, 0, Bitmask, NULL, 0};

		pcPumpConfig = (CSolaMBMap*)new CSolaMBMap(PumpConfig,sizeof(PumpConfig)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE LimitResponses[] = { 0, _T("Lockout"),\
		1, _T("Rcycl+hold")};

	static SOLAMULTIVALUE StackLimitEnables[] = { 0, _T("Disable"),\
		1, _T("Dual snsr sfty"),\
		2, _T("Sngl snsr non-sfty")};

	static SOLAMULTIVALUE StackLimitResponses[] = { 0, _T("Lockout"),\
		1, _T("Rcycl+dly")};

	static SOLAMULTIVALUE DeltaTInOutLimits[] = { 0, _T("Disable"),\
		1, _T("Enbl"),\
		2, _T("Inversion detect"),
		3, _T("Delta-T+invrsn")};

	static SOLAMULTIVALUE DeltaTResponses[] = { 0, _T("Lockout"),\
		1, _T("Rcycl+dly"),
		2, _T("Rcycl+dly w/ rtry lim")};

	static SOLAMULTIVALUE DHWLimitEnables[] = { 0, _T("Disable"),\
		1, _T("Dual snsr sfty"),\
		2, _T("Sngl snsr no-sfty")};

	static SOLAMULTIVALUE DHWLimitResponses[] = { 0, _T("Lockout"),\
		1, _T("Rcycl+hold"),
		2, _T("Suspend DHW")};

	static SOLAMULTIVALUE DeltaTRateLimitEnables[] = { 0, _T("Do not limit"),\
		1, _T("Lim nrng thrshld")};

	static SOLAMBMAP LimitsConfig[] = {_T("Outlet high lim stpt"), 0x01, 0x03, 0x01d0, 0x0001, 0, false, true, false, NULL, 0, TemperatureSetpoint, NULL, 0,\
		_T("Outlet high lim resp"), 0x01, 0x03, 0x01d1, 0x0001, 0, false, true, false, NULL, 0, Multivalue, LimitResponses, sizeof(LimitResponses)/sizeof(SOLAMULTIVALUE),\
		_T("Stack lim enable"), 0x01, 0x03, 0x01d2, 0x0001, 0, false, true, false, NULL, 0, Multivalue, StackLimitEnables, sizeof(StackLimitEnables)/sizeof(SOLAMULTIVALUE),\
		_T("Stack lim stpt"), 0x01, 0x03, 0x01d3, 0x0001, 0, false, true, false, NULL, 0, TemperatureSetpoint, NULL, 0,\
		_T("Stack lim resp"), 0x01, 0x03, 0x01d4, 0x0001, 0, false, true, false, NULL, 0, Multivalue, StackLimitResponses, sizeof(StackLimitResponses)/sizeof(SOLAMULTIVALUE),\
		_T("Stack lim delay"), 0x01, 0x03, 0x01d5, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("Delta-T in/out enbl"), 0x01, 0x03, 0x01d6, 0x0001, 0, false, true, false, NULL, 0, Multivalue, DeltaTInOutLimits, sizeof(DeltaTInOutLimits)/sizeof(SOLAMULTIVALUE),\
		_T("Delta-T in/out diff"), 0x01, 0x03, 0x01d7, 0x0001, 0, false, true, false, NULL, 0, Hysteresis, NULL, 0,\
		_T("Delta-T resp"), 0x01, 0x03, 0x01d8, 0x0001, 0, false, true, false, NULL, 0, Multivalue, DeltaTResponses, sizeof(DeltaTResponses)/sizeof(SOLAMULTIVALUE),\
		_T("Delta-T delay"), 0x01, 0x03, 0x01d9, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("DHW high lim enbl"), 0x01, 0x03, 0x01da, 0x0001, 0, false, true, false, NULL, 0, Multivalue, DHWLimitEnables, sizeof(DHWLimitEnables)/sizeof(SOLAMULTIVALUE),\
		_T("DHW lim stpt"), 0x01, 0x03, 0x01db, 0x0001, 0, false, true, false, NULL, 0, TemperatureSetpoint, NULL, 0,\
		_T("DHW lim resp"), 0x01, 0x03, 0x01dc, 0x0001, 0, false, true, false, NULL, 0, Multivalue, DHWLimitResponses, sizeof(DHWLimitResponses)/sizeof(SOLAMULTIVALUE),\
		_T("CH slow start"), 0x01, 0x03, 0x01dd, 0x0001, 0, false, true, false, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE),\
		_T("DHW slow start"), 0x01, 0x03, 0x01de, 0x0001, 0, false, true, false, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE),\
		_T("Slow start ramp"), 0x01, 0x03, 0x01df, 0x0001, 0, false, true, false, NULL, 0, Numericvalue, NULL, 0,\
		_T("Slow start stpt"), 0x01, 0x03, 0x01e0, 0x0001, 0, false, true, false, NULL, 0, TemperatureSetpoint, NULL, 0,\
		_T("Outlet T-rise enbl"), 0x01, 0x03, 0x01e1, 0x0001, 0, false, true, false, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE),\
		_T("Outlet T-rise deg/sec"), 0x01, 0x03, 0x01e2, 0x0001, 0, false, true, false, NULL, 0, Hysteresis, NULL, 0,\
		_T("Outlet T-rise delay"), 0x01, 0x03, 0x01e3, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("Outlet high lim enbl"), 0x01, 0x03, 0x01e4, 0x0001, 0, false, true, false, NULL, 0, Multivalue, DHWLimitEnables, sizeof(DHWLimitEnables)/sizeof(SOLAMULTIVALUE),\
		_T("Delta-T retry lim"), 0x01, 0x03, 0x01e5, 0x0001, 0, false, true, false, NULL, 0, Numericvalue, NULL, 0,\
		_T("Delta-T rate lim enbl"), 0x01, 0x03, 0x01e6, 0x0001, 0, false, true, false, NULL, 0, Multivalue, DeltaTResponses, sizeof(DeltaTResponses)/sizeof(SOLAMULTIVALUE),\
		_T("Delta-T invrs time"), 0x01, 0x03, 0x01e7, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
		_T("Delta-T invrs lim resp"), 0x01, 0x03, 0x01e8, 0x0001, 0, false, true, false, NULL, 0, Multivalue, DeltaTResponses, sizeof(DeltaTResponses)/sizeof(SOLAMULTIVALUE),\
		_T("Delta-T xchgr/out enbl"), 0x01, 0x03, 0x01e9, 0x0001, 0, false, true, false, NULL, 0, Multivalue, DeltaTInOutLimits, sizeof(DeltaTInOutLimits)/sizeof(SOLAMULTIVALUE),\
		_T("Delta-T xchgr/out diff"), 0x01, 0x03, 0x01ea, 0x0001, 0, false, true, false, NULL, 0, Hysteresis, NULL, 0,\
		_T("Xchgr T-rise enbl"), 0x01, 0x03, 0x01eb, 0x0001, 0, false, true, false, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE),\
		_T("T-rise resp"), 0x01, 0x03, 0x01ec, 0x0001, 0, false, true, false, NULL, 0, Multivalue, DeltaTResponses, sizeof(DeltaTResponses)/sizeof(SOLAMULTIVALUE),\
		_T("T-rise retry lim"), 0x01, 0x03, 0x01ed, 0x0001, 0, false, true, false, NULL, 0, Numericvalue, NULL, 0,\
		_T("Delta-T in/xchgr enbl"), 0x01, 0x03, 0x01ee, 0x0001, 0, false, true, false, NULL, 0, Multivalue, DeltaTInOutLimits, sizeof(DeltaTInOutLimits)/sizeof(SOLAMULTIVALUE),\
		_T("Delta-T in/xchgr diff"), 0x01, 0x03, 0x01ef, 0x0001, 0, false, true, false, NULL, 0, Hysteresis, NULL, 0};

		pcLimitsConfig = (CSolaMBMap*)new CSolaMBMap(LimitsConfig,sizeof(LimitsConfig)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE IgnitionSources[] = {
0,_T("Internal ignition"),
1,_T("External ignition"),
2,_T("Hot surf. ign.(HSI)")};

	static SOLAMULTIVALUE BlowerHSIFuncs[] = {
0,_T("Blower motor"),
1,_T("Hot surf. ign.(HSI)")};

	static SOLAMULTIVALUE IgniterDuration[] = {
0,_T("All PFEP"),
1,_T("1st half PFEP")};

	static SOLAMULTIVALUE PilotTypes[] = {
0,_T("Interrupted"),
1,_T("Intermittent"),
2,_T("Direct const. ign."),
3,_T("Direct pulsd. ign.")};

	static SOLAMULTIVALUE FlameSensorTypes[] = {
0,_T("None"),
1,_T("Flame rod"),
2,_T("UV power tube"),
3,_T("UV PT ignore flame drng ign.")};

	static SOLAMULTIVALUE PurgeRateProvingTypes[] = {
0,_T("None"),
1,_T("High fire sw.(HFS)"),
2,_T("Fan speed")};

	static SOLAMULTIVALUE LightoffRateProvingTypes[] = {
0,_T("None"),
1,_T("Low fire sw.(LFS)"),
2,_T("Fan speed"),
3,_T("Fan speed except during ign.")};

	static SOLAMULTIVALUE PilotFlameEstablishTimes[] = {
0,_T("None"),
1,_T("4 seconds"),
2,_T("10 seconds"),
3,_T("15 seconds"),
0xffff,_T("Not configd")};

	static SOLAMULTIVALUE MainFlameEstablishTimes[] = {
0,_T("None"),
1,_T("5 seconds"),
2,_T("10 seconds"),
3,_T("15 seconds"),
0xffff,_T("Not configd")};

	static SOLAMULTIVALUE InterlockStartChecks[] = {
0,_T("No ILK check"),
1,_T("ILK check")};

	static SOLAMULTIVALUE InterlockOpenResponses[] = {
0,_T("Lockout"),
1,_T("Recycle")};

	static SOLAMULTIVALUE IgniteFailureResponses[] = {
0,_T("Lockout"),
1,_T("Cont. recycle"),
2,_T("Retry,recyc.+hold"),
3,_T("Retry,recyc.+lockout")};

	static SOLAMULTIVALUE IgniteFailureRetries[] = {
0,_T("None"),
1,_T("3 times"),
2,_T("5 times"),
3,_T("1 time"),
0xffff,_T("Not configd")};

	static SOLAMULTIVALUE InterruptedAirSwitchEnables[] = {
0,_T("Disabled"),
1,_T("Enabled for purge"),
2,_T("Enabled for purge+ignition"),
3,_T("Enabled all states")};

	static SOLAMBMAP BurnerControlConfig[] = {_T("Ignition src"), 0x01, 0x03, 0x00e0, 0x0001, 0, false, true, false, NULL, 0, Multivalue, IgnitionSources, sizeof(IgnitionSources)/sizeof(SOLAMULTIVALUE),\
_T("BLR/HSI func."), 0x01, 0x03, 0x00e1, 0x0001, 0, false, true, false, NULL, 0, Multivalue, BlowerHSIFuncs, sizeof(BlowerHSIFuncs)/sizeof(SOLAMULTIVALUE),\
_T("Ign. on during"), 0x01, 0x03, 0x00e2, 0x0001, 0, false, true, false, NULL, 0, Multivalue, IgniterDuration, sizeof(IgniterDuration)/sizeof(SOLAMULTIVALUE),\
_T("Pilot type"), 0x01, 0x03, 0x00e3, 0x0001, 0, false, true, false, NULL, 0, Multivalue, PilotTypes, sizeof(PilotTypes)/sizeof(SOLAMULTIVALUE),\
_T("Flame sens. type"), 0x01, 0x03, 0x00e4, 0x0001, 0, false, true, false, NULL, 0, Multivalue, FlameSensorTypes, sizeof(FlameSensorTypes)/sizeof(SOLAMULTIVALUE),\
_T("Purge rate prvng."), 0x01, 0x03, 0x00e5, 0x0001, 0, false, true, false, NULL, 0, Multivalue, PurgeRateProvingTypes, sizeof(PurgeRateProvingTypes)/sizeof(SOLAMULTIVALUE),\
_T("Lightoff rate prvng."), 0x01, 0x03, 0x00e6, 0x0001, 0, false, true, false, NULL, 0, Multivalue, LightoffRateProvingTypes, sizeof(LightoffRateProvingTypes)/sizeof(SOLAMULTIVALUE),\
_T("Prepurge time"), 0x01, 0x03, 0x00e7, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
_T("Pre-ignition time"), 0x01, 0x03, 0x00e8, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
_T("Pilot flame estab. pd."), 0x01, 0x03, 0x00e9, 0x0001, 0, false, true, false, NULL, 0, Multivalue, PilotFlameEstablishTimes, sizeof(PilotFlameEstablishTimes)/sizeof(SOLAMULTIVALUE),\
_T("Main flame estab. pd."), 0x01, 0x03, 0x00ea, 0x0001, 0, false, true, false, NULL, 0, Multivalue, MainFlameEstablishTimes, sizeof(MainFlameEstablishTimes)/sizeof(SOLAMULTIVALUE),\
_T("Run stblz. time"), 0x01, 0x03, 0x00eb, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
_T("Postpurge time"), 0x01, 0x03, 0x00ec, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
_T("Intlk. strt. chk. nbl."), 0x01, 0x03, 0x00ed, 0x0001, 0, false, true, false, NULL, 0, Multivalue, InterlockStartChecks, sizeof(InterlockStartChecks)/sizeof(SOLAMULTIVALUE),\
_T("Intlk. open resp."), 0x01, 0x03, 0x00ee, 0x0001, 0, false, true, false, NULL, 0, Multivalue, InterlockOpenResponses, sizeof(InterlockOpenResponses)/sizeof(SOLAMULTIVALUE),\
_T("Ignite fail resp."), 0x01, 0x03, 0x00ef, 0x0001, 0, false, true, false, NULL, 0, Multivalue, IgniteFailureResponses, sizeof(IgniteFailureResponses)/sizeof(SOLAMULTIVALUE),\
_T("Ignite fail retries"), 0x01, 0x03, 0x00f0, 0x0001, 0, false, true, false, NULL, 0, Multivalue, IgniteFailureRetries, sizeof(IgniteFailureRetries)/sizeof(SOLAMULTIVALUE),\
_T("Ignite fail delay"), 0x01, 0x03, 0x00f1, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
_T("MFEP flame fail resp."), 0x01, 0x03, 0x00f2, 0x0001, 0, false, true, false, NULL, 0, Multivalue, InterlockOpenResponses, sizeof(InterlockOpenResponses)/sizeof(SOLAMULTIVALUE),\
_T("Run flame fail resp."), 0x01, 0x03, 0x00f3, 0x0001, 0, false, true, false, NULL, 0, Multivalue, InterlockOpenResponses, sizeof(InterlockOpenResponses)/sizeof(SOLAMULTIVALUE),\
_T("Pilot test hold"), 0x01, 0x03, 0x00f4, 0x0001, 0, false, true, false, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE),\
_T("RESERVED"), 0x01, 0x03, 0x00f5, 0x0001, 0, false, true, false, NULL, 0, Novalue, NULL, 0,\
_T("IAS enable"), 0x01, 0x03, 0x00f6, 0x0001, 0, false, true, false, NULL, 0, Multivalue, InterruptedAirSwitchEnables, sizeof(InterruptedAirSwitchEnables)/sizeof(SOLAMULTIVALUE),\
_T("IAS strt. chk. nbl."), 0x01, 0x03, 0x00f7, 0x0001, 0, false, true, false, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE),\
_T("LCI enable"), 0x01, 0x03, 0x00f8, 0x0001, 0, false, true, false, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE),\
_T("PII enable"), 0x01, 0x03, 0x00f9, 0x0001, 0, false, true, false, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE),\
_T("Flame threshold"), 0x01, 0x03, 0x00fa, 0x0001, 0, false, true, false, NULL, 0, Decimal1pl, NULL, 0,\
_T("RESERVED"), 0x01, 0x03, 0x00fb, 0x0001, 0, false, true, false, NULL, 0, Novalue, NULL, 0,\
_T("RESERVED"), 0x01, 0x03, 0x00fc, 0x0001, 0, false, true, false, NULL, 0, Novalue, NULL, 0,\
_T("ILK/LCI bounce detect"), 0x01, 0x03, 0x00fd, 0x0001, 0, false, true, false, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE),\
_T("Frcd. rcyc. intrvl."), 0x01, 0x03, 0x00fe, 0x0001, 0, false, true, false, NULL, 0, Timevalue, NULL, 0,\
_T("Fan spd. err. resp."), 0x01, 0x03, 0x00ff, 0x0001, 0, false, true, false, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE)};

	pcBurnerControlConfig = (CSolaMBMap*)new CSolaMBMap(BurnerControlConfig,sizeof(BurnerControlConfig)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE LLMasterStatusCodes[] = {	0, _T("Unknown"),
									1, _T("Disabled"),
									2, _T("Normal"),
									3, _T("Suspended")};

	static SOLAMULTIVALUE LLSlaveStatusCodes[] = {	0, _T("Disabled"),
									1, _T("Lockout"),
									2, _T("Reserved"),
									3, _T("Reserved"),
									4, _T("Anti short-cycle"),
									5, _T("Unconfig. safety"),
									6, _T("Reserved"),
									7, _T("Reserved"),
									8, _T("Reserved"),
									9, _T("Lcl frz prot req"),
									10, _T("Frz prot burner req"),
									11, _T("DHW frz prot req"),
									12, _T("CH frz prot req"),
									13, _T("Slave is modulating"),
									14, _T("Slave mode > CH & DHW"),
									15, _T("Slave cmd recvd")};

	static SOLAMULTIVALUE LLMasterSPSrcCodes[] = {	0, _T("Unknown"),
										1, _T("Normal setpoint"),
										2, _T("TOD setpoint"),
										3, _T("OD reset")};

	static SOLAMBMAP LLStatus[] = {	_T("LL master status"), 0x01, 0x03, 0x00A0, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLMasterStatusCodes, sizeof(LLMasterStatusCodes)/sizeof(SOLAMULTIVALUE),\
								_T("LL slave status"), 0x01, 0x03, 0x00A1, 0x0001, 0, true, true, true, NULL, 0, Bitmask, LLSlaveStatusCodes, sizeof(LLSlaveStatusCodes)/sizeof(SOLAMULTIVALUE),\
								_T("LL master SP src"), 0x01, 0x03, 0x00A2, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLMasterSPSrcCodes, sizeof(LLMasterSPSrcCodes)/sizeof(SOLAMULTIVALUE),\
								_T("LL master heat dmnd"), 0x01, 0x03, 0x00A3, 0x0001, 0, true, true, true, NULL, 0, Multivalue, BooleanCodes, sizeof(BooleanCodes)/sizeof(SOLAMULTIVALUE),\
								_T("LL slave burner dmnd"), 0x01, 0x03, 0x00A4, 0x0001, 0, true, true, true, NULL, 0, Multivalue, BooleanCodes, sizeof(BooleanCodes)/sizeof(SOLAMULTIVALUE),\
								_T("LL slave req rate"), 0x01, 0x03, 0x00A5, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0};
	
	pcLLStatus = (CSolaMBMap*)new CSolaMBMap(LLStatus, sizeof(LLStatus)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE XLLActiveServiceCodes[] = {	0, _T("No active service"),
									1, _T("CH"),
									2, _T("DHW"),
									3, _T("Mix"),
									4, _T("Frz prot"),
									5, _T("Reserved"),
									6, _T("Reserved"),
									7, _T("Reserved"),
									8, _T("Reserved"),
									9, _T("Reserved"),
									10, _T("Reserved"),
									11, _T("Reserved"),
									12, _T("Reserved"),
									13, _T("Reserved"),
									14, _T("Reserved"),
									15, _T("WWSD") };

	static SOLAMULTIVALUE XLLSlaveStateCodes[] = {	0, _T("Slv unkn"),
									1, _T("Avlbl"),
									2, _T("+ stg"),
									3, _T("Sspnd stg"),
									4, _T("Frng"),
									5, _T("On lve"),
									6, _T("Dsbld"),
									7, _T("Rcvrng") };

	static SOLAMBMAP XLLStatus[] = {	_T("LL active service"), 0x01, 0x03, 0x0300, 0x0001, 0, true, true, true, NULL, 0, Multivalue, XLLActiveServiceCodes, sizeof(XLLActiveServiceCodes)/sizeof(SOLAMULTIVALUE),\
								_T("Slv 1 addr"), 0x01, 0x03, 0x0301, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 1 state"), 0x01, 0x03, 0x0302, 0x0001, 0, true, true, true, NULL, 0, Multivalue, XLLSlaveStateCodes, sizeof(XLLSlaveStateCodes)/sizeof(SOLAMULTIVALUE),\
								_T("Slv 1 stg ord"), 0x01, 0x03, 0x0303, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 1 rate"), 0x01, 0x03, 0x0304, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 2 addr"), 0x01, 0x03, 0x0305, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 2 state"), 0x01, 0x03, 0x0306, 0x0001, 0, true, true, true, NULL, 0, Multivalue, XLLSlaveStateCodes, sizeof(XLLSlaveStateCodes)/sizeof(SOLAMULTIVALUE),\
								_T("Slv 2 stg ord"), 0x01, 0x03, 0x0307, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 2 rate"), 0x01, 0x03, 0x0308, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 3 addr"), 0x01, 0x03, 0x0309, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 3 state"), 0x01, 0x03, 0x030A, 0x0001, 0, true, true, true, NULL, 0, Multivalue, XLLSlaveStateCodes, sizeof(XLLSlaveStateCodes)/sizeof(SOLAMULTIVALUE),\
								_T("Slv 3 stg ord"), 0x01, 0x03, 0x030B, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 3 rate"), 0x01, 0x03, 0x030C, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 4 addr"), 0x01, 0x03, 0x030D, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 4 state"), 0x01, 0x03, 0x030E, 0x0001, 0, true, true, true, NULL, 0, Multivalue, XLLSlaveStateCodes, sizeof(XLLSlaveStateCodes)/sizeof(SOLAMULTIVALUE),\
								_T("Slv 4 stg ord"), 0x01, 0x03, 0x030F, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 4 rate"), 0x01, 0x03, 0x0310, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 5 addr"), 0x01, 0x03, 0x0311, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 5 state"), 0x01, 0x03, 0x0312, 0x0001, 0, true, true, true, NULL, 0, Multivalue, XLLSlaveStateCodes, sizeof(XLLSlaveStateCodes)/sizeof(SOLAMULTIVALUE),\
								_T("Slv 5 stg ord"), 0x01, 0x03, 0x0313, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 5 rate"), 0x01, 0x03, 0x0314, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 6 addr"), 0x01, 0x03, 0x0315, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 6 state"), 0x01, 0x03, 0x0316, 0x0001, 0, true, true, true, NULL, 0, Multivalue, XLLSlaveStateCodes, sizeof(XLLSlaveStateCodes)/sizeof(SOLAMULTIVALUE),\
								_T("Slv 6 stg ord"), 0x01, 0x03, 0x0317, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 6 rate"), 0x01, 0x03, 0x0318, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 7 addr"), 0x01, 0x03, 0x0319, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 7 state"), 0x01, 0x03, 0x031A, 0x0001, 0, true, true, true, NULL, 0, Multivalue, XLLSlaveStateCodes, sizeof(XLLSlaveStateCodes)/sizeof(SOLAMULTIVALUE),\
								_T("Slv 7 stg ord"), 0x01, 0x03, 0x031B, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 7 rate"), 0x01, 0x03, 0x031C, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 8 addr"), 0x01, 0x03, 0x031D, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 8 state"), 0x01, 0x03, 0x031E, 0x0001, 0, true, true, true, NULL, 0, Multivalue, XLLSlaveStateCodes, sizeof(XLLSlaveStateCodes)/sizeof(SOLAMULTIVALUE),\
								_T("Slv 8 stg ord"), 0x01, 0x03, 0x031F, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Slv 8 rate"), 0x01, 0x03, 0x0320, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Lead boiler addr"), 0x01, 0x03, 0x0321, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("Master rate"), 0x01, 0x03, 0x0322, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0 };
	
	pcXLLStatus = (CSolaMBMap*)new CSolaMBMap(XLLStatus, sizeof(XLLStatus)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE LLSlaveEnableCodes[] = {	0, _T("Disabled"),
													1, _T("ECOM slv"),
													2, _T("Modbbus slv"),
													3, _T("SOLA slv")};

	static SOLAMULTIVALUE LLMasterEnableCodes[] = {	0, _T("Disabled"),
													1, _T("Enabled")};

	static SOLAMULTIVALUE LLCHDemandSwitchCodes[] = {	0, _T("Disable CH loop"),
													1, _T("STAT"),
													2, _T("Reserved"),
													3, _T("ECOM rmt STAT"),
													4, _T("Modbus STAT"),
													5, _T("Reserved"),
													6, _T("STAT or ECOM rmt STAT")};

	static SOLAMULTIVALUE LLCHSetpointSourceCodes[] = {	0, _T("Local stpt"),
										1, _T("Modbus stpt"),
										2, _T("4-20mA stpt")};

	static SOLAMULTIVALUE LLModulationSensorCodes[] = {	0, _T("S5 snsr"),
														1, _T("S10 snsr")};

	static SOLAMULTIVALUE LLBackupModulationSensorCodes[] = {	0, _T("No bkup snsr"),
														1, _T("Lead blr outlet"),
														2, _T("Slv outlets avg")};

	static SOLAMULTIVALUE LLCHModbusSTATCodes[] = {	0, _T("No heat call"),
													1, _T("Heat call")};

	static SOLAMULTIVALUE LLSlaveModeCodes[] = {	0, _T("Use first"),
										1, _T("Equalize run-time"),
										2, _T("Use last")};

	static SOLAMULTIVALUE LLSlaveCommandCodes[] = {	0x0100, _T("Cmd rate is bin fraction"),
													0x0200, _T("Slv pump dmnd"),
													0x0400, _T("Turn on aux pump Z"),
													0x0800, _T("Turn on aux pump Y"),
													0x1000, _T("Turn on aux pump X"),
													0x2000, _T("Slv run fan req"),
													0x4000, _T("Slv sspnd startup"),
													0x8000, _T("Slv sspnd startup")};

	static SOLAMULTIVALUE LLModbusPortCodes[] = {	0, _T("No port"),
										1, _T("MB1"),
										2, _T("MB2")};

	static SOLAMULTIVALUE LLRateAllocationMethodCodes[] = {0, _T("Pll comm base lmtd")};

	static SOLAMULTIVALUE LLLeadLagAllocationMethodCodes[] = {	0, _T("Seq order rtn"),
															1, _T("Lwst msrd run time")};

	static SOLAMBMAP LLConfig[] = {	_T("Slv enbl"), 0x01, 0x03, 0x0220, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLSlaveEnableCodes, sizeof(LLSlaveEnableCodes)/sizeof(SOLAMULTIVALUE),\
									_T("Mstr enbl"), 0x01, 0x03, 0x0221, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLMasterEnableCodes, sizeof(LLMasterEnableCodes)/sizeof(SOLAMULTIVALUE),\
									_T("Setpoint"), 0x01, 0x03, 0x0222, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
									_T("TOD setpoint"), 0x01, 0x03, 0x0223, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
									_T("OD reset enbl"), 0x01, 0x03, 0x0224, 0x0001, 0, true, true, true, NULL, 0, Multivalue, DHWEnableList, sizeof(DHWEnableList)/sizeof(SOLAMULTIVALUE),\
									_T("On hyst"), 0x01, 0x03, 0x0225, 0x0001, 0, true, true, true, NULL, 0, Hysteresis, NULL, 0,\
									_T("Off hyst"), 0x01, 0x03, 0x0226, 0x0001, 0, true, true, true, NULL, 0, Hysteresis, NULL, 0,\
									_T("Hyst step time"), 0x01, 0x03, 0x0227, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
									_T("P-gain"), 0x01, 0x03, 0x0228, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
									_T("I-gain"), 0x01, 0x03, 0x0229, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
									_T("D-gain"), 0x01, 0x03, 0x022A, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
									_T("Op sw"), 0x01, 0x03, 0x022B, 0x0001, 0, true, true, true, NULL, 0, Multivalue, BooleanCodes, sizeof(BooleanCodes)/sizeof(SOLAMULTIVALUE),\
									_T("CH dmnd sw"), 0x01, 0x03, 0x022C, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLCHDemandSwitchCodes, sizeof(LLCHDemandSwitchCodes)/sizeof(SOLAMULTIVALUE),\
									_T("CH stpt src"), 0x01, 0x03, 0x022D, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLCHSetpointSourceCodes, sizeof(LLCHSetpointSourceCodes)/sizeof(SOLAMULTIVALUE),\
									_T("Mod snsr"), 0x01, 0x03, 0x022E, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLModulationSensorCodes, sizeof(LLModulationSensorCodes)/sizeof(SOLAMULTIVALUE),\
									_T("Bkup mod snsr"), 0x01, 0x03, 0x022F, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLBackupModulationSensorCodes, sizeof(LLBackupModulationSensorCodes)/sizeof(SOLAMULTIVALUE),\
									_T("CH 4ma tmp"), 0x01, 0x03, 0x0230, 0x0001, 0, true, true, true, NULL, 0, Temperature, NULL, 0,\
									_T("CH 20ma tmp"), 0x01, 0x03, 0x0231, 0x0001, 0, true, true, true, NULL, 0, Temperature, NULL, 0,\
									_T("CH Mbus stpt"), 0x01, 0x03, 0x0232, 0x0001, 0, true, true, true, NULL, 0, Temperature, NULL, 0,\
									_T("CH Mbus STAT"), 0x01, 0x03, 0x0233, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLCHModbusSTATCodes, sizeof(LLCHModbusSTATCodes)/sizeof(SOLAMULTIVALUE),\
									_T("Slave mode"), 0x01, 0x03, 0x0234, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLSlaveModeCodes, sizeof(LLSlaveModeCodes)/sizeof(SOLAMULTIVALUE),\
									_T("Slave cmd"), 0x01, 0x03, 0x0235, 0x0001, 0, true, true, true, NULL, 0, Bitmask, LLSlaveCommandCodes, sizeof(LLSlaveCommandCodes)/sizeof(SOLAMULTIVALUE),\
									_T("Bs ld rt"), 0x01, 0x03, 0x0236, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
									_T("Fn drng off ccl rt"), 0x01, 0x03, 0x0237, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
									_T("Slv seq order"), 0x01, 0x03, 0x0238, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
									_T("Mbus port"), 0x01, 0x03, 0x0239, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLModbusPortCodes, sizeof(LLModbusPortCodes)/sizeof(SOLAMULTIVALUE),\
									_T("Slv d-t-f dly"), 0x01, 0x03, 0x023A, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
									_T("Slv cpcty"), 0x01, 0x03, 0x023B, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
									_T("Base ld comm rt"), 0x01, 0x03, 0x023C, 0x0001, 0, true, true, true, NULL, 0, Percentvalue, NULL, 0,\
									_T("Rt alloc mthd"), 0x01, 0x03, 0x023D, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLRateAllocationMethodCodes, sizeof(LLRateAllocationMethodCodes)/sizeof(SOLAMULTIVALUE),\
									_T("Ld alloc mthd"), 0x01, 0x03, 0x023E, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLLeadLagAllocationMethodCodes, sizeof(LLLeadLagAllocationMethodCodes)/sizeof(SOLAMULTIVALUE),\
									_T("Lg alloc mthd"), 0x01, 0x03, 0x023F, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLLeadLagAllocationMethodCodes, sizeof(LLLeadLagAllocationMethodCodes)/sizeof(SOLAMULTIVALUE)};

	pcLLConfig = (CSolaMBMap*)new CSolaMBMap(LLConfig, sizeof(LLConfig)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE LLDHWDemandSwitch[] =  {0,_T("DHW loop dsabld"),\
		1,_T("STAT terminal"),\
		2,_T("Reserved"),\
		3,_T("ECOM DHW rqst")};

	static SOLAMULTIVALUE LLDHWPriorityVsCH[] =  {0,_T("CH > DHW"),\
		1,_T("DHW > CH")};

	static SOLAMULTIVALUE LLSlaveDropoutReturnCompensation[] =  {0,_T("No slv compnstn"),\
		1,_T("Rplc drpout immed"),\
		2,_T("Adjst rate remain slvs"),\
		3,_T("Rplc&adjst")};

	static SOLAMULTIVALUE LLAddStageMethod[] =  {0,_T("No add stage"),\
		1,_T("Use err thrshld"),\
		2,_T("Use frng rate thrshld"),\
		3,_T("Use err rate chng&thrshld"),
		4,_T("Use frng rate chng&thrshld")};

	static SOLAMULTIVALUE LLDropStageMethod[] =  {0,_T("No drop stage"),\
		1,_T("Use err thrshld"),\
		2,_T("Use frng rate thrshld"),\
		3,_T("Use err rate chng&thrshld"),
		4,_T("Use frng rate chng&thrshld")};

	static SOLAMULTIVALUE LLBoilerOffOptions[] =  {0,_T("None"),\
		1,_T("All off if thrshld"),\
		2,_T("Drop lead @ err thrshld"),\
		3,_T("All off&drop lead @ thrshld")};

	static SOLAMBMAP XLLConfig[] = {_T("LL DHW dmnd sw."), 0x01, 0x03, 0x02c0, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLDHWDemandSwitch, sizeof(LLDHWDemandSwitch)/sizeof(SOLAMULTIVALUE),\
		_T("LL DHW stpt"), 0x01, 0x03, 0x02c1, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
		_T("LL DHW pty. vs. CH"), 0x01, 0x03, 0x02c2, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLDHWPriorityVsCH, sizeof(LLDHWPriorityVsCH)/sizeof(SOLAMULTIVALUE),\
		_T("Reserved"), 0x01, 0x03, 0x02c3, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("Reserved"), 0x01, 0x03, 0x02c4, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("Reserved"), 0x01, 0x03, 0x02c5, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("Reserved"), 0x01, 0x03, 0x02c6, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("Reserved"), 0x01, 0x03, 0x02c7, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("Reserved"), 0x01, 0x03, 0x02c8, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("Slv drpout/rtn comp."), 0x01, 0x03, 0x02c9, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLSlaveDropoutReturnCompensation, sizeof(LLSlaveDropoutReturnCompensation)/sizeof(SOLAMULTIVALUE),\
		_T("Add stg mthd"), 0x01, 0x03, 0x02ca, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLAddStageMethod, sizeof(LLAddStageMethod)/sizeof(SOLAMULTIVALUE),\
		_T("Reserved"), 0x01, 0x03, 0x02cb, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("Add stg dtct time"), 0x01, 0x03, 0x02cc, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
		_T("Reserved"), 0x01, 0x03, 0x02cd, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("Add stg err thrshld"), 0x01, 0x03, 0x02ce, 0x0001, 0, true, true, true, NULL, 0, Hysteresis, NULL, 0,\
		_T("Add stg rate offset"), 0x01, 0x03, 0x02cf, 0x0001, 0, true, true, true, NULL, 0, Percentvalue, NULL, 0,\
		_T("Add stg err gain"), 0x01, 0x03, 0x02d0, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
		_T("Add stg rate gain"), 0x01, 0x03, 0x02d1, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
		_T("Add stg intrstg dly"), 0x01, 0x03, 0x02d2, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
		_T("Drop stg mthd"), 0x01, 0x03, 0x02d3, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLDropStageMethod, sizeof(LLDropStageMethod)/sizeof(SOLAMULTIVALUE),\
		_T("Reserved"), 0x01, 0x03, 0x02d4, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("Drop stg dtct time"), 0x01, 0x03, 0x02d5, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
		_T("Reserved"), 0x01, 0x03, 0x02d6, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("Drop stg err thrshld"), 0x01, 0x03, 0x02d7, 0x0001, 0, true, true, true, NULL, 0, Hysteresis, NULL, 0,\
		_T("Drop stg rate offset"), 0x01, 0x03, 0x02d8, 0x0001, 0, true, true, true, NULL, 0, Percentvalue, NULL, 0,\
		_T("Drop stg err gain"), 0x01, 0x03, 0x02d9, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
		_T("Drop stg rate gain"), 0x01, 0x03, 0x02da, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
		_T("Drop stg intrstg dly"), 0x01, 0x03, 0x02db, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
		_T("Reserved"), 0x01, 0x03, 0x02dc, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("Lead rotn time"), 0x01, 0x03, 0x02dd, 0x0001, 0, true, true, true, NULL, 0, Minutes, NULL, 0,\
		_T("Frc lead rotn time"), 0x01, 0x03, 0x02de, 0x0001, 0, true, true, true, NULL, 0, Minutes, NULL, 0,\
		_T("LL DHW TOD stpt"), 0x01, 0x03, 0x02df, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
		_T("Blr off optns"), 0x01, 0x03, 0x02e0, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LLBoilerOffOptions, sizeof(LLBoilerOffOptions)/sizeof(SOLAMULTIVALUE),\
		_T("All blrs off thrshld"), 0x01, 0x03, 0x02e1, 0x0001, 0, true, true, true, NULL, 0, Hysteresis, NULL, 0};

	pcXLLConfig = (CSolaMBMap*)new CSolaMBMap(XLLConfig, sizeof(XLLConfig)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE ODTemperatureSourceCodes[] = {	0, _T("Uncfgd"),
															1, _T("Snsr S5"),
															2, _T("Snsn S10"),
															3, _T("Modbus"),
															4, _T("ECOM snsr"),
															5, _T("C7089 on S10")};

	static SOLAMULTIVALUE WarmWXShutdownEnableCodes[] = {	0, _T("Disable"),
															1, _T("Shtdn after dmnd"),
															2, _T("Shtdn immed")};

	static SOLAMULTIVALUE LineFrequencyCodes[] = {	0, _T("60 Hz"),
													1, _T("50 Hz"),
													2, _T("Auto dtct 48-63 Hz")};

	static SOLAMULTIVALUE SafetyConfigOptionsCodes[] = {	0x0001, _T("Ignore flame rod faults when UV is configured"),
															0x0002, _T("Rsrvd"),
															0x0004, _T("Rsrvd"),
															0x0008, _T("Rsrvd"),
															0x0010, _T("Rsrvd"),
															0x0020, _T("Rsrvd"),
															0x0040, _T("Rsrvd"),
															0x0080, _T("Rsrvd"),
															0x0100, _T("Rsrvd"),
															0x0200, _T("Rsrvd"),
															0x0400, _T("Rsrvd"),
															0x0800, _T("Rsrvd"),
															0x1000, _T("Rsrvd"),
															0x2000, _T("Rsrvd"),
															0x4000, _T("Rsrvd"),
															0x8000, _T("Rsrvd")};
	static SOLAMBMAP XSystemConfig[] = {_T("OD tmp src"), 0x01, 0x03, 0x0272, 0x0001, 0, true, true, true, NULL, 0, Multivalue, ODTemperatureSourceCodes, sizeof(ODTemperatureSourceCodes)/sizeof(SOLAMULTIVALUE),\
		_T("WWSD enable"), 0x01, 0x03, 0x0273, 0x0001, 0, true, true, true, NULL, 0, Multivalue, WarmWXShutdownEnableCodes, sizeof(WarmWXShutdownEnableCodes)/sizeof(SOLAMULTIVALUE),\
		_T("WWSD setpoint"), 0x01, 0x03, 0x0274, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
		_T("ECOM rmt stat"), 0x01, 0x03, 0x0275, 0x0001, 0, true, true, true, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE),\
		_T("Ln freq"), 0x01, 0x03, 0x0276, 0x0001, 0, true, true, true, NULL, 0, Multivalue, LineFrequencyCodes, sizeof(LineFrequencyCodes)/sizeof(SOLAMULTIVALUE),\
		_T("Sfty cfg optns"), 0x01, 0x03, 0x0277, 0x0001, 0, true, true, true, NULL, 0, Bitmask, SafetyConfigOptionsCodes, sizeof(SafetyConfigOptionsCodes)/sizeof(SOLAMULTIVALUE)};

	pcXSystemConfig = (CSolaMBMap*)new CSolaMBMap(XSystemConfig, sizeof(XSystemConfig)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE LockoutDescriptions[] = {
0,_T("None"),
1,_T("Unconfigured safety data Lockout"),
2,_T("Waiting for safety data verification Lockout"),
3,_T("Internal fault: Hardware fault Hold"),
4,_T("Internal fault: Safety Relay key feedback error Hold"),
5,_T("Internal fault: Unstable power (DCDC) output Hold"),
6,_T("Internal fault: Invalid processor clock Hold"),
7,_T("Internal fault: Safety relay drive error Hold"),
8,_T("Internal fault: Zero crossing not detected Hold"),
9,_T("Internal fault: Flame bias out of range Hold"),
10,_T("Internal fault: Invalid Burner control state Lockout"),
11,_T("Internal fault: Invalid Burner control state flag Lockout"),
12,_T("Internal fault: Safety relay drive cap short Hold"),
13,_T("Internal fault: PII shorted to ILK Hold/Lockout"),
14,_T("Internal fault: HFS shorted to LCI Hold/Lockout"),
15,_T("Internal fault: Safety relay test failed due to feedback ON Lockout"),
16,_T("Internal fault: Safety relay test failed due to safety relay OFF Lockout"),
17,_T("Internal fault: Safety relay test failed due to safety relay not OFF Lockout"),
18,_T("Internal fault: Safety relay test failed due to feedback not ON Lockout"),
19,_T("Internal fault: Safety RAM write Lockout"),
20,_T("Internal fault: Flame ripple and overflow Hold"),
21,_T("Internal fault: Flame number of sample mismatch Hold"),
22,_T("Internal fault: Flame bias out of range Hold"),
23,_T("Internal fault: Bias changed since heating cycle starts Hold"),
24,_T("Internal fault: Spark voltage stuck low or high Hold"),
25,_T("Internal fault: Spark voltage changed too much during flame sensing time Hold"),
26,_T("Internal fault: Static flame ripple Hold"),
27,_T("Internal fault: Flame rod shorted to ground detected Hold"),
28,_T("Internal fault: A/D linearity test fails Hold"),
29,_T("Internal fault: Flame bias cannot be set in range Hold"),
30,_T("Internal fault: Flame bias shorted to adjacent pin Hold"),
31,_T("Internal fault: SLO electronics unknown error Hold"),
32,_T("Internal fault: Safety Key 0 Lockout"),
33,_T("Internal fault: Safety Key 1 Lockout"),
34,_T("Internal fault: Safety Key 2 Lockout"),
35,_T("Internal fault: Safety Key 3 Lockout"),
36,_T("Internal fault: Safety Key 4 Lockout"),
37,_T("Internal fault: Safety Key 5 Lockout"),
38,_T("Internal fault: Safety Key 6 Lockout"),
39,_T("Internal fault: Safety Key 7 Lockout"),
40,_T("Internal fault: Safety Key 8 Lockout"),
41,_T("Internal fault: Safety Key 9 Lockout"),
42,_T("Internal fault: Safety Key 10 Lockout"),
43,_T("Internal fault: Safety Key 11 Lockout"),
44,_T("Internal fault: Safety Key 12 Lockout"),
45,_T("Internal fault: Safety Key 13 Lockout"),
46,_T("Internal fault: Safety Key 14 Lockout"),
47,_T("Flame rod to ground leakage Hold"),
48,_T("Static flame (not flickering) Hold"),
49,_T("24VAC voltage low/high Hold"),
50,_T("Modulation fault Hold"),
51,_T("Pump fault Hold"),
52,_T("Motor tachometer fault Hold"),
53,_T("AC inputs phase reversed Lockout"),
54,_T("Safety GVT model ID doesn't match application's model ID Lockout"),
55,_T("Application configuration data block CRC errors Lockout"),
56,_T("RESERVED"),
57,_T("RESERVED"),
58,_T("Internal fault: HFS shorted to IAS Lockout"),
59,_T("Internal fault: Mux pin shorted Lockout"),
60,_T("Internal fault: HFS shorted to LFS Lockout"),
61,_T("Anti short cycle Hold"),
62,_T("Fan speed not proved Hold"),
63,_T("LCI OFF Hold"),
64,_T("PII OFF Hold/Lockout"),
65,_T("Interrupted Airflow Switch OFF Hold/Lockout"),
66,_T("Interrupted Airflow Switch ON Hold/Lockout"),
67,_T("ILK OFF Hold/Lockout"),
68,_T("ILK ON Hold/Lockout"),
69,_T("Pilot test hold Hold"),
70,_T("Wait for leakage test completion Hold"),
71,_T("Input power frequency mismatch Lockout"),
72,_T("RESERVED"),
73,_T("RESERVED"),
74,_T("RESERVED"),
75,_T("RESERVED"),
76,_T("RESERVED"),
77,_T("RESERVED"),
78,_T("Demand lost in run Hold"),
79,_T("Outlet high limit Hold/Lockout"),
80,_T("DHW high limit Hold/Lockout"),
81,_T("Delta T inlet/outlet limit Hold/Lockout"),
82,_T("Stack limit Hold/Lockout"),
83,_T("Delta T exchanger/outlet limit Hold/Lockout"),
84,_T("Delta T inlet/exchanger limit Hold/Lockout"),
85,_T("Inlet/outlet inversion limit Hold/Lockout"),
86,_T("Exchanger/outlet inversion limit Hold/Lockout"),
87,_T("Inlet/exchanger inversion limit Hold/Lockout"),
88,_T("Outlet T-rise limit Hold/Lockout"),
89,_T("Exchanger T-rise limit Hold/Lockout"),
90,_T("Heat exchanger high limit Hold/Lockout"),
91,_T("Inlet sensor fault Hold"),
92,_T("Outlet sensor fault Hold"),
93,_T("DHW sensor fault Hold"),
94,_T("S2 (J8-6) sensor fault Hold"),
95,_T("Stack sensor fault Hold"),
96,_T("S5 (J8-11) sensor fault Hold"),
97,_T("Internal fault: A2D mismatch Lockout"),
98,_T("Internal fault: Exceeded VSNSR voltage tolerance Lockout"),
99,_T("Internal fault: Exceeded 28V voltage tolerance Lockout"),
100,_T("Pressure sensor fault Hold"),
101,_T("RESERVED"),
102,_T("RESERVED"),
103,_T("RESERVED"),
104,_T("RESERVED"),
105,_T("Flame detected out of sequence Hold/Lockout"),
106,_T("Flame lost in MFEP Lockout"),
107,_T("Flame lost early in run Lockout"),
108,_T("Flame lost in run Lockout"),
109,_T("Ignition failed Lockout"),
110,_T("Ignition failure occurred Hold"),
111,_T("Flame current lower than WEAK threshold Hold"),
112,_T("Pilot test flame timeout Lockout"),
113,_T("Flame circuit timeout Lockout"),
114,_T("RESERVED"),
115,_T("RESERVED"),
116,_T("RESERVED"),
117,_T("RESERVED"),
118,_T("RESERVED"),
119,_T("RESERVED"),
120,_T("RESERVED"),
121,_T("RESERVED"),
122,_T("Lightoff rate proving failed Lockout"),
123,_T("Purge rate proving failed Lockout"),
124,_T("High fire switch OFF Hold"),
125,_T("High fire switch stuck ON Hold"),
126,_T("Low fire switch OFF Hold"),
127,_T("Low fire switch stuck ON Hold"),
128,_T("Fan speed failed during prepurge Hold/Lockout"),
129,_T("Fan speed failed during preignition Hold/Lockout"),
130,_T("Fan speed failed during ignition Hold/Lockout"),
131,_T("Fan movement detected during standby Hold"),
132,_T("Fan speed failed during run Hold"),
133,_T("RESERVED"),
134,_T("RESERVED"),
135,_T("RESERVED"),
136,_T("Interrupted Airflow Switch failed to close Hold"),
137,_T("ILK failed to close Hold"),
138,_T("RESERVED"),
139,_T("RESERVED"),
140,_T("RESERVED"),
141,_T("RESERVED"),
142,_T("RESERVED"),
143,_T("Internal fault: Flame bias out of range 1 Lockout"),
144,_T("Internal fault: Flame bias out of range 2 Lockout"),
145,_T("Internal fault: Flame bias out of range 3 Lockout"),
146,_T("Internal fault: Flame bias out of range 4 Lockout"),
147,_T("Internal fault: Flame bias out of range 5 Lockout"),
148,_T("Internal fault: Flame bias out of range 6 Lockout"),
149,_T("Flame detected Hold/Lockout"),
150,_T("Flame not detected Hold"),
151,_T("High fire switch ON Hold/Lockout"),
152,_T("Combustion pressure ON Hold/Lockout"),
153,_T("Combustion pressure OFF Hold/Lockout"),
154,_T("Purge fan switch ON Hold/Lockout"),
155,_T("Purge fan switch OFF Hold/Lockout"),
156,_T("Combustion pressure and Flame ON Hold/Lockout"),
157,_T("Combustion pressure and Flame OFF Lockout"),
158,_T("Main valve ON Lockout"),
159,_T("Main valve OFF Lockout"),
160,_T("Ignition ON Lockout"),
161,_T("Ignition OFF Lockout"),
162,_T("Pilot valve ON Lockout"),
163,_T("Pilot valve OFF Lockout"),
164,_T("Block intake ON Lockout"),
165,_T("Block intake OFF Lockout"),
166,_T("RESERVED"),
167,_T("RESERVED"),
168,_T("RESERVED"),
169,_T("RESERVED"),
170,_T("RESERVED"),
171,_T("RESERVED"),
172,_T("Main relay feedback incorrect Lockout"),
173,_T("Pilot relay feedback incorrect Lockout"),
174,_T("Safety relay feedback incorrect Lockout"),
175,_T("Safety relay open Lockout"),
176,_T("Main relay ON at safe start check Lockout"),
177,_T("Pilot relay ON at safe start check Lockout"),
178,_T("Safety relay ON at safe start check Lockout"),
179,_T("RESERVED"),
180,_T("RESERVED"),
181,_T("RESERVED"),
182,_T("RESERVED"),
183,_T("RESERVED"),
184,_T("Invalid BLOWER/HSI output setting Lockout"),
185,_T("Invalid Delta T limit enable setting Lockout"),
186,_T("Invalid Delta T limit response setting Lockout"),
187,_T("Invalid DHW high limit enable setting Lockout"),
188,_T("Invalid DHW high limit response setting Lockout"),
189,_T("Invalid Flame sensor type setting Lockout"),
190,_T("Invalid interrupted air switch enable setting Lockout"),
191,_T("Invalid interrupted air switch start check enable setting Lockout"),
192,_T("Invalid Igniter on during setting Lockout"),
193,_T("Invalid Ignite failure delay setting Lockout"),
194,_T("Invalid Ignite failure response setting Lockout"),
195,_T("Invalid Ignite failure retries setting Lockout"),
196,_T("Invalid Ignition source setting Lockout"),
197,_T("Invalid Interlock open response setting Lockout"),
198,_T("Invalid Interlock start check setting Lockout"),
199,_T("Invalid LCI enable setting Lockout"),
200,_T("Invalid lightoff rate setting Lockout"),
201,_T("Invalid Lightoff rate proving setting Lockout"),
202,_T("Invalid Main Flame Establishing Period time setting Lockout"),
203,_T("Invalid MFEP flame failure response setting Lockout"),
204,_T("Invalid NTC sensor type setting Lockout"),
205,_T("Invalid Outlet high limit response setting Lockout"),
206,_T("Invalid Pilot Flame Establishing Period setting Lockout"),
207,_T("Invalid PII enable setting Lockout"),
208,_T("Invalid pilot test hold setting Lockout"),
209,_T("Invalid Pilot type setting Lockout"),
210,_T("Invalid Postpurge time setting Lockout"),
211,_T("Invalid Power up with lockout setting Lockout"),
212,_T("Invalid Preignition time setting Lockout"),
213,_T("Invalid Prepurge rate setting Lockout"),
214,_T("Invalid Prepurge time setting Lockout"),
215,_T("Invalid Purge rate proving setting Lockout"),
216,_T("Invalid Run flame failure response setting Lockout"),
217,_T("Invalid Run stabilization time setting Lockout"),
218,_T("Invalid Stack limit enable setting Lockout"),
219,_T("Invalid Stack limit response setting Lockout"),
220,_T("Unconfigured Delta T limit setpoint setting Lockout"),
221,_T("Unconfigured DHW high limit setpoint setting Lockout"),
222,_T("Unconfigured Outlet high limit setpoint setting Lockout"),
223,_T("Unconfigured Stack limit setpoint setting Lockout"),
224,_T("Invalid DHW demand source setting Lockout"),
225,_T("Invalid Flame threshold setting Lockout"),
226,_T("Invalid Outlet high limit setpoint setting Lockout"),
227,_T("Invalid DHW high limit setpoint setting Lockout"),
228,_T("Invalid Stack limit setpoint setting Lockout"),
229,_T("Invalid Modulation output setting Lockout"),
230,_T("Invalid CH demand source setting Lockout"),
231,_T("Invalid Delta T limit delay setting Lockout"),
232,_T("Invalid Pressure sensor type setting Lockout"),
233,_T("Invalid IAS closed response setting Lockout"),
234,_T("Invalid Outlet high limit enable setting Lockout"),
235,_T("Invalid Outlet connector type setting Lockout"),
236,_T("Invalid Inlet connector type setting Lockout"),
237,_T("Invalid DHW connector type setting Lockout"),
238,_T("Invalid Stack connector type setting Lockout"),
239,_T("Invalid S2 (J8-6) connector type setting Lockout"),
240,_T("Invalid S5 (J8-11) connector type setting Lockout"),
241,_T("Exchanger sensor not allowed with stack connector setting Lockout"),
242,_T("Invalid DHW auto detect configuration Lockout"),
243,_T("Invalid UV with spark interference not compatible with Ignitor on throughout PFEP Lockout"),
244,_T("Internal fault: Safety relay test invalid state Lockout"),
245,_T("Invalid Outlet connector type setting for Trise Lockout"),
246,_T("4-20mA cannot be used for both modulation and setpoint control Lockout"),
247,_T("Invalid ILK bounce detection enable Lockout"),
248,_T("Invalid forced recycle interval Lockout"),
249,_T("STAT cannot be demand source when Remote Stat is enabled Lockout"),
250,_T("Invalid Fan speed error response Lockout"),
251,_T("Lead drop-stage on error setting does not match drop method configuration Lockout"),
252,_T("Invalid Line frequency setting Lockout"),
253,_T("RESERVED"),
254,_T("RESERVED"),
255,_T("RESERVED")
};

	pcSolaLockoutDesc = (CSolaMultiValue*) new CSolaMultiValue(LockoutDescriptions,sizeof(LockoutDescriptions)/sizeof(SOLAMULTIVALUE));

	static SOLAMULTIVALUE BurnerControlStateValues[] = {	0, _T("Initiate"),
												1, _T("Standby Delay"),
												2, _T("Standby"),
												3, _T("Safe Startup"),
												4, _T("Prepurge - Drive to Purge"),
												5, _T("Prepurge - Measured Purge Rate"),
												6, _T("Prepurge - Drive to Lightoff"),
												7, _T("Preignition Test"),
												8, _T("Preignition Time"),
												9, _T("Pilot Flame Establishing Period"),
												10, _T("Main Flame Establishing Period"),
												11, _T("Direct Burner Ignition"),
												12, _T("Run"),
												13, _T("Postpurge"),
												14, _T("Lockout"),
												255, _T("Safety Processor Offline") };
	
	pcBurnerControlStateValues = (CSolaMultiValue*) new CSolaMultiValue(BurnerControlStateValues,sizeof(BurnerControlStateValues)/sizeof(SOLAMULTIVALUE));

	static SOLAMULTIVALUE BurnerControlStatusValues[] = {
		0, _T("Disabled"),
		1, _T("Lockout"),
		2, _T("Reserved"),
		3, _T("Reserved"),
		4, _T("Anti short-cycle"),
		5, _T("Unconfigured safety data"),
		6, _T("Reserved"),
		7, _T("Reserved"),
		8, _T("Reserved"),
		9, _T("Reserved"),
		10, _T("Reserved"),
		11, _T("Reserved"),
		12, _T("Reserved"),
		13, _T("Reserved"),
		14, _T("Reserved"),
		15, _T("Reserved"),
		16, _T("Reserved"),
		17, _T("Reserved"),
		18, _T("Reserved"),
		19, _T("Reserved"),
		20, _T("Reserved"),
		21, _T("Reserved"),
		22, _T("Reserved"),
		23, _T("Reserved"),
		24, _T("Reserved"),
		25, _T("Reserved"),
		26, _T("Reserved"),
		27, _T("Reserved"),
		28, _T("Reserved"),
		29, _T("Reserved"),
		30, _T("Reserved"),
		31, _T("Reserved"),
		32, _T("Reserved"),
		33, _T("Reserved"),
		34, _T("Standby Hold"),
		35, _T("Standby Delay"),
		36, _T("Reserved"),
		37, _T("Reserved"),
		38, _T("Reserved"),
		39, _T("Reserved"),
		40, _T("Reserved"),
		41, _T("Reserved"),
		42, _T("Reserved"),
		43, _T("Reserved"),
		44, _T("Reserved"),
		45, _T("Reserved"),
		46, _T("Reserved"),
		47, _T("Reserved"),
		48, _T("Normal Standby"),
		49, _T("Preparing"),
		50, _T("Firing"),
		51, _T("Postpurge")
};
	
	pcBurnerControlStatusValues = (CSolaMultiValue*) new CSolaMultiValue(BurnerControlStatusValues,sizeof(BurnerControlStatusValues)/sizeof(SOLAMULTIVALUE));

	static SOLAMULTIVALUE BurnerAlarmReasonCodes[] = {	0, _T("None"),
											1, _T("Lockout"),
											2, _T("Other")};

	static SOLAMULTIVALUE BurnerControlFlags[] = {	0, _T("Reserved"),
										1, _T("Flame detected")};

	static SOLAMULTIVALUE BurnerRemoteStatCodes[] = {	0, _T("Mo remote STAT demand"),
											1, _T("remote STAT demand indicated")};

	static SOLAMULTIVALUE BurnerAnnunFirstOutCodes[] = {	0, _T("Undetermined"),
												1, _T("ILK"),
												2, _T("PII"),
												11, _T("Annunciator 1"),
												12, _T("Annunciator 2"),
												13, _T("Annunciator 3"),
												14, _T("Annunciator 4"),
												15, _T("Annunciator 5"),
												16, _T("Annunciator 6"),
												17, _T("Annunciator 7"),
												18, _T("Annunciator 8")};

	pcBurnerAnnunFirstOutCodes = (CSolaMultiValue*) new CSolaMultiValue(BurnerAnnunFirstOutCodes,sizeof(BurnerAnnunFirstOutCodes)/sizeof(SOLAMULTIVALUE));

	static SOLAMULTIVALUE BurnerAnnunHoldCodes[] = {	0, _T("Undetermined"),
											1, _T("ILK"),
											2, _T("PII"),
											3, _T("LCI"),
											11, _T("Annunciator 1"),
											12, _T("Annunciator 2"),
											13, _T("Annunciator 3"),
											14, _T("Annunciator 4"),
											15, _T("Annunciator 5"),
											16, _T("Annunciator 6"),
											17, _T("Annunciator 7"),
											18, _T("Annunciator 8")};

	pcBurnerAnnunHoldCodes = (CSolaMultiValue*) new CSolaMultiValue(BurnerAnnunHoldCodes,sizeof(BurnerAnnunHoldCodes)/sizeof(SOLAMULTIVALUE));

	static SOLAMBMAP BurnerControlStatus[] = {	_T("Burner control status"), 0x01, 0x03, 0x0020, 0x0001, 0, true, true, true, NULL, 0, Multivalue, BurnerControlStatusValues, sizeof(BurnerControlStatusValues)/sizeof(SOLAMULTIVALUE),\
									_T("Burner control state"), 0x01, 0x03, 0x0021, 0x0001, 0, true, true, true, NULL, 0, Multivalue, BurnerControlStateValues, sizeof(BurnerControlStateValues)/sizeof(SOLAMULTIVALUE),\
									_T("Lockout code"), 0x01, 0x03, 0x0022, 0x0001, 0, true, true, true, NULL, 0, Lockoutcode, LockoutDescriptions, sizeof(LockoutDescriptions)/sizeof(SOLAMULTIVALUE),\
									_T("Alarm reason"), 0x01, 0x03, 0x0023, 0x0001, 0, true, true, true, NULL, 0, Alarmreason, BurnerAlarmReasonCodes, sizeof(BurnerAlarmReasonCodes)/sizeof(SOLAMULTIVALUE),\
									_T("Annunciator first out"), 0x01, 0x03, 0x0024, 0x0001, 0, true, true, true, NULL, 0, Multivalue, BurnerAnnunFirstOutCodes, sizeof(BurnerAnnunFirstOutCodes)/sizeof(SOLAMULTIVALUE),\
									_T("Annunciator Hold"), 0x01, 0x03, 0x0025, 0x0001, 0, true, true, true, NULL, 0, Multivalue, BurnerAnnunFirstOutCodes, sizeof(BurnerAnnunFirstOutCodes)/sizeof(SOLAMULTIVALUE),\
									_T("Sequence time"), 0x01, 0x03, 0x0026, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
									_T("Delay time"), 0x01, 0x03, 0x0027, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
									_T("Hold code"), 0x01, 0x03, 0x0028, 0x0001, 0, true, true, true, NULL, 0, Holdcode, LockoutDescriptions, sizeof(LockoutDescriptions)/sizeof(SOLAMULTIVALUE),\
									_T("Burner control flags"), 0x01, 0x03, 0x0029, 0x0001, 0, true, true, true, NULL, 0,  Multivalue, BurnerControlFlags, sizeof(BurnerControlFlags)/sizeof(SOLAMULTIVALUE),\
									_T("Remote Stat"), 0x01, 0x03, 0x002a, 0x0001, 0, true, true, true, NULL, 0, Multivalue, BurnerRemoteStatCodes,sizeof(BurnerRemoteStatCodes)/sizeof(SOLAMULTIVALUE)};
	
	pcBurnerControlStatus = (CSolaMBMap*) new CSolaMBMap(BurnerControlStatus,sizeof(BurnerControlStatus)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE DigitalIOCodes[] = {	0, _T("Pump A"),
											1, _T("Pump B"),
											2, _T("Pump C"),
											3, _T("Blower motor/HSI"),
											4, _T("External ignition"),
											5, _T("Pilot valve"),
											6, _T("Main valve"),
											7, _T("Alarm"),
											8, _T("Interlock (ILK)"),
											9, _T("Pre-ignition interlock (PII)"),
											10, _T("Load Control Input(LCI)"),
											11, _T("Low Fire Switch (LFS)"),
											12, _T("High Fire Switch (HFS)"),
											13, _T("STAT (Demand)"),
											14, _T("Time of Day"),
											15, _T("Safety relay")};
	
	pcDigitalIOCodes = (CSolaMultiValue*) new CSolaMultiValue(DigitalIOCodes,sizeof(DigitalIOCodes)/sizeof(SOLAMULTIVALUE));

	static SOLAMULTIVALUE CHEnableList[] = {	0, _T("Disabled"), \
										1, _T("Enabled") };

	static SOLAMULTIVALUE CHDemandSrcList[] = {	0, _T("Mod. sensor only"), \
											1, _T("STAT terminal"), \
											2, _T("ECOM Remote Stat"), \
											3, _T("LCI"), \
											4, _T("Reserved"), \
											5, _T("Modbus STAT") };

	static SOLAMULTIVALUE CHSensorList[] = {	0, _T("Outlet Sensor"), \
										1, _T("Unknown"), \
										2, _T("Inlet Sensor"), \
										3, _T("S5 sensor"), \
										4, _T("S10 sensor") };

	static SOLAMULTIVALUE CHODResetEnableList[] = {	0, _T("Disabled"), \
												1, _T("Enabled") };

	static SOLAMBMAP CHConfiguration[] = {	_T("CH enable"), 0x01, 0x03, 0x00D0, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHEnableList, sizeof(CHEnableList)/sizeof(SOLAMULTIVALUE),\
								_T("CH demand switch"), 0x01, 0x03, 0x00D1, 0x0001, 0, true, true, true, NULL, 0,  Multivalue, CHDemandSrcList, sizeof(CHDemandSrcList)/sizeof(SOLAMULTIVALUE),\
								_T("CH mod. sensor"), 0x01, 0x03, 0x00D2, 0x0001, 0, true, true, true, NULL, 0,  Multivalue, CHSensorList, sizeof(CHSensorList)/sizeof(SOLAMULTIVALUE),\
								_T("CH setpoint"), 0x01, 0x03, 0x00D3, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
								_T("CH TOD setpoint"), 0x01, 0x03, 0x00D4, 0x0001, 0, true, true, true, NULL, 0,  TemperatureSetpoint, NULL, 0,\
								_T("CH on hyst."), 0x01, 0x03, 0x00D5, 0x0001, 0, true, true, true, NULL, 0,  Hysteresis, NULL, 0,\
								_T("CH off hyst."), 0x01, 0x03, 0x00D6, 0x0001, 0, true, true, true, NULL, 0,  Hysteresis, NULL, 0,\
								_T("CH OD reset enable"), 0x01, 0x03, 0x00D7, 0x0001, 0, true, true, true, NULL, 0,  Multivalue, CHODResetEnableList, sizeof(CHODResetEnableList)/sizeof(SOLAMULTIVALUE),\
								_T("CH P-gain"), 0x01, 0x03, 0x00D8, 0x0001, 0, true, true, true, NULL, 0,  Numericvalue, NULL, 0,\
								_T("CH I-gain"), 0x01, 0x03, 0x00D9, 0x0001, 0, true, true, true, NULL, 0,  Numericvalue, NULL, 0,\
								_T("CH D-gain"), 0x01, 0x03, 0x00DA, 0x0001, 0, true, true, true, NULL, 0,  Numericvalue, NULL, 0,\
								_T("CH hyst. step time"), 0x01, 0x03, 0x00DB, 0x0001, 0, true, true, true, NULL, 0,  Timevalue, NULL , 0,\
								_T("CH press. stpt."), 0x01, 0x03, 0x00DC, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("CH press. on hyst."), 0x01, 0x03, 0x00DD, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("CH press. off hyst."), 0x01, 0x03, 0x00DE, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
								_T("RESERVED"), 0x01, 0x03, 0x00DF, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0};

	pcCHConfiguration = (CSolaMBMap*)new CSolaMBMap(CHConfiguration, sizeof(CHConfiguration)/sizeof(SOLAMBMAP));

	static SOLAMBMAP XCHConfig[] = {	_T("CH min. press."), 0x01, 0x03, 0x021d, 0x0001, 0, true, true, true, NULL, 0, Percentvalue, NULL, 0,\
								_T("CH TOD press. stpt."), 0x01, 0x03, 0x021e, 0x0001, 0, true, true, true, NULL, 0,  Percentvalue, NULL, 0,\
								_T("Analog input hyst."), 0x01, 0x03, 0x021f, 0x0001, 0, true, true, true, NULL, 0,  Decimal1pl, NULL, 0};

	pcXCHConfig = (CSolaMBMap*)new CSolaMBMap(XCHConfig, sizeof(XCHConfig)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE CHModbusSTAT[] = {0,_T("No heat call"),\
		1,_T("Heat call")};

	static SOLAMULTIVALUE CHSetpointSource[] = {0,_T("Local"),\
		1,_T("Modbus"),\
		2,_T("4-20mA")};

	static SOLAMULTIVALUE CHModulationRateSource[] = {0,_T("Local"),\
		1,_T("Modbus bin. fraction"),\
		2,_T("Modbus mod. step"),\
		3,_T("4-20mA sens. on/off"),\
		4,_T("4020mA brnr. on/off")};

	static SOLAMULTIVALUE CHPriorityVsLL[] = {0,_T("LL > CH"),
		1,_T("CH > LL")};

	static SOLAMBMAP X2CHConfig[] = {	_T("CH Modbus STAT"), 0x01, 0x03, 0x0241, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHModbusSTAT, sizeof(CHModbusSTAT)/sizeof(SOLAMULTIVALUE),
		_T("CH stpt. src."), 0x01, 0x03, 0x0242, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHSetpointSource, sizeof(CHSetpointSource)/sizeof(SOLAMULTIVALUE),
		_T("CH Modbus stpt."), 0x01, 0x03, 0x0243, 0x0001, 0, true, true, true, NULL, 0,  TemperatureSetpoint, NULL, 0,
		_T("CH mod. rate stc."), 0x01, 0x03, 0x0244, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHModulationRateSource, sizeof(CHModulationRateSource)/sizeof(SOLAMULTIVALUE),
		_T("CH Modbus rate"), 0x01, 0x03, 0x0245, 0x0001, 0, true, true, true, NULL, 0,  Numericvalue, NULL, 0,
		_T("CH pty. vs LL"), 0x01, 0x03, 0x0246, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHPriorityVsLL, sizeof(CHPriorityVsLL)/sizeof(SOLAMULTIVALUE),
		_T("CH 4mA Temp."), 0x01, 0x03, 0x0247, 0x0001, 0, true, true, true, NULL, 0,  Temperature, NULL, 0,
		_T("CH 20mA Temp."), 0x01, 0x03, 0x0248, 0x0001, 0, true, true, true, NULL, 0,  Temperature, NULL, 0};
//		_T("CH 4mA steam pr."), 0x01, 0x03, 0x0249, 0x0001, 0, true, true, true, NULL, 0,  Decimal1pl, NULL, 0,
//		_T("CH 20mA steam pr."), 0x01, 0x03, 0x024a, 0x0001, 0, true, true, true, NULL, 0,  Decimal1pl, NULL, 0};

	pcX2CHConfig = (CSolaMBMap*)new CSolaMBMap(X2CHConfig, sizeof(X2CHConfig)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE FanPWMFreq[] = {0, _T("Unknown"),
		1, _T("1000"),
		2, _T("2000"),
		3, _T("3000"),
		4, _T("4000"),
		0xffff, _T("Not configd")};

	static SOLAMBMAP FanConfig[] = {_T("Abs. max. fan spd."), 0x01, 0x03, 0x00100, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
		_T("Abs. min. fan spd."),  0x01, 0x03, 0x00101, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
		_T("PWM frq."), 0x01, 0x03, 0x0102, 0x0001, 0, true, true, true, NULL, 0,  Multivalue, FanPWMFreq, sizeof(FanPWMFreq)/sizeof(SOLAMULTIVALUE),\
		_T("Pulses per rev."),  0x01, 0x03, 0x00103, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
		_T("Spd. up ramp"),  0x01, 0x03, 0x00104, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
		_T("Slow dn. ramp"),  0x01, 0x03, 0x00105, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
		_T("Gain up"),  0x01, 0x03, 0x00106, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
		_T("Gain dn."),  0x01, 0x03, 0x00107, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
		_T("Min. duty cycle"),  0x01, 0x03, 0x00108, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
		_T("RESERVED"),  0x01, 0x03, 0x00109, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("RESERVED"),  0x01, 0x03, 0x0010a, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("RESERVED"),  0x01, 0x03, 0x0010b, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("RESERVED"),  0x01, 0x03, 0x0010c, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("RESERVED"),  0x01, 0x03, 0x0010d, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("RESERVED"),  0x01, 0x03, 0x0010e, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0,\
		_T("RESERVED"),  0x01, 0x03, 0x0010f, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0};

	pcFanConfig = (CSolaMBMap*)new CSolaMBMap(FanConfig,sizeof(FanConfig)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE DemandValues[] = {	0, _T("Unknown"),
									1, _T("No demand"),
									2, _T("CH"),
									3, _T("DHW"),
									4, _T("LL slave"),
									5, _T("LL master"),
									6, _T("CH freeze"),
									7, _T("DHW freeze"),
									8, _T("Burner SW off"),
									9, _T("DHW storage"),
									10, _T("Reserved"),
									11, _T("Warm weather s/d") };
	
	static SOLAMULTIVALUE RegisterAccess[]		= { 0, _T("No register access"),
										1, _T("Installer access"),
										2, _T("OEM access"),
										3, _T("All access") };

	pcRegisterAccess = (CSolaMultiValue*) new CSolaMultiValue(RegisterAccess,sizeof(RegisterAccess)/sizeof(SOLAMULTIVALUE));

	static SOLAMBMAP TrendStatus[] = {	_T("Demand Source"), 0x01, 0x03, 0x0006, 0x0001, 0, true, true, true, NULL, 0, Multivalue, DemandValues, sizeof(DemandValues)/sizeof(SOLAMULTIVALUE),\
							_T("Outlet Temperature"), 0x01, 0x03, 0x0007, 0x0001, 0, true, true, true, NULL, 0, Temperature, NULL, 0,\
							_T("Firing rate"), 0x01, 0x03, 0x0008, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
							_T("Fan speed"), 0x01, 0x03, 0x0009, 0x0001, 0, true, true, true, NULL, 0, Fanspeed, NULL, 0,\
							_T("Flame signal"), 0x01, 0x03, 0x000a, 0x0001, 0, true, true, true, NULL, 0, Decimal2pl, NULL, 0,\
							_T("Inlet temperature"), 0x01, 0x03, 0x000b, 0x0001, 0, true, true, true, NULL, 0, Temperature, NULL, 0,\
							_T("DHW temperature"), 0x01, 0x03, 0x000c, 0x0001, 0, true, true, true, NULL, 0, Temperature, NULL, 0,\
							_T("Outdoor temperature"), 0x01, 0x03, 0x000d, 0x0001, 0, true, true, true, NULL, 0, ODTemperature, NULL, 0,\
							_T("Stack temperature"), 0x01, 0x03, 0x000e, 0x0001, 0, true, true, true, NULL, 0, Temperature, NULL, 0,\
							_T("4-20mA channel"), 0x01, 0x03, 0x000f, 0x0001, 0, true, true, true, NULL, 0, Decimal1pl, NULL, 0,\
							_T("Active CH setpoint"), 0x01, 0x03, 0x0010, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
							_T("Active DHW setpoint"), 0x01, 0x03, 0x0011, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
							_T("Active LL Setpoint"), 0x01, 0x03, 0x0012, 0x0001, 0, true, true, true, NULL, 0,  TemperatureSetpoint, NULL, 0,\
							_T("Register access"), 0x01, 0x03, 0x0013, 0x0001, 0, true, true, true, NULL, 0, Multivalue, RegisterAccess, sizeof(RegisterAccess)/sizeof(SOLAMULTIVALUE),\
							_T("Steam pressure"), 0x01, 0x03, 0x0014, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
							_T("Analog mod input"), 0x01, 0x03, 0x0015, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
							_T("Active CH pr stpt"), 0x01, 0x03, 0x0016, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
							_T("Ext Status chg"), 0x01, 0x03, 0x0017, 0x0001, 0, true, true, true, NULL, 0, Bitmask, NULL, 0,\
							_T("RESERVED"), 0x01, 0x03, 0x0018, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
							_T("Active CH op. pt."), 0x01, 0x03, 0x0019, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
							_T("Active DHW op. pt."), 0x01, 0x03, 0x001a, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
							_T("Active LL op. pt."), 0x01, 0x03, 0x001b, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
							_T("Active sys. op. pt."), 0x01, 0x03, 0x001c, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
							_T("Active sys. stpt."), 0x01, 0x03, 0x001d, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0,\
							_T("Active sys. on hyst."), 0x01, 0x03, 0x001e, 0x0001, 0, true, true, true, NULL, 0, Hysteresis, NULL, 0,\
							_T("Active sys. off hyst."), 0x01, 0x03, 0x001f, 0x0001, 0, true, true, true, NULL, 0, Hysteresis, NULL, 0};
	
	pcTrendStatus = (CSolaMBMap*) new CSolaMBMap(TrendStatus, sizeof(TrendStatus)/sizeof(SOLAMBMAP));
	
	static SOLAMBMAP SystemStatus[] = {	_T("Status Change"), 0x01, 0x03, 0x0000, 0x0001, 0, true, true, true, NULL, 0, Bitmask, NULL, 0,\
								_T("Configuration Change"), 0x01, 0x03, 0x0001, 0x0001, 0, true, true, true, NULL, 0, Bitmask, NULL, 0,\
								_T("Digital I/O"), 0x01, 0x03, 0x0002, 0x0001, 0, true, true, true, NULL, 0, Bitmask, NULL, 0,\
								_T("Annunciation I/O"), 0x01, 0x03, 0x0003, 0x0001, 0, true, true, true, NULL, 0, Bitmask, NULL, 0,\
								_T("Limits"), 0x01, 0x03, 0x0004, 0x0001, 0, true, true, true, NULL, 0, Bitmask, NULL, 0,\
								_T("PIM Status"), 0x01, 0x03, 0x0005, 0x0001, 0, true, true, true, NULL, 0, Bitmask, NULL, 0};
	
	pcSystemStatus = (CSolaMBMap*) new CSolaMBMap(SystemStatus, sizeof(SystemStatus)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE SensorStates[] = {	0, _T("None"),
									1, _T("Normal"),
									2, _T("Open"),
									3, _T("Shorted"),
									4, _T("Outside high range"),
									5, _T("Outside low range"),
									6, _T("Not reliable") };

	static SOLAMBMAP SensorStatus[] = {	_T("Outlet OP channel"), 0x01, 0x03, 0x002b, 0x0001, 0, true, true, true, NULL, 0, Temperature, NULL, 0,\
								_T("DHW OP channel"), 0x01, 0x03, 0x002c, 0x0001, 0, true, true, true, NULL, 0, Temperature, NULL, 0,\
								_T("Stack OP channel"), 0x01, 0x03, 0x002d, 0x0001, 0, true, true, true, NULL, 0, Temperature, NULL, 0,\
								_T("Outlet OP sensor state"), 0x01, 0x03, 0x002e, 0x0001, 0, true, true, true, NULL, 0, SensorMultivalue, SensorStates, sizeof(SensorStates)/sizeof(SOLAMULTIVALUE),\
								_T("DHW OP sensor state"), 0x01, 0x03, 0x002f, 0x0001, 0, true, true, true, NULL, 0, SensorMultivalue, SensorStates, sizeof(SensorStates)/sizeof(SOLAMULTIVALUE),\
								_T("Outlet sensor state"), 0x01, 0x03, 0x0030, 0x0001, 0, true, true, true, NULL, 0, SensorMultivalue, SensorStates, sizeof(SensorStates)/sizeof(SOLAMULTIVALUE),\
								_T("Inlet sensor state"), 0x01, 0x03, 0x0031, 0x0001, 0, true, true, true, NULL, 0, SensorMultivalue, SensorStates, sizeof(SensorStates)/sizeof(SOLAMULTIVALUE),\
								_T("DHW sensor state"), 0x01, 0x03, 0x0032, 0x0001, 0, true, true, true, NULL, 0, SensorMultivalue, SensorStates, sizeof(SensorStates)/sizeof(SOLAMULTIVALUE),\
								_T("Stack sensor state"), 0x01, 0x03, 0x0033, 0x0001, 0, true, true, true, NULL, 0, SensorMultivalue, SensorStates, sizeof(SensorStates)/sizeof(SOLAMULTIVALUE),\
								_T("Outdoor sensor state"), 0x01, 0x03, 0x0034, 0x0001, 0, true, true, true, NULL, 0, SensorMultivalue, SensorStates, sizeof(SensorStates)/sizeof(SOLAMULTIVALUE),\
								_T("4-20mA input state"), 0x01, 0x03, 0x0035, 0x0001, 0, true, true, true, NULL, 0, SensorMultivalue, SensorStates, sizeof(SensorStates)/sizeof(SOLAMULTIVALUE),\
								_T("Pressure sensor state"), 0x01, 0x03, 0x0036, 0x0001, 0, true, true, true, NULL, 0, SensorMultivalue, SensorStates, sizeof(SensorStates)/sizeof(SOLAMULTIVALUE),\
								_T("Stack OP sensor state"), 0x01, 0x03, 0x0037, 0x0001, 0, true, true, true, NULL, 0, SensorMultivalue, SensorStates, sizeof(SensorStates)/sizeof(SOLAMULTIVALUE)};

	pcSensorStatus = (CSolaMBMap*)new CSolaMBMap(SensorStatus, sizeof(SensorStatus)/sizeof(SOLAMBMAP));

	static SOLAMBMAP ExtendedSensorStatus[] = {	_T("Outdoor temperature"), 0x01, 0x03, 0x00AA, 0x0001, 0, true, true, true, NULL, 0, Temperature, NULL, 0,\
										_T("Outdoor sensor state"), 0x01, 0x03, 0x00AB, 0x0001, 0, true, true, true, NULL, 0, SensorMultivalue, SensorStates, sizeof(SensorStates)/sizeof(SOLAMULTIVALUE),\
										_T("Outlet T-rise rate"), 0x01, 0x03, 0x00AC, 0x0001, 0, true, true, true, NULL, 0, Hysteresis, NULL, 0,\
										_T("Exchanger T-rise rate"), 0x01, 0x03, 0x00AD, 0x0001, 0, true, true, true, NULL, 0, Hysteresis, NULL, 0,\
										_T("S10 sensor reading"), 0x01, 0x03, 0x00AE, 0x0001, 0, true, true, true, NULL, 0, Temperature, NULL, 0,\
										_T("S10 sensor state"), 0x01, 0x03, 0x00AF, 0x0001, 0, true, true, true, NULL, 0, SensorMultivalue, SensorStates, sizeof(SensorStates)/sizeof(SOLAMULTIVALUE)};

	pcExtendedSensorStatus = (CSolaMBMap*)new CSolaMBMap(ExtendedSensorStatus, sizeof(ExtendedSensorStatus)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE ActiveRateLimiter[] = {	0, _T("None"),
										1, _T("Outlet high limit"),
										2, _T("Delta T limit"),
										3, _T("Stack limit"),
										4, _T("Slow start limit"),
										5, _T("Anti-condensation"),
										6, _T("Minimum modulation"),
										7, _T("Forced rate"),
										8, _T("IAS is open")};

	static SOLAMULTIVALUE ActiveRateOvrd[] = {	0, _T("None"),
									1, _T("Burner control default"),
									2, _T("Burner control"),
									3, _T("Manual firing rate"),
									4, _T("Manual firing rate off"),
									5, _T("Fan on during off cycle")};

	static SOLAMULTIVALUE ActiveSystemSensor[] = {	0, _T("None"),
										1, _T("DHW (S6S7) sensor"),
										2, _T("Outlet (S3S4) sensor"),
										3, _T("Inlet (S1) sensor"),
										4, _T("4-20mA input (S2)"),
										5, _T("S5 sensor"),
										6, _T("S10 sensor"),
										7, _T("Steam (S1) sensor") };

	static SOLAMULTIVALUE ActiveLLSensor[] = {	0, _T("None"),
										1, _T("Unknown"),
										2, _T("Outlet (S3S4) sensor"),
										3, _T("Unknown"),
										4, _T("Unknown"),
										5, _T("S5 sensor"),
										6, _T("S10 sensor")};

	static SOLAMBMAP DemandModulationStatus[] = {	_T("Active rate limiter"), 0x01, 0x03, 0x0038, 0x0001, 0, true, true, true, NULL, 0, Multivalue, ActiveRateLimiter, sizeof(ActiveRateLimiter)/sizeof(SOLAMULTIVALUE),\
										_T("Limited rate"), 0x01, 0x03, 0x0039, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
										_T("Active rate override"), 0x01, 0x03, 0x003A, 0x0001, 0, true, true, true, NULL, 0, Multivalue, ActiveRateOvrd, sizeof(ActiveRateOvrd)/sizeof(SOLAMULTIVALUE),\
										_T("Override rate"), 0x01, 0x03, 0x003B, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
										_T("Demand rate"), 0x01, 0x03, 0x003C, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
										_T("Active system sensor"), 0x01, 0x03, 0x003D, 0x0001, 0, true, true, true, NULL, 0, Multivalue, ActiveSystemSensor, sizeof(ActiveSystemSensor)/sizeof(SOLAMULTIVALUE),\
										_T("Active LL sensor"), 0x01, 0x03, 0x003E, 0x0001, 0, true, true, true, NULL, 0, Multivalue, ActiveLLSensor, sizeof(ActiveLLSensor)/sizeof(SOLAMULTIVALUE)};
	
	pcDemandModulationStatus = (CSolaMBMap*)new CSolaMBMap(DemandModulationStatus, sizeof(DemandModulationStatus)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE CHStatusCodes[] = {	0, _T("Unknown"),
									1, _T("Disabled"),
									2, _T("Normal"),
									3, _T("Suspended")};

	static SOLAMULTIVALUE CHSetpointSourceCodes[] = {	0, _T("Unknown"),
											1, _T("Normal setpoint"),
											2, _T("TOD setpoint"),
											3, _T("Outdoor reset"),
											4, _T("Remote control")};

	static SOLAMULTIVALUE CHDemandCodes[] = {	0, _T("Off"),
									1, _T("On")};

	static SOLAMBMAP CHStatus[] =	{	_T("CH status"), 0x01, 0x03, 0x0040, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHStatusCodes, sizeof(CHStatusCodes)/sizeof(SOLAMULTIVALUE),\
							_T("CH setpoint source"), 0x01, 0x03, 0x0041, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHSetpointSourceCodes, sizeof(CHSetpointSourceCodes)/sizeof(SOLAMULTIVALUE),\
							_T("CH pump demand"), 0x01, 0x03, 0x0042, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHDemandCodes, sizeof(CHDemandCodes)/sizeof(SOLAMULTIVALUE),\
							_T("CH burner demand"), 0x01, 0x03, 0x0043, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHDemandCodes, sizeof(CHDemandCodes)/sizeof(SOLAMULTIVALUE),\
							_T("CH requested rate"), 0x01, 0x03, 0x0044, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
							_T("CH frost pump demand"), 0x01, 0x03, 0x0045, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHDemandCodes, sizeof(CHDemandCodes)/sizeof(SOLAMULTIVALUE),\
							_T("CH frost burner demand"), 0x01, 0x03, 0x0046, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHDemandCodes, sizeof(CHDemandCodes)/sizeof(SOLAMULTIVALUE),\
							_T("Active CH on hysteresis"), 0x01, 0x03, 0x0047, 0x0001, 0, true, true, true, NULL, 0, Hysteresis, NULL, 0,\
							_T("Active CH off hysteresis"), 0x01, 0x03, 0x0048, 0x0001, 0, true, true, true, NULL, 0, Hysteresis, NULL, 0};
	
	pcCHStatus = (CSolaMBMap*)new CSolaMBMap(CHStatus, sizeof(CHStatus)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE ActiveDHWSensor[] = {0,_T("None"),
		1,_T("DHW (S6S7) sensor"),
		2,_T("Outlet (S3S4) sensor"),
		3,_T("Inlet (S1) sensor")};

	static SOLAMBMAP DHWStatus[] = {_T("Active DHW sensor"),  0x01, 0x03, 0x004f, 0x0001, 0, true, true, true, NULL, 0, Multivalue, ActiveDHWSensor, sizeof(ActiveDHWSensor)/sizeof(SOLAMULTIVALUE),\
							_T("DHW status"), 0x01, 0x03, 0x0050, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHStatusCodes, sizeof(CHStatusCodes)/sizeof(SOLAMULTIVALUE),\
							_T("DHW setpoint source"), 0x01, 0x03, 0x0051, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHSetpointSourceCodes, sizeof(CHSetpointSourceCodes)/sizeof(SOLAMULTIVALUE),\
							_T("DHW priority count"), 0x01, 0x03, 0x0052, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
							_T("DHW pump demand"), 0x01, 0x03, 0x0053, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHDemandCodes, sizeof(CHDemandCodes)/sizeof(SOLAMULTIVALUE),\
							_T("DHW burner demand"), 0x01, 0x03, 0x0054, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHDemandCodes, sizeof(CHDemandCodes)/sizeof(SOLAMULTIVALUE),\
							_T("DHW requested rate"), 0x01, 0x03, 0x0055, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
							_T("DHW frost pump demand"), 0x01, 0x03, 0x0056, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHDemandCodes, sizeof(CHDemandCodes)/sizeof(SOLAMULTIVALUE),\
							_T("DHW frost burner demand"), 0x01, 0x03, 0x0057, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHDemandCodes, sizeof(CHDemandCodes)/sizeof(SOLAMULTIVALUE),\
							_T("Active DHW on hysteresis"), 0x01, 0x03, 0x0058, 0x0001, 0, true, true, true, NULL, 0, Hysteresis, NULL, 0,\
							_T("Active DHW off hysteresis"), 0x01, 0x03, 0x0059, 0x0001, 0, true, true, true, NULL, 0, Hysteresis, NULL, 0,\
							_T("DHW Storage Time"), 0x01, 0x03, 0x005a, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
							_T("DHW storage pump demand"), 0x01, 0x03, 0x005b, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHDemandCodes, sizeof(CHDemandCodes)/sizeof(SOLAMULTIVALUE),\
							_T("DHW storage burner demand"), 0x01, 0x03, 0x005c, 0x0001, 0, true, true, true, NULL, 0, Multivalue, CHDemandCodes, sizeof(CHDemandCodes)/sizeof(SOLAMULTIVALUE)};

	pcDHWStatus = (CSolaMBMap*)new CSolaMBMap(DHWStatus, sizeof(DHWStatus)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE PumpStatusCodes[] =	{	0, _T("Unknown"),
										1, _T("Not connected"),
										2, _T("Not Lead Lag master"),
										3, _T("Pump A Off"),
										4, _T("Pump B Off"),
										5, _T("Pump C Off"),
										6, _T("Pump A Off - Anticond. (CH demand)"),
										7, _T("Pump B Off - Anticond. (CH demand)"),
										8, _T("Pump C Off - Anticond. (CH demand)"),
										9, _T("Pump A Off - Anticond. (DHW demand)"),
										10, _T("Pump B Off - Anticond. (DHW demand)"),
										11, _T("Pump C Off - Anticond. (DHW demand)"),
										12, _T("Pump A Off - Anticond. (LL demand)"),
										13, _T("Pump B Off - Anticond. (LL demand)"),
										14, _T("Pump C Off - Anticond. (LL demand)"),
										15, _T("Pump A On - Slave overrun"),
										16, _T("Pump B On - Slave overrun"),
										17, _T("Pump C On - Slave overrun"),
										18, _T("Pump A On - LL master overrun"),
										19, _T("Pump B On - LL master overrun"),
										20, _T("Pump C On - LL master overrun"),
										21, _T("Pump A Off - Start delay (DHW demand)"),
										22, _T("Pump B Off - Start delay (DHW demand)"),
										23, _T("Pump C Off - Start delay (DHW demand)"),
										24, _T("Pump A On - CH demand"),
										25, _T("Pump B On - CH demand"),
										26, _T("Pump C On - CH demand"),
										27, _T("Pump A On - CH antifrz"),
										28, _T("Pump B On - CH antifrz"),
										29, _T("Pump C On - CH antifrz"),
										30, _T("Pump A On - DHW demand"),
										31, _T("Pump B On - DHW demand"),
										32, _T("Pump C On - DHW demand"),
										33, _T("Pump A On - DHW antifrz"),
										34, _T("Pump B On - DHW antifrz"),
										35, _T("Pump C On - DHW antifrz"),
										36, _T("Pump A Off - DHW hi lim"),
										37, _T("Pump B Off - DHW hi lim"),
										38, _T("Pump C Off - DHW hi lim"),
										39, _T("Pump A On - Exercise"),
										40, _T("Pump B On - Exercise"),
										41, _T("Pump C On - Exercise"),
										42, _T("Pump A On - antifrz"),
										43, _T("Pump B On - antifrz"),
										44, _T("Pump C On - antifrz"),
										45, _T("Pump A On - Lead Lag master demand"),
										46, _T("Pump B On - Lead Lag master demand"),
										47, _T("Pump C On - Lead Lag master demand"),
										48, _T("Pump A On - Slave demand"),
										49, _T("Pump B On - Slave demand"),
										50, _T("Pump C On - Slave demand"),
										51, _T("Pump A On - Manual"),
										52, _T("Pump B On - Manual"),
										53, _T("Pump C On - Manual"),
										54, _T("Pump A On - Outlet hi lim"),
										55, _T("Pump B On - Outlet hi lim"),
										56, _T("Pump C On - Outlet hi lim"),
										57, _T("Pump A On - Overrun"),
										58, _T("Pump B On - Overrun"),
										59, _T("Pump C On - Overrun"),
										60, _T("Pump A On - antifrz overrun"),
										61, _T("Pump B On - antifrz overrun"),
										62, _T("Pump C On - antifrz overrun"),
										63, _T("Pump A On – Mix demand"), 
										64, _T("Pump B On – Mix demand"), 
										65, _T("Pump C On – Mix demand"), 
										66, _T("Undefined"), 
										67, _T("Undefined"), 
										68, _T("Undefined"), 
										69, _T("Undefined"), 
										70, _T("Undefined"), 
										71, _T("Undefined"), 
										72, _T("Undefined"), 
										73, _T("Undefined"), 
										74, _T("Undefined"), 
										75, _T("Undefined"), 
										76, _T("Undefined"), 
										77, _T("Undefined"), 
										78, _T("Undefined"), 
										79, _T("Undefined"), 
										80, _T("Undefined"), 
										81, _T("Undefined"), 
										82, _T("Undefined"), 
										83, _T("Undefined"), 
										84, _T("Undefined"), 
										85, _T("Undefined"), 
										86, _T("Undefined"), 
										87, _T("Undefined"), 
										88, _T("Undefined"), 
										89, _T("Undefined"), 
										90, _T("Undefined"), 
										91, _T("Undefined"), 
										92, _T("Force on-Mnl"), 
										93, _T("Force on-Outlet hi lim"), 
										94, _T("Force on-Brnr dmnd"), 
										95, _T("Force on-LL slave dmnd"), 
										96, _T("Force off-Lcl DHW pty svc"), 
										97, _T("Force off LL DHW pty svc"), 
										98, _T("Force off-CH anti-cond"), 
										99, _T("Force off-DHW anti-cond"), 
										100, _T("Force off-DHW hi lim"), 
										101, _T("Force off-DHW ECOM pty svc"), 
										102, _T("On-lcl CH frz prot"), 
										103, _T("On-lcl LL frz prot"), 
										104, _T("On-lcl DHW frz prot"), 
										105, _T("On-LL DHW frz prot"), 
										106, _T("On-lcl CH dmnd"), 
										107, _T("On-LL CH dmnd"), 
										108, _T("On-lcl DHW dmnd"), 
										109, _T("On-LL DHW dmnd"), 
										110, _T("On-lcl Mix dmnd"), 
										111, _T("On-LL Mix dmnd"), 
										112, _T("On-lcl CH svc"), 
										113, _T("On-LL CH svc"), 
										114, _T("On-lcl DHW svc"), 
										115, _T("On-LL DHW svc"), 
										116, _T("On-lcl Mix svc"), 
										117, _T("On-LL Mix svc"), 
										118, _T("On-LL Aux X"), 
										119, _T("On-LL Aux Y"), 
										120, _T("On-LL Aux Z"), 
										121, _T("On-pump strt dly"), 
										122, _T("On-pump ovrd"), 
										123, _T("Off"),
										124, _T("On brnr dmd"),
										125, _T("On exercise"),
										126, _T("On lcl LL svc"),
										127, _T("On lcl LL pmp dmnd") };

	static SOLAMBMAP PumpStatus[] = {	_T("Pump A status"), 0x01, 0x03, 0x005D, 0x0001, 0, true, true, true, NULL, 0, Bitmask, NULL, 0,\
							_T("Pump B status"), 0x01, 0x03, 0x005E, 0x0001, 0, true, true, true, NULL, 0, Bitmask, NULL, 0,\
							_T("CH pump start delay time"), 0x01, 0x03, 0x005F, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
							_T("CH pump status"), 0x01, 0x03, 0x0060, 0x0001, 0, true, true, true, NULL, 0, Multivalue, PumpStatusCodes, sizeof(PumpStatusCodes)/sizeof(SOLAMULTIVALUE),\
							_T("CH pump overrun time"), 0x01, 0x03, 0x0061, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
							_T("CH FP overrun time"), 0x01, 0x03, 0x0062, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
							_T("CH pump idle days count"), 0x01, 0x03, 0x0063, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
							_T("DHW pump status"), 0x01, 0x03, 0x0064, 0x0001, 0, true, true, true, NULL, 0, Multivalue, PumpStatusCodes, sizeof(PumpStatusCodes)/sizeof(SOLAMULTIVALUE),\
							_T("DHW pump start delay time"), 0x01, 0x03, 0x0065, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
							_T("DHW pump overrun time"), 0x01, 0x03, 0x0066, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
							_T("DHW FP overrun time"), 0x01, 0x03, 0x0067, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
							_T("DHW pump idle days count"), 0x01, 0x03, 0x0068, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
							_T("System pump status"), 0x01, 0x03, 0x0069, 0x0001, 0, true, true, true, NULL, 0, Multivalue, PumpStatusCodes, sizeof(PumpStatusCodes)/sizeof(SOLAMULTIVALUE),\
							_T("System pump overrun time"), 0x01, 0x03, 0x006A, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
							_T("System pump idle days count"), 0x01, 0x03, 0x006B, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
							_T("Boiler pump status"), 0x01, 0x03, 0x006C, 0x0001, 0, true, true, true, NULL, 0, Multivalue, PumpStatusCodes, sizeof(PumpStatusCodes)/sizeof(SOLAMULTIVALUE),\
							_T("Boiler pump overrun time"), 0x01, 0x03, 0x006D, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
							_T("Boiler pump idle days count"), 0x01, 0x03, 0x006E, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
							_T("Aux. 1 pump status"), 0x01, 0x03, 0x006F, 0x0001, 0, true, true, true, NULL, 0, Multivalue, PumpStatusCodes, sizeof(PumpStatusCodes)/sizeof(SOLAMULTIVALUE),\
							_T("Aux. 1 pump idle days count"), 0x01, 0x03, 0x0070, 0x0001, 0, true, true, true, NULL, 0,  Numericvalue, NULL, 0,\
							_T("Aux. 2 pump status"), 0x01, 0x03, 0x0071, 0x0001, 0, true, true, true, NULL, 0, Multivalue, PumpStatusCodes, sizeof(PumpStatusCodes)/sizeof(SOLAMULTIVALUE),\
							_T("Aux. 2 pump overrun time"), 0x01, 0x03, 0x0072, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
							_T("Aux. 2 pump idle days count"), 0x01, 0x03, 0x0073, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
							_T("Reserved"), 0x01, 0x03, 0x0074, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
							_T("Reserved"), 0x01, 0x03, 0x0075, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
							_T("Reserved"), 0x01, 0x03, 0x0076, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0,\
							_T("Aux. 1 pump overrun time"), 0x01, 0x03, 0x0077, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0};
	
	pcPumpStatus = (CSolaMBMap*)new CSolaMBMap(PumpStatus, sizeof(PumpStatus)/sizeof(SOLAMBMAP));

	static SOLAMULTIVALUE ModbusExceptionCodes[] = {	0x01, _T("ILLEGAL_FUNCTION"),
											0x02, _T("ILLEGAL_DATA_ADDRESS"),
											0x03, _T("ILLEGAL_DATA_VALUE"),
											0x10, _T("READ_MULTIPLE_NOT_OK"),
											0x11, _T("ACCESS_FAILURE"),
											0x12, _T("LOGIN_FAILURE") };

	static SOLAMULTIVALUE SystemIDProductTypes[] = {	0x0000, _T("Unknown product"),
											0x0100, _T("Residential Hydronic control"),
											0x0101, _T("Commercial Hydronic control"),
											0x0200, _T("Residential Steam control"),
											0x0201, _T("Residential Steam control"),
											0x0300, _T("Reserved"),
											0x0301, _T("Reserved"),
											0x0400, _T("Residential Fulton Pulse Hydronic"),
											0x0401, _T("Commercial Fulton Pulse Hydronic"),
											0x0500, _T("Residential Fulton Pulse Steam"),
											0x0501, _T("Residential Fulton Pulse Steam"),
											0x0600, _T("Residential Cleaver Brooks Hydronic"),
											0x0601, _T("Commercial Cleaver Brooks Hydronic"),
											0x0700, _T("Residential Cleaver Brooks Steam"), 
											0x0701, _T("Commercial Cleaver Brooks Steam") }; 

	static SOLAMULTIVALUE TemperatureUnits[] = {	0, _T("Fahrenheit"),
										1, _T("Celsius")};

	static SOLAMULTIVALUE PowerUpWithLockoutCodes[] = {	0, _T("Clr lkout"),
												1, _T("Prsrv lkout")};

	static SOLAMBMAP SystemConfiguration[] = {	_T("Temp. units"), 0x01, 0x03, 0x00B2, 0x0001, 0, true, true, true, NULL, 0, Multivalue, TemperatureUnits, sizeof(TemperatureUnits)/sizeof(SOLAMULTIVALUE),\
									_T("ASC time"), 0x01, 0x03, 0x00B3, 0x0001, 0, true, true, true, NULL, 0, Timevalue, NULL, 0,\
									_T("Alrm slnc tm"), 0x01, 0x03, 0x00B4, 0x0001, 0, true, true, true, NULL, 0, Minutes, NULL, 0,\
									_T("Power up with lockout"), 0x01, 0x03, 0x00B5, 0x0001, 0, true, true, true, NULL, 0, Multivalue, PowerUpWithLockoutCodes, sizeof(PowerUpWithLockoutCodes)/sizeof(SOLAMULTIVALUE)};

	static SOLAMBMAP SystemIDProductType[] =  { _T("Product type"), 0x01, 0x03, 0x00B0, 0x0001, 0, true, true, true, NULL, 0, Multivalue, SystemIDProductTypes, sizeof(SystemIDProductTypes)/sizeof(SOLAMULTIVALUE)};
	static SOLAMBMAP SystemIDBurnerName[] =  {	_T("Burner name"), 0x01, 0x03, 0x00B7, 0x0001, 0, true, true, true, new char[21], 21, Stringvalue, NULL, 0};
	static SOLAMBMAP SystemIDInstallationData[] = { _T("Instltn data"), 0x01, 0x03, 0x00B8, 0x0001, 0, true, true, true, new char[21], 21, Stringvalue, NULL, 0};
	static SOLAMBMAP SystemIDOEMID[] = { _T("OEM ID"), 0x01, 0x03, 0x00B9, 0x0001, 0, true, true, true, new char[21], 21, Stringvalue, NULL, 0};
	static SOLAMBMAP SystemIDOSNumber[] = { _T("OS number"), 0x01, 0x03, 0x00BA, 0x0001, 0, true, true, true, new char[17], 17, Stringvalue, NULL, 0};
	static SOLAMBMAP SystemIDDateCode[] = { _T("Date code"), 0x01, 0x03, 0x00BB, 0x0001, 0, true, true, true, new char[11], 11, Stringvalue, NULL, 0};
	static SOLAMBMAP SystemIDSafetyProcesorBuild[] = { _T("Safety s/w ver"), 0x01, 0x03, 0x00BC, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0};
	static SOLAMBMAP SystemIDAppProcessorBuild[] = { _T("App s/w ver"), 0x01, 0x03, 0x00BD, 0x0001, 0, true, true, true, NULL, 0, Numericvalue, NULL, 0};

	pcSystemConfiguration = new CSolaMBMap(SystemConfiguration, sizeof(SystemConfiguration)/sizeof(SOLAMBMAP));
	pcSystemIDProductType = new CSolaMBMap(SystemIDProductType, sizeof(SystemIDProductType)/sizeof(SOLAMBMAP));
	pcSystemIDBurnerName = new CSolaMBMap(SystemIDBurnerName, sizeof(SystemIDBurnerName)/sizeof(SOLAMBMAP));
	pcSystemIDInstallationData = new CSolaMBMap(SystemIDInstallationData, sizeof(SystemIDInstallationData)/sizeof(SOLAMBMAP));
	pcSystemIDOEMID = new CSolaMBMap(SystemIDOEMID, sizeof(SystemIDOEMID)/sizeof(SOLAMBMAP));
	pcSystemIDOSNumber = new CSolaMBMap(SystemIDOSNumber, sizeof(SystemIDOSNumber)/sizeof(SOLAMBMAP));
	pcSystemIDDateCode = new CSolaMBMap(SystemIDDateCode, sizeof(SystemIDDateCode)/sizeof(SOLAMBMAP));
	pcSystemIDSafetyProcesorBuild = new CSolaMBMap(SystemIDSafetyProcesorBuild, sizeof(SystemIDSafetyProcesorBuild)/sizeof(SOLAMBMAP));
	pcSystemIDAppProcessorBuild = new CSolaMBMap(SystemIDAppProcessorBuild, sizeof(SystemIDAppProcessorBuild)/sizeof(SOLAMBMAP));

	static SOLAMBMAP AlarmCode[] = { _T("Active Alarm"), 0x01, 0x03, 0x045F, 0x0001, 0, true, true, true, NULL, 0, Alarmcode, NULL, 0};
	
	pcAlarmCode = (CSolaMBMap*)new CSolaMBMap(AlarmCode, sizeof(AlarmCode)/sizeof(SOLAMBMAP));

	static SOLAU32MAP Statistics[] = {	_T("Burner cycles"), 0x01, 0x03, 0x0080, 0x0002, 0L, true, true, true, NULL, 0, Unsigned32, NULL, 0,\
							_T("Burner run time"), 0x01, 0x03, 0x0082, 0x0002, 0L, true, true, true, NULL, 0, Unsigned32, NULL, 0,\
							_T("CH pump cycles"), 0x01, 0x03, 0x0084, 0x0002, 0L, true, true, true, NULL, 0, Unsigned32, NULL, 0,\
							_T("DHW pump cycles"), 0x01, 0x03, 0x0086, 0x0002, 0L, true, true, true, NULL, 0, Unsigned32, NULL, 0,\
							_T("System pump cycles"), 0x01, 0x03, 0x0088, 0x0002, 0L, true, true, true, NULL, 0, Unsigned32, NULL, 0,\
							_T("Boiler pump cycles"), 0x01, 0x03, 0x008A, 0x0002, 0L, true, true, true, NULL, 0, Unsigned32, NULL, 0,\
							_T("Aux. pump cycles"), 0x01, 0x03, 0x008C, 0x0002, 0L, true, true, true, NULL, 0, Unsigned32, NULL, 0,\
							_T("Ctrlr cycles"), 0x01, 0x03, 0x008E, 0x0002, 0L, true, true, true, NULL, 0, Unsigned32, NULL, 0,\
							_T("Ctrlr run time"), 0x01, 0x03, 0x0090, 0x0002, 0L, true, true, true, NULL, 0, Unsigned32, NULL, 0,\
							_T("Aux.2 pump cycles"), 0x01, 0x03, 0x0092, 0x0002, 0L, true, true, true, NULL, 0, Unsigned32, NULL, 0};
	
	pcStatistics = (CSolaMBMap*)new CSolaMBMap(Statistics, sizeof(Statistics)/sizeof(SOLAU32MAP));

	static SOLAMBMAP FrostProtConfig[] = {_T("CH fr. pr. nbl"), 0x01, 0x03, 0x0210, 0x0001, 0, true, true, true, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE), \
		_T("DHW fr. pr. nbl"),  0x01, 0x03, 0x0211, 0x0001, 0, true, true, true, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE), \
		_T("OD fr. pr. stpt."),  0x01, 0x03, 0x0212, 0x0001, 0, true, true, true, NULL, 0, TemperatureSetpoint, NULL, 0, \
		_T("RESERVED"),  0x01, 0x03, 0x0213, 0, 0x0001, true, true, true, NULL, 0, Novalue, NULL, 0, \
		_T("LL fr. pr. nbl"),  0x01, 0x03, 0x0214, 0x0001, 0, true, true, true, NULL, 0, Multivalue, EnableCodes, sizeof(EnableCodes)/sizeof(SOLAMULTIVALUE), \
		_T("LL fr. pr. rt."),  0x01, 0x03, 0x0215, 0x0001, 0, true, true, true, NULL, 0, Percentvalue, NULL, 0, \
		_T("RESERVED"),  0x01, 0x03, 0x0216, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0, \
		_T("RESERVED"),  0x01, 0x03, 0x0217, 0x0001, 0, true, true, true, NULL, 0, Novalue, NULL, 0};

	pcFrostProtConfig = (CSolaMBMap*)new CSolaMBMap(FrostProtConfig,sizeof(FrostProtConfig)/sizeof(SOLAMBMAP));

/* Define groups of Sola register maps */
	static SOLAMBMAPS TrendMaps[] = {pcSystemStatus,
									pcTrendStatus};

	pcTrendMaps = (CSolaMBMaps*) new CSolaMBMaps(TrendMaps,sizeof(TrendMaps)/sizeof(SOLAMBMAPS));

	static SOLAMBMAPS AllSolaMaps[] = {pcSystemStatus,
									pcTrendStatus,
									pcBurnerControlStatus,
									pcSensorStatus,
									pcExtendedSensorStatus,
									pcDemandModulationStatus,
									pcCHStatus,
									pcDHWStatus,
									pcPumpStatus,
									pcLLStatus,
									pcXLLStatus,
									pcSystemConfiguration,
									pcCHConfiguration,
									pcModConfiguration,
									pcODResetConfig,
									pcDHWConfiguration,
									pcPumpConfig,
									pcStatistics,
									pcLimitsConfig,
									pcBurnerControlConfig,
									pcLLConfig,
									pcXLLConfig,
									pcSystemIDProductType,
									pcSystemIDBurnerName,
									pcSystemIDInstallationData,
									pcSystemIDOEMID,
									pcSystemIDOSNumber,
									pcSystemIDDateCode,
									pcSystemIDSafetyProcesorBuild,
									pcSystemIDAppProcessorBuild,
									pcXSystemConfig,
									pcAlarmCode,
									pcAnnuncConfigGen,
									pcAnnuncConfig1,
									pcAnnuncConfig2,
									pcAnnuncConfig3,
									pcAnnuncConfig4,
									pcAnnuncConfig5,
									pcAnnuncConfig6,
									pcAnnuncConfig7,
									pcAnnuncConfig8,
									pcPIIAnnuncConfig,
									pcLCIAnnuncConfig,
									pcILKAnnuncConfig,
									pcXCHConfig,
									pcX2CHConfig,
									pcXModConfig,
									pcFrostProtConfig,
									pcFanConfig};

	pcAllSolaMaps = (CSolaMBMaps*) new CSolaMBMaps(AllSolaMaps,sizeof(AllSolaMaps)/sizeof(SOLAMBMAPS));

	static SOLAMBMAPS SystemIDMaps[] = {	pcSystemIDProductType,
									pcSystemIDBurnerName,
									pcSystemIDInstallationData,
									pcSystemIDOEMID,
									pcSystemIDOSNumber,
									pcSystemIDDateCode,
									pcSystemIDSafetyProcesorBuild,
									pcSystemIDAppProcessorBuild };

	pcSystemIDMaps = (CSolaMBMaps*) new CSolaMBMaps(SystemIDMaps,sizeof(SystemIDMaps)/sizeof(SOLAMBMAPS));

	static SOLALOCKOUT LockoutLog[] = {	0x01, 0x03, 0x0360, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x0371, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x0382, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x0393, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x03A4, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x03B5, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x03C6, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x03D7, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x03E8, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x03F9, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x040A, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x041B, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x042C, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x043D, 0x0011, new SOLALOCKOUTUNION,
									0x01, 0x03, 0x044E, 0x0011, new SOLALOCKOUTUNION };
	
	pcLockoutLog = new CSolaLockout(LockoutLog, sizeof(LockoutLog)/sizeof(SOLALOCKOUT));

	static SOLAALERT AlertLog[] = {	0x01, 0x03, 0x0460, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x0466, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x046C, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x0472, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x0478, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x047E, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x0484, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x048A, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x0490, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x0496, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x049C, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x04A2, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x04A8, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x04AE, 0x0006, new SOLAALERTUNION,
								0x01, 0x03, 0x04B4, 0x0006, new SOLAALERTUNION };
	
	pcAlertLog = new CSolaAlert(AlertLog, sizeof(AlertLog)/sizeof(SOLAALERT));

	static SOLACHANGECODE ConfigChangeCodes[] = {	0x0001, _T("System config."), pcSystemConfiguration, NULL, NULL,
													0x0001, _T("Extnd system config."), pcXSystemConfig, NULL, NULL,
													0x0002, _T("Mod. config."), pcModConfiguration, NULL, NULL,
													0x0002, _T("X Mod. config."), pcXModConfig, NULL, NULL,
													0x0002, _T("XCH config."), pcXCHConfig, NULL, NULL,
													0x0002, _T("X2CH config."), pcX2CHConfig, NULL, NULL,
													0x0004, _T("CH config."), pcCHConfiguration, NULL, NULL,
													0x0004, _T("XCH config."), pcXCHConfig, NULL, NULL,
													0x0004, _T("X2CH config."), pcX2CHConfig, NULL, NULL,
													0x0008, _T("Burner control config."), pcBurnerControlConfig, NULL, NULL,
													0x0010, _T("Fan config."), pcFanConfig, NULL, NULL,
													0x0020, _T("Pump config."), pcPumpConfig, NULL, NULL,
													0x0040, _T("Annunc. config."), pcAnnuncConfigGen, NULL, NULL,
													0x0040, _T("Annunc. 1 config."), pcAnnuncConfig1, NULL, NULL,
													0x0040, _T("Annunc. 2 config."), pcAnnuncConfig2, NULL, NULL,
													0x0040, _T("Annunc. 3 config."), pcAnnuncConfig3, NULL, NULL,
													0x0040, _T("Annunc. 4 config."), pcAnnuncConfig4, NULL, NULL,
													0x0040, _T("Annunc. 5 config."), pcAnnuncConfig5, NULL, NULL,
													0x0040, _T("Annunc. 6 config."), pcAnnuncConfig6, NULL, NULL,
													0x0040, _T("Annunc. 7 config."), pcAnnuncConfig7, NULL, NULL,
													0x0040, _T("Annunc. 8 config."), pcAnnuncConfig8, NULL, NULL,
													0x0040, _T("PII annunc. config."), pcPIIAnnuncConfig, NULL, NULL,
													0x0040, _T("LCI annunc. config."), pcLCIAnnuncConfig, NULL, NULL,
													0x0040, _T("ILK annunc. config."), pcILKAnnuncConfig, NULL, NULL,
													0x0080, _T("DHW config."), pcDHWConfiguration, NULL, NULL,
													0x0100, _T("Limits config."), pcLimitsConfig, NULL, NULL,
													0x0200, _T("Anti-condensation config."), NULL, NULL, NULL,
													0x0400, _T("Outdoor reset config."), pcODResetConfig, NULL, NULL,
													0x0800, _T("Frost prot. config."), pcFrostProtConfig, NULL, NULL,
													0x1000, _T("LL config."), pcLLConfig, NULL, NULL,
													0x1000, _T("Xtnd LL config."), pcXLLConfig, NULL, NULL,
													0x2000, _T("Reserved"), NULL, NULL, NULL,
													0x4000, _T("PM config."), NULL, NULL, NULL,
													0x8000, _T("PCB config."), NULL, NULL, NULL };

	pcConfigChangeCodes = (CSolaChangeCode*) new CSolaChangeCode(ConfigChangeCodes,sizeof(ConfigChangeCodes)/sizeof(SOLACHANGECODE));

	static SOLACHANGECODE StatusChangeCodes[] = {	0x0001, _T("Active Lockout"), pcAlarmCode, NULL, NULL,
										0x0002, _T("Burner control status"), pcBurnerControlStatus, NULL, NULL,
										0x0004, _T("Sensor status"), pcSensorStatus, NULL, NULL,
										0x0004, _T("XSensor status"), pcExtendedSensorStatus, NULL, NULL,
										0x0008, _T("Demand&Modulation status"), pcDemandModulationStatus, NULL, NULL, 
										0x0010, _T("CH status"), pcCHStatus, NULL, NULL,
										0x0020, _T("DHW status"), pcDHWStatus, NULL, NULL,
										0x0040, _T("Pump status"), pcPumpStatus, NULL, NULL,
										0x0080, _T("Statistics"), pcStatistics, NULL, NULL,
										0x0100, _T("LL status"), pcLLStatus, NULL, NULL,
										0x0100, _T("Ext. LL status"), pcXLLStatus, NULL, NULL,
										0x0200, _T("Lockout history"), NULL, NULL, pcLockoutLog,
										0x0400, _T("Alert log"), NULL, pcAlertLog, NULL,
										0x0800, _T("Reserved"), NULL, NULL, NULL,
										0x1000, _T("Reserved"), NULL, NULL, NULL,
										0x2000, _T("Reserved"), NULL, NULL, NULL,
										0x4000, _T("Reserved"), NULL, NULL, NULL,
										0x8000, _T("Reserved"), NULL, NULL, NULL };

	pcStatusChangeCodes = (CSolaChangeCode*) new CSolaChangeCode(StatusChangeCodes,sizeof(StatusChangeCodes)/sizeof(SOLACHANGECODE));

	static SOLAPAGE const SummaryPage[] = {
		_T(""), 2, pcBurnerControlStatus,
		_T(""), 8, pcBurnerControlStatus,
		_T("Demand"), 0, pcTrendStatus,
		_T("Burner state"), 1, pcBurnerControlStatus,
		_T("Brnr seq."), 6, pcBurnerControlStatus,
		_T("Brnr dly."), 7, pcBurnerControlStatus,
		_T("DHW pri. remain"), 3, pcDHWStatus,
		_T("CH pump"), 3, pcPumpStatus,
		_T("DHW pump"), 7, pcPumpStatus,
		_T("System pump"), 12, pcPumpStatus,
		_T("Boiler pump"), 15, pcPumpStatus,
		_T("LL setpoint"), 12, pcTrendStatus,
		_T("Inlet"), 5, pcTrendStatus,
		_T("Outlet"), 1, pcTrendStatus,
		_T("System"), 4, pcExtendedSensorStatus,
		_T("Stack"), 8, pcTrendStatus,
		_T("4-20mA"), 9, pcTrendStatus,
//		_T("Outdoor"), 7, pcTrendStatus,
		_T("Outdoor"), 0, pcExtendedSensorStatus,
		_T("Firing rate"), 2, pcTrendStatus,
		_T("Fan speed"), 3, pcTrendStatus,
		_T("Flame signal"), 4, pcTrendStatus,
		_T("DHW"), 6, pcTrendStatus,
		_T("CH setpoint"), 10, pcTrendStatus,
		_T("DHW setpoint"), 11, pcTrendStatus,
		_T("Active rate limiter"), 0, pcDemandModulationStatus,
		_T("Limited rate"), 1, pcDemandModulationStatus,
		_T("Active rate ovrd"), 2, pcDemandModulationStatus,
		_T("Override rate"), 3, pcDemandModulationStatus,
		_T("Demand rate"), 4, pcDemandModulationStatus,
		_T("CH on hyst."), 7, pcCHStatus,
		_T("CH off hyst."), 8, pcCHStatus,
		_T("DHW on hyst."), 9, pcDHWStatus,
		_T("DHW off hyst."), 10, pcDHWStatus,
		_T("LL on hyst."), 5, pcLLConfig,
		_T("LL off hyst."), 6, pcLLConfig,
		_T("Outlet sensor state"), 5, pcSensorStatus,
		_T("System sensor state"), 5, pcExtendedSensorStatus,
		_T("Inlet sensor state"), 6, pcSensorStatus,
		_T("DHW sensor state"), 7, pcSensorStatus,
		_T("Stack sensor state"), 8, pcSensorStatus,
		_T("Outdoor sensor state"), 9, pcSensorStatus,
		_T("Header sensor state"), 10, pcSensorStatus,
		_T("Alarm code"), 0, pcAlarmCode,
		_T("Setpoint"), 23, pcTrendStatus
	};

	pcSummaryPage = (CSolaPage*) new CSolaPage((LPSOLAPAGE const)SummaryPage,sizeof(SummaryPage)/sizeof(SOLAPAGE));

	return bSuccess;
}