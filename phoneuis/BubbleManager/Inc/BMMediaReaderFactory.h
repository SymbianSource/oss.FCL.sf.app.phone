/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Creator of media readers.
*
*/


#ifndef BUBBLEMEDIAREADERFACTORY_H
#define BUBBLEMEDIAREADERFACTORY_H

#include <e32base.h>

class MBubbleMediaReader;

/**
 *  Creator of media readers.
 *
 *  ?more_complete_description
 *
 *  @lib BubbleManager
 *  @since S60 3.2
 */
class BubbleMediaReaderFactory
    {
public:    	

    /**
     * Factory method to create a reader for given file.
     *
     * @since S60 v3.2
     * @param aFileName File name with path.
     * @return Media reader instance.
     */
    static MBubbleMediaReader* CreateReaderL( 
        const TDesC& aFileName );
    };


#endif // BUBBLEMEDIAREADERFACTORY_H
