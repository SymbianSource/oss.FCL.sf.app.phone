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
* Description:  Number type icon. 
*
*/

#ifndef C_BUBBLENUMBERTYPEICON_H
#define C_BUBBLENUMBERTYPEICON_H

#include "BMBubbleManager.h"
#include "telbubbleimage.h"

/**
 *  Number type icon
 *
 *
 *  @lib BubbleManager.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CBubbleNumberTypeIcon ) : public CTelBubbleImage
    {
public:
    /**
     * Two-phased constructor.
     * @param aBubbleManager Bubble manager.
     */
    static CBubbleNumberTypeIcon* NewL( CBubbleImageManager& aImageManager );
    
    /**
    * Destructor.
    */
    virtual ~CBubbleNumberTypeIcon();
    
    /**
     * Read bubble header.
     *
     * @since S60 5.0
     * @param aHeader Bubble header.
     */
    void ReadBubbleHeader( const CBubbleHeader& aHeader );
    
private:    
    CBubbleNumberTypeIcon( CBubbleImageManager& aImageManager );
    void ConstructL();
    void MakeVisible(TBool aVisible);

private: // data
    CBubbleImageManager& iImageManager;
    CBubbleManager::TPhoneNumberType iNumberType;
    TUint32 iCallFlags;
    };

#endif // C_BUBBLENUMBERTYPEICON_H

// End of file
