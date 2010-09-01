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
* Description:  MPhoneViewObserver 
*
*/



#ifndef MPHONEVIEWOBSERVER_H
#define MPHONEVIEWOBSERVER_H

//  INCLUDES

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  MPhoneViewObserver class
*
*  @lib phoneuiview.lib
*  @since Series60 5.0
*/
class MPhoneViewObserver
    {
    public:
      
    /**
     * Informs when phone view is activated.
     *
     * @since S60 v5.0
     */
    virtual void PhoneViewActivatedL() = 0;         

    };

#endif      // MPHONEVIEWOBSERVER_H

// End of File
