/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Class handles Easy dialing specific commands
*
*/

#include "cphoneeasydialingcontroller.h"

#include "cdialer.h"
#include "tphonecommandparam.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamdynmenu.h"
#include "dialingextensioninterface.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPhoneEasyDialingController::CPhoneEasyDialingController
// ---------------------------------------------------------------------------
//
CPhoneEasyDialingController::CPhoneEasyDialingController(
         CDialer& aDialer )
   : iDialer ( aDialer )
    {
    // Get Easydialing interface. This can be NULL if easydialing is not
    // present in current device configuration.
    iEasyDialing = iDialer.GetEasyDialingInterface();
    }

 // ---------------------------------------------------------------------------
 // CPhoneEasyDialingController::NewL
 // ---------------------------------------------------------------------------
 //
CPhoneEasyDialingController* CPhoneEasyDialingController::NewL( 
        CDialer& aDialer )
     {
     return new (ELeave) CPhoneEasyDialingController( aDialer );
     }

// ---------------------------------------------------------------------------
//  CPhoneEasyDialingController::~CPhoneEasyDialingController
// ---------------------------------------------------------------------------
//
CPhoneEasyDialingController::~CPhoneEasyDialingController()
     {
     }
 
// ---------------------------------------------------------------------------
//  CPhoneEasyDialingController::ExecuteCommandL
// ---------------------------------------------------------------------------
//
void CPhoneEasyDialingController::ExecuteCommandL(
        TPhoneViewCommandId aCmdId,
        TPhoneCommandParam* aCommandParam )
    {
    if ( iEasyDialing )
        {
        switch ( aCmdId )
            {
            case EPhoneViewGetEasyDialingMenuId:
                {
                TPhoneCmdParamInteger* paramInt = 
                    static_cast<TPhoneCmdParamInteger*>( aCommandParam );
                paramInt->SetInteger( iEasyDialing->MenuResourceId() );            
                }
                break;
                    
            case EPhoneViewGetEasyDialingCbaId:
                {
                TPhoneCmdParamInteger* paramInt = 
                    static_cast<TPhoneCmdParamInteger*>( aCommandParam );
                paramInt->SetInteger( iEasyDialing->CbaResourceId() );            
                }
                break;
                
            default:
                break;
            }
        }
    }

// ---------------------------------------------------------------------------
//  CPhoneEasyDialingController::HandleCommandL
// ---------------------------------------------------------------------------
//
TPhoneViewResponseId CPhoneEasyDialingController::HandleCommandL(
        TPhoneViewCommandId aCmdId )
    {
    TPhoneViewResponseId viewResponse = EPhoneViewResponseFailed;

    switch ( aCmdId )
        {
        // commands defined in easydialingcommands.hrh are forwarded to
        // easydialing plugin
        case EEasyDialingVoiceCall:
        case EEasyDialingVideoCall:
        case EEasyDialingSendMessage:
        case EEasyDialingOpenContact:
        case EEasyDialingCallHandlingActivated:
        case EEasyDialingEnterKeyAction:
        case EEasyDialingOn:
        case EEasyDialingOff:
            {
            if( iEasyDialing && iEasyDialing->HandleCommandL( aCmdId ) )
                {
                viewResponse = EPhoneViewResponseSuccess;
                }
            }
            break;
            
        case EPhoneViewGetEasyDialingInFocusStatus:
            {
            if ( iEasyDialing && iEasyDialing->IsFocused() )
                {
                viewResponse = EPhoneViewResponseSuccess;
                }
            }
            break;
        }
    
    return viewResponse;
    }

// ---------------------------------------------------------------------------
//  CPhoneEasyDialingController::InitializeEasyDialingMenuL
// ---------------------------------------------------------------------------
//
TBool CPhoneEasyDialingController::InitializeEasyDialingMenuL( 
        TPhoneCommandParam* aCommandParam )
    {
    TBool retVal( EFalse );
    if ( iEasyDialing )
        {
        TPhoneCmdParamDynMenu* menu = static_cast<TPhoneCmdParamDynMenu*>( aCommandParam );
    
       // TInt resourceId = menu->ResourceId();
        CEikMenuPane*  menuPane = reinterpret_cast<CEikMenuPane*>( menu->DynMenu() );
        
        retVal = iEasyDialing->InitializeMenuPaneL( *menuPane, menu->ResourceId() );
        }
    return retVal;
    }
