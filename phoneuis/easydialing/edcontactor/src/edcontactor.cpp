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
* Description:  Implementation of the ccacontactor
*
*/


// INCLUDE FILES
#include "edcontactorheaders.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// EDContactor::ExecuteServiceL()
// -----------------------------------------------------------------------------
//
void CEDContactor::ExecuteServiceL(VPbkFieldTypeSelectorFactory::TVPbkContactActionTypeSelector aCommMethod, const TDesC& aParam, const TDesC& aName, 
        TBool aForcedService, TUint32 aServiceId)
    {
    CEDContactorOperation* operation = 0;
    
    switch (aCommMethod)
        {
        case VPbkFieldTypeSelectorFactory::EVoiceCallSelector:
            operation = CEDContactorCallOperation::NewL(aParam, aForcedService);
            break;
            
        case VPbkFieldTypeSelectorFactory::EVideoCallSelector:
            operation = CEDContactorVideocallOperation::NewL(aParam);
            break;
        case VPbkFieldTypeSelectorFactory::EUniEditorSelector:
            operation = CEDContactorUniEditorOperation::NewL(aParam, aName);
            break;
        case VPbkFieldTypeSelectorFactory::EEmailEditorSelector:
            operation = CEDContactorEmailOperation::NewL(aParam, aName);
            break;
        case VPbkFieldTypeSelectorFactory::EVOIPCallSelector:
            operation = CEDContactorVOIPOperation::NewL(aParam, aServiceId);
            break;
        case VPbkFieldTypeSelectorFactory::EInstantMessagingSelector:
            operation = CEDContactorIMOperation::NewL(aParam);
            break;
        
        }
    if (operation)
        {
        operation->ExecuteLD();
        operation = NULL;
        }
    }
// End of File

