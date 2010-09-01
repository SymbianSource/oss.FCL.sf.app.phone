/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class extracts phone number for comparison.
*
*/



#ifndef CCNTRAWPHONENUMBEREXTRACTOR_H
#define CCNTRAWPHONENUMBEREXTRACTOR_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class CContactPhoneNumberParser;
class CCntNumberParser;

// CONSTANTS
const TInt KPhMatcherMaximumPhoneNumberLength = 150;
const TInt KPhMatcherNumberCount = 7;


// CLASS DECLARATION

/**
*  MRawPhoneNumberExtractor
*
*  Abstact class for extraction.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
class MRawPhoneNumberExtractor 
    {
    public: // New functions
        /**
        * This should be called when instance is not needed any more.
        */
        virtual void Release() = 0;
        
        /**
        * DEPRECATED.
        */
        virtual TUint ExtractRawNumber(
            const TDesC& aTextualNumber ) = 0;
        
        /**
        * Extracts the given number for comparsion.
        * @param aTextualNumber Number to match for. 7 digits count.
        * @param aRawNumber Extracted number in descriptor.
        * @return Extracted number in integer (extra char at beginning).
        *         Equals extra char if given number is not taken to comparsion.
        */
        virtual TUint ExtractRawNumber(
            const TDesC& aTextualNumber, 
            TDes& aRawNumber ) = 0;
        
        /**
        * DEPRECATED.
        */
        virtual TChar ExtraChar() const = 0;
    };



/**
*  CCntRawPhoneNumberExtractor
*
*  Concrete extarctor class. 
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
class CCntRawPhoneNumberExtractor 
    : public CBase, 
      public MRawPhoneNumberExtractor 
    {
    public: // Constructor

        /**
        * Constructor.
        */
        IMPORT_C CCntRawPhoneNumberExtractor();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();
        
    public: // Functions from base classes

        /**
        * From MRawPhoneNumberExtractor
        */
        void Release();
        
        /**
        * From MRawPhoneNumberExtractor
        * DEPRECTED - Do not use.
        */
        TUint ExtractRawNumber( 
            const TDesC& aTextualNumber );
        
        /**
        * From MRawPhoneNumberExtractor
        */
        TUint ExtractRawNumber( 
            const TDesC& aTextualNumber, 
            TDes& aRawNumber );
        
        /**
        * From MRawPhoneNumberExtractor
        * DEPRECTED - Do not use.
        */
        TChar ExtraChar() const;

    private:

        /**
        * Destructor.
        */
        ~CCntRawPhoneNumberExtractor();
        
    private:    // Data
        // Buffer for data
        TBuf<KPhMatcherMaximumPhoneNumberLength> iMatchBuffer;

        // DEPRECATED.
        const TChar iKPhoneNumberExtraChar;

        // Parser library
        RLibrary iParserLibrary;

        // Parser itself
        CContactPhoneNumberParser* iParser;

        // Pointer to CCntNumberParser
        CCntNumberParser* iNumberParser;
    };


#endif //CCNTRAWPHONENUMBEREXTRACTOR_H

// End of File
