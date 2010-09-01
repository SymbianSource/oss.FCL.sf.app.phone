/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Container for the Data sub-folder
*
*/


// INCLUDE FILES

// From this plugin.
#include "PslnCallImagePluginContainer.h"

// From PSLN framework
#include <pslnfwbaseview.h>
#include <pslnfwiconhelper.h>
#include <csxhelp/skins.hlp.hrh>

// Resources
#include <PslnCallImagePluginRsc.rsg>

// General services
#include <aknlists.h>
#include <featmgr.h>
#include <AknUtils.h>
#include <pslninternalcrkeys.h>
#include <bautils.h>

// Logging
#include "PslnCallImagePluginLogger.h"

// CONSTANTS
// Number of Call Image plugin items.
const TInt KPslnCallImageItems = 3;

// PSLN application UID.
const TUid KUidPslnApp = { 0x10005A32 };

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CPslnCallImagePluginContainer::ConstructL()
// 
// Symbian OS two phased constructor
// ---------------------------------------------------------------------------
//
void CPslnCallImagePluginContainer::ConstructL( const TRect& aRect )
    {
    __CALLLOGSTRING("CPslnCallImagePluginContainer::ConstructL -> START");
    iListBox = new( ELeave ) CAknSingleGraphicStyleListBox;
   					 
    BaseConstructL( aRect, 0, R_PSLN_CI_VIEW_LBX );
    __CALLLOGSTRING("CPslnCallImagePluginContainer::ConstructL -> COMPLETED");
    }

// ---------------------------------------------------------------------------
// CPslnCallImagePluginContainer::~CPslnCallImagePluginContainer()
// 
// Destructor 
// ---------------------------------------------------------------------------
//
CPslnCallImagePluginContainer::~CPslnCallImagePluginContainer()
    {
    __CALLLOGSTRING("CPslnCallImagePluginContainer::~CPslnCallImagePluginContainer -> START");
    delete iItemBuf;
    delete iItems;  
    __CALLLOGSTRING("CPslnCallImagePluginContainer::~CPslnCallImagePluginContainer -> COMPLETED");
    }

// ---------------------------------------------------------------------------
// CPslnCallImagePluginContainer::UpdateListBoxL()
// 
// Update listbox item.
// ---------------------------------------------------------------------------
//
void CPslnCallImagePluginContainer::UpdateListBoxL()
    {
    __CALLLOGSTRING("CPslnCallImagePluginContainer::UpdateListBoxL -> START");
    iItemArray->Reset();
    CreateListBoxItemsL();
    iListBox->HandleItemRemovalL();
    iListBox->DrawDeferred();
    __CALLLOGSTRING("CPslnCallImagePluginContainer::UpdateListBoxL -> COMPLETED");
    }
// ---------------------------------------------------------------------------
// CPslnCallImagePluginContainer::CurrentSelectionIndex
// 
// Sets tap for the selection.
// ---------------------------------------------------------------------------
//
TInt CPslnCallImagePluginContainer::CurrentSelectionIndexL()
	{
	__CALLLOGSTRING("CPslnCallImagePluginContainer::CurrentSelectionIndexL-> START");
	TInt itemIndex(KErrNotFound);
    TInt error(KErrNone);
    TInt currentSetting(0);

    CRepository* callImageSupp = CRepository::NewL ( KCRUidThemes ); 
	CleanupStack::PushL( callImageSupp );     
	error = callImageSupp->Get( KThemesCallImageSetting, currentSetting );
       
    // Verify that the image exists.
    TFileName callImagePath;
    error = callImageSupp->Get( KThemesCallImagePath, callImagePath );
    TBool imageExists = ( callImagePath.Length() && 
        BaflUtils::FileExists( CCoeEnv::Static()->FsSession(), 
        callImagePath ) ); 
    
    if( error != KErrNone ) 
    	{
        return error;
        }
        
    // Map CR value to list box index.
    switch ( currentSetting )
        {
        case EPlsnCRCallImageNone:
            itemIndex = EPlsnCallImageNone;
            break;
        case EPlsnCRUserDefinedImage:
            if ( !imageExists )
                {
                itemIndex = EPlsnCallImageNone;
                }
            else
                {
                itemIndex = EPlsnCallImageUserDefinedImage;
                }            
            break;
        case EPlsnCRThemeImage:
        default:
            itemIndex = EPlsnCallImageThemeImage;
            break;
        }
    CleanupStack::PopAndDestroy( callImageSupp );
    __CALLLOGSTRING("CPslnCallImagePluginContainer::CurrentSelectionIndexL -> COMPLETED");
    return itemIndex;
	}
    
// ---------------------------------------------------------------------------
// CPslnCallImagePluginContainer::ConstructListBoxL()
// 
// Construct the listbox from resource array.
// ---------------------------------------------------------------------------
//
void CPslnCallImagePluginContainer::ConstructListBoxL( TInt aResLbxId )
    {
    __CALLLOGSTRING("CPslnCallImagePluginContainer::ConstructListBoxL-> START");
    iListBox->ConstructL( this, EAknListBoxSelectionList );

    iItemArray = static_cast<CDesCArray*>
        ( iListBox->Model()->ItemTextArray() );
        
    iItems = iCoeEnv->ReadDesC16ArrayResourceL( aResLbxId );
    
    CreateListBoxItemsL();  
    __CALLLOGSTRING("CPslnCallImagePluginContainer::ConstructListBoxL -> COMPLETED");
    }

// ---------------------------------------------------------------------------
// CPslnCallImagePluginContainer::CreateListBoxItemsL()
// 
// Create listbox items.
// ---------------------------------------------------------------------------
//
void CPslnCallImagePluginContainer::CreateListBoxItemsL()
    {
    __CALLLOGSTRING("CPslnCallImagePluginContainer::CreateListBoxItemsL -> START");
    TInt selectedItem = CurrentSelectionIndexL();
    if ( selectedItem < 0 )
        {
        selectedItem = 0;
        } 
        
        for( TInt i = 0; i < KPslnCallImageItems; i++ )
        {
        if ( i > iItems->Count() )
            {
            User::Leave( KErrOverflow );
            }
        iItemBuf = (*iItems)[i].AllocL();
        if ( iItemBuf )
            {            
            iItemBuf = iItemBuf->ReAllocL( iItemBuf->Length() + 4 );
            TPtr ptr = iItemBuf->Des();
            if ( selectedItem == i )
                {
                ptr.Insert( 0, KPslnFWActiveListItemFormat );
                }
            else
                {            
                ptr.Insert( 0, KPslnFWNonActiveListItemFormat );
                }
            iItemArray->InsertL( i, ptr );
            delete iItemBuf;
            iItemBuf = NULL;
            }
        }        
    // Create pre- and post-text icons.
    CPslnFWIconHelper* iconHelper = CPslnFWIconHelper::NewL();
    CleanupStack::PushL( iconHelper );   
    iconHelper->AddIconsToSettingItemsL( 
        ETrue, 
        1,
        iListBox );
    CleanupStack::PopAndDestroy( iconHelper );
    
    iListBox->HandleItemAdditionL();
    __CALLLOGSTRING("CCPslnCallImagePluginContainer::CreateListBoxItemsL -> COMPLETED");
    }
    
// ---------------------------------------------------------------------------
// CPslnCallImagePluginContainer::GetHelpContext()
// 
// Gets Help context.
// ---------------------------------------------------------------------------
//
void CPslnCallImagePluginContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidPslnApp;
    aContext.iContext = KSKINS_HLP_CIMAGE_SETTINGS;
    }

// End of File
