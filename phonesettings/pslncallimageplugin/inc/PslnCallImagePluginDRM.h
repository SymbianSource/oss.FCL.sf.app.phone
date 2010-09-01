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
* Description:  Implementation for DRM functionality.
*
*/


#ifndef PSLNCALLIMAGEPLUGINDRM_H
#define PSLNCALLIMAGEPLUGINDRM_H

//  INCLUDES
#include <e32base.h>
#include <MMGFetchVerifier.h>

// FORWARD DECLARATIONS
class CDRMHelper;

// CLASS DECLARATION

/**
* Implementation for DRM functionality.
*
* @since 3.1
*
* @internal
*/
class CPslnCallImagePluginDRM : public CBase, public MMGFetchVerifier
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CPslnCallImagePluginDRM* NewL();
        
        /**
        * Destructor.
        */
        ~CPslnCallImagePluginDRM();

    public: // Functions from base classes

        /**
        * From MFLDFileObserver. Verifies that the selected image can be set.
        */
        TBool VerifySelectionL( const MDesCArray* aSelectedFiles );
     
    private:

        /**
        * C++ default constructor.
        */
        CPslnCallImagePluginDRM();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        void CPslnCallImagePluginDRM::ShowErrorNoteL( TInt  aResourceId  ) const;

    private:    // Data
    
        // Own: DRM helper
        CDRMHelper* iDRMHelper;
        
    };

#endif // PSLNCALLIMAGEPLUGINDRM_H
            
// End of File
