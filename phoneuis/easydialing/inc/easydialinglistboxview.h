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
* Description:  Easy dialing list box view.
*
*/

#ifndef _EASYDIALINGLISTBOXVIEW_H
#define _EASYDIALINGLISTBOXVIEW_H

// INCLUDE FILES
#include <aknlists.h>

// CLASS DECLARATIONS

/*
 * Easy dialing listbox view.
 */

class CEasyDialingListBoxView : public CFormattedCellListBoxView
    {
public:
    
    /**
    * Sets current index value to outside-of-range value. After this, no
    * item in the listbox is highlighted. 
    */
    void SetCurrentItemIndexToNone();
    };
    
    
#endif // _EASYDIALINGLISTBOXVIEW_H
    








