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
* Description:  Class for handling service logic.
*
*/

#include <e32std.h>
#include <s32mem.h>

#include "edcontactorserviceheaders.h"

// ================= MEMBER FUNCTIONS =======================
//

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::NewLC
// --------------------------------------------------------------------------
//
CEDContactorServiceOperator* CEDContactorServiceOperator::NewLC( MEDContactorObserver* aObserver )
    {
    CEDContactorServiceOperator* self = new (ELeave) CEDContactorServiceOperator(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::NewL
// --------------------------------------------------------------------------
//
CEDContactorServiceOperator* CEDContactorServiceOperator::NewL( MEDContactorObserver* aObserver )
    {
    CEDContactorServiceOperator* self = CEDContactorServiceOperator::NewLC(aObserver);
    CleanupStack::Pop(self);
    return self;
    }

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::~CEDContactorServiceOperator
// --------------------------------------------------------------------------
//
CEDContactorServiceOperator::~CEDContactorServiceOperator()
    {
    LOGSTRING("CEDContactorServiceOperator::~CEDContactorServiceOperator");

    delete iPopupHandler;
    }

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::Execute
// --------------------------------------------------------------------------
//
void CEDContactorServiceOperator::Execute(
    const CEDContactorService::TCSParameter& aParameter)
    {
    LOGSTRING("CEDContactorServiceOperator::Execute");

    iParameter = &aParameter;

    /**
     * LOGIC:
     * 1. Launch AIW popup
     * 2. Special cases: Find out serviceid for voip
     * 3. Launch communication service
     *
     * All actions are trapped here.
     */
    TDesC* result = NULL;
    TBool useDefaults = EFalse;

    // Check default control flag
    if (CEDContactorService::TCSParameter::EEnableDefaults &
        iParameter->iControlFlag)
        {
        useDefaults = ETrue;
        }

    // Launch popup
    TRAPD(err, result = &iPopupHandler->LaunchPopupL(
        iParameter->iContactLinkArray,
        CommunicationEnumMapper(iParameter->iCommTypeSelector),
        useDefaults));

    if (KErrNone != err)
        {
        LOGSTRING1("CEDContactorServiceOperator::ExecuteL: LaunchPopupL: Leaves %d.", err);
        
        if (iObserver)
            {
            iObserver->InformContactorEvent(MEDContactorObserver::ECommunicationCancelled);
            }       
        return;
        }

    // Resolve special cases
    TRAP(err, ResolveSpecialCasesL(*result));

    if (KErrNone != err)
        {
        LOGSTRING1("CEDContactorServiceOperator::ExecuteL: ResolveSpecialCasesL: Leaves %d.", err);
        
        if (iObserver)
            {
            iObserver->InformContactorEvent(MEDContactorObserver::ECommunicationCancelled);
            }       
        return;
        }

    // Launch communication service
    TRAP(err, LaunchCommunicationMethodL(*result));

    if (KErrNone != err)
        {
        LOGSTRING1("CEDContactorServiceOperator::ExecuteL: LaunchCommunicationMethodL: Leaves %d.", err);
        
        if (iObserver)
            {
            iObserver->InformContactorEvent(MEDContactorObserver::ECommunicationCancelled);
            }       
        return;
        }
    LOGSTRING("CEDContactorServiceOperator::Execute: Done.");
    }

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::Cancel
// --------------------------------------------------------------------------
//
void CEDContactorServiceOperator::Cancel()
    {
    if ( iPopupHandler )
        {
        TRAP_IGNORE( iPopupHandler->ClosePopupL() );
        }
    }

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::CEDContactorServiceOperator
// --------------------------------------------------------------------------
//
CEDContactorServiceOperator::CEDContactorServiceOperator( MEDContactorObserver* aObserver ):
    iServiceId((TUint32)KErrNotFound),
    iObserver(aObserver)
    {
    }

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::ConstructL
// --------------------------------------------------------------------------
//
void CEDContactorServiceOperator::ConstructL()
    {
    LOGSTRING("CEDContactorServiceOperator::ConstructL");

    iPopupHandler = CEDContactorPopupHandler::NewL();

    LOGSTRING("CEDContactorServiceOperator::ConstructL: Done.");
    }

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::ResolveSpecialCasesL
// --------------------------------------------------------------------------
//
void CEDContactorServiceOperator::ResolveSpecialCasesL(const TDesC& aFieldData)
    {
    LOGSTRING("CEDContactorServiceOperator::ResolveSpecialCasesL");

    /*
     * Currently only VOIP ServiceId is needed to find out.
     */
    switch(iParameter->iCommTypeSelector)
        {
        case VPbkFieldTypeSelectorFactory::EVOIPCallSelector:
            iServiceId = ResolveServiceIdL(aFieldData);
            break;
        }

    LOGSTRING("CEDContactorServiceOperator::ResolveSpecialCasesL: Done.");
    }

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::ResolveServiceIdL
// --------------------------------------------------------------------------
//
TUint32 CEDContactorServiceOperator::ResolveServiceIdL(const TDesC& aFieldData)
    {
    LOGSTRING("CEDContactorServiceOperator::ResolveServiceIdL");

    __ASSERT_ALWAYS (NULL != &aFieldData, User::Leave (KErrArgument));
    __ASSERT_ALWAYS (0 < aFieldData.Size(), User::Leave (KErrArgument));

    TUint32 ret = (TUint32)KErrNotFound;

    //LOGIC:
    /*
     * 1. Find out is there xSP prefix ( voip address for some specific service provider )
     * 2. If there is, search service id
     * 3. If not, find out how many services support msisdn
     * 4. If only one, find use that service id
     * 5. If more than one, do not use service id. User will be prompt which service to use.
     */

    TPtrC result;

    // 1. Find out is there xSP prefix
    if (ExtractServiceL(aFieldData, result))
        {
        LOGSTRING("CEDContactorServiceOperator::ResolveServiceIdL: ExtractServiceL found.");

        // 2. If there is, search service id
        ret = SearchServiceIdL(result);
        }
    else
        {
        LOGSTRING("CEDContactorServiceOperator::ResolveServiceIdL: ExtractServiceL not found.");

        // 3. If not, find out how many services support msisdn ( normal phone number )
        ret = ResolveMSISDNAddressingSupportedL();
        if ((TUint32)KErrNotFound != ret)
            {
            // 4. If only one, find use that service id
            // Currently no implementation.
            LOGSTRING("CEDContactorServiceOperator::ResolveServiceIdL: ResolveMSISDNAddressingSupportedL only one service.");

            }
        else
            {
            // 5. If more than one or none found, do not use service id.
            // User will be asked to choose among services when call is initiated.
            // Currently no implementation. 
            LOGSTRING("CEDContactorServiceOperator::ResolveServiceIdL: ResolveMSISDNAddressingSupportedL more than one service.");
            }
        }

    LOGSTRING("CEDContactorServiceOperator::ResolveServiceIdL: Done.");

    return ret;
    }

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::ExtractServiceL
// --------------------------------------------------------------------------
//
TBool CEDContactorServiceOperator::ExtractServiceL(
    const TDesC& aFieldData, TPtrC& aXSPId)
    {
    LOGSTRING("CEDContactorServiceOperator::ExtractServiceL");

    TBool found = EFalse;

    TInt pos = aFieldData.Find(KColon);
    if (pos >= 0)
        {
        // ok input
        aXSPId.Set(aFieldData.Left(pos));
        found = ETrue;
        }

    LOGSTRING("CEDContactorServiceOperator::ExtractServiceL: Done.");

    return found;
    }

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::SearchServiceIdL
// --------------------------------------------------------------------------
//
TUint32 CEDContactorServiceOperator::SearchServiceIdL(const TDesC& aFieldData)
    {
    LOGSTRING("CEDContactorServiceOperator::SearchServiceIdL");

    TUint32 ret = (TUint32)KErrNotFound;
    TInt err = KErrNone;
    CDesCArrayFlat* nameArray = NULL;

    RIdArray ids;
    CleanupClosePushL(ids);

    nameArray = new (ELeave) CDesCArrayFlat(2);
    CleanupStack::PushL(nameArray);

    CSPSettings* settings = CSPSettings::NewLC();

    err = settings->FindServiceIdsL(ids);
    User::LeaveIfError(err);
    err = settings->FindServiceNamesL(ids, *nameArray);
    User::LeaveIfError(err);

    TInt count = nameArray->MdcaCount();
    for (TInt i(0); i < count; i++)
        {
        // search the mathching service name
        TPtrC p = nameArray->MdcaPoint(i);
        if (!p.CompareF(aFieldData))
            {
            // Service found
            ret = ids[i];

            LOGSTRING("CEDContactorServiceOperator::SearchServiceIdL: Service found.");
            break;
            }
        }
    CleanupStack::PopAndDestroy( settings ); 
    CleanupStack::PopAndDestroy( nameArray ); 
    CleanupStack::PopAndDestroy( &ids ); 

    LOGSTRING("CEDContactorServiceOperator::SearchServiceIdL: Done.");

    return ret;
    }

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::ResolveMSISDNAddressingSupportedL
// --------------------------------------------------------------------------
//
TUint32 CEDContactorServiceOperator::ResolveMSISDNAddressingSupportedL()
    {
    LOGSTRING("CEDContactorServiceOperator::ResolveMSISDNAddressingSupportedL");

    /*
     * LOGIC:
     * -Find out services that support calling normal mobile/telephone numbers
     * -If only 1 service, return the serviceid
     * -If services are more than 1, then do not use serviceid.
     */

    TInt err = KErrNone;
    TInt foundNo = 0;
    TUint32 ret = (TUint32)KErrNotFound;

    RIdArray ids;
    CleanupClosePushL(ids);

    CSPSettings* settings = CSPSettings::NewLC();

    err = settings->FindServiceIdsL(ids);
    User::LeaveIfError(err);
    
    CSPProperty* property = CSPProperty::NewLC();

    TInt count = ids.Count();
    for (TInt i(0); i < count; i++)
        {
        // Find out property
        err = settings->FindPropertyL(ids[i],
            EPropertyServiceAttributeMask, *property);

        // If service have property
        if (KErrNone == err)
            {
            // read the value of mask property
            TInt mask = 0;
            err = property->GetValue(mask);
            if (KErrNone == err)
                {
                if ((mask & ESupportsMSISDNAddressing) &&
                    (mask & ESupportsInternetCall))
                    {
                    // Found one.
                    ret = ids[i];
                    foundNo++;

                    }// if mask
                }// if err
            }// if err

        }// for

    // If more than 1 service, do not use serviceid

    if (1 < foundNo)
    {
        ret = (TUint32)KErrNotFound;
    }

    CleanupStack::PopAndDestroy( property );
    CleanupStack::PopAndDestroy( settings );
    CleanupStack::PopAndDestroy( &ids );

    LOGSTRING("CEDContactorServiceOperator::ResolveMSISDNAddressingSupportedL: Done.");
    return ret;
    }

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::LaunchCommunicationMethodL
// --------------------------------------------------------------------------
//
void CEDContactorServiceOperator::LaunchCommunicationMethodL(
    const TDesC& aFieldData)
    {
    LOGSTRING("CEDContactorServiceOperator::LaunchCommunicationMethodL");

    __ASSERT_ALWAYS (NULL != &iParameter->iFullName, User::Leave (KErrArgument));
    __ASSERT_ALWAYS (0 < iParameter->iFullName.Size(), User::Leave (KErrArgument));


    TBool forcedService( EFalse );
    
    // Check forced service flag
    if (CEDContactorService::TCSParameter::EForcedService &
        iParameter->iControlFlag)
        {
        forcedService = ETrue;
        }
    
    // If serviceid found, use it.
    if ((TUint32)KErrNotFound != iServiceId)
        {
        LOGSTRING("CEDContactorServiceOperator::ExecuteL: ExecuteServiceL with serviceid");
        CEDContactor::ExecuteServiceL(
            iParameter->iCommTypeSelector,
            aFieldData,
            iParameter->iFullName,
            forcedService,
            iServiceId);
        }
    else
        {
        LOGSTRING("CEDContactorServiceOperator::ExecuteL: ExecuteServiceL without serviceid");
        CEDContactor::ExecuteServiceL(
            iParameter->iCommTypeSelector,
            aFieldData,
            iParameter->iFullName,
            forcedService);
        }

    LOGSTRING("CEDContactorServiceOperator::LaunchCommunicationMethodL: Done.");
    }

// --------------------------------------------------------------------------
// CEDContactorServiceOperator::CommunicationEnumMapper
// --------------------------------------------------------------------------
//
TAiwCommAddressSelectType CEDContactorServiceOperator::CommunicationEnumMapper(
    VPbkFieldTypeSelectorFactory::TVPbkContactActionTypeSelector aCommTypeSelector)
    {
    LOGSTRING("CEDContactorServiceOperator::CommunicationEnumMapper");

    TAiwCommAddressSelectType type;

    switch(aCommTypeSelector)
        {
        case VPbkFieldTypeSelectorFactory::EEmptySelector:
            type = EAiwCommEmpty;
            break;

        case VPbkFieldTypeSelectorFactory::EVoiceCallSelector:
            type = EAiwCommVoiceCall;
            break;

        case VPbkFieldTypeSelectorFactory::EUniEditorSelector:
            type = EAiwCommUniEditor;
            break;

        case VPbkFieldTypeSelectorFactory::EEmailEditorSelector:
            type = EAiwCommEmailEditor;
            break;

        case VPbkFieldTypeSelectorFactory::EInstantMessagingSelector:
            type = EAiwCommInstantMessaging;
            break;

        case VPbkFieldTypeSelectorFactory::EVOIPCallSelector:
            type = EAiwCommVOIPCall;
            break;

        case VPbkFieldTypeSelectorFactory::EVideoCallSelector:
            type = EAiwCommVideoCall;
            break;

        default:
            LOGSTRING("CEDContactorServiceOperator::CommunicationEnumMapper: default -> Empty mapped.");
            type = EAiwCommEmpty;
            break;
        }

    LOGSTRING("CEDContactorServiceOperator::CommunicationEnumMapper: Done.");

    return type;
    }

// End of file
