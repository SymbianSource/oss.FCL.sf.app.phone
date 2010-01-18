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


#ifndef __TELBRANDING_H__
#define __TELBRANDING_H__

//  Include Files
#include <e32base.h>	// CBase
#include <ecom.h>
#include <telbubbleextension.h>

//  Constants

class CTelBrandingSettingsHandler;
class CTelBrandingServerHandler;
class CTelBrandingBubble;
class CElement;

//  Class Definitions
class CTelBranding : public CTelBubbleExtension
    {
public:
    static CTelBranding* NewL();

    virtual ~CTelBranding();

private: // new functions, example API
// from CTelBubbleExtension

    /** @see CTelBubbleExtension::InitializeL */
    void InitializeL( MTelBubbleExtensionObserver& aBubbles );

    /** @see CTelBubbleExtension::StartCustomizedBubble */
    void StartCustomizedBubble(
        MTelBubbleExtensionData& aCallData  );

    /** @see CTelBubbleExtension::StopCustomizedBubble */
    void StopCustomizedBubble(
        MTelBubbleExtensionData& aCallData  );

    /** @see CTelBubbleExtension::BubbleUpdating */
    void BubbleUpdating();


private: // new functions
    void LoadBrandingL(
        MTelBubbleExtensionData& aCallData );

    CTelBranding();

    void ConstructL();

private: // data
    MTelBubbleExtensionObserver* iObserver;

    CTelBrandingSettingsHandler* iSettings;

    RPointerArray<CTelBrandingBubble> iBubbleArray;
    };

#endif  // __TELBRANDING_H__

// end of file
