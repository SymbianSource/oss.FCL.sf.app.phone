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

#include "BMBubbleManager.h"
#include <coecntrl.h>

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
NONSHARABLE_CLASS( CBubbleMainPaneControl ) : public CCoeControl
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
        
private:    
    CBubbleMainPaneControl( CBubbleManager& aCustomManager, 
                            CBubbleCallObjectManager& aCallObjectManager );
    void ConstructL();
    void SizeChanged();
    void PrepareBitmapsL( CFbsBitmap*& aBitmap,
                          TBool& aBitmapOwnership, 
                          CFbsBitmap*& aMask,
                          TBool& aMaskOwnership,
                          TBool aIsScalable ); 

private: // data
    CBubbleManager& iBubbleManager;
    CBubbleCallObjectManager& iCallObjectManager;
    CBubbleManager::TBubbleId iBubble;
    TBool iThreeLinedBubble;
    CTelBubbleCustomElement* iCallImage;
    TBool iIsUsed;
    CFbsBitmap* iBitmap; // not owned
    CFbsBitmap* iMask; // not owned
    };

#endif // C_BUBBLEMAINPANECONTROL_H
