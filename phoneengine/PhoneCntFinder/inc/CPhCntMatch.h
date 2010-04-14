/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation for single match result.
*
*/


#ifndef CPHCNTMATCH_H
#define CPHCNTMATCH_H

//  INCLUDES
#include "mphcntmatch.h"

// FORWARD DECLARATIONS
class CPhCntMatchItem;

// CLASS DECLARATION

/**
*  Implementation for single match result.
*  Passes all function calls to CPhCntMacthItem object.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
NONSHARABLE_CLASS( CPhCntMatch ) : public CBase, public MPhCntMatch
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @param aItem Match data to use
        * @return New instance
        */
        static CPhCntMatch* NewL( CPhCntMatchItem& aItem );
        
        /**
        * Destructor.
        */
        virtual ~CPhCntMatch();

    protected: // Functions from base classes

        /**
        * From MPhCntMatch
        */ 
        void Release();

        /**
        * From MPhCntMatch
        */ 
        CPhCntContactId* ContactId() const;

        /**
        * From MPhCntMatch
        */ 
        TInt NumberType() const;

        /**
        * From MPhCntMatch
        */ 
        TCliType Cli( HBufC*& aCliText ) const;

        /**
        * From MPhCntMatch
        */ 
        TPtrC FirstName() const;

        /**
        * From MPhCntMatch
        */ 
        TPtrC LastName() const;

        /**
        * From MPhCntMatch
        */ 
        TPtrC CompanyName() const;

        /**
        * From MPhCntMatch
        */ 
        TPtrC Number() const;

        /**
        * From MPhCntMatch
        */ 
        TPtrC PersonalRingingTone() const;

        /**
        * From MPhCntMatch
        */ 
        TBool BelongsToGroups( 
            const CArrayFix<TContactItemId>& aGroupArray ) const;

        /**
        * @see MPhCntMatch::TextToSpeechTextL.
        * @since Series 60 3.0
        */
        virtual HBufC* TextToSpeechTextL() const;
        
        /**
        * From MPhCntMatch
        * @since Series 60 3.1
        */ 
        TPtrC CallText() const;
        
       	/**
        * From MPhCntMatch
        * @since Series 60 3.1
        */ 
        TPtrC CallImage() const;
        
        /**
        * From MPhCntMatch
        * @since Series 60 3.1
        */ 
        TBool HasThumbnailImage() const;

    private:

        /**
        * C++ constructor.
        */
        CPhCntMatch( CPhCntMatchItem& aItem );

    private:    // Data
        // Match data
        CPhCntMatchItem& iItem;

    };

#endif      // CPHCNTMATCH_H   
            
// End of File
