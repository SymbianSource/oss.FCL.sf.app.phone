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
* Description:  Launching Editors.
*
*/


// INCLUDE FILES
#include "edcontactorheaders.h"
#include <SendUiConsts.h>

#ifndef KSenduiMtmOnlineAlbumUidValue
#define KSenduiMtmOnlineAlbumUidValue       0x200009D5
#endif

/// Unnamed namespace for local definitions
namespace
	{

#ifdef _DEBUG
	enum TPanicCode
		{
		EPanicPreCond_ExecuteL = 1
		};

	void Panic(TPanicCode aReason)
		{
		_LIT(KPanicText, "CCmsMsgEditors");
		User::Panic (KPanicText, aReason);
		}
#endif // _DEBUG


	} /// namespace


// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------
// CEDMsgEditors::ExecuteL
// ---------------------------------------------------------
void CEDMsgEditors::ExecuteL( const TUid aServiceUid,
		const TDesC& aAddress, const TDesC& aName )
	{
    __ASSERT_DEBUG (NULL != &aAddress && 0 < aAddress.Size() &&
                        NULL != &aName && 0 < aName.Size() && 
                        NULL != &aServiceUid, Panic (EPanicPreCond_ExecuteL));

	//ParsePhoneNumber may only short length of phonenumber, 
	//so it's safe to use same lenght as given number and do the copy.
	HBufC* numBuf = aAddress.AllocLC();
	TPtr numDesc( numBuf->Des() );

	//Unieditor could also use email adresses. If parser returns EFalse, do not care. 
	if ( KSenduiMtmUniMessageUid == aServiceUid)
		{
		CommonPhoneParser::ParsePhoneNumber( numDesc,
				CommonPhoneParser::EPhoneClientNumber );
		}

	CSendUi* sendui = CSendUi::NewLC ();
	CMessageData* msgdata = CMessageData::NewLC ();
	msgdata->AppendToAddressL( *numBuf, aName);
	
	if (KSenduiMtmSmtpUid == aServiceUid)
	    {
	    const TUid KMfEUidValue = { 0x1020695B }; //Where is this defined?
	    TBool mailAvailabe = EFalse;
	    TBool mfeAvailable = EFalse;
	    TUid selectedServiceID = KNullUid;
	    RPointerArray<CSendingServiceInfo> availableServices;
	    sendui->AvailableServicesL(availableServices, KGenericMtmPlugin);
	    for (TInt i=0; i < availableServices.Count(); i++)
	        {
	        if (!(availableServices[i]->ServiceFeatures() & CSendingServiceInfo::EServiceInValid))
	            {
    	        if (availableServices[i]->ServiceId() == KSenduiMtmSmtpUid)
    	            {
                    mailAvailabe = ETrue;
    	            }
    	        else if (availableServices[i]->ServiceId() ==  KMfEUidValue)
    	            {
    	            selectedServiceID = availableServices[i]->ServiceId();
    	            mfeAvailable = ETrue;
    	            }
	            }
	        }
	    availableServices.ResetAndDestroy();
	    	    
	    if (mailAvailabe && mfeAvailable)
	        {
    	    CArrayFixFlat<TUid>* servicesToDim = new (ELeave) CArrayFixFlat<TUid>(1);
    	    CleanupStack::PushL(servicesToDim);
    	    
    	    servicesToDim->AppendL(KSenduiTechnologySmsUid);
    	    servicesToDim->AppendL(KSenduiTechnologyMmsUid);
    	    //servicesToDim->AppendL(KSenduiTechnologyMailUid); Email is not to be dimmed
    	    servicesToDim->AppendL(KSenduiTechnologyUniEditorUid);
    	    servicesToDim->AppendL(KSenduiTechnologyIrUid);
    	    servicesToDim->AppendL(KSenduiTechnologyBtUid);
    	    servicesToDim->AppendL(KSenduiTechnologyIMUid);	    
    	    servicesToDim->AppendL(KMmsDirectUpload);
    	    servicesToDim->AppendL(KMmsIndirectUpload);
    	    servicesToDim->AppendL(KMmsUploadService1Id);
    	    servicesToDim->AppendL(KMmsUploadService2Id);
    	    servicesToDim->AppendL(KMmsUploadService3Id);
    	    servicesToDim->AppendL(KMmsUploadService4Id);
    	    const TUid KShareOnline = { KSenduiMtmOnlineAlbumUidValue }; // From SendUiInternalConsts.h
    	    servicesToDim->AppendL(KShareOnline);
    	    
    	    sendui->ShowQueryAndSendL(msgdata, KCapabilitiesForAllServices, servicesToDim);
    	    servicesToDim->Reset();
    	    CleanupStack::PopAndDestroy(servicesToDim);
	        }
	    else if (mfeAvailable)
	        {
	        sendui->CreateAndSendMessageL(selectedServiceID, msgdata, KNullUid, EFalse);
	        }
	    else
	        {
	        sendui->CreateAndSendMessageL(aServiceUid, msgdata, KNullUid, EFalse);
	        }
	    }
	else
	    {
	    sendui->CreateAndSendMessageL(aServiceUid, msgdata, KNullUid, EFalse);
	    }

	CleanupStack::PopAndDestroy( 3, numBuf ); //msgdata, sendui

	return;
	}
//  End of File  
