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
* Description: Easy dialing plugin implementation.
*
*/


#ifndef __CEASYDIALINGPLUGIN_H__
#define __CEASYDIALINGPLUGIN_H__

// INCLUDES

#include "dialingextensioninterface.h"
#include <MPsResultsObserver.h>
#include <mccaconnection.h>
#include <mccaconnectionext.h>
#include "measydialingcenreplistenerobserver.h"
#include "mcontactdatamanagerobserver.h"
#include "medcontactorobserver.h"
#include "easydialingcontactdatamanager.h"  // for TNameOrder

// ListBox Observer API
#include <eiklbo.h>

// MAknInputBlockCancelHandler
#include <akninputblock.h> 


// CONSTANTS

/** Maximum constact search string length. */
const TInt KEDMaxSearchStringLength = 200;

/** Maximum string length for listbox model string. */
const TInt KEDMaxContactStringLength = 600;

/** Maximum phone number length. */
const TInt KEDMaxPhoneNumberLength = 64;

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

class CPSRequestHandler;
class CPsQuery;
class CVPbkContactStoreUriArray;
class CVPbkContactManager;
class CEasyDialingListBox;
class MVPbkContactLink;
class CEasyDialingCenrepListener;
class CEasyDialingContactDataManager;
class CEDContactorService;
class CEikMenuPane;
class CAsyncCallBack;
class CEikLabel;

// CLASS DECLARATION

/**
* Easy dialing plugin.
*/
class CEasyDialingPlugin : public CDialingExtensionInterface,
                           public MPsResultsObserver,
                           public MCCAObserver,
                           public MEasyDialingCenrepListenerObserver,
                           public MContactDataManagerObserver,
                           public MEDContactorObserver,
                           public MEikListBoxObserver,
                           public MAknInputBlockCancelHandler,
                           public MCoeForegroundObserver
{
public:

    /**
    * Static constructor. 
    * @return: Created object.
    */
    static CEasyDialingPlugin* NewL();
    
    /**
    * Destructor. 
    */
    virtual ~CEasyDialingPlugin();

public: // from CCoeControl 

    /**
    * From CCoeControl.
    */
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
    
    /**
    * From CCoeControl
    */
    TInt CountComponentControls() const;
    
    /**
    * From CCoeControl
    */
    CCoeControl* ComponentControl( TInt aIndex ) const;
    
    /**
    * From CCoeControl
    */
    void MakeVisible( TBool aVisible );
 
    /**
    * From CCoeControl
    */
    void HandleResourceChange( TInt aType );

protected:

    /**
    * See CCoeControl
    */
    void SizeChanged();
    
    /**
    * See CCoeControl
    */
    void FocusChanged( TDrawNow aDrawNow );

public: // from CDialingExtensionInterface

    /**
    * Does further initialization to component.
    * @param: parent container of this control.
    */
    virtual void InitializeL( CCoeControl& aParent );
    
    /**
    * Reset the previour easy dialing search.
    */
    void Reset();
    
    /**
    * Adds an observer to dialing extension.
    * @param: aObserver: observer to be added.
    */
    void AddObserverL( MDialingExtensionObserver* aObserver );
   
    /**
    * Removes an observer from dialing extension.
    * @param: aObserver: observer to be removed.
    */
    void RemoveObserver( MDialingExtensionObserver* aObserver );
    
    /**
     * Sets input string (search string) to easy dialing.
     * @param: aDesC: input string descriptor.
     */
    void SetInputL( const TDesC& aDesC );
    
    /**
     * Returns the number of matching contacts in previous search.
     * @return: number of matches.
     */
    TInt MatchingContactCount();
    
    /**
     * Gets the resource id of plugin's control button area resource.
     * 
     * @return  Resource id.
     */
    TInt CbaResourceId();
    
    /**
     * From CDialingExtensionInterface.
     * Gets the resource id of plugin's menu bar resource.
     * 
     * @return  Resource id.
     */
    TInt MenuResourceId();
    
    /**
     * From CDialingExtensionInterface.
     * Gets the resource id of plugin's menu bar resource.
     * 
     * @param   aMenuPane   Menu pane
     * @param   aMenuResourceId Menu bar resource id.
     * @return  Resource id.
     */
    TBool InitializeMenuPaneL( CEikMenuPane& aMenuPane, TInt aMenuResourceId );
    
    /**
     * From CDialingExtensionInterface.
     * Gets the resource id of plugin's menu bar resource.
     * 
     * @param   aCommand   Command id.
     * @return  ETrue if command was handled, EFalse otherwise.
     */
    TBool HandleCommandL( TInt aCommand );
    
    /**
     * Tells if extension is currently enabled from settings.
     * @return  ETrue if extension is enabled, EFalse otherwise.
     */
    TBool IsEnabled() const;

public:

    /**
    * From MEasyDialingCenrepListenerObserver.
    */
    void EasyDialingSettingsChanged( TInt aValue );
    
    /**
    * From MEasyDialingCenrepListenerObserver.
    */
    void EasyDialingContactThumbnailsSettingsChanged( TInt aThumbnailSettingValue );
        
    /**
    * From MContactDataManagerObserver.
    */
    void AllContactDataLoaded();
    
    /**
    * From MContactDataManagerObserver.
    */
    void NameOrderChanged();
    
    /**
    * From MContactDataManagerObserver.
    */
    void FavouritesChanged();
    
    /**
    * From MEDContactorObserver.
    */
    void InformContactorEvent( MEDContactorObserver::TEvent aEvent );
    
    /**
    * From MEikListBoxObserver.
    */
    void HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType );
    
    /**
    * From MAknInputBlockCancelHandler.
    */
    void AknInputBlockCancel();

    /**
    * From MCoeForegroundObserver.
    */
    void HandleGainingForeground();
    
    /**
    * From MCoeForegroundObserver.
    */
    void HandleLosingForeground();

