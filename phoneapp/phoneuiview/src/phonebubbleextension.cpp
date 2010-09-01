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
* Description:  Bubble extension plugin wrapper
*
*/


#include <telbubbleextension.h>
#include <bmbubblemanager.h>

#include "phonebubbleextension.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CPhoneBubbleExtension* CPhoneBubbleExtension::NewL(
    CBubbleManager& aBubbleManager,
    TUid aUid,
    TUint8 aPriority )
    {
    CPhoneBubbleExtension* self =
        CPhoneBubbleExtension::NewLC( aBubbleManager, aUid, aPriority );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CPhoneBubbleExtension* CPhoneBubbleExtension::NewLC(
    CBubbleManager& aBubbleManager,
    TUid aUid,
    TUint8 aPriority )
    {
    CPhoneBubbleExtension* self =
        new( ELeave ) CPhoneBubbleExtension( aBubbleManager, aUid, aPriority );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhoneBubbleExtension::~CPhoneBubbleExtension()
    {
    delete iPlugin;
    }

// --------------------------------------------------------------------------
// From class MTelBubbleExtensionObserver.
// 
// --------------------------------------------------------------------------
//
void CPhoneBubbleExtension::StartChanges()
    {
    iBubbleManager.StartChanges();
    }

// --------------------------------------------------------------------------
// From class MTelBubbleExtensionObserver.
// 
// --------------------------------------------------------------------------
//
void CPhoneBubbleExtension::EndChanges()
    {
    iBubbleManager.EndChanges();
    }

// --------------------------------------------------------------------------
// From class MTelBubbleExtensionObserver.
// Attaches element to bubble. Causes redraw.
// --------------------------------------------------------------------------
//
inline void CPhoneBubbleExtension::AttachElement(
    TInt aBubbleId,
    CTelBubbleCustomElement* aElement )
    {
    ReplaceElement( aBubbleId, NULL, aElement);
    }

// ---------------------------------------------------------------------------
// From class MTelBubbleExtensionObserver.
// Replaces element in bubble. Causes redraw.
// ---------------------------------------------------------------------------
//
void CPhoneBubbleExtension::ReplaceElement(
    TInt aBubbleId,
    CTelBubbleCustomElement* aOld,
    CTelBubbleCustomElement* aNew )
    {
    iBubbleManager.StartChanges();
    if ( aOld )
        {
        iBubbleManager.RemoveCustomElement( aBubbleId, aOld );
        }
    if ( aNew )
        {
        iBubbleManager.AddCustomElement( aBubbleId, aNew, iPriority );
        }
    iBubbleManager.EndChanges();
    }

// --------------------------------------------------------------------------
// From class MTelBubbleExtensionObserver.
// Detaches element in bubble. Causes redraw.
// --------------------------------------------------------------------------
//
inline void CPhoneBubbleExtension::DetachElement(
    TInt aBubbleId,
    CTelBubbleCustomElement* aElement )
    {
    ReplaceElement( aBubbleId, aElement, NULL);
    }

// ---------------------------------------------------------------------------
// From class MTelBubbleExtensionInterface.
// Notifies that a new customizable bubble is available
// ---------------------------------------------------------------------------
//
inline void CPhoneBubbleExtension::StartCustomizedBubble(
    MTelBubbleExtensionData& aCallData )
    {
    iPlugin->StartCustomizedBubble( aCallData );
    }

// ---------------------------------------------------------------------------
// From class MTelBubbleExtensionInterface.
// Notifies that a customizable bubble has been removed
// ---------------------------------------------------------------------------
//
inline void CPhoneBubbleExtension::StopCustomizedBubble(
    MTelBubbleExtensionData& aCallData )
    {
    iPlugin->StopCustomizedBubble( aCallData );
    }

// ---------------------------------------------------------------------------
// From class MTelBubbleExtensionInterface.
// Indicates that currently bubble drawing is started.
// ---------------------------------------------------------------------------
//
inline void CPhoneBubbleExtension::BubbleUpdating()
    {
    iPlugin->BubbleUpdating();
    }

// ---------------------------------------------------------------------------
// First phase constructor.
// ---------------------------------------------------------------------------
//
CPhoneBubbleExtension::CPhoneBubbleExtension(
    CBubbleManager& aBubbleManager,
    TUid aUid,
    TUint8 aPriority )
    : iBubbleManager(aBubbleManager), iUid(aUid), iPriority(aPriority)
    {
    // no implementation needed
    }


// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhoneBubbleExtension::ConstructL()
    {
    iPlugin = CTelBubbleExtension::NewL( iUid );
    iPlugin->InitializeL( *this );
    }



