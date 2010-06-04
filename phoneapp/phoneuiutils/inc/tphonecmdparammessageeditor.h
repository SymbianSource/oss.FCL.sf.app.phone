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
* Description: 
*     A paramater class for opening message editors.
*
*/


#ifndef __TPHONECMDPARAMMESSAGEEDITOR_H
#define __TPHONECMDPARAMMESSAGEEDITOR_H

//  INCLUDES
#include <CMessageData.h>
#include "tphonecommandparam.h"

// DATA TYPES

// CLASS DECLARATION

/**
*  A paramater class for identifying view.
*/
class TPhoneCmdParamMessageEditor : public TPhoneUICommandParam
    {   
    public:

        /**
        * Constructor.
        */
        IMPORT_C TPhoneCmdParamMessageEditor();

        /**
        * Sets the message editor data.
        * @param    aMessageData
        * @return   None.
        */
        IMPORT_C void SetMessageData( CMessageData* aMessageData );

        /**
        * Returns message editor data.
        * @param    None.
        * @return   iMessageData.
        */
        IMPORT_C CMessageData* MessageData() const;

  private:    
  
        /**
        * Message Editor Data.
        */
        CMessageData* iMessageData;

    };

#endif // __TPHONECMDPARAMMESSAGEEDITOR_H   
            
// End of File
