/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  It provides functionality to exit the foreground application.
*
*/



#ifndef CPHONEAPPLICATIONEXIT_H
#define CPHONEAPPLICATIONEXIT_H

//  INCLUDES
#include    <e32base.h>

// FORWARD DECLARATIONS
class CPhoneViewController;
class RWsSession;

// CLASS DECLARATION

/**
*  It provides functionality to exit the foreground application.
*
*  @since Series 60 2.6
*/
class CPhoneApplicationExit 
    : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aViewController Phone UI View controller
        * @param aWsSession window server session.
        * @param aUikonWgId UIKON server window group id.
        * @return new CPhoneApplicationExit instance.
        */
        static CPhoneApplicationExit* NewL(
            CPhoneViewController* aViewController,
            RWsSession& aWsSession,
            const TInt aUikonWgId );
        
        /**
        * Destructor.
        */
        virtual ~CPhoneApplicationExit();

    public: // New functions
        
        /**
        * Exits the active application.
        * @return error code.
        */
        TInt ExitApplication();

    private:

        // Inner class.
        class CElement;

        // Basic cases.
        class CElementBasic;

        // UID range element.
        class CElementUid;

        // Drive element.
        class CElementDrive;

        /**
        * C++ constructor.
        * @param aViewController Phone UI View controller
        * @param aWsSession window server session.
        * @param aUikonWgId UIKON server window group id.
        */
        CPhoneApplicationExit(
            CPhoneViewController* aViewController,
            RWsSession& aWsSession,
            const TInt aUikonWgId );

        /**
        * Exits the application.
        */
        void ExitApplicationL();

        /**
        * Checks if application should be closed using product
        * specific variation variables.
        * @param aUid application UID.
        * @return ETrue if application should be closed.
        */
        TBool CheckApplicationClosingL( const TUid& aUid );

        /**
        * Checks if included to "close" list.
        * @param aUid application UID.
        * @return ETrue if included.
        */
        TBool IsIncludedToCloseListL( const TUid& aUid ) const;

        /**
        * Checks if included to "don't close" list.
        * @param aUid application UID.
        * @return ETrue if included.
        */
        TBool IsIncludedToDontCloseListL( const TUid& aUid ) const;

        /**
        * Checks if application UID is included to the list.
        * @param aSource list to be checked.
        * @param aUid application UID.
        * @return ETrue if application UID is included to the list.
        */
        TBool CheckListL( 
            const RPointerArray< CElement >& aSource, 
            const TUid& aUid ) const;

        /**
        * Reads lists from variation variables.
        */
        void ReadListsL();

        /**
        * Builds list from text description.
        * @param aTarget it will contain list of elements.
        * @param aSource source description.
        * @param aOomAllowed ETrue if OOM is allowed in the description.
        */
        void BuildListL(
            RPointerArray< CElement >& aTarget,
            const TDesC& aSource,
            TBool aOomAllowed );

        /**
        * Parses string from lexer.
        * @param aLexer lexer.
        * @param aString string to be checked.
        * @return ETrue if string was next element.
        */
        TBool ParseString( 
            TLex& aLexer,
            const TDesC& aString );

        /**
        * Parses UID from lexer.
        * @param aLexer lexer.
        * @param aUidStart it will contain UID.
        * @param aUidEnd it will contain UID. 
        * @return ETrue if UID or UID range was next element.
        */
        TBool ParseUid( 
            TLex& aLexer, 
            TUid& aUidStart,
            TUid& aUidEnd );
        
        /**
        * Parses driver identifier from lexerl.
        * @param aLexer lexer.
        * @param aChar it will contain drive character.
        * @return ETrue if drive identifier was next element.
        */
        TBool ParseDrive(
            TLex& aLexer, 
            TChar& aChar );

        /**
        * Checks that the element ends appropriately - 
        * next character must be space or then input has ended.
        * @param aLexer lexer.
        * @return ETrue if element ends appropriately.
        */
        inline TBool CheckEnd( TLex& aLexer );
        
        /**
        * Checks if application closing is enabled.
        * @return ETrue if application closing is enabled.
        */
        TBool IsApplicationClosingEnabled() const;

    private:    // Data

        // Reference to Phone UI View Controller.
        CPhoneViewController* iViewController;

        // Reference to window server session.
        RWsSession& iWsSession;

        // UIKON server window group id.
        const TInt iUikonWgId;

        // ETrue if product specific variables have been read.
        TBool iListsRead;

        // Owned product specific "close" list.
        RPointerArray<CElement> iCloseList;
        
        // Owned product specific "don't close" list.
        RPointerArray<CElement> iDontCloseList;
        
    };

#endif // CPHONEAPPLICATIONEXIT_H

// End of File
