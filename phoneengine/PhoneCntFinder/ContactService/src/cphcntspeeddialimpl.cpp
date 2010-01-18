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
* Description:  Implementation of CPhCntSpeedDialImpl class.
*
*/


#include <spdiadialogs.h>
#include <MVPbkContactLink.h>
#include <PbkFields.hrh>
#include <avkon.mbg>
#include <eikimage.h> 
#include <aknconsts.h>

#include "CPhCntSpeedDialMonitor.h"
#include "cphcntspeeddialimpl.h"
#include "cphcntspeeddialcontactlinkfetch.h"
#include "MPhoneCntPbkOwner.h"
#include "cphcntfetchcontact.h"
#include "CPhCntContactStores.h"
#include "MPhCntContactManager.h"
#include "CPhCntContact.h"
#include "cphcntvpbkcontactid.h"
#include "CPhCntContactManager.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Default constructor.
// ---------------------------------------------------------------------------
//
CPhCntSpeedDialImpl::CPhCntSpeedDialImpl( MPhoneCntPbkOwner& aPbkOwner ) : 
    iContactManager( *aPbkOwner.ContactManager() ),
    iPbkOwner( aPbkOwner )
    {
    }

// ---------------------------------------------------------------------------
// CPhCntSpeedDialImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CPhCntSpeedDialImpl::ConstructL()
    {
    iSpeedDialContactLinkFetcher = 
        CPhCntSpeedDialContactLinkFetch::NewL( iContactManager );
        
    iContactStores = CPhCntContactStores::NewL( iContactManager );
        
    iContactFetcher = CPhCntFetchContact::NewL( *iContactStores );
    }
    
// ---------------------------------------------------------------------------
//  CPhCntSpeedDialImpl::FetchContact
// ---------------------------------------------------------------------------
//
TInt CPhCntSpeedDialImpl::FetchContact( 
    TInt aSpeedDialPosition,
    CPhCntContact*& aContact )
    {
    TRAPD( err, 
        const MVPbkContactLink& linkToSpeedDialContact = 
            iSpeedDialContactLinkFetcher->FetchSpeedDialLinkL( 
                aSpeedDialPosition );
                
    
        err = 
            iContactFetcher->FetchContact( linkToSpeedDialContact, aContact );
    )
    return err;
    }
    
