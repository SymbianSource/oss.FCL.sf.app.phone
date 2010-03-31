/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  PhoneUI Menu Controller
*
*/


#ifndef CPHONEMENUCONTROLLER_H
#define CPHONEMENUCONTROLLER_H

//  INCLUDES


#include <e32base.h>
#include <e32std.h>
#include <eikmenub.h> 
#include <e32property.h>
#include "mphonepubsubobserver.h"

// FORWARD DECLARATION
class TPhoneCommandParam;
class CEikonEnv;
class MPhonePubSubObserver;
class CAiwServiceHandler;

class CPhoneMenuController : public CBase, public MPhonePubSubObserver
    {
    public:

        /**
        * Two-phased constructor.
        */
        static CPhoneMenuController* NewL( CEikonEnv& aEikEnv );
        
        /**
        * Destructor.
        */
        virtual ~CPhoneMenuController();   

        void OpenMenuBarL();

        void CloseMenuBar();
        
        void OpenMenuBarL( TPhoneCommandParam* aCommandParam );

        void DynInitMenuBar( TPhoneCommandParam* aCommandParam );

        void DynInitMenuPaneL( TPhoneCommandParam* aCommandParam );
       
        void SetMuteFlag( TPhoneCommandParam* aCommandParam );

        void SetIhfFlag( TPhoneCommandParam* aCommandParam );
        
        void SetHoldFlag( TPhoneCommandParam* aCommandParam );
        
        void GetHoldFlag( TPhoneCommandParam* aCommandParam );
        
        void SetConferenceAndSingleFlag( TPhoneCommandParam* aCommandParam );
        
        void GetConferenceAndSingleFlag( TPhoneCommandParam* aCommandParam );
        
        void SetConferenceAndWaitingVideoFlag( TBool aIsConfAndWaitingVideo );
        
        void SetSoftRejectFlag( TPhoneCommandParam* aCommandParam );
        
        void GetSoftRejectFlag( TPhoneCommandParam* aCommandParam );

        void SetServiceCodeFlag( TPhoneCommandParam* aCommandParam );
        
        void SetWiredAccFlag( TPhoneCommandParam* aCommandParam );
        
        void SetMaxConfMembersFlag( TBool aIsMaxConfMember );
        
        void SetBTAccFlag( TPhoneCommandParam* aCommandParam );
        
        void SetBTAccAvailableFlag( TPhoneCommandParam* aCommandParam );
        
        void SetPhoneNumberAvailableInPhoneEngine( TPhoneCommandParam* aCommandParam );
        
        void SetWiredAccAvailableFlag( TPhoneCommandParam* aCommandParam );
        
        void SetNEVisibleFlag( TBool aVisible );

        void SetReplaceFlag( TBool aReplace );
        
        void SetRemoveConfrenceOptionsFlag( TBool aRemoveConfrenceOptions );
        
        void SetNumberEntryEmptyFlag( TBool aNumberEntryEmpty );
        
        void SetCallInProgressFlag( TPhoneCommandParam* aCommandParam );               
        
        TBool IsMenuBarVisible();
        
        void SetConferenceParticipantsListVisibleFlag( TPhoneCommandParam* aCommandParam );
        
        void GetConferenceParticipantsListVisibleFlag(
            TPhoneCommandParam* aCommandParam );

        void SetAlphaModeFlag( TBool aMode );
        
        void SetAllowDtmfOptionsFlag( TBool aAllowDtmfOptions );
        
        void SetVoipEnabledFlag( TPhoneCommandParam* aCommandParam );
        
        void SetHideVideoCallDTMFVisibilityFlag ( TBool aHideVideoCallDTMFVisibility );

		void SendAiwCommandL( TPhoneCommandParam* aCommandParam );      
 
		void SetContextMenu( TPhoneCommandParam* aCommandParam );

        void SetInvalidCsPhoneNumberFlag( TPhoneCommandParam* aCommandParam );
	        
        void GetInvalidCsPhoneNumberFlag( TPhoneCommandParam* aCommandParam );
		
		void SetSecurityMode( TBool aMode );

    public: //from base class 
        
         void HandlePropertyChangedL( 
            const TUid& aCategory,
            const TUint aKey,
            const TInt aValue );
		
    protected:    // Constructors
        
        /**
        * C++ default constructor.
        */
        CPhoneMenuController( CEikonEnv& aEikEnv );
       

    private:

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        void AddMenuPaneL(
            TInt aPaneResourceId,
            CEikMenuPane* aMenuPane,
            TBool aAddSeparator );
            
        /**
        * Checks if task swapper is to be shown in menu.
        * @param aResourceId It is the context menu resource.
        */
        void ShowTaskSwapper( TInt aResourceId ) const;
        
        TBool IsSwivelClosed() const;
    
    private:
        CEikMenuBar* iMenu;
        TInt iGSMMenuResourceOffset;

        TInt iMenuResourceId;
        TInt iOkMenuResourceId;
        TBool iIhfFlag;
        TBool iMuteFlag;
        TBool iHoldFlag;
        TBool iConferenceAndSingleFlag;
        TBool iConferenceAndWaitingVideoFlag;
        TBool iSoftRejectFlag;
        TBool iServiceCodeFlag;
        TBool iWiredAccFlag;
        TBool iMaxConfMembersFlag;
        TBool iBTAccFlag;
        TBool iBTAccTmpUnFlag;
        TBool iBTAccAvailableFlag;
        TBool iPhoneNumberAvailableInPhoneEngine;
        TBool iWiredAccAvailableFlag;
        TBool iNEVisible;
        TBool iReplace;
        TBool iRemoveConfrenceOptionsFlag;
        TBool iNumberEntryEmpty;
        TBool iCallInProgress;
        TBool iConferenceParticipantsListOpenFlag;
        TBool iAlphaMode;
        TBool iAllowDtmfOptionsFlag;
        TBool iVoipEnabledFlag;  
        TBool iHideVideoCallDTMFVisibility;
        TBool iInvalidCsPhoneNumber;
		TBool iSecurityMode;
        // Eikon environment delivered from CPhoneViewController
        CEikonEnv& iEikEnv;       
        // Pointer to AIW service handler. OWNED.
        CAiwServiceHandler* iServiceHandler;       
    };

#endif      // MPAMENUBARCONTROLLER_H   
            
// End of File
