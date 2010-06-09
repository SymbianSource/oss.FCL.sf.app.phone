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
* Description:  View command handle
*
*/

#ifndef MPHONEVIEWCONTROLLEROBSERVER_H_
#define MPHONEVIEWCONTROLLEROBSERVER_H_

// INCLUDES

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  MPhoneViewControllerObserver class
*
*  @lib phoneuiview.lib
*/
class MPhoneViewControllerObserver
    {
    public:
    /**
    * Sets incall small bubble´s visibility in special cases.
    */
    virtual void AllowInCallBubbleInSpecialCases() = 0;
    };

#endif /* MPHONEVIEWCONTROLLEROBSERVER_H_ */
