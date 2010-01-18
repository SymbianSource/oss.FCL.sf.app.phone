/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Dummy details dialog for global implementation
*
*/



#ifndef CVMDETAILSDLGDUMMY_H
#define CVMDETAILSDLGDUMMY_H

//  INCLUDES
#include <e32base.h>


// CLASS DECLARATION

/**
*  Dummy details dialog for global implementation
*
*  Usage:
*  CVmDetailsDialog* dlg = CVmDetailsDialog::NewL();
*  dlg->ExecuteLD( R_VMBX_DETAIL_DIALOG );
*
*  @lib 
*  @since 2.7
*/
class CVmDetailsDialog : public CBase
    {
    public:  // Constructors and destructor
        
       /**
        * Two-phased constructor.
        */
        static CVmDetailsDialog* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CVmDetailsDialog();
        
    private:
        
       /**
        * C++ default constructor.
        */
        CVmDetailsDialog();
        
    public:
    
        /**
        * Executes dummy dialog
        *
        * @param aResourceId Resource id
        * @return Returns 0
        */        
        TInt ExecuteLD( TInt aResourceId );
    };

#endif // CVMDETAILSDLGDUMMY_H
