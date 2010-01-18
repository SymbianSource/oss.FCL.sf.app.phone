/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles the updating dialer, toolbar buttons.
*
*/


#ifndef CPHONEDIALERCONTROLLER_H
#define CPHONEDIALERCONTROLLER_H

#include <e32base.h>
#include <akntoolbarobserver.h>
#include <mnumberentry.h>
#include "mphonenumberentrychangedhandler.h"

class CAknToolbar;
class CPhoneBubbleWrapper;
class CCoeEnv;
class CPhoneMenuController;
class TPhoneCommandParam;
class CAknButton;
class TAknsItemID;
class MPhoneDialerController;
class TPhoneCommandParam;

/**
 *  Dialer controller, updates toolbar buttons
 *
 *  @lib PhoneUIView.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CPhoneDialerController ): 
    public CBase, public MAknToolbarObserver, public MNumberEntryObserver,
    public MPhoneNumberEntryChangedHandler
    {
public:
    /**
     * Two-phased constructor.
     * @param aBubbleWrapper 
     * @param aCoeEnv 
     * @param aMenuController 
     */
    static CPhoneDialerController* NewL( CPhoneBubbleWrapper& aBubbleWrapper,
                                         CCoeEnv& aCoeEnv,
                                         CPhoneMenuController& aMenuController );
    /**
     * Destructor.
     */
    virtual ~CPhoneDialerController();
    
     /**
     * Updates toolbar buttons according to numberentry state.
     *
     * @since S60 v5.0
     */
    void UpdateToolbar();
    
     /**
     * Sets state of dtmf dialer visibility.
     *
     * @since S60 v5.0
     * @param aVisible
     */    
    void SetDtmfDialerVisibleL( TBool aVisible );  
    
     /**
     * Returns dtmf dialer visibility status.
     *
     * @since S60 v5.0
     * @return ETrue if DTMF Dialer is visible
     */     
    TBool IsDTMFDialerVisible() const;
    
     /**
     * Sets state of restricted dialer.
     *
     * @since S60 v5.0
     * @param aRestricted
     */      
	void SetRestrictedDialer( TBool aRestricted );    
	
     /**
     * Sets state of service code flag.
     *
     * @since S60 v5.0
     * @param aCommandParam
     */	
	void SetServiceCodeFlagL( TPhoneCommandParam* aCommandParam );
    
	/**
    * Enables toolbar
    *
    * @since S60 v5.0
    */ 
	void ShowToolbar();

    /**
    * Disables toolbar
    *
    * @since S60 v5.0
    */ 
    void HideToolbar();	

    /**
    * Sets custom dialer visible.
    * @param aCustomController Pointer to dialer extension implementation
    * @since S60 v5.1
    */    
    void ShowCustomizedDialerL( 
            MPhoneDialerController* aCustomController );
    
    /**
    * Sets custom dialer visible.
    * @since S60 v5.1
    */ 
    void HideCustomizedDialer();
	
    /**
    * Returns flag indicating if customized dialer is visible.
    * @return ETrue if customized dialer visible, else EFalse
    * @since S60 v5.1
    */   
    TBool IsCustomizedDialerVisible() const;
	
    /**
    * Returns sotkeys resource of the customized dialer
    * @return Customized dialer softkeys resource id
    * @since S60 v5.1
    */ 
    TInt CustomizedCbaResourceId() const;
	
    /**
    * Returns menu resource of the customized dialer
    * @return Customized dialer menu resource id
    * @since S60 v5.1
    */ 
    TInt CustomizedMenuResourceId() const;

public:     // from MAknToolbarObserver
    /**
     * From base class MAknToolbarObserver
     * Handles toolbar events for a certain toolbar item.
     *
     * @param aCommand The command ID of some toolbar item.
     */ 
    void OfferToolbarEventL( TInt aCommand );
    
    /**
     * From base class MAknToolbarObserver
     * Should be used to set the properties of some toolbar components 
     * before it is drawn.
     *   
     * @param aResourceId The resource ID for particular toolbar
     * @param aToolbar The toolbar object pointer
     */
    void DynInitToolbarL( TInt aResourceId, CAknToolbar* aToolbar );
    
