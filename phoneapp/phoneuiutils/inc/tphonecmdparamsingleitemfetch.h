/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Parameter class for single item fetch dialogs.
*
*/

#ifndef __TPHONECMDPARAMSINGLEITEMFETCH_H
#define __TPHONECMDPARAMSINGLEITEMFETCH_H

//  INCLUDES
#include <cphcntsingleitemfetch.h>
#include "tphonecommandparam.h"

// DATA TYPES

// CLASS DECLARATION

/**
*  A concrete parameter class for query dialogs.
*  
*/
class TPhoneCmdParamSingleItemFetch : public TPhoneUICommandParam
    {
    public:  
        
        /**
        * C++ default constructor.
        */
        IMPORT_C TPhoneCmdParamSingleItemFetch();

    public: 
        
        /**
        * Sets the single item fetch type
        * @param fetch type
        */
        IMPORT_C void SetType( CPhCntSingleItemFetch::TFetchType aType );
        
        /**
        * Returns the single item fetch type
        * @return fetch type
        */
        IMPORT_C CPhCntSingleItemFetch::TFetchType Type();

        /**
        * Sets the title pane resource id for the single item fetch dialog
        * @param title pane resource id
        */
        IMPORT_C void SetTitlePaneResourceId( TInt aTitlePaneResourceId );

        /**
        * Returns the title pane resource id for the single item fetch dialog
        * @return title pane resource id
        */
        IMPORT_C TInt TitlePaneResourceId() const;
        
        /**
        * Sets the CBA resource id for the single item fetch dialog
        * @param CBA resource id
        */
        IMPORT_C void SetCbaResourceId( TInt aCbaResourceId );

        /**
        * Returns the CBA resource id for the single item fetch dialog
        * @return CBA resource id
        */
        IMPORT_C TInt CbaResourceId() const;
        
        /**
        * Sets the string pointer used in the single item fetch dialog
        * @param pointer to string used in dialog
        */
        IMPORT_C void SetString( TDes* aString );
        
        /**
        * Returns the string pointer used for the single item fetch dialog
        * @return string pointer used in dialog
        */
        IMPORT_C TDes* String() const;
        
    private:   

        /**
        * Fetch type
        */
        CPhCntSingleItemFetch::TFetchType iType;

        /**
        * The title pane resource id
        */
        TInt iTitlePaneResourceId;
        
        /**
        * The CBA resource id
        */
        TInt iCbaResourceId;
        
        /**
        * The fetch dialog string pointer
        */
        TDes* iString;
        
    };

#endif      // __TPHONECMDPARAMSINGLEITEMFETCH_H   
            
// End of File
