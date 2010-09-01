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
* Description:  Call status animation
*
*/


#ifndef C_BUBBLECALLSTATUSANIM_H
#define C_BUBBLECALLSTATUSANIM_H

#include "BMBubbleManager.h"
#include "telbubbleanim.h"

/**
 *  Call status animation.
 *
 *
 *  @lib BubbleManager.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CBubbleCallStatusAnim ) : public CTelBubbleAnim
    {
protected:
    /**
    * C`tor.
    */
    CBubbleCallStatusAnim( CBubbleImageManager& aImageManager );
    
    /**
    * 2nd C`tor.
    */
    void ConstructL();

public:
    /**
     * Read bubble header.
     *
     * @since S60 5.0
     * @param aHeader Bubble header.
     */
    void ReadBubbleHeader( const CBubbleHeader& aHeader );

    /**
     * Set container window.
     *
     * @since S60 5.0
     * @param aCallFlags Call flags.
     */
    void SetContainerWindowL( const CCoeControl& aControl );
    
private:
    void MakeVisible(TBool aVisible);
    
private: // From CTelBubbleAnim
    /**
     * Includes animation logic when it is shown
     */
    virtual void SetAnimationContentL() = 0;

protected: // data
    CBubbleImageManager& iImageManager;
    TUint32 iCallFlags;
    };

#endif // C_BUBBLECALLSTATUSANIM_H

// End of file
