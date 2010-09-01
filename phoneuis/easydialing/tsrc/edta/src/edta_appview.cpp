/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Easy dialing test application.
*
*/

// INCLUDE FILES
#include <coemain.h>
#include <aknutils.h>
#include "edta_appview.h"
#include "edta_debugprint.h"
#include "edta_dummylistener.h"
#include <aknnotewrappers.h>        

// AVKON components
#include <aknlists.h>
#include <aknpopup.h>

// Predictive search header files.
#include <CPsSettings.h>
#include <CPsQuery.h>
#include <CPsQueryItem.h>
#include <CPsClientData.h>
#include <CPsPattern.h>
#include <CPsRequestHandler.h>

// Virtual phonebook header files.
#include <VPbkContactStoreUris.h>
#include <CVPbkContactStoreUriArray.h>
#include <TVPbkContactStoreUriPtr.h>
// contains virtual phonebook data fields
#include <vpbkeng.rsg>

// Phonebook engine API 
#include <CPbkContactEngine.h>

// CCA Launcher header files.
#include <ccafactory.h>
#include <mccaparameter.h>
#include <mccaconnection.h>


const TInt KEdtaMaximumMatchingCount = 100;
const TInt KEdtaContactCompanyIndex = 2;
const TInt KEdtaContactEmailHomeIndex = 3;
const TInt KEdtaContactEmailWorkIndex = 4;
const TInt KEdtaContactEmailGenIndex = 5;
const TInt KEdtaListBoxGranularity = 5;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// NewL
// First phase constructor without cleanup stack.
// -----------------------------------------------------------------------------
//
CEdtaAppView* CEdtaAppView::NewL( const TRect& aRect )
    {
    CEdtaAppView* self = CEdtaAppView::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// NewLC
// First phase constructor with cleanup stack.
// -----------------------------------------------------------------------------
//
CEdtaAppView* CEdtaAppView::NewLC( const TRect& aRect )
    {
    CEdtaAppView* self = new ( ELeave ) CEdtaAppView;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// -----------------------------------------------------------------------------
// ConstructL
// Second phase constructor.
// -----------------------------------------------------------------------------
//
void CEdtaAppView::ConstructL( const TRect& aRect )
    {
    DebugPrintF(_L("CEdtaAppView::ConstructL enter, rect: %d, %d - %d, %d"), aRect.iTl.iX, aRect.iTl.iY, aRect.iBr.iX, aRect.iBr.iY  );

    // Create a window for this application view
    CreateWindowL();

    const TDesC& thname = RThread().Name();
    iScreenBuffer = CEdtaScreenTextBuffer::NewL( aRect );
    iScreenBuffer->SetFocus( ETrue );
    iScreenBuffer->WriteLineL(thname);

    LoadPluginL(); 
  
    DebugPrintF(_L("CEdtaAppView::ConstructL: iScreenBuffer: 0x%X, iEasyDialer: 0x%X"), iScreenBuffer, iEasyDialer );

    if (iEasyDialer) 
        {
        DebugPrintF(_L("CEdtaAppView::ConstructL: Easydialer plugin found, adding observer..") );

        iEasyDialer->AddObserverL( this );
        iEasyDialer->SetFocus( EFalse );

        DebugPrintF(_L("CEdtaAppView::ConstructL: Add observer done"));
        }

    // Init PCS Server..
    TRAPD( error, InitPredictiveContactSearchL());
    if( error )
        {
        DebugPrintF(_L("CEdtaAppView::ConstructL: Failed to load predictive search engine: %d"), error );
        iScreenBuffer->WriteLineL(_L("* PCS Server load fail: %d"), error );
        }

    // Init Communication Launcher..
    TRAP( error, iContactLauncher = TCCAFactory::NewConnectionL());
    if( error )
        {
        DebugPrintF(_L("CEdtaAppView::ConstructL: Failed connect communication launcher: %d"), error );
        iScreenBuffer->WriteLineL(_L("* CCA open fail: %d"), error );
        }
        
    iDummyListener = new ( ELeave ) CDummyListener();

    // Set the windows size
    SetRect( aRect );
    ActivateL();

    DebugPrintF(_L("CEdtaAppView::ConstructL exit"));
    }

// -----------------------------------------------------------------------------
// CEdtaAppView
// The C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CEdtaAppView::CEdtaAppView()
    {
    // No implementation here
    }

// -----------------------------------------------------------------------------
// ~CEdtaAppView
// The destructor.
// -----------------------------------------------------------------------------
//
CEdtaAppView::~CEdtaAppView()
    {
    DebugPrintF(_L("CEdtaAppView::~CEdtaAppView enter, iEasyDialer: 0x%x"), iEasyDialer );
    
    delete iDummyListener;

    if (iEasyDialer)
        {
        iEasyDialer->RemoveObserver( this );
        }

    if (iPredictiveContactSearchHandler)
        {
        iPredictiveContactSearchHandler->RemoveObserver(this);
        }

    delete iEasyDialer;
    iContactNames.ResetAndDestroy();
    iMatchingContactLinks.ResetAndDestroy();

    if (iContactLauncher)
        {
        iContactLauncher->Close();
        }

    delete iPredictiveSearchQuery;
    delete iContactManager;
    delete iPredictiveContactSearchHandler;
    delete iContactStoreUriArray;
    iContactDataStores.ResetAndDestroy();

    delete iScreenBuffer;
    DebugPrintF(_L("CEdtaAppView::~CEdtaAppView exit"));
    }


// -----------------------------------------------------------------------------
// Draw
// Draw itself. Clear the canvas.
// -----------------------------------------------------------------------------
//

void CEdtaAppView::Draw( const TRect& aRect ) const
    {
    DebugPrintF(_L("CEdtaAppView::Draw, rect: %d, %d - %d, %d"), aRect.iTl.iX, aRect.iTl.iY, aRect.iBr.iX, aRect.iBr.iY );

    CWindowGc& gc = SystemGc();
    gc.SetClippingRect( aRect );
    
    gc.SetBrushColor( KRgbWhite );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.Clear( aRect );
    }


// -----------------------------------------------------------------------------
// SizeChanged
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CEdtaAppView::SizeChanged()
    {
    DebugPrintF(_L("CEdtaAppView::SizeChanged: iScreenBuffer: 0x%X, iEasyDialer: 0x%X"), iScreenBuffer, iEasyDialer );

    // Controls must set rects, otherwise not drawn
    // If easy dialer plugin is loaded, the controls are reorganized to screen.
    if( iEasyDialer )
        {
        // Screen buffer occupies upper half of the view.
        TRect screen_buffer_rect = iAvkonAppUi->ClientRect();
        screen_buffer_rect.iBr.iY -= (screen_buffer_rect.Height() / 2 );
        TRect easy_dialer_rect(0, screen_buffer_rect.Height(), screen_buffer_rect.Width(), iAvkonAppUi->ClientRect().Height());
        iEasyDialer->SetRect( easy_dialer_rect );
        iScreenBuffer->SetRect(screen_buffer_rect);
        }
    else if ( iScreenBuffer )
        {
        iScreenBuffer->SetRect(iAvkonAppUi->ClientRect());
        }
    }


// -----------------------------------------------------------------------------
// CountComponentControls
// -----------------------------------------------------------------------------
//
TInt CEdtaAppView::CountComponentControls() const
    {
    return iEasyDialer ? 2 : 1;
    }


// -----------------------------------------------------------------------------
// ComponentControl
// -----------------------------------------------------------------------------
//
CCoeControl* CEdtaAppView::ComponentControl(TInt aIndex) const
    {
    switch (aIndex)
        {
        case 0: return iScreenBuffer;
        case 1: return iEasyDialer; // Can be null
        default: return NULL;
        }
    }

// -----------------------------------------------------------------------------
// OfferKeyEventL
// -----------------------------------------------------------------------------
//
TKeyResponse CEdtaAppView::OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    TKeyResponse handled = EKeyWasNotConsumed;
    
    if ( iEasyDialer )
        {
        handled = iEasyDialer->OfferKeyEventL(aKeyEvent, aType);
        }
    
    if ( handled == EKeyWasNotConsumed )
        {
        handled = iScreenBuffer->OfferKeyEventL(aKeyEvent, aType);
        }
    else 
        {
        handled = EKeyWasConsumed;
        }

    return handled;
    }

// -----------------------------------------------------------------------------
// GetScreenBuffer
// Returns the pointer to the screen buffer of the view.
// -----------------------------------------------------------------------------
//
CEdtaScreenTextBuffer* CEdtaAppView::GetScreenBuffer()
    {
    return iScreenBuffer;
    }


// -----------------------------------------------------------------------------
// GetEasyDialer
// Returns the pointer to the easy dialer.
// -----------------------------------------------------------------------------
//
CDialingExtensionInterface* CEdtaAppView::GetEasyDialer()
    {
    return iEasyDialer;
    }



// -----------------------------------------------------------------------------
// InitContactDataStoreUrisL
// Initialises user contact data store URIs into field iContactDataStores.
// -----------------------------------------------------------------------------
//
void CEdtaAppView::InitContactDataStoreUrisL()
    {
    HBufC* default_cdb = VPbkContactStoreUris::DefaultCntDbUri().AllocLC();
    User::LeaveIfError(iContactDataStores.Append( default_cdb ));
    CleanupStack::Pop( default_cdb );

    iContactStoreUriArray = CVPbkContactStoreUriArray::NewL();
    iContactStoreUriArray->AppendL(TVPbkContactStoreUriPtr( VPbkContactStoreUris::DefaultCntDbUri() ));
    }


// -----------------------------------------------------------------------------
// InitPredictiveContactSearchL
// Initialises predictive contact search.
// -----------------------------------------------------------------------------
//
void CEdtaAppView::InitPredictiveContactSearchL()
    {
    DebugPrintF(_L("CEdtaAppView::InitPredictiveContactSearchL enter"));

    iPredictiveContactSearchHandler = CPSRequestHandler::NewL();
    iPredictiveContactSearchHandler->AddObserverL(this);

    DebugPrintF(_L("CEdtaAppView::InitPredictiveContactSearchL => CPSRequestHandler::NewL done"));

    InitContactDataStoreUrisL();

    DebugPrintF(_L("CEdtaAppView::InitPredictiveContactSearchL => InitContactDataStoreUrisL done"));

    iContactManager = CVPbkContactManager::NewL(*iContactStoreUriArray);

    // Put the searched contact fields into array.
    RArray<TInt> contact_fields;
    CleanupClosePushL(contact_fields);

    // All supported fields are searched. The supported fiels are specified in cenrep private\10202BE9\2000B5C6.
    contact_fields.Append(R_VPBK_FIELD_TYPE_FIRSTNAME);
    contact_fields.Append(R_VPBK_FIELD_TYPE_LASTNAME);
    contact_fields.Append(R_VPBK_FIELD_TYPE_COMPANYNAME);
    contact_fields.Append(R_VPBK_FIELD_TYPE_EMAILHOME);
    contact_fields.Append(R_VPBK_FIELD_TYPE_EMAILWORK);
    contact_fields.Append(R_VPBK_FIELD_TYPE_EMAILGEN);

    // Create and fill ps settings object.
    CPsSettings* ps_settings = CPsSettings::NewL();
    CleanupStack::PushL(ps_settings);

    ps_settings->SetSearchUrisL(iContactDataStores);
    ps_settings->SetMaxResults(KEdtaMaximumMatchingCount);
    ps_settings->SetSortType(EAlphabetical);
    ps_settings->SetDisplayFieldsL(contact_fields);

    // Set the ps settings.
    iPredictiveContactSearchHandler->SetSearchSettingsL(*ps_settings);

    DebugPrintF(_L("CEdtaAppView::InitPredictiveContactSearchL => CPSRequestHandler::SetSearchSettingsL done"));

    CleanupStack::PopAndDestroy(ps_settings);
    CleanupStack::PopAndDestroy(&contact_fields);

    iPredictiveSearchQuery = CPsQuery::NewL();

    DebugPrintF(_L("CEdtaAppView::InitPredictiveContactSearchL => CPsQuery::NewL done"));

    DebugPrintF(_L("CEdtaAppView::InitPredictiveContactSearchL exit"));
    }


// -----------------------------------------------------------------------------
// void ShowMatchingContacts(const TDesC& aSearchString)
// Starts an asynchronous search for contacts matching to aSearchString.
// The actual displaying of contact is done in callback HandlePsResultsUpdate
// - Hardcode to use ITU keyboard parameters
// -----------------------------------------------------------------------------
//
void CEdtaAppView::ShowMatchingContactsL(const TDesC& aSearchString)
    {
    DebugPrintF(_L("CEdtaAppView::ShowMatchingContactsL: enter '%S'"), &aSearchString );

    iPredictiveSearchQuery->Reset();

    for(TInt i = 0; i < aSearchString.Length() ;i++)
        {
         // Add a query item
        CPsQueryItem* item = CPsQueryItem::NewL();
        CleanupStack::PushL(item);

        item->SetCharacter( aSearchString[i] );
        item->SetMode( EItut );
        iPredictiveSearchQuery->AppendL(*item);

        // Previous CPsQuery::AppendL takes the ownership of item.
        // Do not delete item.
        CleanupStack::Pop(item);
        }

    // Issue the search. SearchL is asynchronous function => returns immediately.
    iPredictiveContactSearchHandler->SearchL(*iPredictiveSearchQuery);

    DebugPrintF(_L("CEdtaAppView::ShowMatchingContactsL: exit"));
    }


// -----------------------------------------------------------------------------
// ShowNameOrderL
// Check the name ordering from phonebook engine by using phonebook engine API.
// -----------------------------------------------------------------------------
// 
void CEdtaAppView::ShowNameOrderL()
    {
    DebugPrintF(_L("CEdtaAppView::ShowNameOrderL: enter"));

    CPbkContactEngine* pbkEngine = CPbkContactEngine::NewL();
    CPbkContactEngine::TPbkNameOrder nameOrder = pbkEngine->NameDisplayOrderL();

    iScreenBuffer->WriteLineL(_L("Phonebook name order is"));
    switch ( nameOrder )
        {
        case CPbkContactEngine::EPbkNameOrderLastNameFirstName:
            DebugPrintF(_L("CEdtaAppView::ShowNameOrderL: The name order is: %d == LastName -> FirstName"), nameOrder );
            iScreenBuffer->WriteLineL(_L("* Last name, first name"));
            break;
        case CPbkContactEngine::EPbkNameOrderFirstNameLastName:
            DebugPrintF(_L("CEdtaAppView::ShowNameOrderL: The name order is: %d == FirstName -> LastName"), nameOrder );
            iScreenBuffer->WriteLineL(_L("* First name, last name"));
            break;
        case CPbkContactEngine::EPbkNameOrderNotDefined:
            DebugPrintF(_L("CEdtaAppView::ShowNameOrderL: The name order is: %d == Not defined"), nameOrder );
            iScreenBuffer->WriteLineL(_L("* Not defined"));
            break;
        default:
            DebugPrintF(_L("CEdtaAppView::ShowNameOrderL: The name order is: %d == Unknown"), nameOrder );
            iScreenBuffer->WriteLineL(_L("* Unknown value: %d"), nameOrder );
        }

    delete pbkEngine;
    
    DebugPrintF(_L("CEdtaAppView::ShowNameOrderL: exit"));
    }

// -----------------------------------------------------------------------------
// IsPCSServerLoaded
// Checks if the PCSServer is loaded properly at constructor
// -----------------------------------------------------------------------------
//
TBool CEdtaAppView::IsPCSServerLoaded()
    {
    if( !iPredictiveSearchQuery )
        {
        DebugPrintF(_L("CEdtaAppView::CheckPCSServerL: PCS server not loaded properly!"));

        TBuf<KEdtaMaxLineWidth> tempStr;
        tempStr.Copy(_L("PCS server not loaded!"));;
        
        TRAP_IGNORE(
                {
                CAknErrorNote* myNote = new (ELeave) CAknErrorNote();
                myNote->SetTimeout( CAknNoteDialog::ELongTimeout );
                TRAP_IGNORE( myNote->ExecuteLD( tempStr ) );
                }
        );
        return EFalse;
        }
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CheckPCSServerL
// Checks the configuration of the PCSServer and prints the sortorder of fields.
// -----------------------------------------------------------------------------
//
void CEdtaAppView::CheckPCSServerL()
    {
    RArray<TInt> field_order;
    CleanupClosePushL(field_order);

    HBufC* default_cdb = VPbkContactStoreUris::DefaultCntDbUri().AllocLC();

    _LIT(KEdtaField, "*");
    TBuf<KEdtaMaxLineWidth> line(KEdtaField);

    iPredictiveContactSearchHandler->GetDataOrderL(*default_cdb, field_order);

    DebugPrintF(_L("CEdtaAppView::ShowContactFieldOrderL: Number of fields: %d"), field_order.Count() );

    TInt requiredFieldsCount = 3;
    
    for ( TInt i = 0; i < field_order.Count(); i++)
        {
        line.Append((TChar) ' ');
        line.AppendNum(field_order[i]);
        
        if(( field_order[i] ==  R_VPBK_FIELD_TYPE_FIRSTNAME) ||
           ( field_order[i] ==  R_VPBK_FIELD_TYPE_LASTNAME) ||
           ( field_order[i] ==  R_VPBK_FIELD_TYPE_COMPANYNAME ))
            {
            requiredFieldsCount--;
            }
        }

    CleanupStack::PopAndDestroy( default_cdb );
    CleanupStack::PopAndDestroy( &field_order );

    iScreenBuffer->WriteLineL(_L("PCS server fields:"));
    iScreenBuffer->WriteLineL(line);

    DebugPrintF(_L("CEdtaAppView::ShowContactFieldOrderL: '%S'"), &line );

    if( requiredFieldsCount == 0 )
        {
        DebugPrintF(_L("CEdtaAppView::ShowContactFieldOrderL: All required fields found from PCSServer, configuration is OK"));
        iScreenBuffer->WriteLineL(_L("* All required fields found"));
        }
    else
        {
        DebugPrintF(_L("CEdtaAppView::ShowContactFieldOrderL: Not all required fields found from PCSServer, configuration is wrong"));
        iScreenBuffer->WriteLineL(_L("* PCS server misconfigured!"));
        iScreenBuffer->WriteLineL(_L("* Easydialing may not"));
        iScreenBuffer->WriteLineL(_L("* match your queries!"));
        }
    }


// -----------------------------------------------------------------------------
// void LaunchNthContactL(TInt aIx)
// Launches communication launcher for the first contact in the previous
// search. If not contacts found or no search yet done, function does nothing.
// -----------------------------------------------------------------------------
//
void CEdtaAppView::LaunchNthContactL(TInt aIx)
    {
    if (aIx < 0 || iMatchingContactLinks.Count() <= aIx)
        {
        DebugPrintF(_L("CEdtaAppView::LaunchNthContactL: No contacts retrieved yet") );
        return;
        }

    DebugPrintF(_L("CEdtaAppView::LaunchNthContactL: enter, index: %d, setting parameters.."), aIx );

    MCCAParameter* launch_parameters = TCCAFactory::NewParameterL();
    CleanupClosePushL( *launch_parameters );

    launch_parameters->SetConnectionFlag(MCCAParameter::ENormal);
    launch_parameters->SetContactDataFlag(MCCAParameter::EContactLink);

    // Pack the contact link and expand it to 16-bit descriptor.
    HBufC8* contact8 = iMatchingContactLinks[aIx]->PackLC();
    HBufC16* contact16 = HBufC16::NewLC( contact8->Length() );
    contact16->Des().Copy( contact8->Des() );

    launch_parameters->SetContactDataL( contact16->Des() );

    CleanupStack::PopAndDestroy( contact16 );
    CleanupStack::PopAndDestroy( contact8 );

    DebugPrintF(_L("CEdtaAppView::LaunchNthContactL: launching CCA.."));

    iContactLauncher->LaunchAppL( *launch_parameters, this );

    // Ownership of parameter transferred to CCA launcher => pop but do not destroy.
    CleanupStack::Pop(launch_parameters);  // Close parameter

    DebugPrintF(_L("CEdtaAppView::LaunchNthContactL: exit"));
    }



// -----------------------------------------------------------------------------
// void DoMiscellaneousTestsL()
// Does various tests on easy dialing that are difficult to do through normal
// testing. Mainly for coverage testing purposes.
// -----------------------------------------------------------------------------
//
void CEdtaAppView::DoMiscellaneousTestsL()
    {
    // Coverage tests for observer handling.
    iEasyDialer->AddObserverL( iDummyListener );
    iEasyDialer->AddObserverL( NULL );
    iEasyDialer->RemoveObserver( iDummyListener );
    }

// -----------------------------------------------------------------------------
// void HandlePsResultsUpdate(RPointerArray<CPsClientData>& aResult, RPointerArray<CPsPattern>& aSeqs)
// Searched matching contacts using PCS and shows them on the screen.
// -----------------------------------------------------------------------------
//
void CEdtaAppView::HandlePsResultsUpdate(RPointerArray<CPsClientData>& aResults, RPointerArray<CPsPattern>& aSeqs)
    {
    TRAP_IGNORE( HandlePredictiveSearchResultL(aResults, aSeqs) );
    }


// -----------------------------------------------------------------------------
// void HandlePsError(TInt aErrorCode)
// Called when search error happens.
// -----------------------------------------------------------------------------
//
void CEdtaAppView::HandlePsError(TInt aErrorCode)
    {
    DebugPrintF(_L("CEdtaAppView::HandlePsError: %d"), aErrorCode );

    _LIT(KEdtaCachingError, "*** Caching error %d ***");
    TRAP_IGNORE( iScreenBuffer->WriteLineL(KEdtaCachingError, aErrorCode) );

    }


// -----------------------------------------------------------------------------
// void CachingStatus(TCachingStatus& aStatus, TInt& aError)
// Called to update caching status.
// -----------------------------------------------------------------------------
//
void CEdtaAppView::CachingStatus(TCachingStatus& aStatus, TInt& aError )
    {
    DebugPrintF(_L("CEdtaAppView::CachingStatus: %d"), aError);

    switch (aStatus)
        {
        case ECachingNotStarted:
            {
            _LIT(KEdtaCachingStatusStarted, "*** Caching started ***");
            TRAP_IGNORE( iScreenBuffer->WriteLineL(KEdtaCachingStatusStarted) );
            }
            break;
        case ECachingInProgress:
            {
            _LIT(KEdtaCachingStatusInProgress, "*** Caching in progress ***");
            TRAP_IGNORE( iScreenBuffer->WriteLineL(KEdtaCachingStatusInProgress) );
            }
            break;
        case ECachingComplete:
            {
            _LIT(KEdtaCachingStatusComplete, "*** Caching complete ***");
            TRAP_IGNORE( iScreenBuffer->WriteLineL(KEdtaCachingStatusComplete) );
            }
            break;
        case ECachingCompleteWithErrors:
            {
            _LIT(KEdtaCachingStatusCompleteWithErrors, "*** Caching complete with error %d ***");
            TRAP_IGNORE( iScreenBuffer->WriteLineL(KEdtaCachingStatusCompleteWithErrors, aError) );
            }
            break;
        default:
            {
            _LIT(KEdtaCachingStatusUnknown, "*** Unknown caching status %d ***");
            TRAP_IGNORE( iScreenBuffer->WriteLineL(KEdtaCachingStatusUnknown, (TInt) aStatus) );
            }
            break;
        }
    }



// -----------------------------------------------------------------------------
// void HandlePredictiveSearchResultL(RPointerArray<CPsClientData>& aResult, RPointerArray<CPsPattern>& aSeqs)
// Handles predictive search results. A leaving version to be trapped in HandlePsResultsUpdate.
// -----------------------------------------------------------------------------
//
void CEdtaAppView::HandlePredictiveSearchResultL(RPointerArray<CPsClientData>& aResults, RPointerArray<CPsPattern>& /* aSeqs */)
    {
    TInt results = aResults.Count();
    DebugPrintF(_L("CEdtaAppView::HandlePredictiveSearchResultL enter, matching contacts found: %d"), results );

    iContactNames.ResetAndDestroy();
    iMatchingContactLinks.ResetAndDestroy();

    // map results to old contact match data
    for (TInt i = 0; i < aResults.Count(); i ++)
        {
        TBuf<KEdtaMaxLineWidth> temp;
        _LIT(KEdtaContactFormatString, "%S %S");

        // Store each contact name into iContactNames.
        temp.Format(KEdtaContactFormatString, aResults[i]->Data(0), aResults[i]->Data(1));

        DebugPrintF(_L("CEdtaAppView::HandlePredictiveSearchResultL: Contact #%d: '%S'"), i+1, &temp );

        HBufC* contact_name = HBufC::NewL(temp.Length());
        *contact_name = temp;
        iContactNames.Append( contact_name );


        // Each contact link stored to iMatchingContactLinks, to be possible used later by
        // CCA launcher functions.
        MVPbkContactLink* link = iPredictiveContactSearchHandler->ConvertToVpbkLinkLC(*(aResults[i]), *iContactManager);
        iMatchingContactLinks.Append( link );
        CleanupStack::Pop( link );

        // Write the contact name to screen buffer
        iScreenBuffer->WriteLineL( *contact_name );

        TUint8 match_mask = aResults[i]->FieldMatch();

        // Show other search fields only if search matches them.
        if (match_mask & (1 << KEdtaContactCompanyIndex))
            {
            _LIT(KEdtaContactCompany, "* Company: %S");
            iScreenBuffer->WriteLineL(KEdtaContactCompany, aResults[i]->Data(KEdtaContactCompanyIndex));
            }
        if (match_mask & (1 << KEdtaContactEmailHomeIndex))
            {
            _LIT(KEdtaContactEmailHome, "* Home e-mail: %S");
            iScreenBuffer->WriteLineL(KEdtaContactEmailHome, aResults[i]->Data(KEdtaContactEmailHomeIndex));
            }
        if (match_mask & (1 << KEdtaContactEmailWorkIndex))
            {
            _LIT(KEdtaContactEmailWork, "* Work e-mail: %S");
            iScreenBuffer->WriteLineL(KEdtaContactEmailWork, aResults[i]->Data(KEdtaContactEmailWorkIndex));
            }
        if (match_mask & (1 << KEdtaContactEmailGenIndex))
            {
            _LIT(KEdtaContactEmailGen, "* E-mail: %S");
            iScreenBuffer->WriteLineL(KEdtaContactEmailGen, aResults[i]->Data(KEdtaContactEmailGenIndex));
            }
        }
    _LIT(KEdtaNumberOfMatchingContacts, "*** %d contacts found! ***");
    iScreenBuffer->WriteLineL(KEdtaNumberOfMatchingContacts, aResults.Count());

    DebugPrintF(_L("CEdtaAppView::HandlePredictiveSearchResultL exit"));
    }


// -----------------------------------------------------------------------------
// HandleDialingExtensionEvent
// 
// -----------------------------------------------------------------------------
//
void CEdtaAppView::HandleDialingExtensionEvent( MDialingExtensionObserver::TEvent aEvent )
    {
    DebugPrintF(_L("CEdtaAppView::HandleDialingExtensionEvent"));

    if ( aEvent == MDialingExtensionObserver::EFocusChanged )
        {
        if ( iEasyDialer->IsFocused() )
            {
            iScreenBuffer->SetFocus( EFalse, EDrawNow );
            }
        else 
            {
            iScreenBuffer->SetFocus( ETrue, EDrawNow );
            }
        }
    }


// -----------------------------------------------------------------------------
// SelectAndLaunchContactL
// Shows a list box, the launches CCA launcher for user selected contact.
// -----------------------------------------------------------------------------
//
void CEdtaAppView::SelectAndLaunchContactL()
    {
    DebugPrintF(_L("CEdtaAppView::SelectAndLaunchContactL enter"));

    // Create list of matched contacts and show them on the screen

    CAknSingleHeadingPopupMenuStyleListBox* listBox = new(ELeave) CAknSingleHeadingPopupMenuStyleListBox;
    CleanupStack::PushL( listBox );

    CAknPopupList* popupList = CAknPopupList::NewL( listBox,
                                                    R_AVKON_SOFTKEYS_SELECT_CANCEL,
                                                    AknPopupLayouts::EMenuGraphicHeadingWindow );
    CleanupStack::PushL( popupList );

    listBox->ConstructL( popupList, EAknListBoxSelectionList );
    listBox->CreateScrollBarFrameL( ETrue );
    listBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff,
                                                        CEikScrollBarFrame::EAuto );

    _LIT( KMatchedContacts, "Matched contacts:" );
    popupList->SetTitleL( KMatchedContacts );

    // Add items into list

    CDesCArrayFlat* itemArray = new(ELeave)CDesCArrayFlat(KEdtaListBoxGranularity);
    CleanupStack::PushL(itemArray);

    _LIT( KListItemSeparator1, ". \t" );

    for (TInt listIndex = 0; listIndex < iContactNames.Count(); listIndex++ )
        {
        TBuf<KEdtaMaxLineWidth> itemString;
        itemString.Zero();
        itemString.AppendNum( listIndex+1 );
        itemString.Append( KListItemSeparator1 );
        itemString.Append( *iContactNames[listIndex] );

        itemArray->AppendL(itemString);
        }

    CTextListBoxModel* model = listBox->Model();
    model->SetItemTextArray( itemArray );
    model->SetOwnershipType( ELbmOwnsItemArray );
    CleanupStack::Pop( itemArray );     // Pop itemArray - the listBox model owns this now

    if( popupList->ExecuteLD() )        // Show the list finally
        {
        if( listBox->CurrentItemIndex() >= 0 )
            {
            // Launch communication launcher for selected contact
            LaunchNthContactL( listBox->CurrentItemIndex() );
            }
        }

    CleanupStack::Pop( popupList );
    CleanupStack::PopAndDestroy( listBox );

    DebugPrintF(_L("CEdtaAppView::SelectAndLaunchContactL exit"));
    }


// -----------------------------------------------------------------------------
// CCASimpleNotifyL
// Implements MCCAObserver notification interface.
// Nothing better to do but show the parameters.
// -----------------------------------------------------------------------------
//
void CEdtaAppView::CCASimpleNotifyL( TNotifyType aType, TInt aReason )
    {
    DebugPrintF(_L("CEdtaAppView::CCASimpleNotifyL: aType: %d, aReason: %d"), (TInt)aType, aReason );

    _LIT(KEdtaCCAExitReason, "Launcher exit reason %d, %d");
    iScreenBuffer->WriteLineL(KEdtaCCAExitReason, aType, aReason);
    }


// -----------------------------------------------------------------------------
// LoadPluginL
// Load the easydialing ECOM plugin
// -----------------------------------------------------------------------------
//
void CEdtaAppView::LoadPluginL()
    {
    TRAPD( error, 
        {
        iEasyDialer = CDialingExtensionInterface::NewL();
        iEasyDialer->InitializeL( *this );        
        } );

    if( error )
        {
        DebugPrintF(_L("CEdtaAppView::LoadPluginL FAIL, error: %d"), error );
        iScreenBuffer->WriteLineL(_L("* EasyDialing Plugin load fail: %d"), error );
        iEasyDialer = NULL;
        }
    else
        {
        iScreenBuffer->WriteLineL(_L("EasyDialing Plugin loaded"));
        DebugPrintF(_L("CEdtaAppView::LoadPluginL OK" ));
        }
    }

// End of File

