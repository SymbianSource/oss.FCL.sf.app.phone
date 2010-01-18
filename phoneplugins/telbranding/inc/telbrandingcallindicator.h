/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Branding plug-in specific call indication.
*
*/

#ifndef C_CTELBRANDINGCALLINDICATOR_H
#define C_CTELBRANDINGCALLINDICATOR_H

#include <telbubbleanim.h>

class MTelBubbleExtensionData;
class TAknsItemID;


/**
 *  Branding plug-in specific call indication.
 *
 *  @since S60 S60 v5.0
 */
NONSHARABLE_CLASS( CTelBrandingCallIndicator ) : 
    public CTelBubbleAnim
    {
public:
    /**
    * Constructors.
    */
    static CTelBrandingCallIndicator* NewL( 
        MTelBubbleExtensionData& aCallData, CEikImage* aBrand );
    static CTelBrandingCallIndicator* NewLC( 
        MTelBubbleExtensionData& aCallData, CEikImage* aBrand );

    /**
    * Destructor.
    */
    virtual ~CTelBrandingCallIndicator();

public: // From CCoeControl
    void MakeVisible( TBool aVisible );
    void Draw(const TRect& aRect) const;
    void SizeChanged();
    
private: // New functions
    void DoVisibleL();
    
    void ShowAnimationL();
    
    void ShowImageL();
    
    void AddBrandedFrameToAnimationL(
        TAknsItemID aSkinId, 
        TInt aBitmap, 
        TInt aMask );
    
    CEikImage* GetImageL(
        TAknsItemID aSkinId, 
        TInt aBitmap, 
        TInt aMask );
    
    
private:
    CTelBrandingCallIndicator( 
        MTelBubbleExtensionData& aCallData, CEikImage* aBrand );

    void ConstructL();

private: // data
    // Call information
    MTelBubbleExtensionData& iCallData;
    
    /**
     * Brand image
     * Own.
     */
    CEikImage* iBrand;
    
    /**
     * Call indicator image
     * Own.
     */
    CEikImage* iCallIndicator;
    };

#endif // C_CTELBRANDINGCALLINDICATOR_H
