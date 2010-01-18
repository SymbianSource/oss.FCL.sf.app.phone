/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Phone number
*
*/


#ifndef TPHCNTNUMBER_H
#define TPHCNTNUMBER_H

#include <e32std.h>

#include "MPhCntMatch.h"

/**
 *  Represents phone number.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3,1
 */
class TPhCntNumber 
    {
    
public:
    /**
     * Constructor.
     * 
     * @since S60 v3.1
     * @param aNumber Phone number. Given number has to stay accessible 
     * as long as this class is used. Does not copy the number.
     * @param aNumberType Number's type.
     * @param aSpeedDialPosition Speed dial position (a value between 1 and 9).
     */
    TPhCntNumber( const TDesC& aNumber, MPhCntMatch::TNumberType aNumberType, TInt aSpeedDialPosition );
    
    /**
     * Constructs empty number.
     *
     * @since S60 v3.2.
     */
    TPhCntNumber();

    /**
     * Gives the number
     *
     * @since S60 v3.1
     * @return Number
     */
    const TDesC& Number() const;
    
    /**
     * Gives the number type
     *
     * @since S60 v3.1
     * @return Number type
     */
    MPhCntMatch::TNumberType Type() const;
    
    /**
     * Gives the speed dial position
     *
     * @since S60 v3.2
     * @return Position
     */
    TInt Position() const;    
    
    /**
     * Sets the number and its type. Note that ownership of the data is 
     * not taken ie the data has to live as long as this instance.
     *
     * @since S60 v3.2.
     * @param aNumber Phone number. Given number has to stay accessible 
     * as long as this class is used. Does not copy the number.
     * @param aNumberType Number's type.
     * @param aSpeedDialPosition Speed dial position or 0.
     */
    void Set( const TDesC& aNumber, MPhCntMatch::TNumberType aNumberType, TInt aSpeedDialPosition );
    
private: // data

    /**
     * Phone number.
     */
    TPtrC iNumber;
    
    /**
     * Number's type.
     */
    MPhCntMatch::TNumberType iNumberType;     
    
    /**
     * Speed dial position.
     */
    TInt iSpeedDialPosition;    

    };

#endif // TPHCNTNUMBER_H