private:

    /**
    * Constructor.
    */
    CEasyDialingPlugin ();
    
    /**
    * Second phase constructor.
    */
    void ConstructL ();
    
    void InitPredictiveContactSearchL();
    
    void SetSortOrderL( CEasyDialingContactDataManager::TNameOrder aNameOrder );
    
    /**
    * From MPsResultsObserver.
    */
    virtual void HandlePsResultsUpdate( RPointerArray<CPsClientData>& aResults, RPointerArray<CPsPattern>& aSeqs );
    virtual void HandlePsError( TInt aErrorCode );
    virtual void CachingStatus( TCachingStatus& aStatus, TInt& aError );
    
    /**
     * FindContactFieldPCSIndex
     * Searches the index that stores the given contact field.
     * To see possible contact field numbering, see file "vpbkeng.rsg".
     * @param: contact field to search for. See file "vpbkeng.rsg"
     * for contact field numbering.
     * @return: index of the contact field. KErrNotFound if field was not found.
     * Non-negative return values are for indexing CPsClientData array.
     */
    TInt FindContactFieldPCSIndexL( TInt aIndex );
        
    /**
     * HandlePsResultsUpdateL
     * A leaving function to be trapped in HandlePsResultsUpdate.
     */
    void HandlePsResultsUpdateL( RPointerArray<CPsClientData>& aResults, RPointerArray<CPsPattern>& aSeqs );

public:
    
    /**
     * MCCAObserver function handling exit of CCA launcher.
     */
    void CCASimpleNotifyL( TNotifyType aType, TInt aReason );

private:
    
    /*
     * Informs all observers registered with AddObserverL.
     * @param: aEvent: event id to be informed.
     */
    void InformObservers( MDialingExtensionObserver::TEvent aEvent );


private: 

    /**
    * This method perform the drawing functionality of the component.
    * @param: aRect: The rect to be rendered
    * @return None
    */
    void Draw( const TRect& aRect ) const;
    
    /**
    * Initiates predictive contact search.
    */
    void LaunchSearchL();
    
    /**
     * Opens ca launcher for currently selected contact.
     */
    void LaunchCurrentContactL();
    
    /**
     * Creates listbox model string.
     */
    void CreateListBoxContactStringL(
            const TDesC& aContactString,
            MVPbkContactLink *aLink,
            TBool aMatchThumbnails,
            TBool aFav );
    
    /**
     * Creates string with first name, last name and company name.
     */
    HBufC* CreateContactStringLC( CPsClientData* aResult,
            CEasyDialingContactDataManager::TNameOrder aNameOrder );
    
    /** Possible actions launched by this plugin. */
    enum TEasyDialingAction
        {
        ENoActionDefined,
        ECallCurrentContact,
        EVideoCallCurrentContact,
        ESendMessageCurrentContact,
        ELaunchCurrentContact,
        ELaunchSearch,
        EInitializePcs,
        ESimulateKeyEvent
        };
    
    /**
     * Simulates a key event asynchronously
     */
    void AsyncSimulateKeyEvent( const TKeyEvent& aKeyEvent );
    
    /**
     * Initiates asynchronous callback to launch action and sets
     * input blocker active. 
     */
    void AsyncActionLaunchL( TEasyDialingAction aAction );
    
    /**
     * Callback for CAsyncCallBack. Launches action set in iActionToBeLaunched
     * and stops input block when launch is done.
     */
    static TInt AsyncCallBackToLaunchAction( TAny* aPtr );
    
    /**
     * Launches action defined in iActionToBeLaunched.
     */
    void DoLaunchActionL();
    
    /**
     * Cancels async action launch and input block.
     */
    void CancelActionLaunchAndInputBlock();
    
    /**
     * Handles change events from contact database and favourites view.
     */
    void DoHandleContactsChangedL();
    
    /**
     * Makes contact listbox visible. Effect is used if feasible.
     */  
    void ShowContactListBoxWithEffect();
    
    /**
     * Makes contact listbox invisible. Effect is used if feasible.
     */  
    void HideContactListBoxWithEffect();
    
    /**
     * Checks if listbox effect can be triggered. Eg checks if 
     * app is in the foreground.
     */  
    TBool CanListBoxEffectBeUsed() const;

    /**
     * Sets correct info label text color from theme.
     */  
    void SetInfoLabelColourL();
    
    /**
     * Sets info label visibility.
     */  
    void SetInfoLabelVisibleL( TBool aVisible );
    
