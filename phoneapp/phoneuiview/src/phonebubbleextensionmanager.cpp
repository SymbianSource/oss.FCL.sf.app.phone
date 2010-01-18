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


#include <badesca.h>
#include <ecom.h>
#include <telbubbleextension.h>

#include "phonebubbleextension.h"
#include "phonebubbleextensionmanager.h"
#include "phonebubbleextensiondata.h"

const TUint8 KDefaultPriority(255);

/**
* This is a support class that is used to carry relation of plugin UId and the
* assigned priority. Used inside an ordered array only. 
*/
class TPriorityItem
{
public:
    TPriorityItem( const TUid aUid, const TUint8 aPriority )
            : iUid(aUid),iPriority(aPriority){};
    const TUid Uid() const
            { return iUid; };
    TUint8 Priority() const
            { return iPriority; };
    TBool operator==( const TPriorityItem& aOther ) const
            { return iUid == aOther.Uid(); };
    TBool operator<( const TPriorityItem& aOther ) const
            { return iUid.iUid < aOther.Uid().iUid; };
    static TInt Compare( const TPriorityItem& aFirst, 
            const TPriorityItem& aSecond )
            {
            if( aFirst < aSecond )
                {
                return -1;
                }
            if( aFirst == aSecond )
                {
                return 0;
                }
            return 1;
            };
private:
    TPriorityItem();
    
private:
    const TUid iUid;
    const TUint8 iPriority;
};


// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// ECOM array cleanupstack support function
// ---------------------------------------------------------------------------
//
void CleanupEComArray(TAny* aArray)
    {
    (static_cast<RImplInfoPtrArray*> (aArray))->ResetAndDestroy();
    (static_cast<RImplInfoPtrArray*> (aArray))->Close();
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Two-phase constructor
// ---------------------------------------------------------------------------
//
CPhoneBubbleExtensionManager* CPhoneBubbleExtensionManager::NewL(
    CBubbleManager& aBubbleManager )
    {
    CPhoneBubbleExtensionManager* self = 
        CPhoneBubbleExtensionManager::NewLC( aBubbleManager );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two-phase constructor
// ---------------------------------------------------------------------------
//
CPhoneBubbleExtensionManager* CPhoneBubbleExtensionManager::NewLC(
    CBubbleManager& aBubbleManager )
    {
    CPhoneBubbleExtensionManager* self = 
        new( ELeave ) CPhoneBubbleExtensionManager( aBubbleManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
//
CPhoneBubbleExtensionManager::~CPhoneBubbleExtensionManager()
    {
    Reset();
    iPriorityArray.Close();
    }

// ---------------------------------------------------------------------------
// Notifies the plugin framework about a new call.
// ---------------------------------------------------------------------------
//
void CPhoneBubbleExtensionManager::StartCallL( 
    TInt aBubbleId, 
    TPhoneCmdParamCallHeaderData* aParams )
    {
    if ( !iInitialized )
        {
        // Load plugins (happens on first call)
        InitializeL();
        }
    if ( iPlugins.Count() > 0 ) // Call added only if any plugins
        {
        // create call data
        CPhoneBubbleExtensionData* callData = CPhoneBubbleExtensionData::NewLC( 
                aBubbleId,
                aParams,
                ( iCalls.Count() == 0 ) );
        
        // insert to array
        iCalls.AppendL( callData ); // ownership transferred
        CleanupStack::Pop( callData );
        
        // notify all plugins
        const TUint pluginCount = iPlugins.Count();
        for ( TUint index(0); index < pluginCount; index++ )
            {
            iPlugins[index]->StartCustomizedBubble( *callData );
            }
        }
    }

// ---------------------------------------------------------------------------
// Notifies the plugin framework about cleared call.
// ---------------------------------------------------------------------------
//
void CPhoneBubbleExtensionManager::StopCall( TInt aBubbleId)
    {
    // find call data
    TBool found(EFalse);
    TUint callIndex(0);
    const TUint callCount = iCalls.Count();
    while ( callIndex < callCount && !found )
        {
        found = ( iCalls[callIndex]->BubbleId() == aBubbleId );
        if ( !found )
            {
            callIndex++;
            }
        }
    if ( found )
        {
        // notify all plugins
        const TUint pluginCount = iPlugins.Count();
        for ( TUint index(0); index < pluginCount; index++ )
            {
            iPlugins[index]->StopCustomizedBubble( *iCalls[callIndex] );
            }
        // destroy call data
        delete iCalls[callIndex];
        iCalls.Remove( callIndex );
        }
    }

// ---------------------------------------------------------------------------
// Notifies the plugin framework about changed call state.
// ---------------------------------------------------------------------------
//
void CPhoneBubbleExtensionManager::UpdateCallState( 
    TInt aBubbleId, 
    TInt aNewState )
    {
    // find call data
    TBool found(EFalse);
    TUint callIndex(0);
    const TUint callCount = iCalls.Count();
    while ( callIndex < callCount && !found )
        {
        found = ( iCalls[callIndex]->BubbleId() == aBubbleId );
        if ( !found )
            {
            callIndex++;
            }
        }
    if ( found )
        {
        // modify call data
        iCalls[callIndex]->SetState( aNewState, ( iCalls.Count() == 0 ) );
        // notify all plugins
        const TUint pluginCount = iPlugins.Count();
        for ( TUint index(0); index < pluginCount; index++ )
            {
            iPlugins[index]->BubbleUpdating();
            }
        }
    }

// ---------------------------------------------------------------------------
// Loads all plugins
// ---------------------------------------------------------------------------
//
void CPhoneBubbleExtensionManager::InitializeL()
    {
    if ( !iInitialized )
        {
        // load list of plugins
        RImplInfoPtrArray pluginArray;
        TCleanupItem cleanup( CleanupEComArray, &pluginArray );
        CleanupStack::PushL( cleanup );
        REComSession::ListImplementationsL(
                KTelBubbleExtensionInterfaceUid, 
                pluginArray);
    
        // loop through the list, load plugin and insert to array 
        CPhoneBubbleExtension* plugin;
        const TUint count( pluginArray.Count() );
        for ( TUint index(0); index < count; index++ )
            {
            // Plugin UID
            TUid uid = pluginArray[index]->ImplementationUid();
            // Plugin priority
            TUint8 priority( KDefaultPriority ); // for unknown plugins 
            TInt priorityIndex = iPriorityArray.FindInOrder(
                    TPriorityItem( uid, 0 ),
                    TLinearOrder<TPriorityItem>( TPriorityItem::Compare ));
            if ( priorityIndex >= 0)
                {
                // Priority for this plugin found.
                priority = iPriorityArray[priorityIndex].Priority();
                }

            // Load plugin:
            TRAP_IGNORE( 
                plugin = CPhoneBubbleExtension::NewLC( iBubbleManager, 
                                                       uid, 
                                                       priority );

                // Plugin was loaded successfully
                iPlugins.AppendL( plugin ); // ownership trasferred
                CleanupStack::Pop( plugin );
                );
            }
    
        // Clean up
        CleanupStack::PopAndDestroy(); // pluginArray
        iInitialized = ETrue;
        }
    }

// ---------------------------------------------------------------------------
// First phase constructor
// ---------------------------------------------------------------------------
//
CPhoneBubbleExtensionManager::CPhoneBubbleExtensionManager(
    CBubbleManager& aBubbleManager )
    :iBubbleManager( aBubbleManager ), iInitialized(EFalse)
    {
    // no implementation needed
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhoneBubbleExtensionManager::ConstructL()
    {
    // Load priorities
    LoadPrioritiesL();

    // Currently, the plugins are loaded when the first call happens,
    // not during construction. This avoids the situation where phone might
    // panic during boot up if there's a bad plugin which panics when loaded.
    }

// ---------------------------------------------------------------------------
// Loads plugin priorities.
// ---------------------------------------------------------------------------
//
void CPhoneBubbleExtensionManager::LoadPrioritiesL()
    {
    // Currently plugin priorities are not in use. No array is loaded,
    // therefore all plugins get default priority as the priority is not
    // found during the loading.
    // Here's an example implementation how to insert loaded priority data
    // (UID + priority pairs) into the array:
    // iPriorityArray.InsertInOrderL(
    //        TPriorityItem( TUid::Uid(0x12345789), 5 ),
    //        TLinearOrder<TPriorityItem>( TPriorityItem::Compare ));

    }

// ---------------------------------------------------------------------------
// Unloads all plugins, closes all calls
// ---------------------------------------------------------------------------
//
void CPhoneBubbleExtensionManager::Reset()
    {
    // Close all calls
    for ( TInt callIndex(0); callIndex < iCalls.Count(); callIndex++ )
        {
        // notify all plugins
        const TUint pluginCount = iPlugins.Count();
        for ( TUint index(0); index < pluginCount; index++ )
            {
            iPlugins[index]->StopCustomizedBubble( *iCalls[callIndex] );
            }
        // destroy call data
        delete iCalls[callIndex];
        }
    iCalls.Close();

    // Unload plugins
    for ( TInt index(0); index < iPlugins.Count(); index++ )
        {
        delete iPlugins[index];
        }
    iPlugins.Close();
    
    iInitialized = EFalse;
    }

