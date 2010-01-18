/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contact identifier used with virtual phonebook
*
*/


#ifndef CPHCNTVPBKCONTACTID_H
#define CPHCNTVPBKCONTACTID_H

#include "cphcntcontactid.h"

class MVPbkContactLink;
class MPhCntContactManager;

/**
 *  Contact identifier.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntVPbkContactId ) : public CPhCntContactId
    {
public:

    /**
     * Static constructor.
     * 
     * @since S60 v3.1
     * @param aContactLink Contact link, takes ownership.
     * @param aContactManager Contact manager.
     */
    static CPhCntVPbkContactId* NewL( 
        MVPbkContactLink* aContactLink,
        MPhCntContactManager& aContactManager );

    /**
     * Static constructor.
     * 
     * @since S60 v3.1
     * @param aContactLink Contact link, takes ownership.
     * @param aContactManager Contact manager.
     */
    static CPhCntVPbkContactId* NewLC( 
        MVPbkContactLink* aContactLink,
        MPhCntContactManager& aContactManager );
    
    /**
     * Static constructor.
     * 
     * @since S60 v3.1
     * @param aContactId Contact identifier.
     * @param aContactManager Contact manager.
     */
    static CPhCntVPbkContactId* NewL( 
        TContactItemId aContactId,
        MPhCntContactManager& aContactManager );
    
    /**
     * Static constructor.
     * 
     * @since S60 v3.1
     * @param aContactId Contact identifier.
     * @param aContactManager Contact manager.
     */
    static CPhCntVPbkContactId* NewLC( 
        TContactItemId aContactId,
        MPhCntContactManager& aContactManager );
        
    /** 
     * Static constructor.
     * 
     * @since S60 v3.2
     * @param aContactLink MVPbkContactLink as descriptor.
     * @param aContactManager.
     */
    static CPhCntVPbkContactId* NewL( 
        const TDesC8& aContactLink,
        MPhCntContactManager& aContactManager );

        
    /**
     * Destructor. 
     *
     * @since S60 v3.1
     */
    virtual ~CPhCntVPbkContactId();

    /**
     * Gives access to contact link, which
     * identifies a contact in contact store.
     * @precond IsValid() returns ETrue
     * @since S60 v3.1
     * @return Contact link, which identifies contacts.
     */
    const MVPbkContactLink& ContactLink() const;

// from base CPhCntContactId

     /**
      * From CPhCntContactId
      *
      * @since S60 v3.1
      * @see CPhCntContactId
      */
     CPhCntContactId* CloneL() const;
     
     /**
      * From CPhCntContactId
      *
      * @since S60 v3.1
      * @see CPhCntContactId
      */
     void Invalidate();
      
     /**
      * From CPhCntContactId
      *
      * @since S60 v3.1
      * @see CPhCntContactId
      */
     TBool IsValid() const;
     
     /**
      * From CPhCntContactId
      *
      * @since S60 v3.1
      * @see CPhCntContactId
      */
     TContactItemId ContactId() const;
      
     /**
      * From CPhCntContactId
      *
      * @since S60 v3.1
      * @see CPhCntContactId
      */
     HBufC8* PackLC() const;
      

private:

    CPhCntVPbkContactId( 
        MVPbkContactLink* aContactLink,
        MPhCntContactManager& aContactManager );

    void ConstructL( TContactItemId aContactItemId );
    
    void ConstructL( const TDesC8& aContactLink );

private: // data

    /**
     * Contact link, which identifies the contact.
     * Own.
     */
    MVPbkContactLink* iContactLink;
     
    /**
     * Contact manager for converting contact id to contact link.
     * Not own.
     */
    MPhCntContactManager& iContactManager;     

    };



#endif // CPHCNTVPBKCONTACTID_H
