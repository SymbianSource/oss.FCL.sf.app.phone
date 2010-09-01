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
* Description:  Capsulates animation, image, label or control information.
*
*/


#include "telbubblecustomelement.h"
#include "telbubbleanim.h"
#include "telbubbleimage.h"
#include "telbubblelabel.h"
#include "telbubblecallimage.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CBubbleCustomElement::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleCustomElement* CTelBubbleCustomElement::NewL( 
    CCoeControl* aControl, TElementType aElement )
    {
    CTelBubbleCustomElement* self = new( ELeave )CTelBubbleCustomElement( 
        aControl, aElement );
    
    return self;
    }
    
// ---------------------------------------------------------------------------
// CTelBubbleCustomElement::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleCustomElement* CTelBubbleCustomElement::NewL( 
    CTelBubbleAnim* aControl, TElementType aElement )
    {
    CTelBubbleCustomElement* self = new( ELeave )CTelBubbleCustomElement( 
        aControl, aElement );
    
    return self;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCustomElement::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleCustomElement* CTelBubbleCustomElement::NewL( 
    CTelBubbleLabel* aControl, TElementType aElement )
    {
    CTelBubbleCustomElement* self = new( ELeave )CTelBubbleCustomElement( 
        aControl, aElement );
    
    return self;
    }
    
// ---------------------------------------------------------------------------
// CTelBubbleCustomElement::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleCustomElement* CTelBubbleCustomElement::NewL(
    CTelBubbleImage* aControl, TElementType aElement )
    {
    CTelBubbleCustomElement* self = new( ELeave )CTelBubbleCustomElement( 
        aControl, aElement );
    
    return self;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCustomElement::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleCustomElement* CTelBubbleCustomElement::NewL(
    CTelBubbleCallImage* aControl, TElementType aElement )
    {
    CTelBubbleCustomElement* self = new( ELeave )CTelBubbleCustomElement( 
        aControl, aElement );
    
    return self;
    }
    
// ---------------------------------------------------------------------------
// CTelBubbleCustomElement::Control
// ---------------------------------------------------------------------------
//
EXPORT_C CCoeControl* CTelBubbleCustomElement::Control() const
    {
    return iControl;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCustomElement::Element
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleCustomElement::TElementType 
    CTelBubbleCustomElement::ElementType() const
    {
    return iElementType;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCustomElement::Type
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleCustomElement::TControlType 
    CTelBubbleCustomElement::ControlType() const
    {
    return iControlType;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCustomElement::~CTelBubbleCustomElement
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleCustomElement::~CTelBubbleCustomElement()
    {
    delete iControl;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCustomElement::CTelBubbleCustomElement
// ---------------------------------------------------------------------------
//
CTelBubbleCustomElement::CTelBubbleCustomElement(
    CCoeControl* aControl, TElementType aElement ) :
    iControl( aControl ),
    iElementType( aElement )
    {
    iControlType = EBubbleControl;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCustomElement::CTelBubbleCustomElement
// ---------------------------------------------------------------------------
//
CTelBubbleCustomElement::CTelBubbleCustomElement( 
    CTelBubbleAnim* aControl, TElementType aElement ) :
    iControl( aControl ),
    iElementType( aElement )
    {
    iControlType = EBubbleAnimation;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCustomElement::CTelBubbleCustomElement
// ---------------------------------------------------------------------------
//
CTelBubbleCustomElement::CTelBubbleCustomElement( 
    CTelBubbleLabel* aControl, TElementType aElement ) :
    iControl( aControl ),
    iElementType( aElement )
    {
    iControlType = EBubbleLabel;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCustomElement::CTelBubbleCustomElement
// ---------------------------------------------------------------------------
//
CTelBubbleCustomElement::CTelBubbleCustomElement( 
    CTelBubbleImage* aControl, TElementType aElement ) :
    iControl( aControl ),
    iElementType( aElement )
    {
    iControlType = EBubbleImage;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCustomElement::CTelBubbleCustomElement
// ---------------------------------------------------------------------------
//
CTelBubbleCustomElement::CTelBubbleCustomElement( 
    CTelBubbleCallImage* aControl, TElementType aElement ) :
    iControl( aControl ),
    iElementType( aElement )
    {
    iControlType = EBubbleCallImage;
    }    

// end of file