public:     // from MNumberEntryObserver
    /**
     * From base class MNumberEntryObserver
     * Indication that numberentry state changed
     *
     * @since S60 v5.0
     */
    void NumberEntryStateChanged( TBool aHidePromptText );    
    
public:    // from base class MPhoneNumberEntryChangedHandler
        
    /**
     * From MPhoneNumberEntryChangedHandler
     * 
     * Handles number entry state change. 
     * Checks has the content of the number entry been modifed.
     * Informs the via call back functionality the party interested about 
     * the change. (at the moment phone state machine) 
     * 
     * @since S60 5.0
     */
     void HandleNumberEntryChanged( );
        
     /**
      * From MPhoneNumberEntryChangedHandler
      * 
      * Sets the call back function that is used to get notification when the 
      * content of the Number Entry has changed. (Used by the the phone 
      * statemachine.)  
      * 
      * @since S60 5.0
      */
     void SetNumberEntryChangedCallBack( TPhoneCommandParam* aCommandParam );
      
private:

    /**
     * Creates CAknButton with given parameters and leaves it to 
     * CleanupStack.
     * 
     * @param aNormalIconId
     * @param aNormalMaskId
     * @param aTooltipText
     * @param aSkinIconId
     * @param aCommand 
     */  
    CAknButton* CreateButtonLC( TInt aNormalIconId,
                                TInt aNormalMaskId,
                                const TDesC& aTooltipText, 
                                const TAknsItemID& aSkinIconId,
                                TInt aCommand );
    
    /**
     * Creates button set for an empty Number Entry.  
     */
    void CreateButtonSetNumberEntryEmpty();

    /**
     * Creates button set for Number Entry when some charaters 
     * have been entered.
     */
    void CreateButtonSetNumberEntryNotEmpty();
    
    /**
     * Constructor.
     * 
     * @param aBubbleWrapper 
     * @param aCoeEnv 
     * @param aMenuController 
     */
    CPhoneDialerController( CPhoneBubbleWrapper& aBubbleWrapper,
                             CCoeEnv& aCoeEnv,
                             CPhoneMenuController& aMenuController );    
    /**
     * Symbian 2nd phase constructor.
     * 
     */
    void ConstructL();
    
    /**
     * Searches the skin id for the given icon.
     */
    inline TAknsItemID SkinId( TInt aIconIndex );
    
    /**
     * Searches the tooltip text for the button using the 
     * the given command id.
     * 
     * @param aCommandId the command id
     * @param aText     the searched tooltip text
     */
    void  GetTooltipTextL( TInt aCommandId, HBufC*& aText ); 
    
private: // data
    /**
     * AknToolbar. 
     * Not own.  
     */
    CAknToolbar* iToolbar;
    
    /**
     * The reference to the Bubblewrapper
     */
    CPhoneBubbleWrapper& iBubbleWrapper;
    
    /**
     * CoeEnv 
     */
    CCoeEnv& iCoeEnv;
    
    /**
     * Menucontroller
     */
    CPhoneMenuController& iMenuController;    
    
    /**
     * Used to check is the Dialer in DTMF mode
     */
    TBool iDtmfDialerVisible;

    /**
     * Used to check is the Dialer in the restricted mode.
     * 'Send' and 'Add to Contacts' buttons are dimmed.
     */
    TBool iRestrictedDialer; 
    
    /**
     * Used to check is the service code flag set.
     * Tooltip text is changed between 'Send' or 'Call' 
     */
    TBool iServiceCodeFlag;
    
    /*
     * Is used to check has the hide prompt text flag changed.
     */
    TBool iPreviousHidePromptTextStatus;
    
    // Pointer to dialer extension implementation
    MPhoneDialerController* iCustomizedDialer;
    
    /**
     * Is used to check should toolbar be updated. 
     */
    TBool iUpdateToolbar;

    };

#include    "cphonedialercontroller.inl"

#endif // CPHONEDIALERCONTROLLER_H
