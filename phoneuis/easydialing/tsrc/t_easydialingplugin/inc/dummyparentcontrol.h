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
* Description:  EUnit test file.
*
*/

#ifndef DUMMYPARENTCONTROL_H_
#define DUMMYPARENTCONTROL_H_

#include <coecntrl.h>

class CDummyParentControl : public CCoeControl
    {
public:
    
    static CDummyParentControl* NewL();
    
    ~CDummyParentControl();
    
private:
    
    CDummyParentControl();
    
    void ConstructL();
    
    };

#endif /* DUMMYPARENTCONTROL_H_ */