private:

    /** Textual version of current search string. */
    TBuf<KEDMaxSearchStringLength> iSearchString;
    
    /** Handle to predictive search engine. Owned. */
    CPSRequestHandler* iPredictiveContactSearchHandler;
    
    /** PCS type of presentation of the current search string. Owned. */
    CPsQuery* iPredictiveSearchQuery;
    
    /** If ETrue, completing PCS searches are not displayed but just discarded. */
    TBool iDiscardCompletingSearches;
    
    /** ETrue if contact database has been changed and a new search is needed. */
    TBool iNewSearchNeeded;
    
    /** Array of used data stores. Owned. */
    RPointerArray<TDesC> iContactDataStores;
    
    /** Contact store array. Owned. */
    CVPbkContactStoreUriArray* iContactStoreUriArray;
    
    /** Contact manager handle. Owned. */
    CVPbkContactManager* iContactManager;
	
    /** Search result field index for first name field. */
    TInt iFirstNamePCSIndex;
    
    /** Search result field index for last name field. */
    TInt iLastNamePCSIndex;
    
    /** Search result field index for first name field. */
    TInt iCompanyNamePCSIndex;
    
    /** Array containing observers to this class. Owned. */
    RPointerArray<MDialingExtensionObserver> iObservers;
    
    /** Contact listbox. Owned. */
    CEasyDialingListBox* iContactListBox;
    
    /** Listbox model from search results. Owned. */
    CDesCArrayFlat* iListBoxModel;
    
    /** Number of found matching contacts from most recent search. */
    TInt iNumberOfNames;
    
    /** Handle to contact launcher. */
    MCCAConnectionExt* iContactLauncher;
    
    /** ETrue if contact launcher (a.k.a. communication launcher) is open. */
    TBool iContactLauncherActive;
    
    /** If true, contact listbox must not reset focus when contact launcher exits. */
    TBool iRememberFocus;
    
    /** Working buffer for creating contact string to listbox. */
    TBuf<KEDMaxContactStringLength> iContactStringCreationBuffer;
    
    /** Central repository listener. Owned. */
    CEasyDialingCenrepListener* iCenrepListener;
    
    /** Contact data manager. Owned. */
    CEasyDialingContactDataManager* iContactDataManager;
    
    /** Easy dialing contactor service. Owned. */
    CEDContactorService* iContactorService;
    
    /** Easy dialing resource file offset in current application. */
    TInt iResourceFileOffset;
    
    /** Blocks user input when action is being launched. NULL if not active. Owned. */
    CAknInputBlock* iInputBlocker;
    
    /** Used to launch action asynchronously. Owned.*/
    CAsyncCallBack* iAsyncCallBack;
    
    /** Action to be launched next asynchronously. */
    TEasyDialingAction iActionToBeLaunched;
    
    /** Contact link related to the asynchronous action. */
    HBufC8* iContactToBeLaunched;
    
    /** Contact name related to the asynchronous action. */
    HBufC* iContactToBeLaunchedName;
    
    /** Key event to be simulated asynchronously. */
    TKeyEvent iKeyEventToSimulate;
    
    /** Is virtual keyboard currently open or not.*/
    TBool iVirtualKeyboardOpen;

    /** First line of info text shown when number entry is empty. Owned. */
    HBufC* iInfoLabelTextLine1;
    
    /** Second line of info text shown when number entry is empty. Owned. */
    HBufC* iInfoLabelTextLine2;
    
    /** Label for showing first line of info text when number entry is empty. Owned. */
    CEikLabel* iInfoLabelLine1;
    
    /** Label for showing second line of info text when number entry is empty. Owned. */
    CEikLabel* iInfoLabelLine2;
};


#endif //__CEASYDIALINGPLUGIN_H__

// End of File
