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
* Description:  Big call status animation
*
*/


#ifndef C_BUBBLECALLSTATUSANIMBIG_H
#define C_BUBBLECALLSTATUSANIMBIG_H

#include "BMCallStatusAnim.h"
#include "BMBubbleManager.h"
#include "telbubbleanim.h"

/**
 *  Call status animation.
 *
 *
 *  @lib BubbleManager.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CBubbleCallStatusAnimBig ) : public CBubbleCallStatusAnim
    {
public:
    /**
     * Two-phased constructor.
     * @param aBubbleManager Bubble manager.
     */
    static CBubbleCallStatusAnimBig* NewL( CBubbleImageManager& aImageManager );
    
    /**
    * Destructor.
    */
    virtual ~CBubbleCallStatusAnimBig();

private:    
    CBubbleCallStatusAnimBig( CBubbleImageManager& aImageManager );
    
private: // From CTelBubbleAnim
    /**
     * Includes animation logic when it is shown
     */
    void SetAnimationContentL();

    };

#endif // C_BUBBLECALLSTATUSANIMBIG_H

// End of file
