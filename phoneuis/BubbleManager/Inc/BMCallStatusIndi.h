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


#ifndef C_BUBBLECALLSTATUSINDI_H
#define C_BUBBLECALLSTATUSINDI_H

#include "BMBubbleManager.h"
#include "telbubbleimage.h"

/**
 *  Call indicator
 *
 *
 *  @lib BubbleManager.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CBubbleCallStatusIndi ) : public CTelBubbleImage
    {
public:
    /**
     * Read bubble header.
     *
     * @since S60 5.0
     * @param aHeader Bubble header.
     */
    void ReadBubbleHeader( const CBubbleHeader& aHeader );
    
protected:
    /**
     * C´tor
     */
    CBubbleCallStatusIndi( CBubbleImageManager& aImageManager );
    
private: // From CCoeControl
    virtual void MakeVisible( TBool aVisible );
    
    /**
     * Includes logic for CallStatusIndicator image 
     * when image is shown
     */
    virtual void SetImageAndMask() = 0;
protected: // data
    CBubbleImageManager& iImageManager;
    CBubbleManager::TPhoneCallState iCallState;
    TUint32 iCallFlags;
    };

#endif // C_BUBBLECALLSTATUSINDI_H

// End of file

