/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Inline functions for Match Item
*
*/


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPhCntMatchItem::OriginalNumber
// 
// ---------------------------------------------------------
//
inline const TDesC& CPhCntMatchItem::OriginalNumber() const
    {
    return *iOriginalTelNumber;
    }

// ---------------------------------------------------------
// CPhCntMatchItem::AddReference
// 
// ---------------------------------------------------------
//
inline void CPhCntMatchItem::AddReference()
    {
    iReferenceCount++;
    }

// ---------------------------------------------------------
// CPhCntMatchItem::ReferenceCount
// 
// ---------------------------------------------------------
//
inline TInt CPhCntMatchItem::ReferenceCount() const
    {
    return iReferenceCount;
    }

// ---------------------------------------------------------
// CPhCntMatchItem::ContactId
// 
// ---------------------------------------------------------
//
inline CPhCntPbkContactId* CPhCntMatchItem::ContactId() const
    {
    return iContactId;
    }

// ---------------------------------------------------------
// CPhCntMatchItem::NumberType
// 
// ---------------------------------------------------------
//
inline TInt CPhCntMatchItem::NumberType() const
    {
    return iNumberType;
    }

//  End of File  
