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
* Description:  Easy dialing plugin.
*
*/

// INCLUDE FILES

#include "easydialingplugin.h"
#include "easydialinglistbox.h"
#include "easydialinglogger.h"
#include "easydialingpanics.h"
#include "easydialingcenreplistener.h"
#include "easydialingcontactdatamanager.h"
#include "easydialingutils.h"
#include "easydialingcommands.hrh"
#include <easydialingpluginresources.rsg>

// AVKON and drawing header files
#include <gdi.h>
#include <aknlists.h>
#include <AknUtils.h>
#include <e32cmn.h>
#include <bidivisual.h>

#include <aknlayoutscalable_avkon.cdl.h>

// Predictive search header files
#include <CPsSettings.h>
#include <CPsQuery.h>
#include <CPsQueryItem.h>
#include <CPsClientData.h>
#include <CPsPattern.h>
#include <CPsRequestHandler.h>

// Virtual phonebook header files
#include <VPbkContactStoreUris.h>
#include <CVPbkContactStoreUriArray.h>
#include <TVPbkContactStoreUriPtr.h>
#include <CVPbkContactLinkArray.h>
#include <VPbkEng.rsg> // contains virtual phonebook data fields
#include <phoneui.rsg> 

// CCA Launcher header files.
#include <ccafactory.h>
#include <mccaparameter.h>
#include <mccaconnection.h>
#include <mccaconnectionext.h> 

// Service provider settings api
#include <spsettingsvoiputils.h>

// AIW header files
#include <AiwContactAssignDataTypes.h>
#include <AiwContactSelectionDataTypes.h>

#include <StringLoader.h>

// CCA contactor service.
#include "edcontactorservice.h"

// Open system trace
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "easydialingpluginTraces.h"
#endif

#include <phoneappcommands.hrh>
#include <bautils.h>    // for BaflUtils

// Transition effects
#include <gfxtranseffect/gfxtranseffect.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS
const TInt KEDMaximumMatchingContactsCount = 100;
const TInt KEasyDialingListBoxModelGranularity = 8;
const TInt KEasyDialingMaximumMatchingParts = 10;
const TInt KErrEasyDialingNoFirstNamePCSIndexing = -2000;
const TInt KErrEasyDialingNoLastNamePCSIndexing = -2001;

const TInt KEDFlushContactDataManagerLimit = 20;

const TText KHighlightSeparatorChar = 0x1F;
_LIT(KHighlightSeparatorCharAsLiteral,"\x1F");
const TText KListFieldSeparatorChar = '\t';

const TText KArabicAndSouthEastAsianRangeStart = 0x0600;
const TText KArabicAndSouthEastAsianRangeEnd = 0x19FF;
const TText KArabicPresentationFormsARangenStart = 0xFB50;
const TText KArabicPresentationFormsARangeEnd = 0xFDFF;
const TText KArabicPresentationFormsBRangenStart = 0xFE70;
const TText KArabicPresentationFormsBRangeEnd = 0xFEFF;

const TInt KMaxRunInfoArrayCount = 20;

// Transition context_uid for dialer_list component effects
#define KGfxContactListBoxUid TUid::Uid( 0x2000B47B )
const TInt KGfxContactListBoxOpenEffect  = 3;
const TInt KGfxContactListBoxCloseEffect = 4;

// MACROS

// LOCAL CONSTANTS AND MACROS
_LIT(KResourceFile, "\\resource\\easydialingpluginresources.rsc");

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

TInt CompareTPsMatchLocation( const TPsMatchLocation& a1, const TPsMatchLocation& a2 );

void AppendStringWithMatchDataL(
        TDes& aBuffer,
        const TDesC& aText,
        CPSRequestHandler* aPSHandler,
        const CPsQuery* aPSQuery );

TBool HighlightingSupportedForText( const TDesC& aText );

inline TBool HighlightingSupportedForScript( TText aChar );

TBool IsStrictlyBidirectional( const TDesC& aText );

static HBufC* AllocWithoutHighlightSeparatorsLC( TDesC& aDesc );

static TBool IsItuTCharacter( TChar aChar );

static TInt Find( const MVPbkContactLink* aLink, const RPointerArray<MVPbkContactLink>& aArray );

template <class T>
inline void CleanupResetAndDestroyPushL( T& aRef );



// -----------------------------------------------------------------------------
// CEasyDialingPlugin
// The default c++ constructor
// -----------------------------------------------------------------------------
//
CEasyDialingPlugin::CEasyDialingPlugin() 
    : iActionToBeLaunched( ENoActionDefined )
    {
    }

// -----------------------------------------------------------------------------
// NewL
// Create instance of concrete ECOM interface implementation.
// -----------------------------------------------------------------------------
//
CEasyDialingPlugin* CEasyDialingPlugin::NewL()
    {
    OstTrace0( TRACE_NORMAL, CEASYDIALINGPLUGIN_NEWL_LOAD_PLUGIN, "Loading plugin.." );
    LOGSTRING("EasyDialingPlugin: Loading plugin..");

    CEasyDialingPlugin* self = new (ELeave) CEasyDialingPlugin;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    OstTrace0( TRACE_NORMAL, CEASYDIALINGPLUGIN_NEWL_LOAD_PLUGIN_OK, "Loading plugin completed succesfully" );
    LOGSTRING("EasyDialingPlugin: Loading plugin completed succesfully");
    return self;
    }

// -----------------------------------------------------------------------------
// ConstructL
// Main construction handled here. Creates connection to the predictive search
// engine and initialises all member data.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::ConstructL()
    {
    // Create a contact store array.
    HBufC* defaultCdb = VPbkContactStoreUris::DefaultCntDbUri().AllocLC();
    iContactDataStores.AppendL( defaultCdb );
    CleanupStack::Pop( defaultCdb );

    // Create a contact manager instance.
    iContactStoreUriArray = CVPbkContactStoreUriArray::NewL();
    iContactStoreUriArray->AppendL( TVPbkContactStoreUriPtr( VPbkContactStoreUris::DefaultCntDbUri() ) );
    iContactManager = CVPbkContactManager::NewL( *iContactStoreUriArray );
    
    // Set contact store observer to listen to contact store events.
    iContactManager->ContactStoresL().OpenAllL( *this );
    
    iContactDataManager = new (ELeave) CEasyDialingContactDataManager(iContactManager);
    iContactDataManager->ConstructL();
    iContactDataManager->SetObserver(this);

    // Find a handle to ca launcher extension MCCAConnectionExt.
    // Easydialing has to use the extension API, because it needs function CloseAppL
    // only found in extension.
    // MCCAConnection extension has to be obtained through MCCAParameter extension,
    // since MCCAConnection is not designed to be extensible API.
    MCCAParameter* parameter = TCCAFactory::NewParameterL();
    TAny* any = parameter->CcaParameterExtension( KMCCAConnectionExtUid );
    
    // Parameter can be deallocated since "any" containing pointer to contact launcher
    // is not tied to parameter in any way.
    parameter->Close();
    
    // Cast the pointer into contact launcher object. 
    User::LeaveIfNull( any );
    iContactLauncher = static_cast<MCCAConnectionExt*>( any );
    
    iCenrepListener = CEasyDialingCenrepListener::NewL(this);
   
    iContactorService = CEDContactorService::NewL( this );

    // EasyDialing resource file is attempted to be read from the same directory where the
    // executed binary is located
    TFileName dllFileName;
    Dll::FileName( dllFileName );
    
    TParse parse;
    User::LeaveIfError( parse.Set(KResourceFile, &dllFileName, NULL) );
    TFileName resourceFileName( parse.FullName() );

    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), resourceFileName );
    iResourceFileOffset = iCoeEnv->AddResourceFileL( resourceFileName );
  
    SetComponentsToInheritVisibility( ETrue );
    }

// -----------------------------------------------------------------------------
// ~CEasyDialingPlugin
// The desctructor
// -----------------------------------------------------------------------------
//
CEasyDialingPlugin::~CEasyDialingPlugin()
    {
    iObservers.Reset();
    
    if ( iContactManager )
        {
        TRAP_IGNORE( iContactManager->ContactStoresL().CloseAll( *this ) );
        }

    delete iCenrepListener;
    delete iContactDataManager;
    delete iPredictiveSearchQuery;
    delete iContactManager;
    delete iContactStoreUriArray;
    iContactDataStores.ResetAndDestroy();

    if (iPredictiveContactSearchHandler)
        {
        iPredictiveContactSearchHandler->RemoveObserver(this);
        }
    delete iPredictiveContactSearchHandler;

    delete iListBoxModel;
    
    if ( iContactListBox )
        {
        GfxTransEffect::Deregister( iContactListBox );
        }
    
    delete iInfoLabelLine1;
    delete iInfoLabelLine2;
    delete iInfoLabelTextLine1;
    delete iInfoLabelTextLine2;

    delete iContactListBox;

    if (iContactLauncher)
        {
        iContactLauncher->Close();
        }
    
    delete iContactorService;
    
    iCoeEnv->DeleteResourceFile( iResourceFileOffset );
    
    delete iInputBlocker;
    
    if ( iAsyncCallBack )
        {
        iAsyncCallBack->Cancel();
        }
    delete iAsyncCallBack;
    
    delete iContactToBeLaunched;
    delete iContactToBeLaunchedName;
    
    OstTrace0( TRACE_NORMAL, CEASYDIALINGPLUGIN_UNLOAD_PLUGIN, "Plugin unloaded" );
    LOGSTRING("EasyDialingPlugin: Plugin unloaded");
    }