// ---------------------------------------------------------------------------
//  CPhCntSpeedDialImpl::CopyNumberL
// ---------------------------------------------------------------------------
//
void CPhCntSpeedDialImpl::CopyNumberL( 
    TDes& aCopyTo, 
    const TDesC& aNumber )
    {
    if( aCopyTo.MaxLength() >= aNumber.Length() ) 
        {
        aCopyTo.Copy( aNumber );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }
// ---------------------------------------------------------------------------
//  CPhCntSpeedDialImpl::CopyContactInfoToFieldInfoL
// ---------------------------------------------------------------------------
//  
void CPhCntSpeedDialImpl::CopyContactInfoToFieldInfoL( 
        CPhCntContact& aContact,
        TInt aSpeedDialPosition, 
        TSpdDialFieldInfo& aFieldInfo )
    {
    
    MVPbkContactLink* contactLink = aContact.ContactLink()->CloneLC();
    CPhCntVPbkContactId* contactId = 
        CPhCntVPbkContactId::NewL( contactLink, iContactManager );
    CleanupStack::Pop(); // contactLink
    aFieldInfo.iContactId = contactId;
        
    aFieldInfo.iThumbIndex = KErrNotFound;
	TPhCntNumber speedDial = aContact.SpeedDialNumber( aSpeedDialPosition );    
	aFieldInfo.iNumberType = speedDial.Type();
    }

// ---------------------------------------------------------------------------
// CPhCntSpeedDialImpl::NewL
// ---------------------------------------------------------------------------
//
CPhCntSpeedDialImpl* CPhCntSpeedDialImpl::NewL(
    MPhoneCntPbkOwner& aPbkOwner )
    {
    CPhCntSpeedDialImpl* self = 
        CPhCntSpeedDialImpl::NewLC( aPbkOwner );
        
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CPhCntSpeedDialImpl::NewLC
// ---------------------------------------------------------------------------
//
CPhCntSpeedDialImpl* CPhCntSpeedDialImpl::NewLC(
    MPhoneCntPbkOwner& aPbkOwner )
    {
    CPhCntSpeedDialImpl* self = new( ELeave ) 
        CPhCntSpeedDialImpl( aPbkOwner );
        
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntSpeedDialImpl::~CPhCntSpeedDialImpl()
    {
    delete iContactFetcher;
    delete iContactStores;
    delete iSpeedDialContactLinkFetcher;
    delete iSpdDial;
    }

// ---------------------------------------------------------------------------
//  CPhCntSpeedDialImpl::GetSpeedDialFieldInfoL
// ---------------------------------------------------------------------------
//
void CPhCntSpeedDialImpl::GetSpeedDialFieldInfoL( 
    TInt aSpeedDialPosition,
    TSpdDialFieldInfo& aFieldInfo )
    {    
    CPhCntContact* contact = NULL;
    User::LeaveIfError( FetchContact( aSpeedDialPosition, contact ) );
    
    CleanupStack::PushL( contact );
   
    CopyContactInfoToFieldInfoL( *contact, aSpeedDialPosition, aFieldInfo );
        
    CleanupStack::PopAndDestroy( contact );
    }

// ---------------------------------------------------------------------------
// CPhCntSpeedDialImpl::GetSpeedDialFieldL
// ---------------------------------------------------------------------------
//
TInt CPhCntSpeedDialImpl::GetSpeedDialFieldL( 
    TInt aSpeedDialPosition, 
    TDes& aPhoneNumber )
    {
    CPhCntContact* contact = NULL;
    const TInt err = FetchContact( aSpeedDialPosition, contact );
    if( !err )
        {
        CleanupStack::PushL( contact );
        TPhCntNumber speedDial( contact->SpeedDialNumber( aSpeedDialPosition ) );
        CopyNumberL( aPhoneNumber, speedDial.Number() );        
        CleanupStack::PopAndDestroy( contact );
        }
    return err;
    }

// ---------------------------------------------------------------------------
// CPhCntSpeedDialImpl::GetSpeedDialFieldL
// ---------------------------------------------------------------------------
//
TInt CPhCntSpeedDialImpl::GetSpeedDialFieldL(
    TInt aSpeedDialPosition, 
    TDes& aPhoneNumber,
    TSpdDialFieldInfo& aFieldInfo )
    {
    CPhCntContact* contact = NULL;
    const TInt err = FetchContact( aSpeedDialPosition, contact );
    
    if( !err )
        {
        CleanupStack::PushL( contact );
        TPhCntNumber speedDial( contact->SpeedDialNumber( aSpeedDialPosition ) );
        CopyNumberL( aPhoneNumber, speedDial.Number() );         
        CopyContactInfoToFieldInfoL( *contact, aSpeedDialPosition, aFieldInfo );        
        CleanupStack::PopAndDestroy( contact );
        }
    return err;
    }
    
// -----------------------------------------------------------------------------
// CPhCntSpeedDialImpl::FetchNumberL
// -----------------------------------------------------------------------------
//
TInt CPhCntSpeedDialImpl::FetchNumberL(
            TInt aSpeedDialPosition,
            TDes& aPhoneNumber )
    {
    CPhCntContact* contact = NULL;
    TInt err = FetchContact( aSpeedDialPosition, contact );
    
    if( !err )
        {
        CleanupStack::PushL( contact );
        TPhCntNumber speedDial( contact->SpeedDialNumber( aSpeedDialPosition ) );
        CopyNumberL( aPhoneNumber, speedDial.Number() );        
        CleanupStack::PopAndDestroy( contact );
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CPhCntSpeedDialImpl::AssignSpeedDialFieldL
// -----------------------------------------------------------------------------
//
TInt CPhCntSpeedDialImpl::AssignSpeedDialFieldL(
            TInt aSpeedDialPosition,
            TDes& aPhoneNumber )
    {
    CVPbkContactManager* contactMg = 
        &iPbkOwner.ContactManager()->ContactManager();
    if ( !iSpdDial )
        {
        iSpdDial = CSpdiaDialogs::NewL( *contactMg );    
        }    
    MVPbkContactLink *link = NULL;
    TInt err = iSpdDial->ShowAssign( aSpeedDialPosition, link );
    delete iSpdDial;
    iSpdDial = NULL;
    if ( err == KErrNone )
        {
        err = FetchNumberL( aSpeedDialPosition, aPhoneNumber );    
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CPhCntSpeedDialImpl::AssignSpeedDialFieldL
// -----------------------------------------------------------------------------
//
TInt CPhCntSpeedDialImpl::AssignSpeedDialFieldL(
            TInt aSpeedDialPosition,
            TDes& aPhoneNumber,
            TSpdDialFieldInfo& aFieldInfo )
    {
    TInt err = AssignSpeedDialFieldL( aSpeedDialPosition, aPhoneNumber );
    if ( err == KErrNone )
        {
        GetSpeedDialFieldInfoL( aSpeedDialPosition, aFieldInfo );
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CPhCntSpeedDialImpl::Cancel
// -----------------------------------------------------------------------------
//
void CPhCntSpeedDialImpl::Cancel()
    {
    if ( iSpdDial )
        {
        iSpdDial->Cancel();
        }
    }
    
// -----------------------------------------------------------------------------
// CPhCntSpeedDialImpl::CreateNumberTypeIconLC
// -----------------------------------------------------------------------------
//
CEikImage* CPhCntSpeedDialImpl::CreateNumberTypeIconLC( 
    TInt aNumberType )
    {
    TUint icon;
    TUint mask;
    switch ( aNumberType )
        {
        case EPbkFieldIdPhoneNumberMobile:
            icon = EMbmAvkonQgn_prop_nrtyp_mobile;
            mask = EMbmAvkonQgn_prop_nrtyp_mobile_mask;
            break;
        case EPbkFieldIdPagerNumber:
            icon = EMbmAvkonQgn_prop_nrtyp_pager;
            mask = EMbmAvkonQgn_prop_nrtyp_pager_mask;
            break;
        case EPbkFieldIdPhoneNumberVideo:
            icon = EMbmAvkonQgn_prop_nrtyp_video;
            mask = EMbmAvkonQgn_prop_nrtyp_video_mask;
            break;
        case EPbkFieldIdVOIP:
            icon = EMbmAvkonQgn_prop_nrtyp_voip;
            mask = EMbmAvkonQgn_prop_nrtyp_voip_mask;
            break;
        case EPbkFieldIdPhoneNumberGeneral:
        case EPbkFieldIdPhoneNumberHome:
        case EPbkFieldIdPhoneNumberWork:
        case EPbkFieldIdFaxNumber:
        default:
            icon = EMbmAvkonQgn_prop_nrtyp_phone;
            mask = EMbmAvkonQgn_prop_nrtyp_phone_mask;
            break;
        }

    CEikImage* iconImg = new ( ELeave ) CEikImage;
    CleanupStack::PushL( iconImg );

    CFbsBitmap* iconBmp = new ( ELeave ) CFbsBitmap;
    CleanupStack::PushL( iconBmp );
    User::LeaveIfError( iconBmp->Load( 
        KAvkonBitmapFile(),
        icon,
        ETrue ) );

    CFbsBitmap* iconMaskBmp = new ( ELeave ) CFbsBitmap;
    CleanupStack::PushL( iconMaskBmp );
    User::LeaveIfError( iconMaskBmp->Load( 
        KAvkonBitmapFile(), 
        mask,
        ETrue ) );
    
    CleanupStack::Pop( iconMaskBmp );
    CleanupStack::Pop( iconBmp );

    iconImg->SetPicture( iconBmp, iconMaskBmp );
    iconImg->SetPictureOwnedExternally( EFalse );

    return iconImg;
    }
        
//  End of File 
