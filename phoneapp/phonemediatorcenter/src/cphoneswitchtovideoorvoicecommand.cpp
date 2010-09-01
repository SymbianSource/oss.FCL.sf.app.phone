/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneSwitchToVideoOrVoiceCommand class.
*
*/


// INCLUDE FILES
#include "phonelogger.h"
#include "cphoneswitchtovideoorvoicecommand.h"
#include "pevirtualengine.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPhoneSwitchToVideoOrVoiceCommand::CPhoneSwitchToVideoOrVoiceCommand
// ---------------------------------------------------------
//
CPhoneSwitchToVideoOrVoiceCommand::CPhoneSwitchToVideoOrVoiceCommand( 
    MPhoneEngineMessageSender& aEngineMessageSender ) : 
    iEngineMessageSender( aEngineMessageSender )
    {
    }

// ---------------------------------------------------------
// CPhoneSwitchToVideoOrVoiceCommand::ConstructL
// ---------------------------------------------------------
//
void CPhoneSwitchToVideoOrVoiceCommand::ConstructL()
    {
    }

// ---------------------------------------------------------
// CPhoneSwitchToVideoOrVoiceCommand::NewL
// ---------------------------------------------------------
//
EXPORT_C CPhoneSwitchToVideoOrVoiceCommand* CPhoneSwitchToVideoOrVoiceCommand::NewL( 
    MPhoneEngineMessageSender& aEngineMessageSender )
    {
    __LOGMETHODSTARTEND( EPhoneMediatorCenter, "CPhoneSwitchToVideoOrVoiceCommand::NewL( ) ");
    CPhoneSwitchToVideoOrVoiceCommand* self = new( ELeave ) 
        CPhoneSwitchToVideoOrVoiceCommand( aEngineMessageSender );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }
    
// ---------------------------------------------------------
// CPhoneSwitchToVideoOrVoiceCommand::~CPhoneSwitchToVideoOrVoiceCommand
// ---------------------------------------------------------
//
CPhoneSwitchToVideoOrVoiceCommand::~CPhoneSwitchToVideoOrVoiceCommand()
    {
    }

// ---------------------------------------------------------
// CPhoneSwitchToVideoOrVoiceCommand::ExecuteL
// ---------------------------------------------------------
//
void CPhoneSwitchToVideoOrVoiceCommand::ExecuteLD()
	{
    __LOGMETHODSTARTEND( EPhoneMediatorCenter, 
        "CPhoneSwitchToVideoOrVoiceCommand::ExecuteL( ) ");
    iEngineMessageSender.SendPhoneEngineMessage( 
        MPEPhoneModel::EPEMessageSwitchToVideoOrVoice );
    delete this;
	}

//  End of File  
