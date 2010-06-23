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
* Description:  A parameter class for setting call header information
*
*/


#ifndef TPHONECALLHEADERPARAM_H
#define TPHONECALLHEADERPARAM_H

//INCLUDES
#include    "mpeengineinfo.h"
#include    "tphonecmdparamcallheaderdata.h"

//FORWARD
class MPhoneStateMachine;
class MPhoneCallHeaderManagerUtility;
class TPhoneCmdParamCallHeaderData;

/**
 * A parameter class for setting call header information.
 *
 *  @since S60 v9.1
 */
class TPhoneCallHeaderParam
    {
    public: // Constructors & destructors.
            
        /**
        * C++ constructor.
        */
        TPhoneCallHeaderParam( 
                MPhoneCallHeaderManagerUtility& aManagerUtility,
                MPhoneStateMachine& aStateMachine );
        
    public: // New functions.
        
        /**
        * Sets info to outgoing call header.
        * @param aCallId - call id.
        * @param aWaitingCall - if call is waiting value is ETrue otherwise EFalse.
        * @param aVideoCall - if call is video value is ETrue otherwise EFalse.
        * @param aCallHeaderData - Call header parameter into which the text/picture
        *                          parameters will be set.
        */           
        void SetIncomingCallHeaderParamsL(
                const TInt aCallId, 
                const TBool aWaitingCall,
                const TBool aVideoCall,
                TPhoneCmdParamCallHeaderData* aCallHeaderData );
        /**
        * Sets info to outgoing call header.
        * @param aCallId - call id.
        * @param aCallHeaderData - Call header parameter into which the text/picture
        *                          parameters will be set.
        */
        void SetOutgoingCallHeaderParamsL(
                const TInt aCallId,
                TPhoneCmdParamCallHeaderData* aCallHeaderData );
        /**
        * Updates call header info. 
        * NOTE: This method is used when state receives
        * EPEMessageRemotePartyInfoChanged from PhoneEngine.
        * 
        * @param aCallId - call id.
        * @param aWaitingCall - if call is waiting value is ETrue otherwise EFalse.
        * @param aVideoCall - if call is video value is ETrue otherwise EFalse.
        * @param aCallHeaderData - Call header parameter into which the text/picture
        *                          parameters will be set.
        */             
        void UpdateCallHeaderInfoL( 
                const TInt aCallId,
                const TBool aWaitingCall,
                const TBool aVideoCall,
                TPhoneCmdParamCallHeaderData* aCallHeaderData );

        /**
        * Sets the call header type used in the call bubble.
        * @param aCallHeaderType, call header type.
        */    
        void SetCallHeaderType( 
                TInt aCallHeaderType );
        /**
        * Returns the set call header type. Used for constructing right type
        * of call bubble.
        */
        TInt CallHeaderType() const;
            
        /**
        * Setter for divert indication showing in bubble.
        * @param aDivertIndication ETrue to show divert indication,
        *        EFalse to not. Usually setting EFalse isn't necessary
        *        as it's a default value in bubble creation.
        */
        void SetDivertIndication( const TBool aDivertIndication );
        
        /**
        * Return remote info data
        * @return True if secondary CLI is used
        * @param aCallid call id
        * @param aData the returned remote info data
        */
        TBool GetRemoteInfoDataL( 
                const TInt aCallId,
                TDes& aData ) const;           
    
        /**
        * Sets texts for voice call header.
        * @param aCallId - Call Id.
        * @param aWaitingCall - Waiting call indication.
        * @param aCallHeaderData - Call header parameter into which the text
        *                          will be set.
        */
        void SetCallHeaderTexts( 
            const TInt aCallId, 
            const TBool aWaitingCall,
            const TBool aVideoCall,
            TPhoneCmdParamCallHeaderData* aCallHeaderData );

    private:
        
        
        
        /**
        * Set basic info to call header.
        * @param aCallId - call id.
        * @param aCallHeaderData - Call header parameter into which the text/picture
        *                          parameters will be set.
        */
        void SetBasicCallHeaderParamsL(
            const TInt aCallId, 
            TPhoneCmdParamCallHeaderData* aCallHeaderData );
        
        /**
        * Sets divert indication to call header if necessary
        * @param aCallId - Call Id.
        * @param aCallHeaderData - Call header where indication will be set
        *                          if needed.
        */
        void SetDivertIndicatorToCallHeader( 
                const TInt aCallId, 
                TPhoneCmdParamCallHeaderData* aCallHeaderData );
        
        /**
        * Sets divert indication to call header if necessary
        * @param aFeatureKey - feature key id Id.
        * @param aCallId - Call Id.
        */
        TBool IsFeatureSupported( 
                const TInt aFeatureKey, 
                const TInt aCallId ) const;
        
        /**
        * Return CNAP (Calling Name Presentation) text
        * @param aCallid call id
        * @param aData the returned CNAP text
        * @param aDirection the returned CNAP clipping direction
        */
        void GetCNAPText( 
               const TInt aCallId,
               TDes& aData, 
               TPhoneCmdParamCallHeaderData::TPhoneTextClippingDirection& aDirection ) const;
        
        /**
        * Check if contact is available(RemoteName or RemoteCompanyName),
        * if available ETrue is returned otherwise EFalse.
        * @param aCallid call id
        */
        TBool ContactInfoAvailable( 
                const TInt aCallId ) const;
        
        /**
        * Returns call type and stores type to member variable.
        * @param aCallId - Call Id.
        * @param aCallHeaderData - Call header data.
        */
        TPECallType GetCallType( 
                const TInt aCallId,
                TPhoneCmdParamCallHeaderData* aCallHeaderData ); 
        
        /**
        * Sets caller image. 
        * @param aCallId - Call Id.
        * @param aCallHeaderData - Call header parameter where modifications
        *                          are made.
        */
        void SetCallerImage( 
                const TInt aCallId, 
                TPhoneCmdParamCallHeaderData* aCallHeaderData );
        
        /**
        * Updates cli and cnap parameters to call header.
        * @param aCallId - Call Id.
        * @param aCallHeaderData - Call header parameter where modifications
        *                          are made.
        */
        void SetCliAndCnapParamatersL( 
                const TInt aCallId, 
                TPhoneCmdParamCallHeaderData* aCallHeaderData  );
        /**
        * Updates cli parameter to call header.
        * @param aCallId - Call Id.
        * @param aCallHeaderData - Call header parameter where modifications
        *                          are made.
        */
        void SetCliParamatersL(
                const TInt aCallId, 
                TPhoneCmdParamCallHeaderData* aCallHeaderData );  
        
        /**
        * Concludes does the engine have information of the
        * phone number. This information is then send to the view.
        * @param aNumberLength - phone number's length
        * @param aContactInfoAvailable - is contact's info available
        */
        void SetPhoneNumberAvailabilityL( 
                const TInt aNumberLength,
                const TBool aContactInfoAvailable );
                
    private:
        
        MPhoneCallHeaderManagerUtility& iManagerUtility;
        MPhoneStateMachine& iStateMachine;
        TInt iCallHeaderType;
        TBool iSetDivertIndication;
            
    };

#endif // TPHONECALLHEADERPARAM_H
