/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declares coordinates from LAF specification
*
*/


#ifndef USSDLAF_H
#define USSDLAF_H

// INCLUDE FILES
#include    <calslbs.h> 
#include    <AknLayoutDef.h>


// CONSTANTS

// The maximum number of editor lines method NumberOfEditorLines can return.
const TInt KUssdMaxNumberOfEditorLines = 16;

/**
* Layout coordinates from Application LAF
*
* @since 2.0
*/
class UssdLayout
    {
    public: // New functions

        typedef TAknWindowLineLayout TWindowLineLayoutType;
        typedef TAknMultiLineTextLayout TTextLineLayoutType;

        /**
        * Returns current Layout ID
        * @since 2.0
        * @return EAknLayoutIdELAF, EAknLayoutIdABRW, EAknLayoutIdAPAC
        */
        static TAknLayoutId CurrentLayout();

        /**
        * Get background image colour
        * @since 2.0
        * @return White colour
        */ 
        static TInt LayoutColourWhite();

        /**
        * Get number of editor lines
        * @since 2.0
        * @return Number of lines
        */ 
        static TInt NumberOfEditorLines();

        /**
        * One line coordinate definitions from LAF
        * @since 2.0
        * @param aLine Line number from 1 to NumberOfEditorLines()
        * @return Current line coordinates
        */ 
        static TWindowLineLayoutType 
            MessageWritingLayoutElements6( TInt aLine );

        /**
        * Editor coordinate definitions from LAF
        * @since 2.0
        * @return Editor coordinates
        */ 
        static TTextLineLayoutType MessageWritingTexts3();

    };

#endif

// End of File

