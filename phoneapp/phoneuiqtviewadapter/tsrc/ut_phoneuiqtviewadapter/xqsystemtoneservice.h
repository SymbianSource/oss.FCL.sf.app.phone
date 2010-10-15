/** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


#ifndef XQSYSTEMTONESERVICE_H
#define XQSYSTEMTONESERVICE_H

#include <QObject>
#include <systemtoneservice.h>

 
class XQSystemToneService : public QObject
{
  Q_OBJECT

public:

/**
 * Supported tone types.
*/
   enum ToneType {
	   // Capture Tones
	   BurstModeTone = CSystemToneService::EBurstMode,
	   CaptureTone = CSystemToneService::ECapture,
	   CallRecordingTone = CSystemToneService::ECallRecording,
	   RecordingStartTone = CSystemToneService::ERecordingStart,
	   RecordingStopTone = CSystemToneService::ERecordingStop,
	   SelfTimerTone = CSystemToneService::ESelfTimer,
	   
	   // General Tones
	   ConfirmationBeepTone = CSystemToneService::EConfirmationBeep,
	   DefaultBeepTone = CSystemToneService::EDefaultBeep,
	   ErrorBeepTone = CSystemToneService::EErrorBeep,
	   InformationBeepTone = CSystemToneService::EInformationBeep,
	   WarningBeepTone = CSystemToneService::EWarningBeep,
	   IntegratedHandsFreeActivatedTone = CSystemToneService::EIntegratedHandsFreeActivated,
	   
	   // Key Tones
	   TouchScreenTone = CSystemToneService::ETouchScreen,
	   
	   // Location Tones
	   LocationRequestTone = CSystemToneService::ELocationRequest,
	   
	   // Messaging Tones
	   ChatAlertTone = CSystemToneService::EChatAlert,
	   EmailAlertTone = CSystemToneService::EEmailAlert,
	   MmsAlertTone = CSystemToneService::EMmsAlert,
	   SmsAlertTone = CSystemToneService::ESmsAlert,
	   DeliveryReportTone = CSystemToneService::EDeliveryReport,
	   MessageSendFailureTone = CSystemToneService::EMessageSendFailure,
	   
	   // Power Tones
	   BatteryLowTone = CSystemToneService::EBatteryLow,
	   BatteryRechargedTone = CSystemToneService::EBatteryRecharged,
	   PowerOnTone = CSystemToneService::EPowerOn,
	   PowerOffTone = CSystemToneService::EPowerOff,
	   WakeUpTone = CSystemToneService::EWakeUp,
	   WrongChargerTone = CSystemToneService::EWrongCharger,
	   
	   // Telephony Tones
	   AutomaticRedialCompleteTone = CSystemToneService::EAutomaticRedialComplete,
	   
	   // Voice Recognition Tones
	   VoiceStartTone = CSystemToneService::EVoiceStart,
	   VoiceErrorTone = CSystemToneService::EVoiceError,
	   VoiceAbortTone = CSystemToneService::EVoiceAbort
  };
  
 enum AlarmType {
       // Calendar Tones
       CalendarAlarmTone = CSystemToneService::ECalendarAlarm,
       ClockAlarmTone = CSystemToneService::EClockAlarm,
       ToDoAlarmTone = CSystemToneService::EToDoAlarm,

       // Telephony Alarms
       IncomingCallTone = CSystemToneService::EIncomingCall,
       IncomingCallLine2Tone = CSystemToneService::EIncomingCallLine2,
       IncomingDataCallTone = CSystemToneService::EIncomingDataCall,
  };

  
public:
/**
* XQSystemToneService object constructor. 
*/
XQSystemToneService(QObject *parent = 0);

/**
* Simple destructor.
*/
~XQSystemToneService();

/**
* Play selected tone.
*
* @param  toneType  selected tone type.
*/
 void playTone(ToneType toneType);


 void playAlarm(AlarmType alarmType, unsigned int& contextId);
  
  
 void stopAlarm(unsigned int contextId);


 void playAlarm(ToneType toneType, unsigned int& contextId);
  


void toneStarted();

void alarmStarted(unsigned int contextId);
  
void alarmFinished(unsigned int contextId);

  
};

#endif /*SYSTEMTONESERVICE_H*/
