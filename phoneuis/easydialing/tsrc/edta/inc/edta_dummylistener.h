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

#ifndef EDTA_DUMMYLISTENER_H_
#define EDTA_DUMMYLISTENER_H_

#include <coecntrl.h>
#include <dialingextensioninterface.h>

class CDummyListener : public CBase, public MDialingExtensionObserver
    {
public:
    
    CDummyListener();
    ~CDummyListener();
    
    void HandleDialingExtensionEvent( MDialingExtensionObserver::TEvent aEvent );    
    };

#endif /* EDTA_DUMMYLISTENER_H_ */
