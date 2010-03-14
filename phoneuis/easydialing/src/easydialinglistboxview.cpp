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
* Description:  Easy dialing listbox view.
*
*/


// INCLUDE FILES

#include "easydialinglistboxview.h"
#include "easydialinglistbox.h"
#include "easydialinglistboxitemdrawer.h"
#include "easydialinglistboxdata.h"
#include "easydialingcontactdatamanager.h"
#include <phonebook2.mbg>
#include <phonebook2ece.mbg>

#include <ccappcommlauncherpluginrsc.rsg>

// AvKON and drawing header files
#include <aknlayoutscalable_avkon.cdl.h>

#include <aknlistloadertfx.h>
#include <aknlistboxtfxinternal.h>
#include <aknlistboxtfx.h>

#include <akntransitionutils.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

/*
 * ==============================================================================
 * 
 * 
 * class CEasyDialingListBoxView
 * 
 * 
 * ==============================================================================
 */


// -----------------------------------------------------------------------------
// SetCurrentItemIndexToNone
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingListBoxView::SetCurrentItemIndexToNone()
    {
    iCurrentItemIndex = -1;
    }

//  End of File.

