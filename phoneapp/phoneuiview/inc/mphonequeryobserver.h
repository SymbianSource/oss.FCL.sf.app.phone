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
* Description:  
*   Defines an interface for observing queries
*
*/


#ifndef MPHONEQUERYOBSERVER_H
#define MPHONEQUERYOBSERVER_H

//  INCLUDES

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
*   Defines an interface for observing queries
*/
class MPhoneQueryObserver
    {
    public: // New functions

        /**
        * Set by the query when it is asynchronously deleted
        * @param aQueryDeleted ETrue if query is deleted
        */
        virtual void SetQueryDeletedFlag( TBool aQueryDeleted ) = 0; 
        
        /**
        * Reports query dialog's exit
        * @param aCommand user selected action.
        */  
        virtual void ReportQueryCompleteAsync( TInt aCommand ) = 0;        
    };

#endif      // MPHONEQUERYOBSERVER_H   
            
// End of File
