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
* Description:  Easy dialing contact data container.
*
*/

#include <e32std.h>
#include <MVPbkContactLink.h>
#include <fbs.h>

#include "easydialingcontactdata.h"

// ---------------------------------------------------------------------------
// CEasyDialingContactData::CEasyDialingContactData
// ---------------------------------------------------------------------------
//
CEasyDialingContactData::CEasyDialingContactData(MVPbkContactLink* aContactLink)
        : iContactLink(aContactLink), iThumbnail(NULL), iFav(EFalse), iLoaded(EFalse)
    {
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactData::~CEasyDialingContactData
// ---------------------------------------------------------------------------
//
CEasyDialingContactData::~CEasyDialingContactData()
    {
    delete iThumbnail;
    delete iContactLink;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactData::Id
// ---------------------------------------------------------------------------
//
MVPbkContactLink* CEasyDialingContactData::ContactLink()
    {
    return iContactLink;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactData::Thumbnail
// ---------------------------------------------------------------------------
//
CFbsBitmap* CEasyDialingContactData::Thumbnail()
    {
    return iThumbnail;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactData::SetThumbnail
// ---------------------------------------------------------------------------
//
void CEasyDialingContactData::SetThumbnail(CFbsBitmap* aThumbnail)
    {
    delete iThumbnail;
    iThumbnail = aThumbnail;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactData::Fav
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactData::Fav()
    {
    return iFav;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactData::SetFav
// ---------------------------------------------------------------------------
//
void CEasyDialingContactData::SetFav(TBool aFav)
    {
    iFav = aFav;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactData::IsSimContact
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactData::IsSimContact()
    {
    return iSimContact;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactData::SetSimContact
// ---------------------------------------------------------------------------
//
void CEasyDialingContactData::SetSimContact(TBool aSim)
    {
    iSimContact = aSim;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactData::IsSdnContact
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactData::IsSdnContact()
    {
    return iSdnContact;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactData::SetSimContact
// ---------------------------------------------------------------------------
//
void CEasyDialingContactData::SetSdnContact(TBool aSdn)
    {
    iSdnContact = aSdn;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactData::IsLoaded
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactData::IsLoaded() 
    {
    return iLoaded;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactData::LoadingComplete
// ---------------------------------------------------------------------------
//
void CEasyDialingContactData::LoadingComplete()
    {
    iLoaded = ETrue;
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactData::DeleteThumbnail
// ---------------------------------------------------------------------------
//
void CEasyDialingContactData::DeleteThumbnail()
    {
    delete iThumbnail;
    iThumbnail = NULL;
    iLoaded = EFalse;
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactData::VoiceCallAvailable
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactData::VoiceCallAvailable()
    {
    return iVoiceCallAvailable;
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactData::SetVoiceCallAvailable
// ---------------------------------------------------------------------------
//
void CEasyDialingContactData::SetVoiceCallAvailable( TBool aVailable )
    {
    iVoiceCallAvailable = aVailable;
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactData::VideoCallAvailable
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactData::VideoCallAvailable()
    {
    return iVideoCallAvailable;
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactData::SetVideoCallAvailable
// ---------------------------------------------------------------------------
//
void CEasyDialingContactData::SetVideoCallAvailable( TBool aVailable )
    {
    iVideoCallAvailable = aVailable;
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactData::UniEditorAvailable
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactData::UniEditorAvailable()
    {
    return iUniEditorAvailable;
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactData::SetUniEditorAvailable
// ---------------------------------------------------------------------------
//
void CEasyDialingContactData::SetUniEditorAvailable( TBool aVailable )
    {
    iUniEditorAvailable = aVailable;
    }

// end of file
