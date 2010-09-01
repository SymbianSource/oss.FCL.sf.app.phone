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
* Description:  Small call status indicator
*
*/

#ifndef C_BUBBLECALLSTATUSINDISMALL_H
#define C_BUBBLECALLSTATUSINDISMALL_H

#include "BMCallStatusIndi.h"
#include "BMBubbleManager.h"
#include "telbubbleimage.h"

/**
 *  Call indicator
 *
 *
 *  @lib BubbleManager.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CBubbleCallStatusIndiSmall ) : public CBubbleCallStatusIndi
    {
public:
    /**
     * Two-phased constructor.
     * @param aBubbleManager Bubble manager.
     */
    static CBubbleCallStatusIndiSmall* NewL( CBubbleImageManager& aImageManager );
    
    /**
    * Destructor.
    */
    virtual ~CBubbleCallStatusIndiSmall();

    
private:    
    CBubbleCallStatusIndiSmall( CBubbleImageManager& aImageManager );
    
private: // From CBubbleCallStatusIndi
    /**
     * Includes logic for CallStatusIndicator image 
     * when image is shown
     */
    void SetImageAndMask();
    };

#endif // C_BUBBLECALLSTATUSINDISMALL_H

// End of file
