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
* Description:  Class implementing the video call operation
*
*/


#ifndef EDCONTACTORVIDEOCALLOPERATION_H
#define EDCONTACTORVIDEOCALLOPERATION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION
class CEDContactorOperation;

/**
 * Class implementing the video call operation
 *
 *  @code
 *   ?good_class_usage_example(s)
 *  @endcode
 *
 *  @lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(CEDContactorVideocallOperation) : public CEDContactorOperation
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CEDContactorVideocallOperation();

    /**
     * Two-phased constructor.
     * 
     * @param aParam The phone number to video call
     */
    static CEDContactorVideocallOperation* NewL(const TDesC& aParam);

    /**
     * Two-phased constructor.
     * 
     * @param aParam The phone number to video call
     */
    static CEDContactorVideocallOperation* NewLC(const TDesC& aParam);
    
    /**
     * Executes the operation (makes the video call)
     */
    virtual void ExecuteLD();

private:

    /**
     * Constructor for performing 1st stage construction
     * 
     * @param aParam The phone number to video call
     */
    CEDContactorVideocallOperation(const TDesC& aParam);

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

    };

#endif // EDCONTACTORVIDEOCALLOPERATION_H
// End of File
