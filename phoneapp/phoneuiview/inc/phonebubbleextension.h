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
* Description:  Bubble extension plugin wrapper
*
*/


#ifndef C_CPHONEBUBBLEEXTENSION_H
#define C_CPHONEBUBBLEEXTENSION_H

#include <e32base.h>
#include <telbubbleextensionobserver.h>
#include <telbubbleextensioninterface.h>

class CTelBubbleCustomElement;
class MTelBubbleExtensionData;
class CTelBubbleExtension;
class CBubbleManager;

/**
 *  Bubble extension plugin wrapper
 *  This class wraps a plugin, hiding the actual plugin from the
 *  phoneapp and phoneapp from the plugin.
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CPhoneBubbleExtension : public CBase,
                              public MTelBubbleExtensionObserver,
                              public MTelBubbleExtensionInterface
    {
public:

    /**
     * Two-phased constructor.
     *
     * @param aBubbleManager Reference to bubble manager
     * @param aUid Plugin UID
     * @param aPriority Priority of the plugin
     */
    static CPhoneBubbleExtension* NewL(
            CBubbleManager& aBubbleManager,
            TUid aUid,
            TUint8 aPriority );

    /**
     * Two-phased constructor.
     *
     * @param aBubbleManager Reference to bubble manager
     * @param aUid Plugin UID
     * @param aPriority Priority of the plugin
     */
    static CPhoneBubbleExtension* NewLC(
            CBubbleManager& aBubbleManager,
            TUid aUid,
            TUint8 aPriority );

    /**
    * Destructor.
    */
    virtual ~CPhoneBubbleExtension();

// from base class MTelBubbleExtensionObserver

    /**
     * Call this before attach, detach and replace operations,
     * when doing multipe changes outside BubbleUpdating() method. 
     * UI is not redrawn until calling EndChanges() is called. 
     */
    void StartChanges();
    
    /**
     * Call this when all changes have been done and UI can be
     * redrawn.
     */
    void EndChanges();
    
    /**
     * Attaches element to bubble. Causes redraw.
     *
     * @param aBubbleId Bubble id
     * @param aElement Element to be attached, ownership is not transferred
     */
    void AttachElement( TInt aBubbleId, CTelBubbleCustomElement* aElement );

    /**
     * Replaces element in bubble. Causes redraw.
     *
     * @param aId Bubble id
     * @param aOld Element to be detached, ownership is not transferred
     * @param aNew Element to be attached, ownership is not transferred
     */
    void ReplaceElement(
            TInt aBubbleId,
            CTelBubbleCustomElement* aOld,
            CTelBubbleCustomElement* aNew );

    /**
     * Detaches element in bubble. Causes redraw.
     *
     * @param aId Bubble id
     * @param aElement Element to be detached, ownership is not transferred
     */
    void DetachElement( TInt aBubbleId, CTelBubbleCustomElement* aElement );


// from base class MTelBubbleExtensionInterface

    /**
     * Notifies that a new customizable bubble is available
     *
     * @param aCallData Reference to call data.
     */
    void StartCustomizedBubble( MTelBubbleExtensionData& aCallData );

    /**
     * Notifies that a customizable bubble has been removed
     *
     * @param aCallData Reference to call data.
     */
    void StopCustomizedBubble( MTelBubbleExtensionData& aCallData );

    /**
     * Indicates that currently bubble drawing is started.
     * Customizing is preferred to be done during this call.
     */
    void BubbleUpdating();

private:

    /**
     * First-phase constructor.
     *
     * @param aBubbleManager Reference to bubble manager
     * @param aUid Plugin UID
     * @param aPriority Priority of the plugin
     */
    CPhoneBubbleExtension( CBubbleManager& aBubbleManager, TUid aUid, TUint8 aPriority );

    void ConstructL();

private: // data

    /**
     * Bubble manager reference
     */
    CBubbleManager& iBubbleManager;

    /**
     * Plugin UID
     */
    const TUid iUid;

    /**
     * Plugin priority
     */
    const TUint8 iPriority;

    /**
     * Loaded plugin.
     * Own.
     */
    CTelBubbleExtension* iPlugin;

    };

#endif // C_CPHONEBUBBLEEXTENSION_H
