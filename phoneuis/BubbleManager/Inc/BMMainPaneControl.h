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
* Description:  Call status indicator
*
*/


#ifndef C_BUBBLEMAINPANECONTROL_H
#define C_BUBBLEMAINPANECONTROL_H

#include <coecntrl.h>
#include "BMBubbleManager.h"
#include "telbubbleimagescaler.h"

class CBubbleCallObjectManager;
class CTelBubbleCustomElement;
class CBubbleCustomManager;
class CFbsBitmap;

/**
 *  Displays caller image in main pane.
 *
 *
 *  @lib BubbleManager.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CBubbleMainPaneControl ) : public CCoeControl,
    public MTelBubbleImageScalerObserver
    {
public:
    /**
     * Two-phased constructor.
     * @param aBubbleManager Bubble manager.
     * @param aCallObjectManager Call object manager.
     */
    static CBubbleMainPaneControl* NewL(
            CBubbleManager& aBubbleManager, 
            CBubbleCallObjectManager& aCallObjectManager );
    
    /**
    * Destructor.
    */
    virtual ~CBubbleMainPaneControl();
    
    /**
     * Read bubble header.
     * @param aHeader Bubble header.
     */
    void ReadBubbleHeader( const CBubbleHeader& aHeader );
    
    /**
     * Reset.
     */
    void Reset();
    
    /**
     * Draw bitmaps.
     * @param aRect Rectangle to be redrawn.
     */
    void DrawBitmaps( const TRect& aRect ) const;
    
    /**
     * Returns bubble id that image is related to.
     */
    CBubbleManager::TBubbleId BubbleId() const;
    
    /**
     * Returns ETrue when image is being displayed.
     */
    TBool IsUsed() const;
    
    // from MTelBubbleImageScalerObserver    
    /**
     * Callback function from CTelBubbleImageScaler
     * 
     * @param aError Error happened during scaling
     * @param aBitmap Scaled bitmap
     * @return None 
     */ 
    void ImageScalingComplete
                    (TInt aError, CFbsBitmap* aBitmap);
            
private:    
    CBubbleMainPaneControl( CBubbleManager& aCustomManager, 
                            CBubbleCallObjectManager& aCallObjectManager );
    void ConstructL();
    void SizeChanged();
    void PrepareBitmapsL( CFbsBitmap*& aBitmap,
                          TBool& aBitmapOwnership, 
                          CFbsBitmap*& aMask,
                          TBool& aMaskOwnership,
                          TBool aIsScalable,
                          TBool aThumbnail = EFalse );

    void StartScaling( CFbsBitmap *aSourceBitmap );
private: // data
    CBubbleManager& iBubbleManager;
    CBubbleCallObjectManager& iCallObjectManager;
    CBubbleManager::TBubbleId iBubble;
    TBool iThreeLinedBubble;
    CTelBubbleCustomElement* iCallImage;
    TBool iIsUsed;
    CFbsBitmap* iBitmap; // not owned
    CFbsBitmap* iMask; // not owned
    CTelBubbleImageScaler *iScaler;
    CFbsBitmap *iScaledImage;
    
    // to prevent loading & scaling the same image again.
    CFbsBitmap* iOldBitmap;	
    };

#endif // C_BUBBLEMAINPANECONTROL_H
