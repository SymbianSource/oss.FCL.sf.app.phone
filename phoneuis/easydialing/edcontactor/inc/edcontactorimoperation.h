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
* Description:  Class implementing the instant messaging operation
*
*/


#ifndef EDCONTACTORIMOPERATION_H
#define EDCONTACTORIMOPERATION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>


// CLASS DECLARATION
class CCmsContactorImPluginBase;
class CSPSettings;
class CEDContactorOperation;

/**
 * Class implementing the instant messaging operation
 *
 *  @code
 *   ?good_class_usage_example(s)
 *  @endcode
 *
 *  @lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(CEDContactorIMOperation) : public CEDContactorOperation
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CEDContactorIMOperation();

    /**
     * Two-phased constructor.
     * 
     * @param aParam The IM "address" to send the message to
     */
    static CEDContactorIMOperation* NewL(const TDesC& aParam);

    /**
     * Two-phased constructor.
     * 
     * @param aParam The IM "address" to send the message to
     */
    static CEDContactorIMOperation* NewLC(const TDesC& aParam);
    
    /**
     * Executes the operation (opens the message editor)
     */
    virtual void ExecuteLD();

private:

    /**
     * Constructor for performing 1st stage construction
     * 
     * @param aParam The IM "address" to send the message to
     */
    CEDContactorIMOperation(const TDesC& aParam);

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
private:
    /**
     * Resolve IM Launcher ECom UID
     * 
     * @since S60 v5.0     
     * @param aServiceId service id 
     * @return ECom UID
     */ 
    TUid ResolveEcomUidL( const TDesC& aServiceId );     
    
    /**
     * Resolve IM Launcher ECom UID
     * 
     * @since S60 v5.0     
     * @param aServiceId service id 
     * @return Ecom UId in interegr
     */ 
    TInt DoResolveEcomUidL( const TDesC& aServiceId );     
                    
    /**
     * Load the ECom plugin
     * 
     * @since S60 v5.0       
     * @param aUidImp implementation UID
     * @param aXspId service provider id
     */
    void LoadEcomL( TUid aUidImp, const TDesC& aXspId );
    
    /**
     * Extracts service part from XspId
     * 
     * @since S60 v5.0       
     * @param aXspId service provider id   
     * @return service part (uri scheme part)
     */
    TPtrC ExtractService( const TDesC& aXspId );
private:
    
    /**
     * Im Plugin. 
     * OWN
     */
    CCmsContactorImPluginBase* iPlugin;        

    /**
     * Service Provider Settings
     * OWN
     */
    CSPSettings* iSPSettings;

    };

#endif // EDCONTACTORIMOPERATION_H
// End of File
