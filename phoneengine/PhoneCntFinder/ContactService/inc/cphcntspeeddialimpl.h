/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of CPhCntSpeedDialImpl class.
*
*/


#ifndef CPHCNTSPEEDDIALIMPL_H
#define CPHCNTSPEEDDIALIMPL_H

#include    <e32base.h>
#include    "CPhCntSpeedDialMonitor.h"

class CSpdiaDialogs;
class MPhCntContactManager;
class CPhCntSpeedDialContactLinkFetch;
class CPhCntFetchContact;
class CPhCntContactStores;
class CPhCntContact;
class CPhCntSpeedDialMonitor;
class MPhoneCntPbkOwner;
class CEikStatusPane;

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
NONSHARABLE_CLASS( CPhCntSpeedDialImpl ) 
    : public CPhCntSpeedDialMonitor
    {
    public:
        
        static CPhCntSpeedDialImpl* NewL( MPhoneCntPbkOwner& aPbkOwner );

        static CPhCntSpeedDialImpl* NewLC( MPhoneCntPbkOwner& aPbkOwner );

        virtual ~CPhCntSpeedDialImpl();
    
    public: // from base class CPhCntSpeedDialMonitor

         /**
         * From base class CPhCntSpeedDialMonitor
         * @since S60 v3.2
         * @see CPhCntSpeedDialMonitor
         */
        void GetSpeedDialFieldInfoL( TInt aSpeedDialPosition,
            TSpdDialFieldInfo& aFieldInfo ); 
         
        /**
         * From base class CPhCntSpeedDialMonitor
         * @since S60 v3.2
         * @see CPhCntSpeedDialMonitor
         */
        TInt GetSpeedDialFieldL( 
            TInt aSpeedDialPosition, 
            TDes& aPhoneNumber );

        /**
         * From base class CPhCntSpeedDialMonitor
         * @since S60 v3.2
         * @see CPhCntSpeedDialMonitor
         */
        TInt GetSpeedDialFieldL(
            TInt aSpeedDialPosition, 
            TDes& aPhoneNumber,
            TSpdDialFieldInfo& aFieldInfo );
            
            
        /**
         * From base class CPhCntSpeedDialMonitor
         * @since S60 v3.2
         * @see CPhCntSpeedDialMonitor
         */
        TInt AssignSpeedDialFieldL(
            TInt aSpeedDialPosition,
            TDes& aPhoneNumber );

        /**
         * From base class CPhCntSpeedDialMonitor
         * @since S60 v3.2
         * @see CPhCntSpeedDialMonitor
         */
        TInt AssignSpeedDialFieldL(
            TInt aSpeedDialPosition,
            TDes& aPhoneNumber,
            TSpdDialFieldInfo& aFieldInfo );
            
        /**
         * From base class CPhCntSpeedDialMonitor
         * @since S60 v3.2
         * @see CPhCntSpeedDialMonitor
         */
        virtual void Cancel();
        
        /**
         * From base class CPhCntSpeedDialMonitor
         * @since S60 v3.2
         * @see CPhCntSpeedDialMonitor
         */
        virtual CEikImage* CreateNumberTypeIconLC( TInt aNumberType );

    private:

        CPhCntSpeedDialImpl( MPhoneCntPbkOwner& aPbkOwner );

        void ConstructL();

        TInt FetchNumberL( TInt aSpeedDialPosition, TDes& aPhoneNumber );
                
        TInt FetchContact( TInt aSpeedDialPosition, CPhCntContact*& aContact );
        
        void CopyNumberL( TDes& aAppendTo, const TDesC& aNumber );
        
        /**
         * Setups the field info with new attributes.
         * @param aContact Contact information
         * @param aSpeedDialPosition A speed dial position.
         * @param aFieldInfo Speed dial contact information.
         */
        void CopyContactInfoToFieldInfoL( 
            CPhCntContact& aContact,
            TInt aSpeedDialPosition, 
            TSpdDialFieldInfo& aFieldInfo );

    private: // data

        /**
         * Contact manager for accessing virtual phonebook services.
         * Not own.
         */
        MPhCntContactManager& iContactManager;
        
        /**
         * Phonebook owner
         * Own.
         */ 
        MPhoneCntPbkOwner& iPbkOwner;

        /**
         * Fetches speed dial contact link.
         * Own.
         */
        CPhCntSpeedDialContactLinkFetch* iSpeedDialContactLinkFetcher;
        
        /**
         * Fetches speed dial contact using speed dial contact link.
         * Own.
         */
        CPhCntFetchContact* iContactFetcher;
        
        /**
         * Service for contact fetching.
         * Own.
         */
        CPhCntContactStores* iContactStores;
        
        /**
         * Speed Dial Dialogs.
         * Own.
         */
        CSpdiaDialogs* iSpdDial;   
    };


#endif // CPHCNTSPEEDDIALIMPL_H
