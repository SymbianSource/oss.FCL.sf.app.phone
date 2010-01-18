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
* Description:  Big call status indicator
*
*/


#ifndef C_BUBBLECALLSTATUSINDIBIG_H
#define C_BUBBLECALLSTATUSINDIBIG_H

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
NONSHARABLE_CLASS( CBubbleCallStatusIndiBig ) : public CBubbleCallStatusIndi
    {
public:
    /**
     * Two-phased constructor.
     * @param aBubbleManager Bubble manager.
     */
    static CBubbleCallStatusIndiBig* NewL( CBubbleImageManager& aImageManager );
    
    /**
    * Destructor.
    */
    virtual ~CBubbleCallStatusIndiBig();
    
private:    
    CBubbleCallStatusIndiBig( CBubbleImageManager& aImageManager );
    
private: // From CBubbleCallStatusIndi
    /**
     * Includes logic for CallStatusIndicator image 
     * when image is shown
     */
    void SetImageAndMask();

    };

#endif // C_BUBBLECALLSTATUSINDIBIG_H

// End of file
