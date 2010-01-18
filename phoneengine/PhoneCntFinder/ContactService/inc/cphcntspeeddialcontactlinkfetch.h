/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of CPhCntSpeedDialContactLinkFetch class.
*
*/


#ifndef CPHCNTSPEEDDIALCONTACTFETCH_H
#define CPHCNTSPEEDDIALCONTACTFETCH_H

#include "cphcntcontactlinkarrayfetch.h"

class MPhCntContactManager;
class MVPbkContactLink;
class MVPbkContactOperationBase;

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
NONSHARABLE_CLASS( CPhCntSpeedDialContactLinkFetch ) : public CPhCntContactLinkArrayFetch
    {

public:

    static CPhCntSpeedDialContactLinkFetch* NewL( 
        MPhCntContactManager& aContactManager );

    static CPhCntSpeedDialContactLinkFetch* NewLC(
        MPhCntContactManager& aContactManager );

    virtual ~CPhCntSpeedDialContactLinkFetch();
    
    /**
     * Fetches contact link which is assigned to speed dial position.
     *
     * @since S60 v3.1
     * @param aSpeedDialPosition Speed dial position.
     * @return Contact link to speed dial contact.
     */
    const MVPbkContactLink& FetchSpeedDialLinkL( 
        TInt aSpeedDialPosition );

    /**
     * From CPhCntAsyncToSync
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */
    void DoMakeAsyncRequestL();
    

// from base class ?base_class1

    /**
     * From ?base_class1.
     * ?description
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     */

// from base class ?base_class2

private:

    CPhCntSpeedDialContactLinkFetch( 
        MPhCntContactManager& aContactManager );

    void ConstructL();
    

private: // data

    /**
     * Contact manager.
     * Not own.
     */
    MPhCntContactManager& iContactManager;
     
    /**
     * Operation of contact link fetch.
     * Own.
     */
    MVPbkContactOperationBase* iOperation;
    
    /**
     * Parameter stored from FetchSpeedDialLinkL
     */
    TInt iSpeedDialPosition;

    };
    
#endif // CPHCNTSPEEDDIALCONTACTFETCH_H
