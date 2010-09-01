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
* Description:  Class implementing the voip operation
*
*/


#ifndef EDCONTACTORVOIPOPERATION_H
#define EDCONTACTORVOIPOPERATION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION
class CEDContactorOperation;

/**
 * Class implementing the voip operation
 *
 *  @code
 *   ?good_class_usage_example(s)
 *  @endcode
 *
 *  @lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(CEDContactorVOIPOperation) : public CEDContactorOperation
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CEDContactorVOIPOperation();

    /**
     * Two-phased constructor.
     * 
     * @param aParam The address to make the voip call to
     * @param aServiceID The voip service ID
     */
    static CEDContactorVOIPOperation* NewL(const TDesC& aParam, TUint32 aServiceID);

    /**
     * Two-phased constructor.
     * 
     * @param aParam The address to make the voip call to
     * @param aServiceID The voip service ID
     */
    static CEDContactorVOIPOperation* NewLC(const TDesC& aParam, TUint32 aServiceID);
    
    /**
     * Executes the operation (launches the voip call)
     */
    virtual void ExecuteLD();

private:

    /**
     * Constructor for performing 1st stage construction
     * 
     * @param aParam The address to make the voip call to
     * @param aServiceID The voip service ID
     */
    CEDContactorVOIPOperation(const TDesC& aParam, TUint32 aServiceID);

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
    /**
     * The voip service id
     */
    TUint32 iServiceId;

    };

#endif // EDCONTACTORVOIPOPERATION_H
// End of File
