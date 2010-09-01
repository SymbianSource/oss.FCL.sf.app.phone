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
* Description:  Manages the call bubble extension plugins.
*
*/


#ifndef C_CPHONEBUBBLEEXTENSIONMANAGER_H
#define C_CPHONEBUBBLEEXTENSIONMANAGER_H

class TPhoneCmdParamCallHeaderData;
class CPhoneBubbleExtensionData;
class CPhoneBubbleExtension;
class CBubbleManager;
class TPriorityItem;

/**
 * Bubble extension plugin manager.
 * Keeps track of the extension plugins.
 *
 *  @lib ?library
 *  @since S60 5.0
 */
class CPhoneBubbleExtensionManager : public CBase
    {
public:

    /**
     * Two-phased constructor
     *
     * @param aBubbleManager Reference to bubble manager
     */
    static CPhoneBubbleExtensionManager* NewL( CBubbleManager& aBubbleManager );

    /**
     * Two-phased constructor
     *
     * @param aBubbleManager Reference to bubble manager
     */
    static CPhoneBubbleExtensionManager* NewLC( CBubbleManager& aBubbleManager );

    virtual ~CPhoneBubbleExtensionManager();

    /**
     * Notifies the plugin framework about a new call.
     *
     * @param aBubbleId Bubble id of the new call
     * @param aParams Call parameters
     */
    void StartCallL( TInt aBubbleId, TPhoneCmdParamCallHeaderData* aParams );

    /**
     * Notifies the plugin framework about cleared call.
     *
     * @param aId Bubble id of the call
     */
    void StopCall( TInt aBubbleId);
    
    /**
     * Notifies the plugin framework about changed call state.
     *
     * @param aId Bubble id of the call
     * @param aNewState New call state. (TPEState, not bubble state)
     */
    void UpdateCallState( TInt aBubbleId, TInt aNewState );

    /**
     * Loads all plugins
     */
    void InitializeL();
    
private:

    /**
     * First phase constructor
     *
     * @param aBubbleManager Reference to bubble manager
     */
    CPhoneBubbleExtensionManager( CBubbleManager& aBubbleManager );

    void ConstructL();

    /**
     * Loads plugin priorities.
     * TODO: Currently there is no plugin priorities, all plugins
     *       use the default priority. Nothing is loaded.
     */
    void LoadPrioritiesL();
    
    /**
     * Unloads all plugins, closes all calls
     */
    void Reset();
    
private: // data

    /**
     * Bubble manager reference
     */
    CBubbleManager& iBubbleManager;
    
    /**
     * Array that holds all the loaded plugins
     */
    RPointerArray<CPhoneBubbleExtension> iPlugins;

    /**
     * Array that holds all the call data
     */
    RPointerArray<CPhoneBubbleExtensionData> iCalls;

    /**
     * Are the plugins already loaded?
     */
    TBool iInitialized;

    /**
     * Plugin priorities
     */
    RArray<TPriorityItem> iPriorityArray;
    
    };

#endif // C_CPHONEBUBBLEEXTENSIONMANAGER_H
