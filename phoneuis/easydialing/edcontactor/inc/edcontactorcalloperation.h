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
* Description:  Class implementing the call operation
*
*/


#ifndef EDCONTACTORCALLOPERATION_H
#define EDCONTACTORCALLOPERATION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION
class CEDContactorOperation;

/**
 * Class implementing the call operation
 *
 *  @code
 *   ?good_class_usage_example(s)
 *  @endcode
 *
 *  @lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(CEDContactorCallOperation) : public CEDContactorOperation
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CEDContactorCallOperation();

    /**
     * Two-phased constructor.
     * 
     * @param aParam The phone number to call.
     * @param aForcedCallType Should (voice) call be initiated as forced cs call or not.
     */
    static CEDContactorCallOperation* NewL(const TDesC& aParam, TBool aForcedCallType);

    /**
     * Two-phased constructor.
     * 
     * @param aParam The phone number to call.
     * @param aForcedCallType Should (voice) call be initiated as forced cs call or not.
     */
    static CEDContactorCallOperation* NewLC(const TDesC& aParam, TBool aForcedCallType);
    
    /**
     * Executes the operation (makes the call)
     */
    virtual void ExecuteLD();

private:

    /**
     * Constructor for performing 1st stage construction
     * 
     * @param aParam The phone number to call.
     * @param aForcedCallType Should (voice) call be initiated as forced cs call or not.
     */
    CEDContactorCallOperation(const TDesC& aParam, TBool aForcedCallType);

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
private:
    
    /** Perform voice call as forced cs call or not.*/
    TBool iForcedCallType;
    };

#endif // EDCONTACTORCALLOPERATION_H
// End of File
