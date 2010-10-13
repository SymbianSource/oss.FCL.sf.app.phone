/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Ecom interface header
*
*/

#ifndef __CDIALEREXTENSIONINTERFACE_H
#define __CDIALEREXTENSIONINTERFACE_H

#include <ecom/ecom.h>
#include <coecntrl.h>
#include <eikmenup.h>


/** UID of dialing extension ECom interface. */
const TUid KEasyDialingPluginInterfaceUID = { 0x200212A1 };

/** UID of easy dialing plugin. */
const TUid KEasyDialingPluginImplementationUID = { 0x200212A0 };

/**
 * Observer class for easy dialing.
 */
class MDialingExtensionObserver
    {
public:
    
    /**
     * Identifiers for easy dialing events.
     */
    typedef enum TEvent_enum
        {
        
        /** Contact search has completed. */
        ESearchComplete = 1,
        
        /** Easy dialing has gained or given up the focus. */
        EFocusChanged,
        
        /** Easy dialing has started communication launcher (a.k.a. contact view) */
        ECCALauncherStarted,
        
        /** Communication launcher a.k.a. contact view has been close. */
        ECCALauncherExit,
        
        /** Easy dialing feature has been switched on. */
        EEasyDialingEnabled,
        
        /** Easy dialing feature has been switched off. */
        EEasyDialingDisabled,
        
        /** User has started communication (SMS, IM, E-mail ...). Not used for phone calls. */
        ECommunicationStarted,
        
        /** User has cancelled communication once started. */
        ECommunicationCancelled
       
        } TEvent;


    /**
     * Handles event from easy dialing plugin.
     * 
     * @param aEvent    Event id.
     * 
     */
    virtual void HandleDialingExtensionEvent( MDialingExtensionObserver::TEvent aEvent ) = 0;
    };




/**
 * Easy dialing plugin class.
 */

class CDialingExtensionInterface : public CCoeControl
    {
public:
    
    /**
     * Creates an instance of plugin.
     */
    static CDialingExtensionInterface* NewL();


    /**
     * Destructor.
     */
    virtual ~CDialingExtensionInterface();
    
    
    /**
     * Does the rest of the initialization of the plugin.
     * Must be called after NewL before using any other 
     * functions of the plugin.
     * 
     * @param aParent   Parent container of the plugin.
     */
    virtual void InitializeL( CCoeControl& aParent ) = 0;
    

    /**
     * Resets the previous search results. 
     */
    virtual void Reset() = 0;
    
        
    /**
     * Adds an observer to the plugin.
     * 
     * @param aObserver Observer to be added.
     */
    virtual void AddObserverL( MDialingExtensionObserver* aObserver ) = 0;
   
    
    /**
     * Removes an observer from the plugin.
     * 
     * @param aObserver Observer to be removed.
     */
    virtual void RemoveObserver( MDialingExtensionObserver* aObserver ) = 0;

    
    /**
     * Sets input string (search string) to plugin.
     * 
     * @param aDesC Input string descriptor.
     */
    virtual void SetInputL( const TDesC& aDesC ) = 0;


    /**
     * Gets the resource id of plugin's control button area resource.
     * 
     * @return  Resource id.
     */
    virtual TInt CbaResourceId( ) = 0;
    
    
    /**
     * Gets the resource id of plugin's menu bar resource.
     * 
     * @return  Resource id.
     */
    virtual TInt MenuResourceId() = 0;
    
    
    /**
     * Gets the resource id of plugin's menu bar resource.
     * 
     * @param   aMenuPane   Menu pane
     * @param   aMenuResourceId Menu bar resource id.
     * @return  Resource id.
     */
    virtual TBool InitializeMenuPaneL( CEikMenuPane& aMenuPane, TInt aMenuResourceId ) = 0;

    
    /**
     * Handle a command. Supported command IDs are defined in header
     * easydialingcommands.hrh.
     * 
     * @param   aCommand   Command id.
     * @return  ETrue if command was handled, EFalse otherwise.
     */
    virtual TBool HandleCommandL( TInt aCommand ) = 0;

    
    /**
     * Tells if extension is currently enabled from settings.
     * @return  ETrue if extension is enabled, EFalse otherwise.
     */
    virtual TBool IsEnabled() const = 0;
    
private: // data

    /** ECom instance identifier key. */
    TUid iDestructorIDKey;
    };



// INLINE FUNCTIONS

// -----------------------------------------------------------------------------
// CDialingExtensionInterface::NewL
//
// -----------------------------------------------------------------------------
//
inline CDialingExtensionInterface* CDialingExtensionInterface::NewL()
    {
    // TEComResolverParams resolverParams; // data passed to the plugin
    // resolverParams.SetDataType();    // no data yet

    TAny* ptr = REComSession::CreateImplementationL( KEasyDialingPluginImplementationUID,
                                                     _FOFF( CDialingExtensionInterface, iDestructorIDKey )
                                                     // resolverParams 
                                                    );

    return STATIC_CAST( CDialingExtensionInterface*, ptr );
    }


// -----------------------------------------------------------------------------
// CDialingExtensionInterface::~CDialingExtensionInterface()
//
// -----------------------------------------------------------------------------
//
inline CDialingExtensionInterface::~CDialingExtensionInterface()
    {
    REComSession::DestroyedImplementation( iDestructorIDKey );
    }
    
#endif //__CDIALEREXTENSIONINTERFACE_H

// End of file
