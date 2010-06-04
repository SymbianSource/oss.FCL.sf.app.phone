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
* Description:  Customization interface for plugins to use.
*
*/


#ifndef TELBUBBLEEXTENSIONOBSERVER_H
#define TELBUBBLEEXTENSIONOBSERVER_H

#include <e32base.h>

class CTelBubbleCustomElement;

class MTelBubbleExtensionObserver
    {
public:
    /**
     * Call this before attach, detach and replace operations,
     * when doing multipe changes outside BubbleUpdating() method. 
     * UI is not redrawn until calling EndChanges() is called. 
     */
    virtual void StartChanges() = 0;
    
    /**
     * Call this when all changes have been done and UI can be
     * redrawn.
     */
    virtual void EndChanges() = 0;
    
    /**
     * Attaches element to bubble. Causes redraw.
     * @param aBubbleId Bubble id
     * @param aElement Element to be attached, ownership is not transferred
     */
    virtual void AttachElement( 
            TInt aBubbleId, 
            CTelBubbleCustomElement* aElement ) = 0;
    
    /**
     * Replaces element in bubble. Causes redraw.
     * @param aBubbleId Bubble id
     * @param aOld Element to be detached, ownership is not transferred
     * @param aNew Element to be attached, ownership is not transferred
     */
    virtual void ReplaceElement( 
            TInt aBubbleId, 
            CTelBubbleCustomElement* aOld, 
            CTelBubbleCustomElement* aNew ) = 0;
    
    /**
     * Detaches element in bubble. Causes redraw.
     * @param aBubbleId Bubble id
     * @param aElement Element to be detached, ownership is not transferred
     */
    virtual void DetachElement( 
            TInt aBubbleId, 
            CTelBubbleCustomElement* aElement ) = 0;
    
    };

#endif // TELBUBBLEEXTENSIONOBSERVER_H

// end of file