// -----------------------------------------------------------------------------
// InitializeL
// Initialises easy dialing.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::InitializeL( CCoeControl& aParent )
    {
    SetContainerWindowL( aParent );
    SetMopParent( &aParent );
    
    TCallBack asyncCallBack( AsyncCallBackToLaunchAction, this );
    iAsyncCallBack = new ( ELeave ) CAsyncCallBack( asyncCallBack, 
                                                    CActive::EPriorityStandard );

    iContactListBox = new (ELeave) CEasyDialingListBox();
    iListBoxModel = new(ELeave) CDesCArrayFlat( KEasyDialingListBoxModelGranularity );

    iContactListBox->ConstructL( CEikListBox::EPaintedSelection
            | CEikListBox::ENoFirstLetterMatching
            | CEikListBox::EDisableItemSpecificMenu, 
            iContactDataManager );
    iContactListBox->SetListBoxObserver(this);

    iContactListBox->SetMopParent(this);

    CTextListBoxModel* model = iContactListBox->Model();
    model->SetItemTextArray( iListBoxModel );
    model->SetOwnershipType( ELbmDoesNotOwnItemArray );

    iContactListBox->MakeVisible( EFalse );
    
    SetFocus( EFalse );
    iContactListBox->ActivateL();
    
    _LIT( KEDNewLine, "\n" );
    HBufC* infoLabelText = StringLoader::LoadLC( R_QTN_EASYDIAL_ENTER_NUMBER, iCoeEnv );
    TInt newLine = infoLabelText->Find( KEDNewLine );
    if ( newLine == KErrNotFound )
        {
        iInfoLabelTextLine1 = infoLabelText;
        CleanupStack::Pop( infoLabelText );
        iInfoLabelTextLine2 = HBufC::NewL(0);
        }
    else
        {
        iInfoLabelTextLine1 = infoLabelText->Left( newLine ).AllocL();
        iInfoLabelTextLine2 = infoLabelText->Mid( newLine + 1 ).AllocL();
        CleanupStack::PopAndDestroy( infoLabelText );
        }
            
    iInfoLabelLine1 = new( ELeave ) CEikLabel;
    iInfoLabelLine1->SetContainerWindowL( *this );
    iInfoLabelLine1->SetParent( this );
    iInfoLabelLine1->SetMopParent( this ); 
    iInfoLabelLine1->SetLabelAlignment( ELayoutAlignCenter );
    iInfoLabelLine1->SetTextL( *iInfoLabelTextLine1 );
    iInfoLabelLine1->ActivateL();

    iInfoLabelLine2 = new( ELeave ) CEikLabel;
    iInfoLabelLine2->SetContainerWindowL( *this );
    iInfoLabelLine2->SetParent( this );
    iInfoLabelLine2->SetMopParent( this ); 
    iInfoLabelLine2->SetLabelAlignment( ELayoutAlignCenter );
    iInfoLabelLine2->SetTextL( *iInfoLabelTextLine2 );
    iInfoLabelLine2->ActivateL();

    SetInfoLabelColourL();
    
    GfxTransEffect::Register( iContactListBox, 
                              KGfxContactListBoxUid, EFalse );
    
    iCoeEnv->AddForegroundObserverL( *this );
    
    // Do delayed initialization of PCS. PCS constructions takes a long time.
    // On the other hand, easy dialing initialization is done in phone application
    // constructor, so it contributes the whole system boot time. These are good 
    // reasons not to construct PCS in easy dialing constructor, but do it later as
    // an asynchronous operation.
    AsyncActionLaunchL( EInitializePcs );
    }


// -----------------------------------------------------------------------------
// Reset
// Resets easydialing plugin.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::Reset()
    {
    // PCS searches completing after the Reset() call must be discarded
    iDiscardCompletingSearches = ETrue;
    
    iNewSearchNeeded = EFalse;
    iSearchString.Zero();
    iListBoxModel->Reset();
    iNumberOfNames = 0;
    iContactListBox->MakeVisible( EFalse );
    iContactDataManager->Reset();
    if ( IsFocused() )
        {
        SetFocus( EFalse );
        }
    }


// -----------------------------------------------------------------------------
// InitPredictiveContactSearchL
// Initialises predictive contact search.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::InitPredictiveContactSearchL()
    {
    iPredictiveContactSearchHandler = CPSRequestHandler::NewL();
    iPredictiveContactSearchHandler->AddObserverL(this);

    // Put the searched contact fields into array.
    RArray<TInt> contact_fields;
    CleanupClosePushL(contact_fields);

    // Check which relevant contact fields are indexed in PCS search.
    iFirstNamePCSIndex = FindContactFieldPCSIndexL( R_VPBK_FIELD_TYPE_FIRSTNAME );
    iLastNamePCSIndex = FindContactFieldPCSIndexL( R_VPBK_FIELD_TYPE_LASTNAME );
    iCompanyNamePCSIndex = FindContactFieldPCSIndexL( R_VPBK_FIELD_TYPE_COMPANYNAME );

    // If first name and last name are not indexed in PCS, easy dialing plugin
    // can not function reasonably. The function leaves, which in turn causes
    // the plugin not to be initialized.
    // Company name not being indexed is not as severe case, and does not cause a leave.
    // Fields used in PCS indexing are configured in cenrep 2000B5C6.
    if ( iFirstNamePCSIndex == KErrNotFound )
        {
        OstTrace0( TRACE_ERROR, CEASYDIALINGPLUGIN_INITPREDICTIVECONTACTSEARCHL_ERROR_NO_FIRST_NAME, "ERROR, PCS does not support first name indexing!" );
        LOGSTRING("EasyDialingPlugin: PCS does not support first name indexing => Leave");
        User::Leave( KErrEasyDialingNoFirstNamePCSIndexing );
        }
    if ( iLastNamePCSIndex == KErrNotFound )
        {
        OstTrace0( TRACE_ERROR, CEASYDIALINGPLUGIN_INITPREDICTIVECONTACTSEARCHL_ERROR_NO_LAST_NAME, "ERROR, PCS does not support last name indexing!" );
        LOGSTRING("EasyDialingPlugin: PCS does not support last name indexing => Leave");
        User::Leave( KErrEasyDialingNoLastNamePCSIndexing );
        }

    // First name, last name and company name (if supported) are used in PCS search.
    contact_fields.Append(R_VPBK_FIELD_TYPE_FIRSTNAME);
    contact_fields.Append(R_VPBK_FIELD_TYPE_LASTNAME);
    if ( iCompanyNamePCSIndex != KErrNotFound )
        {
        contact_fields.Append(R_VPBK_FIELD_TYPE_COMPANYNAME);
        }

    SetSortOrderL( iContactDataManager->NameOrder() );

    // Create and fill ps settings object.
    CPsSettings* ps_settings = CPsSettings::NewL();
    CleanupStack::PushL(ps_settings);

    ps_settings->SetSearchUrisL(iContactDataStores);
    ps_settings->SetMaxResults(KEDMaximumMatchingContactsCount);
    ps_settings->SetSortType(EAlphabetical);
    ps_settings->SetDisplayFieldsL(contact_fields);

    // Set the PCS settings.
    iPredictiveContactSearchHandler->SetSearchSettingsL(*ps_settings);

    CleanupStack::PopAndDestroy(ps_settings);
    CleanupStack::PopAndDestroy(&contact_fields);

    iPredictiveSearchQuery = CPsQuery::NewL();
    }

// -----------------------------------------------------------------------------
// SetSortOrderL
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::SetSortOrderL( CEasyDialingContactDataManager::TNameOrder aNameOrder )
    {
    RArray<TInt> fields;
    CleanupClosePushL( fields );
    if ( aNameOrder == CEasyDialingContactDataManager::EFirstnameLastname )
        {
        fields.Append(R_VPBK_FIELD_TYPE_FIRSTNAME);
        fields.Append(R_VPBK_FIELD_TYPE_LASTNAME);
        }
    else
        {
        fields.Append(R_VPBK_FIELD_TYPE_LASTNAME);
        fields.Append(R_VPBK_FIELD_TYPE_FIRSTNAME);
        }
    if ( iCompanyNamePCSIndex != KErrNotFound )
        {
        fields.Append(R_VPBK_FIELD_TYPE_COMPANYNAME);
        }
    iPredictiveContactSearchHandler->ChangeSortOrderL( *iContactDataStores[0], fields );
    CleanupStack::PopAndDestroy( &fields );
    }

// -----------------------------------------------------------------------------
// OfferKeyEventL
// Check the received keypad event and performs user
// actions related to it.
// -----------------------------------------------------------------------------
//
TKeyResponse CEasyDialingPlugin::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
    {
    TKeyResponse keyResponse = EKeyWasNotConsumed;

    if ( aType != EEventKey )
        {
        return keyResponse;
        }

    TInt keyCode = aKeyEvent.iCode;
    
    // Swap right and left key codes in mirrored layout.
    if ( AknLayoutUtils::LayoutMirrored() ) 
        {
        if ( keyCode == EKeyRightArrow ) keyCode = EKeyLeftArrow;
        else if ( keyCode == EKeyLeftArrow ) keyCode = EKeyRightArrow;
        }
    

    if ( IsFocused() )
        {
        if ( keyCode == EKeyUpArrow )
            {
            // if the focus is on the top-most item
            if ( iContactListBox->CurrentItemIndex() == 0)
                {
                // then focus jumps off the component.
                SetFocus( EFalse );
                DrawDeferred();
                keyResponse = EKeyWasConsumed;
                }
            else
                {
                keyResponse = iContactListBox->OfferKeyEventL( aKeyEvent, aType );
                }
            }
        else if ( keyCode == EKeyDownArrow )
            {
            // if the focus is on the bottom-most item
            if ( iContactListBox->CurrentItemIndex() == (iNumberOfNames - 1) )
                {
                // then focus jumps off the component.
                SetFocus( EFalse );
                DrawDeferred();
                keyResponse = EKeyWasConsumed;
                }
            else
                {
                keyResponse = iContactListBox->OfferKeyEventL( aKeyEvent, aType );
                }
            }

        else if ( keyCode == EKeyRightArrow )
            {
            iRememberFocus = ETrue;
            AsyncActionLaunchL( ELaunchCurrentContact );
            keyResponse = EKeyWasConsumed;
            }

        else if ( keyCode == EKeyLeftArrow )
            {
            // then focus jumps off the component.
            SetFocus( EFalse );
            DrawDeferred();
            keyResponse = EKeyWasConsumed;
            }

        else
            {
            // then focus jumps off the component.
            SetFocus( EFalse );
            
            // FEP hasn't had chance to handle this key event as focus was not in the editor field.
            // Give that chance now by consuming original event and simulating a new one.
            // The simulation must be asynchronous because FEP focusing state is updated
            // with high priority active object.
            // As an exception, dialer simulated keys are given directly to editor. They are
            // not handled by FEP anyway, and asynchronous handling would be problematic for
            // the * key multitapping logic of Phone.
            TBool simulatedByDialer = (aKeyEvent.iModifiers&EModifierNumLock) &&
                                      (aKeyEvent.iModifiers&EModifierKeypad);
            if ( !simulatedByDialer )
                {
                keyResponse = EKeyWasConsumed;
                AsyncSimulateKeyEvent( aKeyEvent );
                }
            }
        }

    else if ( iNumberOfNames > 0 ) // not in focus but there are contacts to show
        {
        if ( keyCode == EKeyUpArrow || keyCode == EKeyDownArrow )
            {
            SetFocus( ETrue );
            keyResponse = iContactListBox->SetFocusedWithKeyEventL( aKeyEvent, aType );
            }
        }
    return keyResponse;
    }


