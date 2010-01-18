/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declares a container control for the application.
*
*/


#ifndef VM_CONTAINER_H
#define VM_CONTAINER_H

// INCLUDES
#include <e32std.h>
#include <bldvariant.hrh>
#include <coecntrl.h>
#include <badesca.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>
#include <AknIconArray.h> //for sim icon
#include <aiwdialdata.h>
#include <vmnumber.h>
#include "VmAppUi.h"

// CONSTANTS
enum TVmbxIndicators
    {
    EVmbxVoiceMessageAmountInd = 0,
    EVmbxVoiceMailboxNumberInd,
    EVmbxDetailsOfLastMessageInd,
    EVmbxVideoMailboxNumberInd,
    EVmbxIntCallVoiceMailboxNumberInd
    };

// FORWARD DECLARATIONS
class CAknDoubleGraphicStyleListBox;
class CPhCltExtPhoneBase;
class CAiwServiceHandler;
class CPNGNumberGrouping;
class CAknsBasicBackgroundControlContext;
class CCaUiEngApi;
class CSPSettings;

// CLASS DECLARATION

/**
*  CVmContainer, container control class.
*
*  @lib ?library
*  @since ?Series60_version
*/
class CVmContainer : public CCoeControl,
                     public MVmbxNotifyHandler,
                     public MVmbxAlsObserver,
                     public MCenRepNotifyHandlerCallback,
                     public MEikListBoxObserver,
                     public MServiceNotifyHandler
                     
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CVmContainer();

        /**
        * Symbian OS default constructor.
        * @param aRect Frame rectangle for container.
        * @param aServiceHandler AIW service handler.
        */
        void ConstructL( const TRect& aRect,
                                        CAiwServiceHandler* aServiceHandler );

        /**
        * Destructor.
        */
        ~CVmContainer();

    public:  // New functions
        /**
        * Query for a new voice mailbox number. Calls method of vmbx.dll.
        * A different query is created depending on whether the vmbx number
        * is defined before calling this.
        *
        * @param aQueryType query type ( 2 alternatives )
        */
        void QueryNumberL( TVmbxQueryType aQueryType );

        /**
        * Gets the vmbx number from SIM/SharedData and updates it
        * in the display.
        */
        void UpdateVmbxNumberL();

        /**
        * Check wether is possible to edit the voice mailbox number.
        * @since 2.7
        * @return ETrue is editing is allowed.
        */
        TBool AllowedToChangeTheNumber();

        /**
        * Resets to zero the voicemail message counter in SD.
        * @since 2.7
        */
        void ResetVoicemailMessageCounterL();

        /**
        * Check if VoIP profiles is found.
        * @since 3.0
        * @return ETrue if there is at least one VoIP profile, EFalse otherwise
        */
        TBool IsVoIPProfilesFound();

        /**
        * Gets index of the current item in listbox
        * @return index of current item
        */
        TInt CurrentItem();


        /**
        * Checks if clear icon is set
        * @return ETrue if clear icon
        */
        TBool IsClearIcon();

        /**
        * Calls the vmbx number using PhoneClient.
        */
        void CallVoiceMailBoxL();

        /**
        * Calls the VoIP vmbx number using PhoneClient.
        */
        void CallIntVoiceMailBoxL();

        /**
        * Makes a call request to AiwProvider.
        * @aCommand AiwCommand id.
        * @aCallType voice or Video call.
        * @return KErrNone if successful.
        **/
        void AiwCallRequestL( TInt aCommand, CAiwDialData::TCallType aCallType );

        /**
        * Ask Vmbx type.
        * @since 3.0
        * @return returns type of Vmbx.
        */
        TInt AskTypeL();

        /**
        * Calls the vmbx number using PhoneClient.
        * @since 3.1
        */
        void CallVideoMailBoxL();

        /**
        * Constructs the icons that are needed to indicate where
        * the vmbx number is stored
        * @since 3.0
        */
        void ConstructIconsForListboxL();

        /**
        * Query for a new video mailbox number. Calls method of vmbx.dll.
        * A different query is created depending on whether the video mailbox
        * number is defined before calling this.
        *
        * @param aQueryType query type ( 2 alternatives )
        * @since 3.1
        */
        void QueryVideoNumberL( TVmbxQueryType aQueryType );

        /**
        * Query for a voice mailbox number.
        *
        * @return Voice mailbox number
        * @since 3.1
        */ const TDesC& Number() const;

        /**
        * Query for a video mailbox number.
        *
        * @return Video mailbox number
        * @since 3.1
        */
        const TDesC& VideoNumber() const;

        /**
        * Informs that listbox must be updated because returned to Vmbx
        * from other application
        *
        * @since
        */
        void UpdateData( TBool aState );

        /**
         * Checks if there are configured IP voicemail services
         * @since 3.2
         * @return ETrue if at least one IP voicemail service was found
         */
        TBool IsIpVoiceMailboxServices();

        /**
         * Get Service Id.
         * @since 3.2
         * @return TUint Service Id.
         */
        TUint ServiceId();

        /**
         * Informs that listbox must be updated immediately.
         * @since 3.2
         */
        void UpdateListboxL();

        /**
         * Add brand icon if it exist.
         * @since 3.2
         * @param aServiceId Used Service Id
         * @param aIconId On completion, contains branded icon's icon list id
         */
        void AddBrandIconL( TUint aServiceId, TInt& aIconId );

        /**
         * Is service updated.
         * @since 3.2
         * @return TBool ETrue if service has updated, otherwise EFalse.
         */
        TBool IsServiceUpdated() const;

    public: // Functions from base classes

        /**
        * From MEikListBoxObserver, Handles list box events.
        * @param aEventType The event identifier.
        */
        void HandleListBoxEventL( CEikListBox* /*aListBox*/,
            					  TListBoxEvent aEventType );
        /**
         * Check if Video telphony is activated at FeatureManager
         * @since 3.2
         * @return Value of VT support
         */
        TBool IsVideoSupported();

    private:  // Functions from base classes

        /**
        * From CCoeControl, responds to focus changes
        * @since 3.0
        * @param aDrawNow ?description
        */
        void FocusChanged( TDrawNow aDrawNow );

        /**
        * From CoeControl, responds to size changes to sets the size and
        * position of the contents of this control
        */
        void SizeChanged();

        /**
        * From CoeControl, gets the number of controls contained in
        * a compound control
        */
        TInt CountComponentControls() const;

        /**
        * From CCoeControl, gets the specified component of a compound control
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
        * From CCoeControl, handles key events
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                                 TEventCode aType );

        /**
        * From CCoeControl, handles a change to the control's resources
        */
        void HandleResourceChange( TInt aType );

        /**
        * From CCoeControl, draw a control - called by window server
        */
        void Draw( const TRect& aRect ) const;


        /**
        * From MVmbxNotifyHandler, user has completed an action
        * to a vmbx number
        */
        void HandleNotifyL( TInt aLine, const TDesC& aValue );

        /**
        * From MVmbxAlsObserver, ALS line changed
        */
        void HandleAlsChangeL( TVmbxAlsLine aLine );

        /**
        * From MCenRepNotifyHandlerCallback, user has pressed integer value keys
        **/
        void HandleNotifyInt( TUint32 aKey, TInt aValue );

        /**
        * From MServiceNotifyHandler user has changed Services.
        * @since S60 v3.2
        */
        void HandleServiceNotifyL();

    private:   // New functions

        /**
        * Updates the vmbx number in the listbox shown on the app screen.
        */
        void UpdateVmbxNumberInListBoxL();

        /**
        * Updates the new voice message to the listbox
        */
        void UpdateNewVoiceMessagesInListBoxL();

        /**
        * Updates the last message details to the listbox
        */
        void UpdateLastMessageDetailsInListBoxL();

        /**
        * Fill array with indications
        */
        void FillIndArray();

    private:  // data

        // Video line 1
        HBufC* iVideoLine1;  // owned

        // Video line 2
        HBufC* iVideoLine2;  // owned

        // Video number
        TBuf<KVmbxMaxNumberLength> iVideoNumber;


        // Handler of the Application Interworking
        CAiwServiceHandler* iServiceHandler;

        //Listbox text lines
        HBufC* iLine1;  // owned
        HBufC* iLine2;  // owned
        HBufC* iLine3;  // owned
        HBufC* iLine4;  // owned

        // Vmbx number handler
        RVmbxNumber iVmbxNumber;

        // Array for the listbox texts
        CDesCArrayFlat iArray;  // owned

        // Vmbx listbox
        CAknDoubleGraphicStyleListBox* iListBox;  // owned


        // Mailbox number
        TBuf<KVmbxMaxNumberLength> iNumber;

        // Phone groups
        CPNGNumberGrouping* iNumberGrouping;  // owned

        // Central repository handlers
        CCenRepNotifyHandler* iNotifyHandler1;
        CCenRepNotifyHandler* iNotifyHandler2;
        CCenRepNotifyHandler* iNotifyHandler3;

        // Central repository session
        CRepository* iSession;  // owned

        // Array for indications in listbox
        RArray<TInt> iIndArray;

        //PRI Settings
        TInt iVmUiOpts;

        // Icon array
        CAknIconArray* iListIcons; //not own.. list box will delete this one

        // skin data
        CAknsBasicBackgroundControlContext* iSkinContext; // owned

        // Mailbox type
        TInt iType;

        // Update data when back to application
        TBool iUpdateData;

        // CenRep key for video support
        TBool iIsVideoSupported;

        // Set to true if call is ongoing
        TBool iIsCallOngoing;

        /**
         * Instance of service settings provider.
         * Used especially to retrieve VoIP specific info.
         * Own.
         */
        CSPSettings* iServiceProviderSettings;

        TBool iServiceUpdated;

        TBool iFeatureManagerInitialized;

    };

#endif  // VM_CONTAINER_H

// End of File
