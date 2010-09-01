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
* Description: Definition of CPEContactExtensionWrapper class.
*
*/



#ifndef C_CPECONTACTEXTENSIONWRAPPER_H
#define C_CPECONTACTEXTENSIONWRAPPER_H

#include <e32base.h>
#include <telmatchingextension.h>

/**
 *  Provides container friendly interface for extension plugin handling.
 *    
 *  @lib loghandling.dll
 *  @since S60 v5.1
 */
NONSHARABLE_CLASS( CPEContactExtensionWrapper ) : public CTelMatchingExtension
    {

public:

    /**
     * Two-phased constructor.
     * @param   aImplementationUid  An Ecom plugin implementation identifier.
     */
    static CPEContactExtensionWrapper* NewL( TUid aImplementationUid );

    /**
     * Two-phased constructor.
     * @param   aImplementationUid  An Ecom plugin implementation identifier.
     */
    static CPEContactExtensionWrapper* NewLC( TUid aImplementationUid );

    /**
    * Destructor.
    */
    virtual ~CPEContactExtensionWrapper();

    /**
     * Provides comparison algorithm for CPEContactExtensionWrapper objects using
     * extension plugin implementation identifier as a search key.
     * 
     * @param   aKey        A search key.
     * @param   aWrapper    An object to match with a key.
     * @since   S60 v5.1
     * @return  ETrue if match, EFalse otherwise.
     */
    static TBool MatchByUid( 
        const TUid* aKey, const CPEContactExtensionWrapper& aWrapper );
    
    // from base class CTelMatchingExtension
    /**
    * From CTelMatchingExtension.
    * @see CTelMatchingExtension.
    */
    void InitializeL( TUint aServiceId, const TDesC& aOrigAddress );
    
    /**
    * From CTelMatchingExtension.
    * @see CTelMatchingExtension.
    */
    TInt GetAddressForMatching( RBuf& aParsedAddress, TInt& aMeaningfulDigits );

    /**
    * From CTelMatchingExtension.
    * @see CTelMatchingExtension.
    */
    TInt GetContactStoreUris( CDesCArray& aStoreUris );

    /**
    * From CTelMatchingExtension.
    * @see CTelMatchingExtension.
    */
    TInt GetRemotePartyName( RBuf& aRemotePartyName );

private:

    CPEContactExtensionWrapper() {};
    
    CPEContactExtensionWrapper( TUid aImplementationUid );

    void ConstructL();
    
    /**
     * Creates contact extension plugin.
     * @since   S60 v5.1
     * @return A new contact extension plugin instance.
     */
    CTelMatchingExtension* CreateContactExtensionL() const;
    
    /**
     * Returns identifier of the wrapped extension plugin.
     * @since   S60 v5.1
     * @return Identifier of the extension plugin.
     */
    TUid Identifier() const;

private: // data

    /**
     * Extension plugin identifier.
     */
    TUid iPluginUid;

    /**
     * Contact matching extension plugin.
     * Own.
     */
    CTelMatchingExtension* iPlugin;

    };

#endif // C_CPECONTACTEXTENSIONWRAPPER_H
