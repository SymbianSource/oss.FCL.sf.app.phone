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
* Description:  Class for handling popup query.
*
*/


#include <e32std.h>
#include <s32mem.h>
#include <aknappui.h>
#include <coemain.h> 
#include <phoneappcommands.hrh>

#include "edcontactorserviceheaders.h"

#include <edcontactor.rsg>

// ================= MEMBER FUNCTIONS =======================
//

// --------------------------------------------------------------------------
// CEDContactorPopupHandler::NewL
// --------------------------------------------------------------------------
//
CEDContactorPopupHandler* CEDContactorPopupHandler::NewL()
    {
    CEDContactorPopupHandler* self = new (ELeave) CEDContactorPopupHandler();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// --------------------------------------------------------------------------
// CEDContactorPopupHandler::~CEDContactorPopupHandler
// --------------------------------------------------------------------------
//
CEDContactorPopupHandler::~CEDContactorPopupHandler()
    {
    LOGSTRING("CEDContactorPopupHandler::~CEDContactorPopupHandler");
    delete iServiceHandler;
    delete iSchedulerWait;
    iCoeEnv.DeleteResourceFile (iResourceOffset);
    delete iQueryResults;
    }

// --------------------------------------------------------------------------
// CEDContactorPopupHandler::LaunchPopupL
// --------------------------------------------------------------------------
//
TDesC& CEDContactorPopupHandler::LaunchPopupL(
    const TDesC8& aContactLinkArray,
    TAiwCommAddressSelectType aCommAddressSelectType,
    TBool aUseDefaults )
    {
    LOGSTRING("CEDContactorPopupHandler::LaunchPopupL");

    __ASSERT_ALWAYS (NULL != &aContactLinkArray, User::Leave (KErrArgument));
    __ASSERT_ALWAYS (0 < aContactLinkArray.Size(), User::Leave (KErrArgument));

    TUint fetchFlags = 0;

    // Stop scheduler just in case this is called while operation is still ongoing.
    AsyncStop();
    
    // Set service data
    TAiwSingleItemSelectionDataV3 data;

    if (!aUseDefaults)
        {
        fetchFlags |= ::EDoNotUseDefaultField;
        }

    data.SetFlags(fetchFlags);
    data.SetCommAddressSelectType(aCommAddressSelectType);
    CAiwGenericParamList& inParamList = iServiceHandler->InParamListL();
    inParamList.AppendL(TAiwGenericParam(EGenericParamContactSelectionData,
        TAiwVariant(TAiwSingleItemSelectionDataV3Pckg(data))));

    // Set contactlink
    inParamList.AppendL(TAiwGenericParam(EGenericParamContactLinkArray,
        TAiwVariant(aContactLinkArray)));

    iServiceHandler->ExecuteServiceCmdL(KAiwCmdSelect, inParamList,
        iServiceHandler->OutParamListL(), 0, this);

    // Waiting for popup selection.
    // Make this whole operation look synchronous
    CAknAppUi* appUi = static_cast<CAknAppUi*>( iCoeEnv.AppUi() );
    appUi->HandleCommandL( EPhoneCmdBlockingDialogLaunched );
    iSchedulerWait->Start();
    appUi->HandleCommandL( EPhoneCmdBlockingDialogClosed );
    
    User::LeaveIfError(iError);

    LOGSTRING("CEDContactorPopupHandler::LaunchPopupL: Done.");

    return *iQueryResults;
    }

// --------------------------------------------------------------------------
// CEDContactorPopupHandler::ClosePopupL
// --------------------------------------------------------------------------
//
void CEDContactorPopupHandler::ClosePopupL()
    {
    if ( iSchedulerWait->IsStarted() )
        {
        iServiceHandler->ExecuteServiceCmdL( KAiwCmdSelect, 
                iServiceHandler->InParamListL(),
                iServiceHandler->OutParamListL(), 
                KAiwOptCancel, 
                this );
        }
    }

// --------------------------------------------------------------------------
// CEDContactorPopupHandler::HandleNotifyL
// --------------------------------------------------------------------------
//
TInt CEDContactorPopupHandler::HandleNotifyL(TInt aCmdId, TInt aEventId,
        CAiwGenericParamList& aEventParamList,
        const CAiwGenericParamList& /*aInParamList*/)
    {
    LOGSTRING("CEDContactorPopupHandler::HandleNotifyL");
    TInt returnValue = KErrNone;

    if (aCmdId == KAiwCmdSelect)
        {
        switch(aEventId)
            {
            case KAiwEventCompleted:
                {
                LOGSTRING("CEDContactorPopupHandler::HandleNotifyL: KAiwEventCompleted");
                // Selection done.
                // Read and store results
                TRAP(iError, ReadFieldDataL(aEventParamList));
                }
                break;

            case KAiwEventCanceled:
                LOGSTRING("CEDContactorPopupHandler::HandleNotifyL: KAiwEventCanceled");
                // Selection canceled.
                iError = KErrCancel;
                break;

            case KAiwEventError:
            case KAiwEventStopped:
                LOGSTRING("CEDContactorPopupHandler::HandleNotifyL: KAiwEventError or KAiwEventStopped");
                // Error occurred.
                iError = KErrGeneral;
                break;

            case KAiwEventQueryExit:
                iError = KErrAbort;
                returnValue = EFalse; // Telephony must not be allowed to be closed down
                break;

            default:
                LOGSTRING("CEDContactorPopupHandler::HandleNotifyL: Unknown");
                iError = KErrGeneral;
                break;
            }
        // Stop scheduler if service execution has finished.
        if ( aEventId != KAiwEventQueryExit )
            {
            AsyncStop();
            }
        }

    LOGSTRING("CEDContactorPopupHandler::HandleNotifyL: Done.");
    return returnValue;
    }

// --------------------------------------------------------------------------
// CEDContactorPopupHandler::CEDContactorPopupHandler
// --------------------------------------------------------------------------
//
CEDContactorPopupHandler::CEDContactorPopupHandler() :
    iCoeEnv(*CCoeEnv::Static()),
    iError(KErrNone)
    {
    }

// --------------------------------------------------------------------------
// CEDContactorPopupHandler::ConstructL
// --------------------------------------------------------------------------
//
void CEDContactorPopupHandler::ConstructL()
    {
    LOGSTRING("CEDContactorPopupHandler::ConstructL");

    PrepareResourcesL();

    iServiceHandler = CAiwServiceHandler::NewL();
    iServiceHandler->AttachL(R_EDCONTACTORSERVICE_CONTACT_SELECTION_INTEREST);

    iSchedulerWait = new (ELeave) CActiveSchedulerWait();

    LOGSTRING("CEDContactorPopupHandler::ConstructL: Done.");
    }

// --------------------------------------------------------------------------
// CEDContactorPopupHandler::PrepareResourcesL
// --------------------------------------------------------------------------
//
void CEDContactorPopupHandler::PrepareResourcesL()
    {
    LOGSTRING("CEDContactorPopupHandler::PrepareResourcesL");

    // preparing resources for use
    TFileName fileName;
    fileName.Append (KEDContactorServiceResourceFileName);

    BaflUtils::NearestLanguageFile (iCoeEnv.FsSession (), fileName);
    if (ConeUtils::FileExists (fileName))
        {
        iResourceOffset = iCoeEnv.AddResourceFileL (fileName);
        }
    else // not found from user drive, leave badly.
        {
        LOGSTRING("CEDContactorPopupHandler::PrepareResourcesL: Resource file not exists.");
        User::Leave (KErrGeneral);
        }

    LOGSTRING("CEDContactorPopupHandler::PrepareResourcesL: Done.");
    }

// --------------------------------------------------------------------------
// CEDContactorPopupHandler::ReadFieldDataL
// --------------------------------------------------------------------------
//
void CEDContactorPopupHandler::ReadFieldDataL(
    CAiwGenericParamList& aEventParamList)
    {
    LOGSTRING("CEDContactorPopupHandler::ReadFieldDataL");

    if (iQueryResults)
        {
        delete iQueryResults;
        iQueryResults = NULL;
        }

    TInt index = 1; // Get field data from index 1
    const TAiwGenericParam* param =
         aEventParamList.FindFirst(index, EGenericParamContactFieldData);
    if (KErrNotFound != index)
        {
        TInt length = param->Value().AsDes().Length();
        iQueryResults = HBufC::NewL(length);
        iQueryResults->Des().Append(param->Value().AsDes());

        // Data must be found!
        if (0 >= iQueryResults->Length())
            {
            LOGSTRING("CEDContactorPopupHandler::ReadFieldDataL: Failed to get field data!");
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        LOGSTRING("CEDContactorPopupHandler::ReadFieldDataL: Not found!");
        User::Leave(KErrNotFound);
        }

    LOGSTRING("CEDContactorPopupHandler::ReadFieldDataL: Done.");
    }

// --------------------------------------------------------------------------
// CEDContactorPopupHandler::AsyncStop
// --------------------------------------------------------------------------
//
void CEDContactorPopupHandler::AsyncStop()
    {
    LOGSTRING("CEDContactorPopupHandler::AsyncStop");

    if(iSchedulerWait->IsStarted())
        {
        iSchedulerWait->AsyncStop();
        }

    LOGSTRING("CEDContactorPopupHandler::AsyncStop: Done.");
    }

// End of file
