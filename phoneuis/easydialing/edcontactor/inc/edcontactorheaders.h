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
* Description:  ccacontactor's headers
*
*/


#ifndef __EDCONTACTORHEADERS_H__
#define __EDCONTACTORHEADERS_H__


// Constants
#include <e32cons.h>
_LIT( KColon, ":" );

// system
#include <e32base.h>
#include <bautils.h>
#include <coeutils.h>
#include <coemain.h>
#include <apgcli.h> 
#include <apgtask.h>
#include <eikenv.h>

//Phbk 
#include <VPbkFieldTypeSelectorFactory.h>

// AIW
#include <AiwCommon.h>
#include <AiwServiceHandler.h>
#include <AiwGenericParam.h>
#include <AiwContactSelectionDataTypes.h>
#include <AiwVariant.h>
#include <AiwGenericParam.hrh>
#include <aiwdialdataext.h>


//SPSettings
#include <spsettings.h>
#include <spproperty.h>

//Sendui
#include <SenduiMtmUids.h>
#include <sendui.h>
#include <CMessageData.h>
#include <commonphoneparser.h>
#include <CSendingServiceInfo.h>

//IM plugin
#include <cmscontactorimpluginbase.h>

//own
#include "edcontactor.h"
#include "edcontactoroperation.h"
#include "edcontactorcalloperation.h"
#include "edcontactorvideocalloperation.h"
#include "edcontactorunieditoroperation.h"
#include "edcontactoremailoperation.h"
#include "edcontactorvoipoperation.h"
#include "edcontactorimoperation.h"
#include "edmsgeditors.h"
#include "edphonecall.h"

#endif // __EDCONTACTORHEADERS_H__
