/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declares window owning control for editor lines.
*
*/


#ifndef USSDEDITORLINES_H
#define USSDEDITORLINES_H

// INCLUDES
#include    <calslbs.h>
#include    <aknview.h>
#include    "UssdLayout.h"

/**
*  Declares window owning control for editor lines.
*
*  @since 1.0
*/
class CUssdEditorLines : public CCoeControl
    {
    public: // Constructors and destructor
            
        // Default constructor
        CUssdEditorLines();

        /**
        * Symbian OS default constructor.
        * @param aParent The parent control.
        */
        void ConstructL( const CCoeControl* aParent );

        /**
        * Destructor.
        */
        ~CUssdEditorLines();

    private: // Functions from base classes

       /**
        * From CoeControl,SizeChanged.
        */
        void SizeChanged();

       /**
        * From CCoeControl,Draw.
        */
        void Draw( const TRect& aRect ) const;
        
        /**
        * From CoeControl,HandlePointerEventL.
        * Redirect pointer event to CEikEdwin.
        */
        void HandlePointerEventL(const TPointerEvent& aPointerEvent);

    private: //data

        // The line rectangles.
        CArrayPtrFlat< TAknLayoutRect >*           iRects;

        // Buffer for RRegion objects.
        RRegionBuf< KUssdMaxNumberOfEditorLines >* iRegBuf;
    
    };

#endif // USSDEDITORLINES_H

// End of File
