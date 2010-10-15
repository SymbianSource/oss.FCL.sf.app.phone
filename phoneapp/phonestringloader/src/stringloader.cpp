/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  String loader class.
*
*/

#include <exception> // must be before e32base.h so uncaught_exception gets defined
#include <StringLoader.h>
#include "phoneresourceadapter.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// StringLoader::Load
// -----------------------------------------------------------------------------
//
EXPORT_C void StringLoader::Load( 
        TDes & aDest,
        TInt aResourceId,
        CCoeEnv * aLoaderEnv )
    {
    Q_UNUSED (aLoaderEnv);
    QString text = PhoneResourceAdapter::Instance()->convert(aResourceId);
    aDest.Copy(text.utf16());
    }

// -----------------------------------------------------------------------------
// StringLoader::LoadL
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadL( 
        TInt aResourceId,
        CCoeEnv * aLoaderEnv )
    {
    HBufC *buf = LoadLC(aResourceId, aLoaderEnv);
    CleanupStack::Pop(buf);
    return buf;
    }


// -----------------------------------------------------------------------------
// StringLoader::LoadL
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadL( 
        TInt aResourceId,
        TInt aInt,
        CCoeEnv * aLoaderEnv )
    {
    HBufC *buf = LoadLC(aResourceId, aInt, aLoaderEnv);
    CleanupStack::Pop(buf);
    return buf;
    }


// -----------------------------------------------------------------------------
// StringLoader::LoadL
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadL( 
        TInt aResourceId,
        const TDesC & aString,
        CCoeEnv * aLoaderEnv )
    {
    HBufC *buf = LoadLC(aResourceId, aString, aLoaderEnv);
    CleanupStack::Pop(buf);
    return buf;
    }


// -----------------------------------------------------------------------------
// StringLoader::LoadL
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadL( 
        TInt aResourceId,
        const TDesC & aString,
        TInt aInt,
        CCoeEnv * aLoaderEnv )
    {
    HBufC *buf = LoadLC(aResourceId, aString, aInt, aLoaderEnv);
    CleanupStack::Pop(buf);
    return buf;
    }


// -----------------------------------------------------------------------------
// StringLoader::LoadL
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadL( 
        TInt aResourceId,
        const CArrayFix<TInt> & aInts,
        CCoeEnv * aLoaderEnv )
    {
    HBufC *buf = LoadLC(aResourceId, aInts, aLoaderEnv);
    CleanupStack::Pop(buf);
    return buf;
    }


// -----------------------------------------------------------------------------
// StringLoader::LoadL
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadL( 
        TInt aResourceId,
        const MDesCArray & aStrings,
        CCoeEnv * aLoaderEnv )
    {
    HBufC *buf = LoadLC(aResourceId, aStrings, aLoaderEnv);
    CleanupStack::Pop(buf);
    return buf;
    }


// -----------------------------------------------------------------------------
// StringLoader::LoadL
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadL( 
        TInt aResourceId,
        const MDesCArray & aStrings,
        const CArrayFix<TInt> & aInts,
        CCoeEnv * aLoaderEnv )
    {
    HBufC *buf = LoadLC(aResourceId, aStrings, aInts, aLoaderEnv);
    CleanupStack::Pop(buf);
    return buf;
    }


// -----------------------------------------------------------------------------
// StringLoader::LoadLC
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadLC( 
        TInt aResourceId,
        CCoeEnv * aLoaderEnv )
    {
    Q_UNUSED (aLoaderEnv);
    HBufC *buf;
    QString text = PhoneResourceAdapter::Instance()->convert(aResourceId);
    buf = HBufC::NewLC(text.size());
    *buf = text.utf16();
    return buf;
    }


// -----------------------------------------------------------------------------
// StringLoader::LoadLC
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadLC( 
        TInt aResourceId,
        TInt aInt,
        CCoeEnv * aLoaderEnv )
    {
    Q_UNUSED (aLoaderEnv);
    HBufC *buf;
    QString text = PhoneResourceAdapter::Instance()->convert(aResourceId, aInt);
    buf = HBufC::NewLC(text.size());
    *buf = text.utf16();
    return buf;
    }


// -----------------------------------------------------------------------------
// StringLoader::LoadLC
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadLC( 
        TInt aResourceId,
        const TDesC & aString,
        CCoeEnv * aLoaderEnv )
    {
    Q_UNUSED (aLoaderEnv);
    HBufC *buf;
    QString text = PhoneResourceAdapter::Instance()->convert(aResourceId, &aString);
    buf = HBufC::NewLC(text.size());
    *buf = text.utf16();
    return buf;
    }


// -----------------------------------------------------------------------------
// StringLoader::LoadLC
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadLC( 
        TInt aResourceId,
        const TDesC & aString,
        TInt aInt,
        CCoeEnv * aLoaderEnv )
    {
    Q_UNUSED (aLoaderEnv);
    HBufC *buf;
    QString text = PhoneResourceAdapter::Instance()->convert(aResourceId, &aString, aInt);
    buf = HBufC::NewLC(text.size());
    *buf = text.utf16();
    return buf;
    }



// -----------------------------------------------------------------------------
// StringLoader::LoadLC
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadLC( 
        TInt aResourceId,
        const CArrayFix<TInt> & aInts,
        CCoeEnv * aLoaderEnv )
    {
    Q_UNUSED (aLoaderEnv);
    HBufC *buf;
    QString text = PhoneResourceAdapter::Instance()->convert(aResourceId, &aInts);
    buf = HBufC::NewLC(text.size());
    *buf = text.utf16();
    return buf;
    }


// -----------------------------------------------------------------------------
// StringLoader::LoadLC
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadLC( 
        TInt aResourceId,
        const MDesCArray & aStrings,
        CCoeEnv * aLoaderEnv )
    {
    Q_UNUSED (aLoaderEnv);
    HBufC *buf;
    QString text = PhoneResourceAdapter::Instance()->convert(aResourceId, &aStrings);
    buf = HBufC::NewLC(text.size());
    *buf = text.utf16();
    return buf;
    }


// -----------------------------------------------------------------------------
// StringLoader::LoadLC
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC * StringLoader::LoadLC( 
        TInt aResourceId,
        const MDesCArray & aStrings,
        const CArrayFix<TInt> & aInts,
        CCoeEnv * aLoaderEnv )
    {
    Q_UNUSED (aLoaderEnv);
    HBufC *buf;
    QString text = PhoneResourceAdapter::Instance()->convert(aResourceId, &aStrings, &aInts);
    buf = HBufC::NewLC(text.size());
    *buf = text.utf16();
    return buf;
    }
