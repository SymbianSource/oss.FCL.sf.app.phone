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

#ifndef __EDTA_SCREENTEXTBUFFER_H__
#define __EDTA_SCREENTEXTBUFFER_H__

// INCLUDES
#include <coecntrl.h>
#include <eiklabel.h>
#include <coecntrl.h>
#include <gdi.h>
#include <eiksbobs.h>

// FORWARD DECLARATIONS
class CEikScrollBarFrame;
class CFont;
class TBidiText;
class CAknsBasicBackgroundControlContext;

// CONSTANTS
const TInt KEdtaMaxLineWidth = 200;


// CLASS DECLARATION
class CEdtaScreenTextBuffer : public CCoeControl, public MEikScrollBarObserver
    {
    
public:
    static CEdtaScreenTextBuffer* NewL( const TRect& aRect );
    static CEdtaScreenTextBuffer* NewLC( const TRect& aRect );
    
public:
    CEdtaScreenTextBuffer();
    ~CEdtaScreenTextBuffer();

    void ConstructL( const TRect& aRect );
    void HandleResourceChange(TInt aType);
    void WriteLineL( TRefByValue<const TDesC> aText,... ); // text wrapping, was: const TDesC& aText
    void DeleteLastLine(TInt aCount = 1);
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aModifiers );
    
protected:
    
    virtual void FocusChanged(TDrawNow aDrawNow);
    
private:
    
    void SizeChanged();

    void Draw( const TRect& aRect ) const;
    void ActivateL();
    void HandleScrollEventL ( CEikScrollBar* aScrollBar, TEikScrollEvent aEventType);
    TTypeUid::Ptr MopSupplyObject(TTypeUid aId);
    
private:
    void CalculateL(const TRect& aRect);
    void UpdateScrollIndicatorL();

    
    
private: // Data

    CArrayPtr<HBufC>* iText;
    CArrayFixFlat<TInt>* iScreenStarts;
    TInt iCurrentScreen;
    TBool iDoNotShowLastLineAgain;
    CGraphicsContext::TTextAlign iTextAlign;
    CEikScrollBarFrame* iSBFrame;
    const CFont* iFont; // not owned
    
    TInt iLineWidth;
    TInt iBaseLineDelta;
    TInt iTopBaseLineX;
    TInt iTopBaseLineY;
    TInt iLinesPerScreen;
    
    };


#endif // __EDTA_SCREENTEXTBUFFER_H__

// End of File
