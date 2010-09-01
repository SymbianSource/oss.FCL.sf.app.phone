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
* Description:  Base class for communication method launching operations
*
*/


#ifndef EDCONTACTOROPERATION_H
#define EDCONTACTOROPERATION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 * Base class for communication method launching operations
 */
NONSHARABLE_CLASS(CEDContactorOperation) : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    virtual ~CEDContactorOperation();
    
    /**
     * Executes the operation
     */
    virtual void ExecuteLD()=0;

protected:

    /**
     * Constructor for performing 1st stage construction
     * 
     * @param aParam Communication parameter, for example phone number or e-mail address
     * @param aName Name of the contact to which the communication is started (optional)
     */
    CEDContactorOperation(const TDesC& aParam, const TDesC& aName = KNullDesC);
    
protected:

    const TDesC& iParam;
    
    const TDesC& iName;

    };

#endif // EDCONTACTOROPERATION_H
// End of File