// -----------------------------------------------------------------------------
// CountComponentControls
//
// -----------------------------------------------------------------------------
//
TInt CEasyDialingPlugin::CountComponentControls() const
    {
    return iNumberOfNames > 0 ? 3 : 2;
    }


// -----------------------------------------------------------------------------
// ComponentControl
//
// -----------------------------------------------------------------------------
//
CCoeControl* CEasyDialingPlugin::ComponentControl( TInt aIndex ) const
    {
    switch ( aIndex )
        {
        case 0:
            return iInfoLabelLine1;
        case 1:
            return iInfoLabelLine2;
        case 2:
            return iContactListBox;
        default:
            return NULL;
        }
    }


// -----------------------------------------------------------------------------
// MakeVisible
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::MakeVisible( TBool aVisible )
    {
    if ( aVisible != IsVisible() )
        {
        CCoeControl::MakeVisible( aVisible );
        
        if ( !aVisible && IsFocused() )
            {
            SetFocus( EFalse );
            }
        }
    }

// -----------------------------------------------------------------------------
// HandleResourceChange
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::HandleResourceChange( TInt /*aType*/ )
    {
    TRAP_IGNORE( SetInfoLabelColourL() );
    }

// -----------------------------------------------------------------------------
// SizeChanged
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::SizeChanged()
    {
    TRect rect = Rect();
    // convert rect to absolute coordinates
    rect.SetRect( PositionRelativeToScreen(), rect.Size() );
    
    iContactListBox->SetMaxRect( rect );
    iContactListBox->SetRectToNumberOfItems( iNumberOfNames );
    
    TInt itemToMakeVisible = iContactListBox->CurrentItemIndex();  
    if ( itemToMakeVisible == KErrNotFound && iNumberOfNames )
        {
        // if no current item is set, make sure that the list is then 
        // scrolled to the bottom.
        itemToMakeVisible = iNumberOfNames - 1;
        } 
    iContactListBox->ScrollToMakeItemVisible( itemToMakeVisible );

    // get info label locations and fonts from layout
    TRect baseRect = Rect();
    TAknTextComponentLayout labelLayoutLine1 = 
        AknLayoutScalable_Avkon::main_pane_empty_t1( 0 );
    TAknTextComponentLayout labelLayoutLine2 = 
        AknLayoutScalable_Avkon::main_pane_empty_t2( 0 );
    AknLayoutUtils::LayoutLabel( iInfoLabelLine1, baseRect, labelLayoutLine1 );
    AknLayoutUtils::LayoutLabel( iInfoLabelLine2, baseRect, labelLayoutLine2 );
    
    // the layouts used place the text too low, so center the labels vertically
    TInt labelHeight = iInfoLabelLine2->Rect().iBr.iY - iInfoLabelLine1->Rect().iTl.iY;
    TInt centeredTop = ( baseRect.Height() - labelHeight ) / 2;
    TInt offset = centeredTop - iInfoLabelLine1->Rect().iTl.iY;
    iInfoLabelLine1->SetPosition( TPoint( iInfoLabelLine1->Rect().iTl.iX,
                                          iInfoLabelLine1->Rect().iTl.iY + offset ) );
    iInfoLabelLine2->SetPosition( TPoint( iInfoLabelLine2->Rect().iTl.iX,
                                          iInfoLabelLine2->Rect().iTl.iY + offset ) );
    
    TRAP_IGNORE( SetInfoLabelColourL() );
    }


// -----------------------------------------------------------------------------
// FocusChanged
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::FocusChanged( TDrawNow aDrawNow )
    {
    iContactListBox->SetFocus( IsFocused() );
    if( !IsFocused() )
        {
        // To be on the safe side, cancel async callback and reset input block.
        CancelActionLaunchAndInputBlock();
        iContactListBox->View()->ItemDrawer()->ClearFlags( CListItemDrawer::ESingleClickDisabledHighlight );
        }
    CCoeControl::FocusChanged( aDrawNow );
    InformObservers( MDialingExtensionObserver::EFocusChanged );
    }


// -----------------------------------------------------------------------------
// SetInputL
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::SetInputL( const TDesC& aSearchString )
    {
    if ( !IsEnabled() )
        {
        return;
        }
    OstTraceExt1( TRACE_NORMAL, CEASYDIALINGPLUGIN_SET_INPUT, "SetInput: '%S' ", aSearchString );
    LOGSTRING1("EasyDialingPlugin: SetInput: '%S'", &aSearchString );
    
    // Earlier here was a test if the search string is the same as the one used in previous search.
    // If it was, no search was done but function returned immediately.
    // This has now been removed. It is possible that contacts have changed, and a new search
    // may be needed even if the search string is the same. 
    
    // Check if search string is just empty space or 0-length. Decision was made that no
    // matches are shown if search string is just space (would show all contacts)
    // even if somebody might have unnamed contacts. Should be quite rare.
    // It's assumed later on in the code that string is not just empty space.
    TLex searchString( aSearchString );
    searchString.SkipSpace();
    if ( searchString.Eos() ) // the end of the string reached after skipping empty space
        {
        // It's assumed here that if we get here due to switching to call
        // handling etc, then dialer and thus ed is already set invisible
        // and effect will not be shown as listbox is already invisible.
        // However if user empties number entry, then it's feasible to show
        // effect.
        HideContactListBoxWithEffect();
        iInfoLabelLine1->SetTextL( *iInfoLabelTextLine1 );
        iInfoLabelLine2->SetTextL( *iInfoLabelTextLine2 );
        iInfoLabelLine1->DrawDeferred();
        iInfoLabelLine2->DrawDeferred();
        Reset();
        }
    else // proper search string
        {
        iInfoLabelLine1->SetTextL( KNullDesC );
        iInfoLabelLine2->SetTextL( KNullDesC );
        iInfoLabelLine1->DrawDeferred();
        iInfoLabelLine2->DrawDeferred();
        iSearchString.Copy( aSearchString.Left( iSearchString.MaxLength() ) );
        LaunchSearchL();
        }
    }


// -----------------------------------------------------------------------------
// MatchingContactCount
//
// -----------------------------------------------------------------------------
//
TInt CEasyDialingPlugin::MatchingContactCount()
    {
    return iNumberOfNames;
    }


// -----------------------------------------------------------------------------
// CEasyDialingPlugin::StoreReady
// From MVPbkContactStoreListObserver.
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::StoreReady(MVPbkContactStore& /* aContactStore */)
    {
    
    }


// -----------------------------------------------------------------------------
// CEasyDialingPlugin::StoreUnavailable
// From MVPbkContactStoreListObserver.
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::StoreUnavailable(MVPbkContactStore& /* aContactStore */, TInt /* aReason */)
    {
    
    }


