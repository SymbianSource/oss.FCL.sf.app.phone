/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of TPhTouchPaneWrapper class.
*
*/


// INCLUDE FILES
#include "tphonetouchpanewrapper.h"   


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// TPhTouchPaneWrapper::TPhTouchPaneWrapper
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TPhTouchPaneWrapper::TPhTouchPaneWrapper()
    {
    }

// ---------------------------------------------------------
// TPhTouchPaneWrapper::TPhTouchPaneWrapper
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt TPhTouchPaneWrapper::NumberOfButtonsInPane() const
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------
// TPhTouchPaneWrapper::ButtonCommandId
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt TPhTouchPaneWrapper::ButtonCommandId( 
        TInt /*aButtonIndex*/ ) const
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------
// TPhTouchPaneWrapper::SetButtonDimmed
// (other items were commented in a header).
// ---------------------------------------------------------
//
void TPhTouchPaneWrapper::SetButtonDimmed( TInt /*aCommand*/, TBool /*aDimmed*/ )
   {
   }

//  End of File
