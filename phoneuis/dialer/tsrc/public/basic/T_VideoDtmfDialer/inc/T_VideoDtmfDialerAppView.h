/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of CT_VideoDtmfDialerAppView class. 
*
*/

#ifndef T_VIDEODTMFDIALER_APPVIEW_H
#define T_VIDEODTMFDIALER_APPVIEW_H


#include <coecntrl.h>
#include <coeview.h>

class MNumberEntry;

class CT_VideoDtmfDialerAppView : public CCoeControl, public MCoeView
    {
public:

    static CT_VideoDtmfDialerAppView* NewL(const TRect& aRect);

    static CT_VideoDtmfDialerAppView* NewLC(const TRect& aRect );

     ~CT_VideoDtmfDialerAppView();

    void SetControl( CCoeControl* aControl, TBool aDrawNow = EFalse );
    CCoeControl* Control() const;
    void SetNumberEntry( MNumberEntry* aNumberEntry );
    
public:  // from CCoeControl
    void Draw(const TRect& aRect) const;

    void SizeChanged();

    TInt CountComponentControls() const;

    CCoeControl* ComponentControl( TInt aIndex ) const;
    
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

	TVwsViewId ViewId() const;

	void ViewActivatedL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage);

	void ViewDeactivated();

    void ActivateL();


private:

    void ConstructL(const TRect& aRect);

    CT_VideoDtmfDialerAppView();

public:
    void UpdateControlRect();
    
private:
    
    CCoeControl* iControl;
    CCoeControl* iPrevControl;
    
    // Not own. Forwards key events for number entry control.
    MNumberEntry* iNumberEntry;
    };


#endif // T_VIDEODTMFDIALER_APPVIEW_H
