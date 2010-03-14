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
* Description:  Launching Call.
*
*/


// INCLUDE FILES
#include "edcontactorheaders.h"

_LIT8( KAiwContentTypeAll, "*" );

/// Unnamed namespace for local definitions
namespace
	{

#ifdef _DEBUG
	enum TPanicCode
		{
		EPanicPreCond_ExecuteL = 1,
		EPanicPreCond_ExecuteL2
		};

	void Panic(TPanicCode aReason)
		{
		_LIT(KPanicText, "CCmsPhoneCall");
		User::Panic (KPanicText, aReason);
		}
#endif // _DEBUG


	} /// namespace

// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------
// CEDPhoneCall::ExecuteL
// ---------------------------------------------------------
void CEDPhoneCall::ExecuteL( const TDesC& aPhoneNumber, TEDPhoneCallType aCallType,
        TBool aForcedCallType, TUint32 aServiceId )
	{   
    __ASSERT_DEBUG( NULL != &aPhoneNumber && 0 < aPhoneNumber.Size(), Panic(EPanicPreCond_ExecuteL) );

#ifdef _DEBUG
	HBufC8* buf = HBufC8::NewLC( aPhoneNumber.Length() );
	buf->Des().Copy( aPhoneNumber );
	TPtrC8 numberDesc( buf->Des() );
	CleanupStack::PopAndDestroy( buf );
#endif

	//aPhoneNumber will be an xsp id of format "ServiceName:UserId"
	//here we need to truncate the servicename and pass only the UserId
	
    //Find if aPhoneNumber of type "ServiceName:UserId"
	//if true, then remove the service part
    TPtrC phoneNum( aPhoneNumber );
    TInt phoneNumberStart = phoneNum.Locate(':');
    
    if ( KErrNotFound != phoneNumberStart )
        {
        phoneNumberStart++; //To get rid of ':'
        phoneNum.Set( phoneNum.Mid( phoneNumberStart ) );
        }
    
    HBufC* numBuf = phoneNum.AllocLC();
    TPtr numDesc( numBuf->Des() );
	CommonPhoneParser::ParsePhoneNumber( numDesc,
			CommonPhoneParser::EPhoneClientNumber ); 

	CAiwDialDataExt* dialDataExt = CAiwDialDataExt::NewLC();
	dialDataExt->SetPhoneNumberL( *numBuf );
	switch (aCallType)
	    {
	    case ECCACallTypeVoice:
	        if ( aForcedCallType )
	            {
	            // Use this with Action menu's Voice call icon
	            dialDataExt->SetCallType( CAiwDialData::EAIWForcedCS );
	            }
	        else
	            {
	            // To be on the safe side, use this with Send key and big Call icon
	            dialDataExt->SetCallType( CAiwDialData::EAIWVoice );
	            }
	        break;
	    case ECCACallTypeVoIP:
	        dialDataExt->SetServiceId( aServiceId );
	        dialDataExt->SetCallType( CAiwDialData::EAIWVoiP );
	        break;
	    case ECCACallTypeVideo:
	        dialDataExt->SetCallType( CAiwDialData::EAIWForcedVideo );
	        break;
	    default:
	        dialDataExt->SetCallType( CAiwDialData::EAIWForcedCS );
	        break;
	    }

	dialDataExt->SetWindowGroup( CCoeEnv::Static()->RootWin().Identifier() );

	DoAIWCallL( *dialDataExt );

	CleanupStack::PopAndDestroy( dialDataExt );
	CleanupStack::PopAndDestroy( numBuf );
	}

// ---------------------------------------------------------
// CEDPhoneCall::DoAIWCallL
// ---------------------------------------------------------
void CEDPhoneCall::DoAIWCallL( CAiwDialDataExt& aDialData )
	{
	RCriteriaArray interest;
	CleanupClosePushL( interest );
	CAiwCriteriaItem* criteria = CAiwCriteriaItem::NewLC( KAiwCmdCall,
			KAiwCmdCall, KAiwContentTypeAll );
	criteria->SetServiceClass( TUid::Uid(KAiwClassBase) );
	User::LeaveIfError( interest.Append( criteria ) );

	CAiwServiceHandler* serviceHandler = CAiwServiceHandler::NewLC();
	serviceHandler->AttachL( interest );

	CAiwGenericParamList& paramList = serviceHandler->InParamListL();
	aDialData.FillInParamListL( paramList );

	serviceHandler->ExecuteServiceCmdL( KAiwCmdCall, paramList,
			serviceHandler->OutParamListL(), 0, NULL );
	serviceHandler->DetachL( interest );

	CleanupStack::PopAndDestroy( serviceHandler );
	CleanupStack::PopAndDestroy( criteria );
	CleanupStack::PopAndDestroy( &interest );
	}

//  End of File  
