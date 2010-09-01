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
* Description:  Extension control for displaying an icon (static image).
*
*/


#ifndef TELBUBBLEIMAGE_H
#define TELBUBBLEIMAGE_H

#include <eikimage.h>

/**
 *  Extension control for displaying an icon (static image).
 *
 *
 *  @lib BMCustomization.lib
 *  @since S60 v5.0
 */
class CTelBubbleImage : public CEikImage
    {
public:
    /** Two phase constructor */
    IMPORT_C static CTelBubbleImage* NewL();

    /** destructor */
    IMPORT_C virtual ~CTelBubbleImage();

protected: // from CCoeControl
    IMPORT_C virtual void PositionChanged();
    IMPORT_C virtual void SizeChanged();

protected:
    IMPORT_C virtual void ConstructL();
    IMPORT_C CTelBubbleImage();
    };

#endif // TELBUBBLEIMAGE_H

// end of file
