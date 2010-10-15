
/*
* Copyright (c) 2010-2010 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  MT_PhoneRingingTonePlayer
*
*/

#ifndef __MT_PHONERINGINGTONEPLAYER_H__
#define __MT_PHONERINGINGTONEPLAYER_H__

#include <CEUnitTestSuiteClass.h>

#include "devsoundstub.h"
#include "rphonetoneclient.h"

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

NONSHARABLE_CLASS( MT_PhoneRingingTonePlayer )
     : public CEUnitTestSuiteClass
    {
public:

    static MT_PhoneRingingTonePlayer* NewL();
    static MT_PhoneRingingTonePlayer* NewLC();
    ~MT_PhoneRingingTonePlayer();

private:

    MT_PhoneRingingTonePlayer();
    void ConstructL();
    
private:
    
     void SetupL();
     
     void Teardown();
     
     void EmptySetupL();
     
     void EmptyTeardown();
     
     // Test cases :
     
     void MT_Empty();
     
  /*done*/   void MT_CreateServerInstanceAndConnectL();
     
  /*done*/   void MT_CreateServerInstanceAndConnectTwiceL(); // Inter-process communication fuzzing ( IPC - fuzz )
     
  /*done*/   void MT_PlayDefaultL();
     
  /*done*/   void MT_PlayCustomSampleL();
     
  /*done*/   void MT_PlayCustomSampleWithDelayL();
     
  /*done*/   void MT_PlayCustomSampleWithErrorL(); // Switch to default playback
     
  /*done*/   void MT_PlayCustomSampleWithDelayAndErrorL(); // timer callback
     
  /*done*/   void MT_PlayCustomToneL();
     
  /*done*/   void MT_PlayCustomToneWithDelayL();
     
  /*done*/   void MT_PlayCustomToneWithErrorL();
     
  /*done*/   void MT_PlayCustomToneWithDelayAndErrorL();
     
  /*done*/   void MT_PlayAndStopL();
   
  /*done*/   void MT_PlayAndStopDuringInitL();
     
  /*done*/   void MT_PlaySilentL();
     
     
private:
    
    // Utility functions
    
    TBool SamplePlayerContainsCall( CMdaAudioPlayerUtility::TFunctionCall aCall );
    TBool SamplePlayerContainsCallData( CTestFunctionCallData* aData );
    TBool SamplePlayerContainsOnlyOneCall( CMdaAudioPlayerUtility::TFunctionCall aCall );
    TBool TonePlayerContainsOnlyOneCall( CMdaAudioToneUtility::TFunctionCall aCall );
    TBool TonePlayerContainsCall( CMdaAudioToneUtility::TFunctionCall aCall );
    TBool TonePlayerContainsCallData( CTestFunctionCallData* aData );

private:
    
    RPhoneToneClient iClient;


    EUNIT_DECLARE_TEST_TABLE; 

    
    
    };

#endif // __MT_PHONERINGINGTONEPLAYER_H__

// End of file
