/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Easy dialing test application.
*
*/

#ifndef _EDTA_QUERYDIALOG_H
#define _EDTA_QUERYDIALOG_H


//  INCLUDES
#include <aknquerydialog.h>

// CLASS DECLARATION

/**
*  CEdtaQueryDialog class.
*  This class is used to query the user for text information
*/
class CEdtaQueryDialog : public CAknTextQueryDialog
    {
    public:  // Constructors and destructor

        CEdtaQueryDialog( TDes& aBuf, const HBufC *aDefInput );
        virtual ~CEdtaQueryDialog() {};
        

    private:    // from CEikDialog

        void  PreLayoutDynInitL();

    private:    // Data

        HBufC&  iDefInput;
    };

#endif // _EDTA_QUERYDIALOG_H

// End of File

