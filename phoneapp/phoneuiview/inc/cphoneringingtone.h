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
* Description:  Class representing ringing tone
*
*/



#ifndef CPHONERINGINGTONE_H
#define CPHONERINGINGTONE_H

//  INCLUDES
#include <e32base.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Ringing tone
*  Wrapper for file name. In addtion, provides methods
*  for ringing tone type identification. 
*
*  @lib Phone.app
*  @since Series 60 3.1
*/
class CPhoneRingingTone : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @param aFileName Ringing tone file name with path.
        * @param aDrmInPlayback ETrue to use IsFileDrmProtected().
        */
        static CPhoneRingingTone* NewL( 
            const TDesC& aFileName,
            TBool aDrmInPlayback = EFalse );
        
        /**
        * Destructor.
        */
        virtual ~CPhoneRingingTone();

    public: // New functions
        
        /**
        * Sets the file name including path.
        * @since Series 60 3.1
        * @param aFileName file name plus path.
        */
        void SetFileName( const TDesC& aFileName );
        
        /**
        * Returns file name including path.
        * @since Series 60 3.1
        * @return File name.
        */
        const TDesC& FileName() const;
        
        /**
        * Returns MIME type.
        * @since Series 60 3.1
        * @return MIME type.
        */
        const TDesC& MimeType() const;
        
        /**
        * Checks if video ringing tone.
        * @since Series 60 3.1
        * @return ETrue if video ringing tone.
        */
        TBool IsVideoRingingTone();
        
        /**
        * Checks if the file is a DCF file.
        * @since Series 60 3.1
        * @return ETrue if files is DRM protected.
        */
        TBool IsFileDrmProtected() const;
        
        /**
        * Checks if the file is in ROM.
        * @since Series 60 3.1
        */
        TBool IsFileInRom() const;
        
    private:

        /**
        * Refresh MIME type. This must be called
        * before using MimeType().
        */
        TInt RefreshMime();    
        
        /**
        * Refresh MIME type.
        */
        void RefreshMimeL();
        
        /**
        * Checks if file is located in video directory.
        */
        TBool IsFileInVideoDirectory() const;

    private:

        /**
        * C++ default constructor.
        */
        CPhoneRingingTone( TBool aDrmInPlayback );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( const TDesC& aFileName );

    private:    // Data

        // File name with path
        HBufC* iFileName;
        
        // MIME type
        HBufC* iMimeType;
        
        // Extended security
        TBool iDrmInPlayback;
    };

#endif      // CPHONERINGINGTONE_H   
            
// End of File
