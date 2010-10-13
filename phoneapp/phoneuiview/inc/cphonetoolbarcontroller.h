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


#ifndef CPHONETOOLBARCONTROLLER_H
#define CPHONETOOLBARCONTROLLER_H

#include <e32base.h>
#include <akntoolbarobserver.h>

class CAknToolbar;
class CCoeEnv;
class TPhoneCommandParam;
class CAknButton;
class TAknsItemID;
class TPhoneCommandParam;


/**
 *  Toolbar controller, updates toolbar buttons
 *
 *  @lib PhoneUIView.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CPhoneToolbarController ): 
    public CBase, public MAknToolbarObserver 
    {
public:
    /**
     * Two-phased constructor.
     * @param aCoeEnv 
     * @param aMenuController 
     */
    static CPhoneToolbarController* NewL( CCoeEnv& aCoeEnv);
    /**
     * Destructor.
     */
    virtual ~CPhoneToolbarController();
    
     /**
     * Updates toolbar buttons.
     */
    void UpdateToolbar();
  

    
	/**
    * Enables toolbar
    */ 
	void ShowToolbar();

    /**
    * Disables toolbar
    */ 
    void HideToolbar();	
    
    void SetMuteFlag( TPhoneCommandParam* aCommandParam );

    void SetIhfFlag( TPhoneCommandParam* aCommandParam );
    
    void SetWiredAccFlag( TPhoneCommandParam* aCommandParam );
    
    void SetCallInProgressFlag( TPhoneCommandParam* aCommandParam ); 
    
    /**
     * Dims (greys out) or undims all toolbar items.
     *
     * @param aDimmed Use ETrue to dim this toolbar item or EFalse to
     *      un-dim this toolbar item.
     */
    void DimToolbar( const TBool aDimmed );
    
    /**
     * Dims (greys out) or undims a toolbar item.
     *
     * @param aCommandId The command (as defined in an .hrh file)
     *      associated with this toolbar item. This identifies the toolbar
     *      item, whose text is to be dimmed or un-dimmed.
     * @param aDimmed Use ETrue to dim this toolbar item or EFalse to
     *      un-dim this toolbar item.
     */
    void SetToolbarButtonDimmed( const TInt aCommandId,
                                 const TBool aDimmed );

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

protected:
    /**
     * Constructor.
     * 
     * @param aCoeEnv 
     */
    CPhoneToolbarController( CCoeEnv& aCoeEnv );    
    
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
     * Symbian 2nd phase constructor.
     * 
     */
    void ConstructL();
    
    /**
     * Searches the tooltip text for the button using the 
     * the given command id.
     * 
     * @param aCommandId the command id
     * @param aText     the searched tooltip text
     */
    void  GetTooltipTextL( TInt aCommandId, HBufC*& aText ); 
    
    TAknsItemID GetSkinIdL( TInt aCommandId );

        
private: // data
    /**
     * AknToolbar. 
     * Not own.  
     */
    CAknToolbar* iToolbar;

    /**
     * CoeEnv 
     */
    CCoeEnv& iCoeEnv;    
        
    TBool iMuteFlag;
    
    TBool iIhfFlag;
    
    TBool iWiredAccFlag;
    
    TBool iCallInProgress;

    // Dim activate IHF button
    TBool iDimActivateIhf;
    
    };

#endif // CPHONETOOLBARCONTROLLER_H
