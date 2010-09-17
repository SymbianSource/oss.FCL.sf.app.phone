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
* Description:  A parameter class for setting call header information
*
*/


#ifndef PHONECALLHEADERUTIL_H
#define PHONECALLHEADERUTIL_H

//INCLUDES
#include    "mpeengineinfo.h"
#include    "tphonecmdparamcallheaderdata.h"

//FORWARD
class MPEEngineInfo;

/**
 * A parameter class for setting call header information.
 *
 *  @since S60 v10.1
 */
class PhoneCallHeaderUtil
    {
    public: // Constructors & destructors.
            
        /**
        * C++ constructor.
        */
        PhoneCallHeaderUtil( MPEEngineInfo& engineInfo );
        
        virtual ~PhoneCallHeaderUtil();
        
    public: // New functions.
        
        /**
        * Sets info to outgoing call header.
        * @param aCallId - call id.
        * @param aWaitingCall - if call is waiting value is ETrue otherwise EFalse.
        * @param aVideoCall - if call is video value is ETrue otherwise EFalse.
        * @param aCallHeaderData - Call header parameter into which the text/picture
        *                          parameters will be set.
        */           
        void SetIncomingCallHeaderParams(
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
        void SetOutgoingCallHeaderParams(
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
        void UpdateCallHeaderInfo( 
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
        TBool GetRemoteInfoData( 
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
        
        /**
        * Returns the call header label text
        * @return Returns the label
        */
        const TDesC& LabelText( TInt aCallId );

        const TDesC& EmergencyHeaderText();
        
        const TDesC& AttemptingEmergencyText();
        
        /**
        * Returns the call header call state.
        * @return Returns the call state
        */
        TInt CallState( TInt aCallId ) const;
        
        void LoadResource( TDes& aData, const TInt aResource ) const;
                
        HBufC* LoadResource( const TInt aResource ) const;
        
    private:
        
        
        
        /**
        * Set basic info to call header.
        * @param aCallId - call id.
        * @param aCallHeaderData - Call header parameter into which the text/picture
        *                          parameters will be set.
        */
        void SetBasicCallHeaderParams(
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
        void SetCliAndCnapParamaters( 
                const TInt aCallId, 
                TPhoneCmdParamCallHeaderData* aCallHeaderData  );
        /**
        * Updates cli parameter to call header.
        * @param aCallId - Call Id.
        * @param aCallHeaderData - Call header parameter where modifications
        *                          are made.
        */
        void SetCliParamaters(
                const TInt aCallId, 
                TPhoneCmdParamCallHeaderData* aCallHeaderData );
        
        void LoadCallHeaderTexts(
            const TInt aLabelId, 
            TPhoneCmdParamCallHeaderData* aCallHeaderData );
        
        TBool IsVideoCall(int aCallId ) const;
        
    private:
        
        MPEEngineInfo& m_engineInfo;
        TInt iCallHeaderType;
        TBool iSetDivertIndication;
        HBufC *iLabelText;
        HBufC *iEmergencyHeaderText;
        HBufC *iAttemptingEmergencyText;
            
    };

#endif // PHONECALLHEADERUTIL_H