// -----------------------------------------------------------------------------
// CEasyDialingPlugin::HandleStoreEventL
// From MVPbkContactStoreListObserver.
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::HandleStoreEventL( MVPbkContactStore& /* aContactStore */, 
                        TVPbkContactStoreEvent aStoreEvent)
    {
    // Store's observers are informed one by one using active object so one shouldn't
    // perform search syncronously as one must try to ensure that PCS has had a
    // chance to update its store. However there seems to be no way to be
    // 100% sure that PCS is up-to-date when search is launched: telephony app
    // has such a high priority and there are no APIs to query PCS' status.
    switch ( aStoreEvent.iEventType )
        {
        case TVPbkContactStoreEvent::EContactAdded:
        case TVPbkContactStoreEvent::EContactDeleted:
        case TVPbkContactStoreEvent::EContactChanged:
            {
            DoHandleContactsChangedL();
            }
            break;
       
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CEasyDialingPlugin::OpenComplete
// From MVPbkContactStoreListObserver.
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::OpenComplete()
    {
    }

// -----------------------------------------------------------------------------
// EasyDialingSettingsChanged
// From MEasyDialingCenrepListenerObserver
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::EasyDialingSettingsChanged( TInt aValue )
    {
    if ( aValue == 0 )
        {
        Reset();
        InformObservers( MDialingExtensionObserver::EEasyDialingDisabled );
        }
    else if ( aValue == 1 )
        {
        InformObservers( MDialingExtensionObserver::EEasyDialingEnabled );
        }
    MakeVisible( aValue );
    }

// -----------------------------------------------------------------------------
// EasyDialingContactThumbnailsSettingsChanged
// From MEasyDialingCenrepListenerObserver
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::EasyDialingContactThumbnailsSettingsChanged( TInt aThumbnailSettingValue )
    {
    iContactDataManager->SetContactThumbnailSetting( aThumbnailSettingValue );
    iContactDataManager->Reload();
    DrawDeferred();
    }

// -----------------------------------------------------------------------------
// AllContactDataLoaded
// From MContactDataManagerObserver
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::AllContactDataLoaded()
    {
    iContactListBox->DrawDeferred();
    }

// -----------------------------------------------------------------------------
// NameOrderChanged
// From MContactDataManagerObserver
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::NameOrderChanged()
    {
    if ( iPredictiveContactSearchHandler )
        {
        TRAP_IGNORE( SetSortOrderL( iContactDataManager->NameOrder() ) );
        }
    }

// -----------------------------------------------------------------------------
// FavouritesChanged
// From MContactDataManagerObserver
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::FavouritesChanged()
    {
    TRAP_IGNORE( DoHandleContactsChangedL() );
    }

// -----------------------------------------------------------------------------
// InformContactorEvent
// From MEDContactorObserver
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::InformContactorEvent( MEDContactorObserver::TEvent aEvent )
    {

    // This callback function simply propagates the events to its own listener.
    switch ( aEvent )
        {
        case MEDContactorObserver::ECommunicationStarted:
            InformObservers( MDialingExtensionObserver::ECommunicationStarted );
            break;
        case MEDContactorObserver::ECommunicationCancelled:
            InformObservers( MDialingExtensionObserver::ECommunicationCancelled );
            break;
        default:
            break;
        }
    
    // Reset focus unless it is flagged to be remembered.
    if ( !iRememberFocus )
        {
        SetFocus( EFalse );
        DrawDeferred();
        }
    
    iRememberFocus = EFalse;
    }

// -----------------------------------------------------------------------------
// Draw
// The actual UI drawing function. Draws the easy dialing popup bubble on the
// idle screen.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::Draw( const TRect& /* aRect */ ) const
    {
    return;
    }


// -----------------------------------------------------------------------------
// LaunchSearchL
// Initiates predictive contact search.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::LaunchSearchL()
    {
    if ( iSearchString.Length() == 0 || !iPredictiveContactSearchHandler )
        {
        return;
        }
    
    iDiscardCompletingSearches = EFalse;
    iNewSearchNeeded = EFalse;
    
    iPredictiveSearchQuery->Reset();

    for ( TInt i = 0; i < iSearchString.Length(); i++ )
        {
         // Add a query item
        CPsQueryItem* item = CPsQueryItem::NewL();
        CleanupStack::PushL(item);

        item->SetCharacter( iSearchString[i] );
        
        // The PCS mode used with character is based on character itself,
        // not to the keyboard it is made with. While this is not strictly
        // identical to checking the used keyboard, this behaves identically
        // in most of the normal cases, and makes the logic simpler.
        if ( IsItuTCharacter( iSearchString[i] ) )
            {
            item->SetMode( EPredictiveItuT );
            }
        else 
            {
            item->SetMode( EPredictiveDefaultKeyboard );
            }
        
        iPredictiveSearchQuery->AppendL(*item);

        // Previous CPsQuery::AppendL takes the ownership of item.
        // Do not delete item.
        CleanupStack::Pop(item);
        }

    // Issue the search. SearchL is asynchronous function => returns immediately.
    iPredictiveContactSearchHandler->SearchL(*iPredictiveSearchQuery);
    }


// -----------------------------------------------------------------------------
// HandlePsResultsUpdate from MPsResultsObserver
// Called after predictive search has been completed.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::HandlePsResultsUpdate( RPointerArray<CPsClientData>& aResults, RPointerArray<CPsPattern>& aSeqs )
    {
    if ( !IsEnabled() )
        {
        // Easydialing is off. We should arrive here only if user turned it off while a search was happening.
        Reset();
        return;
        }
    TRAPD( leaveError, HandlePsResultsUpdateL( aResults, aSeqs ) );

    if ( leaveError )
        {
        OstTrace1( TRACE_ERROR, CEASYDIALINGPLUGIN_HANDLEPSRESULTSUPDATE, "HandlePsResultsUpdate failed: %d", leaveError );
        LOGSTRING1("EasyDialingPlugin: HandlePsResultsUpdate failed: %d", leaveError );
        }
    }


// -----------------------------------------------------------------------------
// HandlePsError from MPsResultsObserver
// Called when search error happens.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::HandlePsError( TInt aErrorCode )
    {
    OstTrace1( TRACE_ERROR, CEASYDIALINGPLUGIN_HANDLEPSERROR, "PCS Error: %d", aErrorCode );
    LOGSTRING1("EasyDialingPlugin: PCS Error: %d", aErrorCode );
    }

// -----------------------------------------------------------------------------
// CachingStatus from MPsResultsObserver
// Called to update caching status.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::CachingStatus( TCachingStatus& aStatus, TInt& aError )
    {
    OstTraceExt2( TRACE_NORMAL, CEASYDIALINGPLUGIN_CACHINGSTATUS, "PCS CachingStatus: %d, error: %d", ( TUint )( aStatus ), aError );
    LOGSTRING2("EasyDialingPlugin: PCS CachingStatus: %d, error: %d", aStatus, aError );
    }


// -----------------------------------------------------------------------------
// FindContactFieldPCSIndex
// Searches the index that stores the given contact field.
// To see possible contact field numbering, see file "vpbkeng.rsg".
// -----------------------------------------------------------------------------
//
TInt CEasyDialingPlugin::FindContactFieldPCSIndexL( TInt aIndex )
    {
    RArray<TInt> fieldOrder;
    CleanupClosePushL( fieldOrder );

    // Current implementation searches only from default database.
    // Later this may be expanded to search SIM contacts as well.
    const TDesC& defaultCdb = VPbkContactStoreUris::DefaultCntDbUri();

    iPredictiveContactSearchHandler->GetDataOrderL( defaultCdb, fieldOrder );
    TInt pcsIndex = fieldOrder.Find( aIndex );

    CleanupStack::PopAndDestroy( &fieldOrder );
    return pcsIndex;
    }


// -----------------------------------------------------------------------------
// HandlePsResultsUpdateL
// This function does the actual callback fork, and must be trapped inside
// HandlePsResultsUpdateL.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::HandlePsResultsUpdateL( RPointerArray<CPsClientData>& aResults, RPointerArray<CPsPattern>& /*aSeqs*/ )
    {
    // If iDiscardCompletingSearches is ETrue, don't show search results but just return.
    // If easydialing has been reset with reset function, no searches completed after that
    // can be shown.
    // This is done like this because there seems not to be a feasible way of cancelling all 
    // PCS search request in PCS API. CancelRequest does not work here.
    if ( iDiscardCompletingSearches )
        {
        return;
        }
    
    iListBoxModel->Reset();
    iContactListBox->HandleItemRemovalL();

    TInt numberOfPCSMatches( aResults.Count() );
    
    // Resetting the contactdatamanager causes the thumbnails to flicker a bit, because
    // we need to load the thumbnails again. But if there are lots of search results, or
    // lots of loaded thumbnails in the manager, matching the search results to the
    // thumbnail manager contents may take too much time. If there are lots of results,
    // it is likely that the contents of the listbox change anyway, so the flickering
    // doesn't matter too much. This also limits the memory usage of the manager.
    //   But if there are fewer search results, it is worth trying to match them
    // with the contents of thumbnail manager.
    
    TBool matchThumbnails(ETrue);
    if ( numberOfPCSMatches > KEDFlushContactDataManagerLimit )
        {
        iContactDataManager->Reset();
        matchThumbnails = EFalse;
        }

    OstTrace1( TRACE_NORMAL, CEASYDIALINGPLUGIN_HANDLEPSRESULTSUPDATEL_MATCHES, "Matching results from PCS: %d", numberOfPCSMatches );
    LOGSTRING1("EasyDialingPlugin: Matching results from PCS: %d", numberOfPCSMatches );

    // retrieve the name order before adding
    CEasyDialingContactDataManager::TNameOrder nameOrder = iContactDataManager->NameOrder();

    RPointerArray<MVPbkContactLink> favsFoundByPcs;
    CleanupResetAndDestroyPushL( favsFoundByPcs );
    
    // Update the model
    // ----------------
    for ( TInt i = 0; i < numberOfPCSMatches; i++ )
        {
        TInt indexFromEnd = numberOfPCSMatches - i - 1;

        MVPbkContactLink* link = iPredictiveContactSearchHandler->ConvertToVpbkLinkLC(
                *(aResults[indexFromEnd]), *iContactManager );
        if ( !iContactDataManager->IsFavL( link ) )
            {
            // handle favourites separately, in another loop
            HBufC* contactString = CreateContactStringLC( aResults[ indexFromEnd ], nameOrder );
            CreateListBoxContactStringL( *contactString, link, matchThumbnails, EFalse );
            CleanupStack::PopAndDestroy( contactString );
            CleanupStack::PopAndDestroy( link );
            }
        else
            {
            // favourites are handled later
            favsFoundByPcs.AppendL( link );
            CleanupStack::Pop( link );
            }
        
        OstTraceExt2( TRACE_NORMAL, CEASYDIALINGPLUGIN_HANDLEPSRESULTSUPDATEL_SHOW_MATCH, "Contact #%d: '%S'", i+1, iContactStringCreationBuffer );
        LOGSTRING2( "EasyDialingPlugin: Contact #%d: '%S'", i+1, &iContactStringCreationBuffer );
        }

    // Search synchronously through all the favourite contacts to ensure
    // that all favourite matches are added to bottom even when number of mathces
    // exceeds the maximum number set to PCS
    TInt numberOfFavs( iContactDataManager->NumberOfFavsL() );
    TBuf<KBufferMaxLen> results;
    for ( TInt i = numberOfFavs - 1; i >= 0; i-- )
        {
        HBufC* favContactString = iContactDataManager->FavContactStringLC( i, nameOrder );
        MVPbkContactLink* link = iContactDataManager->FavLinkLC( i );
        
        // Check if this fav contact was returned in aResults.
        // It's at least theoretically possible that all matches in aResults
        // are not matched when using LookupMatchL. PCS has completely separate logics
        // for functions SearchL and LookupMatchL and especially with Chinese variant
        // they may return different results.
        TBool found = ( Find( link, favsFoundByPcs ) != KErrNotFound );
        
        // If this fav contact was not in aResults, then use LookupMatchL
        // to check if this contact is still a match and was excluded from aResults
        // because maximum number of results was exceeded.
        if ( !found )
            {
            results = KNullDesC;
            iPredictiveContactSearchHandler->LookupMatchL(
                    *iPredictiveSearchQuery, *favContactString, results );
            found = ( results.Length() > 0 );
            }

        if ( found )
            {
            // matches, add this fav to listbox.
            CreateListBoxContactStringL( *favContactString, link, matchThumbnails, ETrue );
            }

        CleanupStack::PopAndDestroy(); // link
        CleanupStack::PopAndDestroy( favContactString );
        }
    
    CleanupStack::PopAndDestroy( &favsFoundByPcs ); // ResetAndDestroy
    
    // Update the view
    // ---------------
    iNumberOfNames = iListBoxModel->Count();
    if ( iNumberOfNames )
        {
        TInt oldRectHeight = iContactListBox->Rect().Height();
        
        iContactListBox->SetRectToNumberOfItems( iNumberOfNames );
        
        // If the window is resized (->position changes too) while it's visible, 
        // it has to be redrawn immediately or otherwise listbox will flash
        // in a wrong place on the screen during execution of this method
        // HandlePsResultsUpdateL. In the worst case it'll flash on top of
        // dialer's numeric keypad when listbox is made smaller.
        if ( oldRectHeight != iContactListBox->Rect().Height() )
            {
            iContactListBox->DrawNow();
            }

        iContactListBox->HandleItemAdditionL();
        
        // Scroll the list to bottom
        iContactListBox->ScrollToMakeItemVisible( iNumberOfNames-1 );

        ShowContactListBoxWithEffect();
        iContactListBox->View()->ItemDrawer()->ClearFlags( CListItemDrawer::ESingleClickDisabledHighlight );
        static_cast<CEasyDialingListBoxView*>( iContactListBox->View() )->SetCurrentItemIndexToNone();
        }
    else
        {  
        HideContactListBoxWithEffect();
        }

    if ( IsFocused() )
        {
        SetFocus( EFalse );
        }

    DrawDeferred();

    InformObservers( MDialingExtensionObserver::ESearchComplete );
    }


// -----------------------------------------------------------------------------
// void CCASimpleNotifyL()
// Implements MCCAObserver notification interface.
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::CCASimpleNotifyL( TNotifyType aType, TInt aReason )
    {
    OstTraceExt2( TRACE_NORMAL, CEASYDIALINGPLUGIN_CCASIMPLENOTIFYL, "CCASimpleNotifyL: type: %d, reason: %d", ( TUint )( aType ), aReason );
    LOGSTRING2("EasyDialingPlugin: CCASimpleNotifyL: type: %d, reason: %d", (TInt)aType, aReason );

    iContactLauncherActive = EFalse;

    CAknAppUi* appUi = static_cast<CAknAppUi*>( iCoeEnv->AppUi() );
    appUi->HandleCommandL( EPhoneCmdBlockingDialogClosed );
    
    // If contacts have been edited during contact launcher being open, a new search
    // needs to be done.
    if ( iNewSearchNeeded )
        {
        // The cached information in contact data manager may be outdated. Call to reload makes sure that when the 
        // search is made, all data is loaded again.
        iContactDataManager->Reload();
        
        LaunchSearchL();
        }

    // Give up focus, if iRememberFocus flag is not set.
    if ( !iRememberFocus )
        {
        SetFocus( EFalse );
        DrawDeferred();
        }    
    iRememberFocus = EFalse;

    // Inform observers.
    InformObservers( MDialingExtensionObserver::ECCALauncherExit );
    }


// -----------------------------------------------------------------------------
// void LaunchCurrentContactL()
// Implements MCCAObserver notification interface.
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::LaunchCurrentContactL()
    {
    __ASSERT_DEBUG( iNumberOfNames > 0, EasyDialingPanic( EEasyDialingPanicNoResults ) );
    __ASSERT_DEBUG( iContactToBeLaunched, EasyDialingPanic( EEasyDialingPanicNoContactSelected ) );

    if (( iContactLauncherActive ) || ( iNumberOfNames == 0 ) || ( !iContactListBox ) || ( !iContactToBeLaunched ))
        {
        OstTrace0( TRACE_ERROR, CEASYDIALINGPLUGIN_LAUNCHCURRENTCONTACTL_ERROR, "LaunchCurrentContactL: Parameter error" );
        LOGSTRING("EasyDialingPlugin: LaunchCurrentContactL - Parameter error");
        return;
        }

    MCCAParameter* launchParameters = TCCAFactory::NewParameterL();
    CleanupClosePushL( *launchParameters );

    // ESoftExit flag causes that ca launcher will not close the 
    // client application in any circumstance. 
    // Application hosting easydialing is telephone application.
    // It must never be closed, so we must use this flag.
    launchParameters->SetConnectionFlag(MCCAParameter::ESoftExit);
    
    launchParameters->SetContactDataFlag(MCCAParameter::EContactLink);

    // Expand contact link into 16-bit descriptor because cca launcher api expects this. 
    HBufC16* contact16 = HBufC16::NewLC( iContactToBeLaunched->Length() );
    contact16->Des().Copy( *iContactToBeLaunched );

    launchParameters->SetContactDataL( *contact16 );

    CleanupStack::PopAndDestroy( contact16 );

    OstTraceExt1( TRACE_NORMAL, CEASYDIALINGPLUGIN_LAUNCHCURRENTCONTACTL_LAUNCH_CCA, "Launch CL for contact: '%S'", *iContactToBeLaunchedName );
    LOGSTRING1("EasyDialingPlugin: Launch CL for contact: '%S'", iContactToBeLaunchedName );

    delete iContactToBeLaunched;
    iContactToBeLaunched = NULL;
    delete iContactToBeLaunchedName;
    iContactToBeLaunchedName = NULL;    
    
    iContactLauncher->LaunchAppL( *launchParameters, this );

    // Ownership of parameter transferred to CCA launcher => pop but do not destroy.
    CleanupStack::Pop( launchParameters );

    if ( !IsVisible() )
        {
        // MCCAConnection::LaunchAppL uses CActiveSchedulerWait to hide asynchronous
        // opening of CCA launcher. It is possible that during opening of CCA launcher
        // phone has moved to in-call view (at least in case of an incoming call).
        // In that case we need to close CCA launcher again.
        iContactLauncher->CloseAppL();
        }
    else
        {
        iContactLauncherActive = ETrue;
        CAknAppUi* appUi = static_cast<CAknAppUi*>( iCoeEnv->AppUi() );
        appUi->HandleCommandL( EPhoneCmdBlockingDialogLaunched );
        }
    }


// -----------------------------------------------------------------------------
// CreateListBoxContactStringL
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::CreateListBoxContactStringL(
        const TDesC& aContactString,
        MVPbkContactLink *aLink,
        TBool aMatchThumbnails,
        TBool aFav )
    {
    // Construct the contact string for the listbox model.
    iContactStringCreationBuffer.Zero();
    
    // Append contact thumbnail id.
    HBufC* thumbnailId = iContactDataManager->GetThumbnailIdL( aLink, aMatchThumbnails, aFav );
    iContactStringCreationBuffer.Append( *thumbnailId );
    delete thumbnailId;
    iContactStringCreationBuffer.Append( KListFieldSeparatorChar );
    
    AppendStringWithMatchDataL(
            iContactStringCreationBuffer,
            aContactString,
            iPredictiveContactSearchHandler,
            iPredictiveSearchQuery );
    
    // Append the whole string to listbox model.
    iListBoxModel->AppendL( iContactStringCreationBuffer );
    }

// -----------------------------------------------------------------------------
// CreateContactStringLC
//
// -----------------------------------------------------------------------------
//
HBufC* CEasyDialingPlugin::CreateContactStringLC( CPsClientData* aResult,
        CEasyDialingContactDataManager::TNameOrder aNameOrder )
    {
    TPtrC firstName = *aResult->Data( iFirstNamePCSIndex );
    TPtrC lastName = *aResult->Data( iLastNamePCSIndex );
    TPtrC companyName( KNullDesC );
    
    if ( iCompanyNamePCSIndex != KErrNotFound )
        {
        companyName.Set( *aResult->Data( iCompanyNamePCSIndex ) );
        }
    return EasyDialingUtils::CreateContactStringLC( firstName, lastName, companyName, aNameOrder );
    }


// -----------------------------------------------------------------------------
// AddObserverL
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::AddObserverL( MDialingExtensionObserver* aObserver )
    {
    if ( !aObserver )
        {
        return;
        }
    
    iObservers.AppendL( aObserver );
    }


// -----------------------------------------------------------------------------
// RemoveObserver
//
// Removes the parameter observer from observer list if found. If same observer
// has registered multiple times (which is unnecessary), removed just the first
// registration.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::RemoveObserver( MDialingExtensionObserver* aObserver )
    {
    for ( TInt i = 0; i < iObservers.Count(); i++ )
        {
        if ( iObservers[i] == aObserver)
            {
            iObservers.Remove( i );
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// InformObservers
// Informs all registered observers of easy dialing events.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::InformObservers( MDialingExtensionObserver::TEvent aEvent )
    {
    for ( TInt i = 0; i < iObservers.Count(); i++ )
        {
        iObservers[i]->HandleDialingExtensionEvent( aEvent );
        }
    }

// -----------------------------------------------------------------------------
// CbaResourceId
// Returns EasyDialingPlugin specific CBA resource ID that
// Phone application can use when updating Soft Keys.
// -----------------------------------------------------------------------------
//
TInt CEasyDialingPlugin::CbaResourceId()
    {
    return R_EASYDIALING_CBA;
    }


// -----------------------------------------------------------------------------
// MenuResourceId
// Returns EasyDialingPlugin specific menu resource ID that
// Phone application can use when easydialing is in focus.
// -----------------------------------------------------------------------------
//
TInt CEasyDialingPlugin::MenuResourceId()
    {
    return R_EASYDIALING_MENUBAR;
    }


// -----------------------------------------------------------------------------
// InitializeMenuPaneL
// Initializes easy dialing menu pane. This function is meant to be called in 
// DynInitMenuPaneL of the application, if the application wants to use easy
// dialing menu.
// -----------------------------------------------------------------------------
//
TBool CEasyDialingPlugin::InitializeMenuPaneL( CEikMenuPane& aMenuPane, TInt aMenuResourceId )
    {   
    if ( aMenuResourceId == R_PHONEUIDIALER_OPTIONS_MENU )
        {
        TInt index( KErrNotFound );
        // To be on the safe side - place holder was added into phoneui.rss.
        if ( aMenuPane.MenuItemExists( EEasyDialingSettingsItemPlaceHolder, index ) )
            {
            // Add first menu item (= on/off submenu) after placeHolder item and 
            // then delete that (empty) placeHolder.
            aMenuPane.AddMenuItemsL( R_EASYDIALING_OPTIONS_ON_OFF_MENU_ITEM, 
                                     EEasyDialingSettingsItemPlaceHolder );
            aMenuPane.DeleteMenuItem( EEasyDialingSettingsItemPlaceHolder );
            
            LOGSTRING("EasyDialingPlugin::InitializeMenuPaneL: Added on/off menu into dialer's menu" );
            }

        // Return EFalse as only one menu item was added here.
        return EFalse; 
        }
    
    else if ( aMenuResourceId == R_EASYDIALING_MENU )
        {
        TBool voiceCall = EFalse;
        TBool videoCall = EFalse;
        TBool message = EFalse;
        
        if ( iContactListBox->CurrentItemIndex() >= 0 ) 
            {
            TInt index = iContactListBox->CurrentContactDataIndex();
            
            voiceCall = iContactDataManager->VoiceCallAvailable( index );
            videoCall = iContactDataManager->VideoCallAvailable( index );
            message = iContactDataManager->UniEditorAvailable( index );
            }
            
        // Call menu item is not show if neither voice call nor video call are possible. 
        aMenuPane.SetItemDimmed( EEasyDialingVoiceCall, !voiceCall && !videoCall ); 
        aMenuPane.SetItemDimmed( EEasyDialingSendMessage, !message ); 
        
        LOGSTRING("EasyDialingPlugin::InitializeMenuPaneL: InitializeMenuPaneL done" );
        
        return ETrue;  
        }
    
    else if ( aMenuResourceId == R_EASYDIALING_OPTIONS_CALL_MENU )
        {
        TBool voiceCall = EFalse;
        TBool videoCall = EFalse;
        
        if ( iContactListBox->CurrentItemIndex() >= 0 ) 
            {
            TInt index = iContactListBox->CurrentContactDataIndex();
            
            voiceCall = iContactDataManager->VoiceCallAvailable( index );
            videoCall = iContactDataManager->VideoCallAvailable( index );
            }
        
        aMenuPane.SetItemDimmed( EEasyDialingVoiceCall, !voiceCall ); 
        aMenuPane.SetItemDimmed( EEasyDialingVideoCall, !videoCall ); 

        LOGSTRING("EasyDialingPlugin::InitializeMenuPaneL: InitializeMenuPaneL for call submenu done" );
        
        return ETrue;
        }
    
    else if ( aMenuResourceId == R_EASYDIALING_OPTIONS_ON_OFF_CASCADE_MENU )
        {
        if ( IsEnabled() )
            {
            aMenuPane.SetItemButtonState( EEasyDialingOn, EEikMenuItemSymbolOn );
            }
        else
            {
            aMenuPane.SetItemButtonState( EEasyDialingOff, EEikMenuItemSymbolOn );
            }
        
        LOGSTRING("EasyDialingPlugin::InitializeMenuPaneL: InitializeMenuPaneL for on/off submenu done" );
        
        return ETrue;
        }
    
    else 
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// HandleCommandL
// Method for handling EasyDialingPlugin specific commands.
// -----------------------------------------------------------------------------
//
TBool CEasyDialingPlugin::HandleCommandL( TInt aCommand )
    {      
    // Input blocker can't block controls higher on the control stack than 
    // ECoeStackPriorityDialog so we will get HandleCommandL calls from 
    // phoneappui (CBA) when input blocker is active (=not NULL).
    
    if ( iInputBlocker && aCommand != EEasyDialingCallHandlingActivated &&
         aCommand != EEasyDialingVkbOpened && aCommand != EEasyDialingVkbClosed )
        {
        // Some action is already being launched since iInputBlocker exists.
        // Only call activation command requires always action from this plugin.
        // Vkb status flag should be updated always too (invokes no action).
        return ETrue;
        }
 
    
    TBool ret(EFalse);
    
    switch ( aCommand )
        {
        case EEasyDialingOpenContact:
            
            iRememberFocus = ETrue;
            AsyncActionLaunchL( ELaunchCurrentContact );
            ret = ETrue;
            break;

        // EEasyDialingEnterKeyAction is sent when Enter key is pressed.
        // EEasyDialingMakeCall is sent when send key or send toolbar button or send menu item is pressed.
        case EEasyDialingVoiceCall: // fall through
        case EEasyDialingEnterKeyAction:
            // Make a call.
            iRememberFocus = ETrue;
            AsyncActionLaunchL( ECallCurrentContact );
            ret = ETrue;
            break;
            
        // Video call is selectable only through menu.
        case EEasyDialingVideoCall:
            // Make a video call.
            iRememberFocus = ETrue;
            AsyncActionLaunchL( EVideoCallCurrentContact );
            ret = ETrue;
            break;
                
        // Uni-editor message is selectable only through menu.
        case EEasyDialingSendMessage:
            // Create a message.
            iRememberFocus = ETrue;
            AsyncActionLaunchL( ESendMessageCurrentContact );
            ret = ETrue;
            break;
                    
        // EEasyDialingCallHandlingActivated is sent when the in-call-ui of telephony gets activated
        case EEasyDialingCallHandlingActivated:
            
            iRememberFocus = EFalse;
            // Stop input block and async action launch in case they are active.
            CancelActionLaunchAndInputBlock();
            // Close down number selection popup in case it happens to be open.
            iContactorService->CancelService();
            // Close down communication launcher if it's active
            if ( iContactLauncherActive )
                {
                iNewSearchNeeded = EFalse;
                iContactLauncher->CloseAppL();
                }
            ret = ETrue;
            break;
            
        case EEasyDialingOn:
            
            iCenrepListener->SetEasyDialingSettingsValue( 1 );
            ret = ETrue;
            break;

        case EEasyDialingOff:
            
            iCenrepListener->SetEasyDialingSettingsValue( 0 );
            ret = ETrue;
            break;    
            
        case EEasyDialingClosePopup:
            
            // Not only Number Entry is removed but also closes down number selection popup in case it happens to be open.
            iContactorService->CancelService();
            ret = ETrue;
            break;
            
        case EEasyDialingVkbOpened:
            
            iVirtualKeyboardOpen = ETrue;
            ret = ETrue;
            break;
                  
        case EEasyDialingVkbClosed:
            
            iVirtualKeyboardOpen = EFalse;
            ret = ETrue;
            break;
            
        default:
            break;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// IsEnabled
// Check if Easy dialing is enabled in the settings
// -----------------------------------------------------------------------------
//
TBool CEasyDialingPlugin::IsEnabled() const
    {
    return ( iCenrepListener && iCenrepListener->Value() != 0 );
    }

// -----------------------------------------------------------------------------
// AsyncSimulateKeyEvent
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::AsyncSimulateKeyEvent( const TKeyEvent& aKeyEvent )
    {
    // Do the simulation only if input hasn't been blocked
    if ( !iInputBlocker )
        {
        iKeyEventToSimulate = aKeyEvent;
        iActionToBeLaunched = ESimulateKeyEvent;
        iAsyncCallBack->SetPriority( CActive::EPriorityStandard );
        iAsyncCallBack->CallBack(); // activates callback request
        }
    }

// -----------------------------------------------------------------------------
// AsyncActionLaunchL
// Use asynchronous callback to launch action. While action is being launched,
// input blocker is used to avoid OfferKeyEvent and HandlePointerEvent calls
// coming in. Both end key and application key work despite of input blocker.
// Input blocker can't block phoneappui's cba so extra check is needed in
// HandleCommandL method.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::AsyncActionLaunchL( TEasyDialingAction aAction )
    {
    iActionToBeLaunched = aAction;

    delete iContactToBeLaunched;
    iContactToBeLaunched = NULL;
    delete iContactToBeLaunchedName;
    iContactToBeLaunchedName = NULL;
       
    if ( aAction == ECallCurrentContact
            || aAction == EVideoCallCurrentContact
            || aAction == ESendMessageCurrentContact
            || aAction == ELaunchCurrentContact )
        {
        // Need to save current contact link and name. Listbox current item
        // index might not be correct when callback is handled.
        iContactToBeLaunched = iContactListBox->CurrentContactLinkLC();
        CleanupStack::Pop( iContactToBeLaunched );
        
        TPtrC contactString( iListBoxModel->MdcaPoint( iContactListBox->CurrentItemIndex() ) );
        TPtrC fullNameSeparators;
        TInt error = TextUtils::ColumnText( fullNameSeparators, 1, &contactString );
        __ASSERT_DEBUG( error == KErrNone, EasyDialingPanic( EEasyDialingPanicInvalidListBoxModelString ) );
        
        // Remove highlight separators. It is possible that some contactor API has problem with them.
        iContactToBeLaunchedName = AllocWithoutHighlightSeparatorsLC( fullNameSeparators );
        CleanupStack::Pop( iContactToBeLaunchedName );
        }
    
    CancelActionLaunchAndInputBlock();
    
    if ( aAction == ELaunchSearch )
        {
        // Must be set as low as possible to enable PCS to update its store.
        iAsyncCallBack->SetPriority( CActive::EPriorityIdle );
        }
    else
        {
        iAsyncCallBack->SetPriority( CActive::EPriorityStandard );
        }
    
    iAsyncCallBack->CallBack(); // activates callback request
    
    // By defining cancel handler, we don't block all input but system is still
    // able to deactivate the view etc.
    iInputBlocker = CAknInputBlock::NewCancelHandlerLC( this );
    CleanupStack::Pop( iInputBlocker );
    
    // This means that iInputBlocker is deleted by CAknInputBlock when
    // it's cancelled ( we get a callback where iInputBlocker is set to NULL).
    iInputBlocker->SetCancelDelete( iInputBlocker );
    }

// -----------------------------------------------------------------------------
// AsyncCallBackToLaunchAction
// Callback function for CAsyncCallBack class.
// -----------------------------------------------------------------------------
//
TInt CEasyDialingPlugin::AsyncCallBackToLaunchAction( TAny* aPtr )
    {
    CEasyDialingPlugin* plugin = static_cast<CEasyDialingPlugin*>( aPtr );
    
    TRAP_IGNORE( plugin->DoLaunchActionL() );
    
    // Stop input block if active (=not NULL).
    if ( plugin->iInputBlocker )
        {
        plugin->iInputBlocker->Cancel();
        }
    
    plugin->iActionToBeLaunched = ENoActionDefined;
    
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// DoLaunchActionL
//
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::DoLaunchActionL( )
    {
    if ( iActionToBeLaunched == EInitializePcs )
        {
        PERF_MEASURE_START
        InitPredictiveContactSearchL();
        PERF_MEASURE_STOP
 
        return;
        }
    else if ( !IsVisible() )
        {
        // If ED is not visible, don't launch the action. This can happen if
        // we get incoming call in the middle of action launching.
        return;
        }
    // If ELaunchCurrentContact, then we launch cca launcher.
    else if ( iActionToBeLaunched == ELaunchCurrentContact )
        {
        LaunchCurrentContactL();
        return;
        }
    else if ( iActionToBeLaunched == ELaunchSearch )
        {
        LaunchSearchL();
        return;
        }
    else if ( iActionToBeLaunched == ESimulateKeyEvent )
        {
        iEikonEnv->SimulateKeyEventL( iKeyEventToSimulate, EEventKey );
        return;
        }
    
    // If not for launching current contact or performing search, 
    // the action is launching some communication method.
    
    if ( iContactorService->IsBusy() )
        {
        LOGSTRING("EasyDialingPlugin: LaunchActionL - contactor service is busy!");
        return;
        }
 
    VPbkFieldTypeSelectorFactory::TVPbkContactActionTypeSelector selector( 
            VPbkFieldTypeSelectorFactory::EEmptySelector );
    
    switch ( iActionToBeLaunched ) 
        {
        case ECallCurrentContact:
            {
            // Easy Dialing provides pretty much the same functionality for big Call icon 
            // and Send key than phonebook's contact list view by using aiw interface 
            // in edcontactor.
            // At this point one must however check whether to use voip or voice call selector - 
            // otherwise correct logic is provided by aiw interface.
            // Note that Easy Dialing ignores default numbers and addresses set for
            // action types and shows always the number and address list if there are
            // more than one possible number/address for some action.
             
            CSPSettingsVoIPUtils* sPSettings = CSPSettingsVoIPUtils::NewLC();
            
            if ( sPSettings->IsPreferredTelephonyVoIP() )
                {
                selector = VPbkFieldTypeSelectorFactory::EVOIPCallSelector;
                }
            else
                {
                selector = VPbkFieldTypeSelectorFactory::EVoiceCallSelector;
                }
            
            CleanupStack::PopAndDestroy( sPSettings );
            }
            break;
            
        case EVideoCallCurrentContact:
            
            selector = VPbkFieldTypeSelectorFactory::EVideoCallSelector;
            break;
            
        case ESendMessageCurrentContact:
            
            selector = VPbkFieldTypeSelectorFactory::EUniEditorSelector;
            break;
            
        default:
            delete iContactToBeLaunched;
            iContactToBeLaunched = NULL;
            delete iContactToBeLaunchedName;
            iContactToBeLaunchedName = NULL;
            __ASSERT_DEBUG( EFalse, EasyDialingPanic( EEasyDialingActionNotSupported ) );
            return;
        }

    CEDContactorService::TCSParameter param( selector, *iContactToBeLaunched, 
                                             CEDContactorService::TCSParameter::EEnableDefaults, 
                                             *iContactToBeLaunchedName );

    iContactorService->ExecuteServiceL( param );

    delete iContactToBeLaunched;
    iContactToBeLaunched = NULL;
    delete iContactToBeLaunchedName;
    iContactToBeLaunchedName = NULL;
    }


// -----------------------------------------------------------------------------
// CancelActionLaunchAndInputBlock
// Should be called when focus is lost or when call ui is activated.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::CancelActionLaunchAndInputBlock()
    {
    iAsyncCallBack->Cancel();
    if ( iInputBlocker )
        {
        iInputBlocker->Cancel();
        }
    }

// -----------------------------------------------------------------------------
// HandleListBoxEventL
// EasyDialerListBox calls this function when listbox events are reported
// to observers.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::HandleListBoxEventL( CEikListBox* /*aListBox*/, TListBoxEvent aEventType )
    {
    LOGSTRING1( "EasyDialingPlugin: HandleListBoxEventL( %d ) - Enter", aEventType );

    switch( aEventType )
        {
        case EEventItemSingleClicked:
            // 9.2. Selecting contact initiates call
            AsyncActionLaunchL( ECallCurrentContact );
            break;

        case KEasyDialingContactLongTapped:
            AsyncActionLaunchL( ELaunchCurrentContact );
            break;
            
        // Pause contact data manager when panning and flicking listbox.
        // This ensures smooth and responsive listbox touch handling.
        case EEventFlickStarted:
        case EEventPanningStarted:
        case KEasyDialingScrollingStarted:
            iContactDataManager->Pause( ETrue );
            break;
            
        case EEventFlickStopped:
        case EEventPanningStopped:
        case KEasyDialingScrollingStopped:
            iContactListBox->View()->ItemDrawer()->ClearFlags( CListItemDrawer::ESingleClickDisabledHighlight );
            static_cast<CEasyDialingListBoxView*>( iContactListBox->View() )->SetCurrentItemIndexToNone();
            iContactDataManager->Pause( EFalse );
            
            // Touching the listbox always removes the visual focus from any list item.
            // Move the focus away from the listbox after panning has ended to
            // align our internal state with the visual lack of focus.
            if ( IsFocused() )
                {
                SetFocus( EFalse );
                DrawDeferred();
                }
            break;
            
        // We are not interested about the other listbox events.
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// From MAknInputBlockCancelHandler.
// Called when input block is cancelled.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::AknInputBlockCancel()
    {
    LOGSTRING("EasyDialingPlugin: AknInputBlockCancel");
    
    // iInputBlocker will be deleted right after this callback by CAknInputBlock
    // cause we are using CAknInputBlock::SetCancelDelete method.
    iInputBlocker = NULL;
    }

// -----------------------------------------------------------------------------
// HandleGainingForeground
// From MCoeForegroundObserver.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::HandleGainingForeground()
    {
    }

// -----------------------------------------------------------------------------
// HandleLosingForeground
// From MCoeForegroundObserver.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::HandleLosingForeground()
    {
    // Make sure contact data manager is not left in paused state when
    // ED loses foreground while scrolling is active.
    iContactDataManager->Pause( EFalse );
    
    // Simulate an EButton1Up event for scrollbar so that it will be in correct
    // state if e.g. some popup appears while scrollbar is dragged.
    // No need to check if scrollbar has received button1Down event or
    // is indeed dragged currently: no harm done if button1Up is simulated 
    // in vain.
    CEikScrollBarFrame* scrollBarFrame = iContactListBox->ScrollBarFrame();
    if ( scrollBarFrame )
        {
        CEikScrollBar* scrollBar = scrollBarFrame->VerticalScrollBar();
        if ( scrollBar && scrollBar->IsVisible() )
            {
            TPointerEvent simulatedPointerEvent( TPointerEvent::EButton1Up, 0,
                                                 TPoint(), TPoint() );
            TRAP_IGNORE( scrollBar->HandlePointerEventL( simulatedPointerEvent ) );
            }
        }
    }

// -----------------------------------------------------------------------------
// CEasyDialingPlugin::DoHandleContactsChangedL
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::DoHandleContactsChangedL()
    {
    if ( iSearchString.Length() > 0 )
        {
        if ( iContactLauncherActive )
            {
            // Set the flag to make a search when communication launcher exits.
            iNewSearchNeeded = ETrue;
            }
        else
            {
            // We get here if user e.g. leaves dialer open and goes to Contacts
            // application and does some editing.
            iContactDataManager->Reload(); // to update thumbnails
            AsyncActionLaunchL( ELaunchSearch );
            }
        }
    }

// -----------------------------------------------------------------------------
// CEasyDialingPlugin::ShowContactListBoxWithEffect
// Use appear/disappear effects when in foreground and when listbox truly
// changes visiblity.
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::ShowContactListBoxWithEffect()
    {
    if ( !IsVisible() )
        {
        // never show listbox if easydialing is not visible
        return;
        }
    // Show effect only if listbox is about to come visible.
    else if ( !iContactListBox->IsVisible() && CanListBoxEffectBeUsed() )
        {
        GfxTransEffect::Begin( iContactListBox, KGfxContactListBoxOpenEffect );
        iContactListBox->MakeVisible( ETrue );
        GfxTransEffect::SetDemarcation( iContactListBox, iContactListBox->Rect() );
        GfxTransEffect::End( iContactListBox );
        }
    else
        {
        iContactListBox->MakeVisible( ETrue );
        }
    }

// -----------------------------------------------------------------------------
// CEasyDialingPlugin::HideContactListBoxWithEffect
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::HideContactListBoxWithEffect()
    {
    // Show effect only if listbox is about to disappear from the screen.
    if ( iContactListBox->IsVisible() && CanListBoxEffectBeUsed() )
        {
        GfxTransEffect::Begin( iContactListBox, KGfxContactListBoxCloseEffect );
        iContactListBox->MakeVisible( EFalse );
        GfxTransEffect::SetDemarcation( iContactListBox, iContactListBox->Rect() );
        GfxTransEffect::End( iContactListBox );
        }
    else
        {
        iContactListBox->MakeVisible( EFalse );
        }
    }

// -----------------------------------------------------------------------------
// CEasyDialingPlugin::CanListBoxEffectBeUsed
// -----------------------------------------------------------------------------
//
TBool CEasyDialingPlugin::CanListBoxEffectBeUsed() const
    {
    TBool canBeUsed( EFalse );
    
    CAknAppUi* appUi = static_cast<CAknAppUi*>( iCoeEnv->AppUi() );
    
    // Note that when vkb is open, phone still keeps foreground and focus so
    // vkb status must be checked separately (vkb's window group has just higher
    // priority than phone's window group).
    if ( appUi && appUi->IsForeground() &&
         !iVirtualKeyboardOpen &&
         GfxTransEffect::IsEnabled() &&
         GfxTransEffect::IsRegistered( iContactListBox ) )
        {
        canBeUsed = ETrue;
        }
    
    return canBeUsed;
    }

// -----------------------------------------------------------------------------
// CEasyDialingPlugin::SetInfoLabelColourL
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin::SetInfoLabelColourL()
    {
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRgb skinColor;
    TInt error = AknsUtils::GetCachedColor( skin, skinColor,
            KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG6 );
            
    if ( error == KErrNone )
        {
        iInfoLabelLine1->OverrideColorL( EColorLabelText, skinColor );
        iInfoLabelLine2->OverrideColorL( EColorLabelText, skinColor );
        }    
    }


/*
 * ==============================================================================
 * 
 * Local functions
 * 
 * ==============================================================================
 */



// -----------------------------------------------------------------------------
// CompareTPsMatchLocation
// Compares two TPsMatchLocation objects based on where the match 
// is located in the search string. This used in ordering matches
// based on their location.
// -----------------------------------------------------------------------------
//
TInt CompareTPsMatchLocation( const TPsMatchLocation& a1, const TPsMatchLocation& a2)
    {
    return a1.index - a2.index;
    }


// -----------------------------------------------------------------------------
// AppendStringWithMatchDataL
// Appends a string with match data into a TDes buffer. Match data is a string 
// where matching and non-matching parts are separated by matching hightlight separator
// characters.
// -----------------------------------------------------------------------------
//
void AppendStringWithMatchDataL(
        TDes& aBuffer,
        const TDesC& aText,
        CPSRequestHandler* aPSHandler,
        const CPsQuery* aPSQuery )
    {
    if ( HighlightingSupportedForText( aText ) )
        {
        CDesCArrayFlat* matchingParts = new (ELeave) CDesCArrayFlat(KEasyDialingMaximumMatchingParts);
        CleanupStack::PushL( matchingParts );
     
        RArray<TPsMatchLocation> matchIndices;
        CleanupClosePushL( matchIndices );
        
        aPSHandler->LookupL( *aPSQuery , aText , *matchingParts, matchIndices );
        
        // Sort matchIndices array. Later handling assumes it to be sorted according to index field.
        TLinearOrder<TPsMatchLocation> order( CompareTPsMatchLocation );
        matchIndices.Sort( order );
       
        TInt numberOfIndices = matchIndices.Count();
        TInt textOffset = 0;
        
        for ( TInt i = 0; i < numberOfIndices ; i++ )
            {
            TInt matchingPartStart = matchIndices[i].index;
            TInt matchingPartLength = matchIndices[i].length;
            
            // Append the non-matching part (if exists) 
            if ( matchingPartStart > textOffset )
                {
                TPtrC notMatchingPart = aText.Mid( textOffset, matchingPartStart - textOffset );
                aBuffer.Append( notMatchingPart );
                }
            
            // Append matching separator charactes.
            aBuffer.Append( KHighlightSeparatorChar );
            
            TPtrC matchingPart = aText.Mid( matchingPartStart, matchingPartLength );
            aBuffer.Append( matchingPart );
            
            // Append matching separator charactes.
            aBuffer.Append( KHighlightSeparatorChar );
            
            textOffset = matchingPartStart + matchingPartLength;
            }

        CleanupStack::PopAndDestroy( &matchIndices );
        CleanupStack::PopAndDestroy( matchingParts );

        // Now there still may be one not matching part to be appended.
        if ( textOffset < aText.Length() )
            {
            TPtrC notMatchingPart = aText.Mid( textOffset, aText.Length() - textOffset );
            aBuffer.Append( notMatchingPart );
            }
        }
    else // HighlightingSupportedForText( aText )
        {
        aBuffer.Append( aText );
        }
    }

// -----------------------------------------------------------------------------
// HighlightingSupportedForText
// Checks if highlighting of matching text part is supported for the combination
// of scripts used in the given text
// -----------------------------------------------------------------------------
//
TBool HighlightingSupportedForText( const TDesC& aText )
    {
    // Highlighting for text is supported unless it contains one ore more
    // characters written with unsupported script.
    TBool supported = ETrue;
    for ( TInt i = 0 ; i < aText.Length() && supported ; ++i )
        {
        supported = HighlightingSupportedForScript( aText[i] );
        }
    
    // ... or it contains both LTR and RTL blocks
    if ( supported )
        {
        supported = !IsStrictlyBidirectional( aText );
        }
    
    return supported;
    }

// -----------------------------------------------------------------------------
// HighlightingSupportedForScript
// Checks if highlighting of matching text part is supported for the script
// of given character
// -----------------------------------------------------------------------------
//
TBool HighlightingSupportedForScript( TText aChar )
    {
    // For now, we don't support highlighting for any Arabic or South East Asian 
    // script. This is because many of these scripts use rendering rules
    // which cause problems for our simple highlighting logic designed for
    // Latin script.

    TBool belongsToUnsupportedRange = 
        ( aChar >= KArabicAndSouthEastAsianRangeStart && aChar <= KArabicAndSouthEastAsianRangeEnd ) ||
        ( aChar >= KArabicPresentationFormsARangenStart && aChar <= KArabicPresentationFormsARangeEnd ) ||
        ( aChar >= KArabicPresentationFormsBRangenStart && aChar <= KArabicPresentationFormsBRangeEnd );
    return !belongsToUnsupportedRange;
    }

// -----------------------------------------------------------------------------
// IsStrictlyBidirectional
// 
// Returns true if argument descriptor contains both left-to-right and
// right-to-left blocks
// -----------------------------------------------------------------------------
//
TBool IsStrictlyBidirectional( const TDesC& aText )
    {
    TBool bothDirectionsFound = EFalse;
    
    // TRunInfoArray contains information of the directionalities of the different sections of the aText    
    TBidirectionalState::TRunInfo array[ KMaxRunInfoArrayCount ];
    
    // Initialize the TBidiLogicalToVisual converter for making the conversion from logical to visual order
    TBidiLogicalToVisual converter( aText, array, KMaxRunInfoArrayCount );
    
    // Do the reordering. Amount of different directionality sections is returned.
    TInt blockCount = converter.Reorder();
    if ( blockCount > KMaxRunInfoArrayCount )
        {
        // If there are  more directionality blocks than we are prepared to handle, then we don't
        // know the directionality of them all. Report this as bidirectional to be on the safe side.
        // This should be an extremely rare special case.
        bothDirectionsFound = ETrue;
        }
    else if ( blockCount > 1 )
        {
        // If there are more than one directionality blocks, go through all of them and
        // check if the resolved direction changes in any of the blocks.
        TUint8 firstDirection = array[0].iDirection;
        for ( TInt i = 1 ; i < blockCount && !bothDirectionsFound ; i++ )
            {
            if ( array[i].iDirection != firstDirection )
                {
                bothDirectionsFound = ETrue;
                }
            }
        }

    return bothDirectionsFound;
    }


// -----------------------------------------------------------------------------
// AllocWithoutHighlightSeparatorsLC
// 
// Allocates a copy of the parameter descriptor with highlight separators 
// removed.
// -----------------------------------------------------------------------------
//
static HBufC* AllocWithoutHighlightSeparatorsLC( TDesC& aDesc )
    {
    HBufC* resultDesc = aDesc.AllocLC();
    TPtr ptr = resultDesc->Des();
    AknTextUtils::StripCharacters( ptr, KHighlightSeparatorCharAsLiteral );
    return resultDesc;
    }


// -----------------------------------------------------------------------------
// IsItuTCharacter
// 
// Test whether parameter character is a number pad character "0123456789#*+"
// -----------------------------------------------------------------------------
//
static TBool IsItuTCharacter( TChar aChar )
    {
    return aChar.IsDigit() || 
         aChar.GetCategory() == TChar::EArabicNumber ||
         aChar == TChar('#') ||
         aChar == TChar('*') ||
         aChar == TChar('+');
    }

// -----------------------------------------------------------------------------
// Find contact link pointing to the same contact as given link
// -----------------------------------------------------------------------------
//
static TInt Find( const MVPbkContactLink* aLink, const RPointerArray<MVPbkContactLink>& aArray )
    {
    TInt idx = KErrNotFound;
    for ( TInt i = 0 ; i < aArray.Count() ; ++i )
        {
        if ( aArray[i]->IsSame( *aLink ) )
            {
            idx = i;
            i = aArray.Count();
            }
        }
    return idx;
    }

// -----------------------------------------------------------------------------
// CleanupStack helpers for item owning RPointerArrays (etc)
// -----------------------------------------------------------------------------
//
template <class T>
class CleanupResetAndDestroy
    {
public:
    inline static void PushL( T& aRef )
        { CleanupStack::PushL( TCleanupItem(&ResetAndDestroy,&aRef) ); }
private:
    inline static void ResetAndDestroy( TAny *aPtr )
        { static_cast<T*>(aPtr)->ResetAndDestroy(); }
    };

template <class T>
inline void CleanupResetAndDestroyPushL( T& aRef )
    { CleanupResetAndDestroy<T>::PushL(aRef); }

//  End of File

