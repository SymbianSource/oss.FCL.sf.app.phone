/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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

#ifndef __TELBRANDINGSERVERHANDLER_H__
#define __TELBRANDINGSERVERHANDLER_H__

//  Include Files
#include <e32base.h>	// CBase

//  Constants

class CFbsBitmap;
class MBSAccess;
class CBSFactory;

//  Class Definitions
class CTelBrandingServerHandler : public CBase
    {
public:
    /**
     * Constructs branding server and creates access
     * @param aBrandId Specifies brand which is used
     */
    static CTelBrandingServerHandler* NewLC( const TDesC8& aBrandId );
    
    virtual ~CTelBrandingServerHandler();

    /**
     * Gets brand bitmap and mask according parameters
     * @param aImageId Specifies brand image which is wanted
     * @param aBitmap On succesful return contains bitmap
     * @param aBitmask On succesful return contains mask
     * @leave If brand not found or other error situations
     */
    void GetBrandIconL( 
        const TDesC8& aImageId,
        CFbsBitmap*& aBitmap,
        CFbsBitmap*& aBitmask ) const;
    
private: // new functions
    CTelBrandingServerHandler();
    void ConstructL( const TDesC8& aBrandId );

private: // data
    
    /**
     * Own.
     */
    MBSAccess* iAccess;
    
    /**
     * Own.
     */
    CBSFactory* iFactory;
    };

#endif  // __TELBRANDINGSERVERHANDLER_H__

// end of file
