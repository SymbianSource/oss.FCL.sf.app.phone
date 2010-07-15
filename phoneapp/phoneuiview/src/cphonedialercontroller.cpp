/*
* Copyright (c) 2009, 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles the updating of dialer buttons
*
*/


#include <eikappui.h>
#include <eikenv.h>
#include <eikapp.h>
#include <eikcolib.h>
#include <gulicon.h>
#include <StringLoader.h>
#include <AknsUtils.h>
#include <phoneui.rsg>
#include <phoneui.mbg>
#include <pevirtualengine.h>
#include <featmgr.h>
#include <data_caging_path_literals.hrh>

#include "cphonedialercontroller.h"
#include "cphoneeasydialingcontroller.h"
#include "cphonebubblewrapper.h"
#include "phoneui.hrh"
#include "tphonecmdparaminteger.h"
#include "cphonemainresourceresolver.h"
#include "tphonecmdparamcallstatedata.h"
#include "tphonecmdparamboolean.h"

_LIT ( KPhoneMifFileName, "phoneui.mif" );


/** States for Call button. */
enum TCallButtonState
    {
    ECallButtonCallState = 0,
    ECallButtonLogState,
    ECallButtonSendState
    };
static const CPhoneDialerController::TLocalButtonData KCallButtonDataTable[] =
    {
        { 
        EPhoneNumberAcqCmdSendCommand, 
        EAknsMinorGenericQgnIndiButtonAnswerWhite,
        EMbmPhoneuiQgn_indi_button_answer_white, 
        EMbmPhoneuiQgn_indi_button_answer_white_mask,
        R_PHONEUI_DIALER_TOOLTIP_CALL
        },
        { 
        EPhoneDialerCmdLog,
        EAknsMinorGenericQgnIndiButtonLogdialerWhite,
        EMbmPhoneuiQgn_indi_button_logdialer_white,
        EMbmPhoneuiQgn_indi_button_logdialer_white_mask,
        R_PHONEUI_DIALER_TOOLTIP_RECENT_CALLS
        },
        { 
        EPhoneNumberAcqCmdSendCommand, 
        EAknsMinorGenericQgnIndiButtonAnswerWhite,
        EMbmPhoneuiQgn_indi_button_answer_white, 
        EMbmPhoneuiQgn_indi_button_answer_white_mask,
        R_PHONEUI_DIALER_TOOLTIP_SEND
        }
    };
static const TInt KCallButtonDataTableCount = 
    sizeof( KCallButtonDataTable ) / sizeof( CPhoneDialerController::TLocalButtonData );

/** States for Phonebook button */
enum TPhonebookButtonState
    {
    EPhonebookButtonContactsState = 0,
    EPhonebookButtonAddContactState,
    // Video call button send state
    EPhonebookButtonVideocallSendState,
    // Video call button call state
    EPhonebookButtonVideocallState
    };
static const CPhoneDialerController::TLocalButtonData KPhonebookButtonDataTable[] =
    {
        { 
        EPhoneDialerCmdContacts,
        EAknsMinorGenericQgnIndiDialerContacts,
        EMbmPhoneuiQgn_indi_dialer_contacts,
        EMbmPhoneuiQgn_indi_dialer_contacts_mask,
        R_PHONEUI_DIALER_TOOLTIP_CONTACTS
        },
        { 
        EPhoneNumberAcqCmdAddToContacts,
        EAknsMinorGenericQgnIndiDialerAddContacts,      
        EMbmPhoneuiQgn_indi_dialer_add_contacts,
        EMbmPhoneuiQgn_indi_dialer_add_contacts_mask,
        R_PHONEUI_DIALER_TOOLTIP_ADD_TO_CONTACTS
        },
        // Video call button 
        {
        EPhoneNumberAcqCmdVideoCall,
        EAknsMinorGenericQgnIndiDialerVideocallSend,
        EMbmPhoneuiQgn_indi_dialer_videocall_send,
        EMbmPhoneuiQgn_indi_dialer_videocall_send_mask,
        R_PHONEUI_DIALER_TOOLTIP_SEND
        },
        {
        EPhoneNumberAcqCmdVideoCall,
        EAknsMinorGenericQgnIndiDialerVideocallSend,
        EMbmPhoneuiQgn_indi_dialer_videocall_send,
        EMbmPhoneuiQgn_indi_dialer_videocall_send_mask,
        R_PHONEUI_DIALER_TOOLTIP_VIDEO_CALL
        }
    };
static const TInt KPhonebookButtonDataTableCount = 
    sizeof( KPhonebookButtonDataTable ) / sizeof( CPhoneDialerController::TLocalButtonData );

/** States for Clear button */
enum TClearButtonState
    {
    EClearButtonClearState = 0
    };
