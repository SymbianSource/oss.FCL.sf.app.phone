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
* Description:  Handles the updating of touch buttons
*
*/


#ifndef CPHONEBUTTONSCONTROLLER_H
#define CPHONEBUTTONSCONTROLLER_H

#include <e32base.h>
#include <bmtouchpaneinterface.h>
#include "mphonebuttoncustomization.h"

class TPhoneCommandParam;

/**
 *  Updates touch buttons
 *
 *  @lib PhoneUIView.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CPhoneButtonsController ): 
    public CBase, MBubbleTouchPaneIconProvider
    {
public:

    static CPhoneButtonsController* NewL(
        MBubbleTouchPaneInterface& aTouchPane );

    virtual ~CPhoneButtonsController();
    
    /**
     * Loads buttons form given resouce.
     *
     * @since S60 v5.0
     * @param aCommandParam Mute information
     */
    void SetButtonSet( TPhoneCommandParam* aCommandParam );
    
    /**
     * SetMuteFlag
     *
     * @since S60 v5.0
     * @param aCommandParam 
     */
    void SetMuteFlag( TPhoneCommandParam* aCommandParam );
    
    /**
     * SetHoldFlag
     *
     * @since S60 v5.0
     * @param aCommandParam 
     */
    void SetHoldFlag( TPhoneCommandParam* aCommandParam );
   
    /**
     * Handles IHF change event.
     *
     * @since S60 v5.0
     * @param aCommandParam IHF information.
     */
    void SetIhfFlag( TPhoneCommandParam* aCommandParam );

    /**
     * Handles BT accessory output change event.
     *
     * @since S60 v5.0
     * @param aCommandParam BTA information.
     */    
    void SetBTAccFlag( TPhoneCommandParam* aCommandParam );

    /**
     * Handles Wired accessory output change event.
     *
     * @since S60 v5.0
     * @param aCommandParam wired information.
     */    
    void SetWiredAccFlag( TPhoneCommandParam* aCommandParam );
    
    /**
     * Handles BT accessory availability change event.
     *
     * @since S60 v5.0
     * @param aCommandParam BTA availability 
     */    
    void SetBTAccAvailableFlag( TPhoneCommandParam* aCommandParam );

    /**
     * Handles switch to video change event.
     *
     * @since S60 v5.0
     * @param aCommandParam  
     */    
    void SetVideoCallFlag( TPhoneCommandParam* aCommandParam );
    
    /**
     * Handles conference join availability change event.
     *
     * @since S60 v5.0
     * @param aCommandParam Add to conference availability 
     */    
    void SetInCallJoinEnabled( TPhoneCommandParam* aCommandParam );
 
    /**
     * Replace Audio Button
     *
     * @since S60 v5.0
     */
    void ReplaceAudioButton(); 
    
    /**
     * Sets buttons to correct states.
     *
     * @since S60 v5.0
     */
     void SetToggleButtons();
     
    /**
     * Set button dimming.
     *
     * @since S60 v5.0
     */
    void SetButtonDimming();
    
    /**
     * Set button enabled.
     *
     * @since S60 v5.0
     */
    void SetButtonEnabled( TPhoneCommandParam* aCommandParam );
    
    /**
     * Set button disabled.
     *
     * @since S60 v5.0
     */
    void SetButtonDisabled( TPhoneCommandParam* aCommandParam );

    /**
     * Set customization.
     *
     * @since S60 v5.0
     */
    void SetCustomization( MPhoneButtonCustomization* aCustomization );

private: // From MBubbleTouchPaneIconProvider
    CGulIcon* GetIconForCommandL( TInt aCommandId );
    
    // From MBubbleTouchPaneIconProvider
    TAknsItemID GetButtonForCommandL( TInt aCommandId );
    

private:

    CPhoneButtonsController( MBubbleTouchPaneInterface& aTouchPane );
    
    void ConstructL();
    
private: // data
    MBubbleTouchPaneInterface& iTouchPane;

    // Tool bar button customization
    MPhoneButtonCustomization* iCustomization;

    // Flags
    enum TPhoneButtonsFlags
        {
        EButtonsPhoneMuted = 1,
        EButtonsCallOnHold = 2,
        EButtonsIhfActive  = 4,
        EButtonsWiredAcc   = 8,
        EButtonsBTAcc      = 16,
        EButtonsBTAccAvail = 32,
        EButtonsBTAccTmpUn = 64,
        EButtonsDimAnswer  = 128,
        EButtonsDimPrivate = 256,
        EButtonsDimJoin    = 512,
        EButtonsVideoCallOngoing = 1024,
        EButtonsDimSilence  = 2048,
        EButtonsDimCreateConference = 4096  
        };
    TInt iFlags;
    };

#endif // CPHONEBUTTONSCONTROLLER_H
