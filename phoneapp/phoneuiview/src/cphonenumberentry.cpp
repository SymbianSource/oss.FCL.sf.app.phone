/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles the updating of number entry
*
*/


#include <featmgr.h>
#include <bmbubblemanager.h>
#include <AknPhoneNumberEditor.h>

#include "cphonenumberentry.h"
#include "mnumberentry.h"
#include "phoneconstants.h"
#include "tphonecmdparamnumberentryobserver.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhoneNumberEntry::CPhoneNumberEntry( CBubbleManager& aBubbleManager ):
    iBubbleManager ( aBubbleManager )
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhoneNumberEntry::ConstructL()
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ) )
        {
        iUseDialer = ETrue;
        }
    
    iNumberEntryContents = HBufC::NewL( KPhoneNumberEntryBufferSize );
    iPreviousNumberEntryContent = HBufC::NewL( KPhoneNumberEntryBufferSize );
   
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhoneNumberEntry* CPhoneNumberEntry::NewL( CBubbleManager& aBubbleManager )
    {
    CPhoneNumberEntry* self = 
        new (ELeave) CPhoneNumberEntry( aBubbleManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhoneNumberEntry::~CPhoneNumberEntry()
    {
    delete iNumberEntryContents;
    delete iPreviousNumberEntryContent;
    
    }

// ---------------------------------------------------------------------------
// EnableTactileFeedback
// ---------------------------------------------------------------------------
//
void CPhoneNumberEntry::EnableTactileFeedback( const TBool aEnable )
    {
    if ( iUseDialer )
        {
        iDialerNumberEntry->EnableTactileFeedback( aEnable );
        }
    }

// ---------------------------------------------------------------------------
// IsNumberEntryUsed
// ---------------------------------------------------------------------------
//
TBool CPhoneNumberEntry::IsNumberEntryUsed() const
    {
    // Get the initial number from number entry window
    if ( iUseDialer )
        {
        return iDialerNumberEntry->IsNumberEntryUsed();
        }
    else
        {
        return iBubbleManager.IsNumberEntryUsed();
        }
    }
    

// ---------------------------------------------------------------------------
//  
// ---------------------------------------------------------------------------
//
TBool CPhoneNumberEntry::CountNumberEntryCharacters( )
    {
    TInt result = 0;

    if ( iUseDialer )
        {
        if ( iDialerNumberEntry->IsNumberEntryUsed() )
            {
            TPtr ptr( iNumberEntryContents->Des() );
            iDialerNumberEntry->GetTextFromNumberEntry( ptr );        
            result = iNumberEntryContents->Des().Length();
            }
        }
    else
        {
        if ( iBubbleManager.IsNumberEntryUsed() )
            {
            TPtr ptr( iNumberEntryContents->Des() );
            iBubbleManager.GetTextFromNumberEntry( ptr );        
            result = iNumberEntryContents->Des().Length();
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// SetNumberEntry
// -----------------------------------------------------------------------------
//     
void CPhoneNumberEntry::SetNumberEntry( MNumberEntry* aNumberEntry )
    {
    if ( iUseDialer )
        {
        iDialerNumberEntry = aNumberEntry;
        }
    }


// -----------------------------------------------------------------------------
// SetNumberEntryContent
// -----------------------------------------------------------------------------
//     
void CPhoneNumberEntry::SetNumberEntryContent( const TDesC& aContent )
    {
    if ( iUseDialer )
        {
        iDialerNumberEntry->SetTextToNumberEntry( aContent );
        }
    else
        {
        iBubbleManager.StartChanges();
        iBubbleManager.SetTextToNumberEntry( aContent );
        iBubbleManager.EndChanges();
        }
    }

// -----------------------------------------------------------------------------
// SetNumberEntryPromptText
// -----------------------------------------------------------------------------
//
void CPhoneNumberEntry::SetNumberEntryPromptText( const TDesC& aPromptText ) 
    {
    if ( iUseDialer )
        {
        iDialerNumberEntry->SetNumberEntryPromptText( aPromptText );
        }    
    }

// -----------------------------------------------------------------------------
// CPhoneNumberEntry::HandleNumberEntryChanged
// -----------------------------------------------------------------------------
//
void CPhoneNumberEntry::HandleNumberEntryChanged( )
    {
    
    TBool contentUpdated = CheckNumberEntryContent();
    
    if( contentUpdated && iNEChangedCallBack.iFunction )
        {
        iNEChangedCallBack.CallBack();
        }
    
    }

// -----------------------------------------------------------------------------
// CPhoneNumberEntry::SetNumberEntryCallBack
// -----------------------------------------------------------------------------
//
void CPhoneNumberEntry::SetNumberEntryChangedCallBack( 
        TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamNumberEntryObserver* neObserver = 
           static_cast<TPhoneCmdParamNumberEntryObserver*>( aCommandParam );
    
    iNEChangedCallBack = neObserver->Observer();
           
    }

// -----------------------------------------------------------------------------
// CPhoneNumberEntry::CheckNumberEntryContent
// -----------------------------------------------------------------------------
//
TBool CPhoneNumberEntry::CheckNumberEntryContent()
    {
    
    TBool update = UpdatePreviousNumberEntryContent( 
                                DialerNumberEntryEditor() );
    
    if( !update  )
        {
        update = UpdatePreviousNumberEntryContent( BubbleNumberEntryEditor() );
        }
    
    return update;
    }

// -----------------------------------------------------------------------------
// CPhoneNumberEntry::CheckNumberEntryContent
// -----------------------------------------------------------------------------
//
TBool CPhoneNumberEntry::UpdatePreviousNumberEntryContent( 
        CCoeControl* aEditor )
    {
    TBool ret( EFalse );
    
    if( aEditor &&  iPreviousNumberEntryContent )
        {
        CAknPhoneNumberEditor* control = 
                               static_cast<CAknPhoneNumberEditor*> ( aEditor );
        TInt res = iPreviousNumberEntryContent->Compare( control->Text(0) );
              
        if( res != 0 ) //string are different
            {
            iPreviousNumberEntryContent->Des().Zero();
            TPtr ptr( iPreviousNumberEntryContent->Des() );
            control->GetText( ptr );
            ret = ETrue;
            }
        }
    
    return ret;
    }
       
       
// -----------------------------------------------------------------------------
// CPhoneNumberEntry::CheckNumberEntryContent
// -----------------------------------------------------------------------------
//
CCoeControl* CPhoneNumberEntry::DialerNumberEntryEditor() const
    {
    CCoeControl* control = NULL;
             
    if ( iUseDialer )
        {  
        if ( iDialerNumberEntry->IsNumberEntryUsed() )
            {
            control = iDialerNumberEntry->GetNumberEntry();
            }
        }
    
    return control;
    }

// -----------------------------------------------------------------------------
// CPhoneNumberEntry::CheckNumberEntryContent
// -----------------------------------------------------------------------------
//
CCoeControl* CPhoneNumberEntry::BubbleNumberEntryEditor() const
    {
    CCoeControl* control = NULL;

    if ( iBubbleManager.IsNumberEntryUsed() )
        {
        control = iBubbleManager.GetNumberEntry(); 
        }

    return control;
    }
       
// END