static const CPhoneDialerController::TLocalButtonData KClearButtonDataTable[] =
    {
        { 
        EPhoneDialerCmdClear,
        EAknsMinorGenericQgnIndiDialerClear,
        EMbmPhoneuiQgn_indi_dialer_clear,
        EMbmPhoneuiQgn_indi_dialer_clear_mask,
        0   //no tooltip for clear key
        }
    };
static const TInt KClearButtonDataTableCount = 
    sizeof( KClearButtonDataTable ) / sizeof( CPhoneDialerController::TLocalButtonData );


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPhoneDialerController::CPhoneDialerController( 
                                        CPhoneBubbleWrapper* aBubbleWrapper,
                                        CCoeEnv& aCoeEnv ):
    iCoeEnv( aCoeEnv ),
    iBubbleWrapper( aBubbleWrapper ),
    iRestrictedDialer( ETrue )
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneDialerController::ConstructL()
    {
    }
    
// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhoneDialerController* CPhoneDialerController::NewL( 
                                    CPhoneBubbleWrapper* aBubbleWrapper,
                                    CCoeEnv& aCoeEnv )
    {
    CPhoneDialerController* self = 
        new (ELeave) CPhoneDialerController( aBubbleWrapper, aCoeEnv );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPhoneDialerController::~CPhoneDialerController()
    {
    }

// ---------------------------------------------------------------------------
// SetRestrictedDialerL
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::SetRestrictedDialer( TBool aRestricted )
    {
    iRestrictedDialer = aRestricted;
    }

// ---------------------------------------------------------------------------
// SetServiceCodeFlagL
//
// The flag rules wether green key is called "Send" or "Call"
// 
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::SetServiceCodeFlag( 
    TPhoneCommandParam* aCommandParam )
    {
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* serviceCodeFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
        iServiceCodeFlag = serviceCodeFlag->Boolean();
        }
    }

// ---------------------------------------------------------------------------
// SetEasyDialingController
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::SetEasyDialingController( 
        CPhoneEasyDialingController* aController )
    {
    iEasyDialingController = aController;
    }

