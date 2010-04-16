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
* Description:  Bubble manager wrapper.
*
*/


#ifndef CPHONECALLHEADERMANAGER_H
#define CPHONECALLHEADERMANAGER_H

//INCLUDES
#include <e32base.h>
#include "mphonecallheadermanagerutility.h"
#include "mphonestatemachine.h"
#include "mphoneviewcommandhandle.h"
#include "mphonecustomization.h"

class TPhoneCmdParamCallHeaderData;
class TPhoneCallHeaderParam;

/**
 * Class contains helper functions for bubble manager.
 *
 * @lib phoneuicontrol
 * @since S60 v9.1
 */
NONSHARABLE_CLASS( CPhoneCallHeaderManager ) :  public CBase,
                                                public MPhoneCallHeaderManagerUtility
    {
    public:
        
        /**
        * Creates new instance of CPhoneCallHeaderManager
        * @param aStateMachine: a state machine
        * @param aViewCommandHandle: handle to the PhoneUIView
        * @param aCustomization: handle to customization.
        * @return CPhoneCallHeaderManager* object  
        */
        static CPhoneCallHeaderManager* NewL(
                    MPhoneViewCommandHandle& aViewCommandHandle,
                    MPhoneStateMachine& aStateMachine,
                    MPhoneCustomization* aCustomization );
        /**
        * Destructor.
        */
        virtual ~CPhoneCallHeaderManager();
        
    public:
        
        /**
        * Returns updated remote information data.
        * @param aCallId - Call Id.
        */
        TPhoneCmdParamCallHeaderData UpdateCallHeaderInfoL( 
                const TInt aCallId );
        
        void GetRemoteInfoDataL(  const TInt aCallId, TDes& aData ) const;
       
        /**
        * Display call header for call coming in ( the remote info data 
        * and picture is displayed immediately )
        * @param aCallid call id
        * @param aWaitingCall waiting call indication
        */
        void DisplayHeaderForCallComingInL(
                const TInt aCallId, 
                const TBool aWaitingCall );
       
        /**
        * Display call header for outgoing call (the phone number is initially
        * displayed)
        * @param aCallid call id
        */
        void DisplayHeaderForOutgoingCallL(
                const TInt aCallId );

        /**
        * Sets the call header type used in the call bubble.
        * @param aCallHeaderType, call header type.
        */
        void SetCallHeaderType( 
                TInt aCallHeaderType );
        
        /**
        * Returns the set call header type.
        */
        TInt CallHeaderType() const;
                
        /**
        * Sets divert indication to call header if necessary
        * @param aCallId - Call Id.
        * @param aCallHeaderData - Call header where indication will be set
        *                          if needed.
        */
        void SetDivertIndication( const TBool aDivertIndication );
        
        /**
        * Checks if call is waiting, returns ETrue if call is waiting
        * otherwise EFalse.
        * @param aCallId, call id.
        */
        TBool IsWaitingCallL( const TInt aCallId ) const;
        
        /**
        * Checks if call is video, returns ETrue if call is video
        * otherwise EFalse.
        * @param aCallId, call id.
        */
        TBool IsVideoCall( const TInt aCallId ) const;
 
        /**
        * Sets texts to call header
        * @param aCallId - Call Id.
        * @param aWaitingCall waiting call indication
        * @param aCallHeaderData - Call header where indication will be set
        *                          if needed.
        */         
        void SetCallHeaderTextsForCallComingIn(
                const TInt aCallId, 
                const TBool aWaitingCall,
                TPhoneCmdParamCallHeaderData* aCallHeaderData );
        
    public:  // Functions from base class
        
        /**
        * @see MPhoneBubbleManagerUtility.
        */
        void SetPhoneNumberAvailabilityL( 
                const TInt aNumberLength,
                const TBool aContactInfoAvailable );
        
        /**
        * @see MPhoneBubbleManagerUtility.
        */
        void GetInCallNumberTextL( TInt aCallId, TDes& aData ) const;
        
        /**
        * @see MPhoneBubbleManagerUtility.
        */
        void LoadCallHeaderTexts(
            const TInt aLabelId, 
            const TInt aShortLabelId,
            TPhoneCmdParamCallHeaderData* aCallHeaderData ); 
        
        /**
        * @see MPhoneBubbleManagerUtility.
        */
        MPhoneCustomization* Customization(){ return iCustomization; }
        
        /**
        * @see MPhoneBubbleManagerUtility.
        */
        void LoadResource( TDes& aData, const TInt aResource );
        
    private:
    
        /**
        * C++ default constructor.
        */
        CPhoneCallHeaderManager(
                MPhoneViewCommandHandle& aViewCommandHandle,
                MPhoneStateMachine& aStateMachine,
                MPhoneCustomization* aCustomization );
        
        /**
        * Symbian constructor
        */
        void ConstructL();
        
    private:
        
        MPhoneViewCommandHandle& iViewCommandHandle;
        MPhoneStateMachine& iStateMachine;
        MPhoneCustomization* iCustomization;
        TPhoneCallHeaderParam* iCallHeaderParam;
    };


#endif // CPHONECALLHEADERMANAGER_H
