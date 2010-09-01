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
* Description:  Sets brand according service id.
*
*/


#ifndef __TELBRANDINGSETTINGSHANDLER_H__
#define __TELBRANDINGSETTINGSHANDLER_H__

//  Include Files
#include <e32base.h>	// CBase

//  Constants


//  Class Definitions
class CTelBrandingSettingsHandler : public CBase
    {
public:
    static CTelBrandingSettingsHandler* NewL();
    
    virtual ~CTelBrandingSettingsHandler();

    HBufC* BrandIdLC( TUint32 aServiceId ) const;
private: // new functions
    CTelBrandingSettingsHandler();
    void ConstructL();

private: // data
    };

#endif  // __TELBRANDINGSETTINGSHANDLER_H__

// end of file
