/*
* Copyright (c) 2007 - 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CDialingExtensionObserver provides an observer class for 
*               dialingextension plug-in.
*
*/

#ifndef CDIALINGEXTENSIONOBSERVER_H
#define CDIALINGEXTENSIONOBSERVER_H

#include <dialingextensioninterface.h>

class CDialingExtensionInterface;
class CDialerNumberEntry;
class CDialer;


NONSHARABLE_CLASS( CDialingExtensionObserver ) : public CBase, public MDialingExtensionObserver
    {
public:
    
    /**
    * Two phase constructor
    * @param aDialingExtension  Dialer extension. Does not take ownership.
    * @param aNumberEditor      Number editor. Does not take ownership.
    * @param aDialer            Dialer. Does not take ownership.
    * @return New instance
    */
    static CDialingExtensionObserver* NewL( 
            CDialingExtensionInterface* aDialingExtension, 
            CDialerNumberEntry* aNumberEntry,
            CDialer* aDialer );
    
    /**
     * Destructor
     */
    ~CDialingExtensionObserver();
    
    /**
    * MDialingExtensionObserver function.
    */
    void HandleDialingExtensionEvent( MDialingExtensionObserver::TEvent aEvent );
    
private:
    
    
    /**
    * Cconstructor
    * @param aDialingExtension  Dialer extension. Does not take ownership.
    * @param aNumberEditor      Number editor. Does not take ownership.
    * @param aDialer            Dialer. Does not take ownership.
    */
    CDialingExtensionObserver( 
            CDialingExtensionInterface* aDialingExtension, 
            CDialerNumberEntry* aNumberEntry,
            CDialer* aDialer );
    
    void UpdateCba();
    
    void SearchL();
    
private:
    
    // Not owned.
    CDialingExtensionInterface* iDialingExtension;
    
    // Not owned.
    CDialerNumberEntry* iNumberEntry;
    
    // Not owned.
    CDialer* iDialer;
    
    // Not owned.
    CEikonEnv* iEikonEnvironment;    
    };

#endif // CDIALINGEXTENSIONOBSERVER_H