// ---------------------------------------------------------------------------
// CbaResourceId
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPhoneDialerController::CbaResourceId() const
    {
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// MenuResourceId
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPhoneDialerController::MenuResourceId() const
    {
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// NumberEntryPromptTextL
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CPhoneDialerController::NumberEntryPromptTextL()
    {
    return KNullDesC;
    }

// ---------------------------------------------------------------------------
// GetButtonData
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPhoneDialerController::GetButtonData( TButtonIndex aIndex, 
        RPointerArray<CButtonData>& aData ) const
    {
    TInt err = KErrNone;
    
    const TLocalButtonData* dataTable = NULL;
    TInt count = 0;
    
    // Select correct data table
    switch ( aIndex )
        {
        case ECallButton:
            dataTable = KCallButtonDataTable;
            count = KCallButtonDataTableCount;
            break;
        case EPhonebookButton:
            dataTable = KPhonebookButtonDataTable;
            count = KPhonebookButtonDataTableCount;
            break;
        case EClearButton:
            dataTable = KClearButtonDataTable;
            count = KClearButtonDataTableCount;
            break;
        default:
            break;
        }
    
    // Create data from the table
    if ( dataTable )
        {
        // Construct MIF file path
        TFileName mifPath( KDriveZ );
        mifPath.Append( KDC_APP_BITMAP_DIR );
        mifPath.Append( KPhoneMifFileName );
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    
        for ( TInt i = 0 ; i < count ; ++i )
            {
            CButtonData* btnData = NULL;
            TRAP( err, btnData = CreateButtonDataL( dataTable[i], skin, mifPath ) );
            if ( !err )
                {
                err = aData.Append( btnData );
                }
            if ( err )
                {
                // free all memory and stop here if something went wrong
                delete btnData;
                aData.ResetAndDestroy();
                i = count;
                }
            }
        }
    return err;
    }

// ---------------------------------------------------------------------------
// SetNumberEntryEmptyIsEmpty
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneDialerController::SetNumberEntryIsEmpty( TBool aEmpty )
    {
    iNumberAvailable = !aEmpty;
    }

// ---------------------------------------------------------------------------
// ButtonState
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPhoneDialerController::ButtonState( TButtonIndex aIndex ) const
    {
    TInt state( KErrNotFound );
    // Figure out correct state for each key
    switch ( aIndex )
        {
        case ECallButton:
            {
            if ( !iNumberAvailable )
                {
                // Show "Recent calls" when no number available
                state = ECallButtonLogState;
                }
            else if ( iServiceCodeFlag && !EasyDialingFocused() )
                {
                // Show "Send" if service code entered and contact list
                // is not focused.
                state = ECallButtonSendState;
                }
            else
                {
                // Show "Call" in all other cases
                state = ECallButtonCallState;
                }
            }
            break;
        case EPhonebookButton:
            {
            if ( !iNumberAvailable )
                {
                state = EPhonebookButtonContactsState;
                }
            else if ( FeatureManager::FeatureSupported( KFeatureIdFfTdScdmaVideocallKey ) )
                {
                if ( iServiceCodeFlag && !EasyDialingFocused() )
                    {
                    state = EPhonebookButtonVideocallSendState;
                    }
                else
                    {
                    state = EPhonebookButtonVideocallState;
                    }
                }
            else
                {
                state = EPhonebookButtonAddContactState;
                }
            }
            break;
        case EClearButton:
            {
            state = EClearButtonClearState;
            }
            break;
        default:
            break;
        }
    return state;
    }

// ---------------------------------------------------------------------------
// ButtonDimmed
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneDialerController::ButtonDimmed( TButtonIndex aIndex ) const
    {
    TBool dimmed = EFalse;
    // Figure out correct state for each key
    switch ( aIndex )
        {
        case ECallButton:
			{
        	if ( iNumberAvailable )
        		{
        		// Dim send key if emergency call is active. 
        		// Do not dim send key if emergency call is not active,
        		// user must be able to make an emergency call whether 
        		// security mode is enabled or disabled.
        		dimmed = EmergencyCallActive();
        		}
        	else
        		{
        		// If there's no number available in dialer, we should dim
        		// send key if security mode is enabled.
        		// User cannot launch logs application.
        		dimmed = iRestrictedDialer;
        		}
			}
        	break;
            
        case EPhonebookButton:
            {
            dimmed = iRestrictedDialer;
            // Check if video call button supported
            TBool videoCallKeySupported = FeatureManager::FeatureSupported( KFeatureIdFfTdScdmaVideocallKey );
            // Prevent video call but don't prevent opening Contacts during emergency call.
            if ( !dimmed && videoCallKeySupported && iNumberAvailable )
                {
                dimmed = EmergencyCallActive();
                }
            }
            break;
        case EClearButton:
            {
            dimmed = !iNumberAvailable;
            }
            break;
        default:
            break;
        }
    return dimmed;
    }

// ---------------------------------------------------------------------------
// EasyDialingAllowed
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneDialerController::EasyDialingAllowed() const
    {
    return !iRestrictedDialer && !EmergencyCallActive();
    }

// ---------------------------------------------------------------------------
// EmergencyCallActive
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneDialerController::EmergencyCallActive() const
    {
    TBool ret = EFalse;
    if ( iBubbleWrapper )
        {
        TPhoneCmdParamCallStateData callStateData;
        callStateData.SetCallState( EPEStateConnected );
        iBubbleWrapper->GetCallIdByState( &callStateData );
        TInt connected = callStateData.CallId();
        ret = ( connected == KEmergencyCallId );
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// EasyDialingFocused
// ---------------------------------------------------------------------------
//
TBool CPhoneDialerController::EasyDialingFocused() const
    {
    TBool edFocused = EFalse;
    if ( iEasyDialingController )
        {
        TInt commandResponse( EPhoneViewResponseFailed );
        TRAP_IGNORE( commandResponse = iEasyDialingController->HandleCommandL(
                EPhoneViewGetEasyDialingInFocusStatus) );
        edFocused = ( commandResponse == EPhoneViewResponseSuccess );
        }
    return edFocused;
    }

// ---------------------------------------------------------------------------
// CreateButtonDataL
// ---------------------------------------------------------------------------
//
MPhoneDialerController::CButtonData* CPhoneDialerController::CreateButtonDataL( 
        const TLocalButtonData& aSourceData, 
        MAknsSkinInstance* aSkin, 
        const TDesC& aMifFile ) const
    {
    // Create tooltip text
    HBufC* tooltip = NULL;
    if ( aSourceData.iTooltipResourceId > 0 )
        {
        tooltip = StringLoader::LoadLC( aSourceData.iTooltipResourceId );
        }
    else
        {
        tooltip = HBufC::NewLC(0);
        }
    
    // Create icon
    TAknsItemID skinId = { EAknsMajorGeneric, aSourceData.iSkinItemIdMinor };
    CGulIcon* icon = AknsUtils::CreateGulIconL( aSkin, skinId, aMifFile, 
            aSourceData.iBitmapIndex, aSourceData.iBitmapMaskIndex );
    CleanupStack::PushL( icon );
    
    // Create button data object
    CButtonData* buttonData =
        new ( ELeave ) CButtonData( aSourceData.iCommandId, icon, tooltip );
    CleanupStack::Pop( icon );
    CleanupStack::Pop( tooltip );
    
    return buttonData;
    }

// END
