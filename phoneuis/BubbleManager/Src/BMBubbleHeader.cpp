/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header
*
*/


// INCLUDE FILES
#include    "BMBubbleHeader.h"
#include    "BMPanic.h"

#include    <AknsUtils.h>

// CONSTANTS
const TUint8 KBubbleHeaderIsInConf = 0x01;          // 1 if is in conf
const TUint8 KBubbleHeaderCLIClipDirLeft = 0x02;    // 1 if clipped from left
const TUint8 KBubbleHeaderLabelClipDirLeft = 0x04;
const TUint8 KBubbleHeaderCNAPClipDirLeft = 0x08;    // 1 if clipped from left

// ========================= MEMBER FUNCTIONS ================================

// Constructor
CBubbleHeader::CBubbleHeader( )
    {
    }


// ---------------------------------------------------------------------------
// CBubbleHeader::ConstructL
// Symbian OS two phased constructor called by inherited classes
// 
// ---------------------------------------------------------------------------
//
void CBubbleHeader::ConstructL( const CBubbleManager::TBubbleId& aBubbleId )
    {
    iBubbleId = aBubbleId;
    }

// Destructor
CBubbleHeader::~CBubbleHeader()
    {
    delete iThumbnailIconBitmap;
    if ( iCOImageDataOwnership & EBubbleThumbnailImageOwned )
        {
        delete iThumbnailBitmap;
        delete iThumbnailBitmapMask;
        }
        
    if ( iCOImageDataOwnership & EBubbleCallImageOwned )
        {
        delete iCOImageBitmap;
        delete iCOImageBitmapMask;
        }
    delete iText;
    delete iCOFileName;
    delete iCOText;
    }


// ---------------------------------------------------------------------------
// CBubbleHeader::SetIsInConference
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetIsInConference( const TBool& aIsInConference )
    {
    // can't put conference in conference
    if ( IsConference( ) )
        {
        __ASSERT_ALWAYS( !aIsInConference, 
                         Panic( EBMPanicReferredHeaderIsConferenceHeader ) );
        }
    
    if ( aIsInConference )
        {
        iInternalFlags |= iInternalFlags|KBubbleHeaderIsInConf;
        return;
        }

    iInternalFlags &= iInternalFlags&~KBubbleHeaderIsInConf;
    }


// ---------------------------------------------------------------------------
// CBubbleHeader::SetTnBitmap
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetTnBitmap( CFbsBitmap* aThumbnailBitmap )
    {
    // can't set thumbnail for conference call
    if ( IsConference( ) )
        {
        __ASSERT_DEBUG( aThumbnailBitmap == NULL, 
                        Panic( EBMPanicReferredHeaderIsConferenceHeader ) );
        }
    
    if ( iCOImageDataOwnership & EBubbleThumbnailImageOwned )
        {
        delete iThumbnailBitmap;
            
        }
    iThumbnailBitmap = aThumbnailBitmap;
    if ( aThumbnailBitmap != NULL &&
         !(iCOImageDataOwnership & EBubbleThumbnailImageOwned ) )
        {
        CFbsBitmap* bitmap = new CFbsBitmap;
        if ( bitmap != NULL )
           {
           bitmap->Duplicate( aThumbnailBitmap->Handle() );
           delete iThumbnailIconBitmap;
           iThumbnailIconBitmap = NULL;
           TRAP_IGNORE( 
               iThumbnailIconBitmap = AknIconUtils::CreateIconL( bitmap ));
           }
        }
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetTnBitmapMask
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetTnBitmapMask( CFbsBitmap* aThumbnailBitmapMask )
    {
    // can't set thumbnail for conference call
    if ( IsConference( ) )
        {
        __ASSERT_DEBUG( aThumbnailBitmapMask == NULL, 
                        Panic( EBMPanicReferredHeaderIsConferenceHeader ) );
        }
    
    if ( iCOImageDataOwnership & EBubbleThumbnailImageOwned )
        {
        delete iThumbnailBitmapMask;
            
        }
    iThumbnailBitmapMask = aThumbnailBitmapMask;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetTnDataOwnership
// ---------------------------------------------------------------------------
//    
void CBubbleHeader::SetTnDataOwnership( TBool aOwnership )
    {
    if ( aOwnership )
        {
        iCOImageDataOwnership |= EBubbleThumbnailImageOwned; 
        }
    else
        {
        iCOImageDataOwnership &= ~EBubbleThumbnailImageOwned;     
        }        
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetCallObjectImage
// ---------------------------------------------------------------------------
//    
void CBubbleHeader::SetCallObjectImage( CFbsBitmap* aCOImageBitmap )
    {
    if ( iCOImageDataOwnership & EBubbleCallImageOwned )
        {
        delete iCOImageBitmap;    
        }

    iCOImageBitmap = aCOImageBitmap;
    }
    
// ---------------------------------------------------------------------------
// CBubbleHeader::SetCallObjectImageMask
// ---------------------------------------------------------------------------
//    
void CBubbleHeader::SetCallObjectImageMask( CFbsBitmap* aCOImageBitmapMask )
    {
    if ( iCOImageDataOwnership & EBubbleCallImageOwned )
        {
        delete iCOImageBitmapMask;    
        }        
        
    iCOImageBitmapMask = aCOImageBitmapMask;            
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetCallObjectFileName
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetCallObjectFileName( const HBufC* aCOFileName )
    {
    delete iCOFileName;    
    iCOFileName = aCOFileName;                
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetCallObjectText
// ---------------------------------------------------------------------------
//    
void CBubbleHeader::SetCallObjectText( const HBufC* aCOText )
    {
    // can't set text for conference call
    if ( IsConference( ) )
        {
        __ASSERT_DEBUG( aCOText == NULL, 
                        Panic( EBMPanicReferredHeaderIsConferenceHeader ) );
        }
    
    delete iCOText;    
    iCOText = aCOText;            
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetCallObjectImageDataOwnership
// ---------------------------------------------------------------------------
//    
void CBubbleHeader::SetCallObjectImageDataOwnership( TBool aOwnership )        
    {
    if ( aOwnership )
        {
        iCOImageDataOwnership |= EBubbleCallImageOwned;
        }
    else
        {
        iCOImageDataOwnership &= ~EBubbleCallImageOwned;     
        }    
    }
    
// ---------------------------------------------------------------------------
// CBubbleHeader::SetCallObjectImageType()
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetCallObjectImageType( TBMCallObjectImageType aType ) 
    {
    iCOImageType = aType;    
    }    

// ---------------------------------------------------------------------------
// CBubbleHeader::SwitchToThumbnailImage
// ---------------------------------------------------------------------------
//    
void CBubbleHeader::SwitchToThumbnailImage()
    {
    if ( iThumbnailBitmap )
        {
        // Release big image
        if ( iCOImageDataOwnership & EBubbleCallImageOwned )
            {
            delete iCOImageBitmap;
            delete iCOImageBitmapMask;        
            }
        
        // Switch bitmaps
        iCOImageBitmap = iThumbnailBitmap;
        iThumbnailBitmap = NULL;
        iCOImageBitmapMask = iThumbnailBitmapMask;
        iThumbnailBitmapMask = NULL;
        
        // Update ownerships
        if ( iCOImageDataOwnership & EBubbleThumbnailImageOwned )
            {
            iCOImageDataOwnership |= EBubbleCallImageOwned;    
            }
        else
            {
            iCOImageDataOwnership &= ~EBubbleCallImageOwned;    
            }            
        iCOImageDataOwnership &= ~EBubbleThumbnailImageOwned;
        }
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetCallState
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetCallState( 
                      const CBubbleManager::TPhoneCallState& aPhoneCallState )
    {
    // can't set conference to incoming, outgoing or waiting state
    if ( IsConference( ) )
        {
        __ASSERT_ALWAYS( aPhoneCallState!=CBubbleManager::EWaiting &&
            aPhoneCallState!=CBubbleManager::EIncoming &&
            aPhoneCallState!=CBubbleManager::EAlerting &&
            aPhoneCallState!=CBubbleManager::EOutgoing,
            Panic( EBMPanicReferredHeaderIsConferenceHeader ) );
        }
    
    // if state is changed to disconnected, previous stage must be checked
    // to get the correct disconnect mode.
    if ( aPhoneCallState == CBubbleManager::EDisconnected )
        {
        if ( iPhoneCallState == CBubbleManager::EWaiting ||
            iPhoneCallState == CBubbleManager::EIncoming ||
            iPhoneCallState == CBubbleManager::EAlerting ||
            iPhoneCallState == CBubbleManager::EOutgoing ||
            iPhoneCallState == CBubbleManager::EAlertToDisconnected )
            {
            // changes the bubbles to be rectangles
            if ( aPhoneCallState != iPhoneCallState )
                {
                iPreviousCallState = iPhoneCallState;
                }
            iPhoneCallState = CBubbleManager::EAlertToDisconnected;
            return;
            }
        }
    // else put normal disconnected or what ever the new state is
    if ( aPhoneCallState != iPhoneCallState )
        {
        iPreviousCallState = iPhoneCallState;
        }
    iPhoneCallState = aPhoneCallState;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetNumberType
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetNumberType( 
    const CBubbleManager::TPhoneNumberType& aNumberType )
    {
    // can't set the number type else than ENotSet for conference
    if ( IsConference( ) )
        {
        __ASSERT_DEBUG( aNumberType==CBubbleManager::ENotSet, 
                        Panic( EBMPanicReferredHeaderIsConferenceHeader ) );
        }
    
    iNumberType = aNumberType;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetCallFlags 
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetCallFlags( const TUint32& aCallFlags ) 
    {
    iCallFlags = aCallFlags;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetCallFlag
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetCallFlag( 
    const CBubbleManager::TPhoneCallTypeFlags& aFlag )
    {
    if ( aFlag == CBubbleManager::ENormal )
        {
        iCallFlags = 0;
        return;
        }
    
    iCallFlags |= aFlag;    
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::RemoveCallFlag
// ---------------------------------------------------------------------------
//
void CBubbleHeader::RemoveCallFlag( 
    const CBubbleManager::TPhoneCallTypeFlags& aFlag )
    {
    if ( aFlag == CBubbleManager::ENormal )
        {
        return;
        }
    iCallFlags &= ~aFlag;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetBubbleId
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetBubbleId( const CBubbleManager::TBubbleId& aBubbleId )
    {
    iBubbleId = aBubbleId;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetText
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetText(
    const TDesC16& aText,
    const CBubbleManager::TPhoneClippingDirection& aClipDirection )
    {
    if ( iText )
        {
        delete iText;
        iText = NULL;
        }

    iText = aText.Alloc();

    if ( aClipDirection == CBubbleManager::ELeft )
        {
        iInternalFlags |= iInternalFlags|KBubbleHeaderLabelClipDirLeft;
        return;
        }
    iInternalFlags &= iInternalFlags&~KBubbleHeaderLabelClipDirLeft;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetCLI
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetCLI( 
    const CBubbleManager::TBubbleLabelString& aCLI, 
    const CBubbleManager::TPhoneClippingDirection& aClipDirection )
    {
    iCLI = aCLI;

    if ( aClipDirection == CBubbleManager::ELeft )
        {
        iInternalFlags |= iInternalFlags|KBubbleHeaderCLIClipDirLeft;
        return;
        }
    iInternalFlags &= iInternalFlags&~KBubbleHeaderCLIClipDirLeft;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetTimerCost
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetTimerCost( 
    const CBubbleManager::TBubbleLabelString& aTimerCost )
    {
    iTimerCost = aTimerCost;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetCNAP
// ---------------------------------------------------------------------------
//
void CBubbleHeader::SetCNAP( 
    const CBubbleManager::TBubbleLabelString& aCNAP, 
    const CBubbleManager::TPhoneClippingDirection& aClipDirection )
    {
    iCNAP = aCNAP;

    if ( aClipDirection == CBubbleManager::ELeft )
        {
        iInternalFlags |= iInternalFlags|KBubbleHeaderCNAPClipDirLeft;
        return;
        }
    iInternalFlags &= iInternalFlags&~KBubbleHeaderCNAPClipDirLeft;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::SetParticipantListCLI
// ---------------------------------------------------------------------------
//    
void CBubbleHeader::SetParticipantListCLI( 
    CBubbleManager::TBubbleParticipantListCLI aParticipantCLI )
    {
    iParticipantCLI = aParticipantCLI;    
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::IsInConference 
// ---------------------------------------------------------------------------
//
TBool CBubbleHeader::IsInConference( ) const
    {
    if ( iInternalFlags&KBubbleHeaderIsInConf )
        {
        return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::TnIconBitmap
// ---------------------------------------------------------------------------
//
CFbsBitmap* CBubbleHeader::TnIconBitmap( ) const
    {
    return iThumbnailIconBitmap;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::TnBitmap
// ---------------------------------------------------------------------------
//
CFbsBitmap* CBubbleHeader::TnBitmap( ) const
    {
    return iThumbnailBitmap;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::TnBitmapMask 
// ---------------------------------------------------------------------------
//
CFbsBitmap* CBubbleHeader::TnBitmapMask( ) const
    {
    return iThumbnailBitmapMask;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::CallObjectImage
// ---------------------------------------------------------------------------
//
CFbsBitmap* CBubbleHeader::CallObjectImage( ) const
    {
    return iCOImageBitmap;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::CallObjectImageMask 
// ---------------------------------------------------------------------------
//
CFbsBitmap* CBubbleHeader::CallObjectImageMask( ) const
    {
    return iCOImageBitmapMask;
    }
    
// ---------------------------------------------------------------------------
// CBubbleHeader::CallObjectFileName 
// ---------------------------------------------------------------------------
//
const TDesC& CBubbleHeader::CallObjectFileName( ) const
    {
    if ( iCOFileName && iCOFileName->Length() )
        {
        return *iCOFileName;    
        }
    else
        {
        return KNullDesC;    
        }    
    }    
    
// ---------------------------------------------------------------------------
// CBubbleHeader::CallObjectText 
// ---------------------------------------------------------------------------
//
const TDesC& CBubbleHeader::CallObjectText( ) const
    {
    if ( iCOText && iCOText->Length() )
        {
        return *iCOText;    
        }
    else
        {
        return KNullDesC;    
        }
    }    

// ---------------------------------------------------------------------------
// CBubbleHeader::CallObjectImageType()
// ---------------------------------------------------------------------------
//
CBubbleHeader::TBMCallObjectImageType CBubbleHeader::CallObjectImageType()
    const
    {
    return iCOImageType;    
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::CallState 
// ---------------------------------------------------------------------------
//
CBubbleManager::TPhoneCallState CBubbleHeader::CallState( ) const
    {
    return iPhoneCallState;
    }
    
// ---------------------------------------------------------------------------
// CBubbleHeader::PreviousCallState 
// ---------------------------------------------------------------------------
//
CBubbleManager::TPhoneCallState CBubbleHeader::PreviousCallState( ) const
    {
    return iPreviousCallState;
    }    

// ---------------------------------------------------------------------------
// CBubbleHeader::NumberType 
// ---------------------------------------------------------------------------
//
CBubbleManager::TPhoneNumberType CBubbleHeader::NumberType( ) const
    {
    return iNumberType;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::CallFlags
// ---------------------------------------------------------------------------
//
TUint32 CBubbleHeader::CallFlags( ) const    
    {
    return iCallFlags;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::Text 
// ---------------------------------------------------------------------------
//
const TDesC16& CBubbleHeader::Text() const
    {
    if ( iText )
        {
        return *iText;        
        }
    
    return KNullDesC;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::TextClipDirection 
// ---------------------------------------------------------------------------
//
CBubbleManager::TPhoneClippingDirection 
    CBubbleHeader::TextClipDirection( ) const
    {
    if ( iInternalFlags&KBubbleHeaderLabelClipDirLeft )
        {
        return CBubbleManager::ELeft;
        }
    return CBubbleManager::ERight;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::CLI 
// ---------------------------------------------------------------------------
//
const CBubbleManager::TBubbleLabelString& CBubbleHeader::CLI() const
    {
    return iCLI;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::CLIClipDirection  
// ---------------------------------------------------------------------------
//
CBubbleManager::TPhoneClippingDirection 
    CBubbleHeader::CLIClipDirection( ) const
    {
    if ( iInternalFlags&KBubbleHeaderCLIClipDirLeft )
        {
        return CBubbleManager::ELeft;
        }
    return CBubbleManager::ERight;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::TimerCost 
// ---------------------------------------------------------------------------
//
const CBubbleManager::TBubbleLabelString& CBubbleHeader::TimerCost() const
    {
    return iTimerCost;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::CNAP 
// ---------------------------------------------------------------------------
//
const CBubbleManager::TBubbleLabelString& CBubbleHeader::CNAP() const
    {
    return iCNAP;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::CNAPClipDirection  
// ---------------------------------------------------------------------------
//
CBubbleManager::TPhoneClippingDirection 
    CBubbleHeader::CNAPClipDirection( ) const
    {
    if ( iInternalFlags&KBubbleHeaderCNAPClipDirLeft )
        {
        return CBubbleManager::ELeft;
        }
    return CBubbleManager::ERight;
    }
// ---------------------------------------------------------------------------
// CBubbleHeader::BubbleId 
// ---------------------------------------------------------------------------
//
CBubbleManager::TBubbleId CBubbleHeader::BubbleId() const
    {
    return iBubbleId;
    }

// ---------------------------------------------------------------------------
// CBubbleHeader::ParticipantListCLI 
// ---------------------------------------------------------------------------
//    
CBubbleManager::TBubbleParticipantListCLI 
    CBubbleHeader::ParticipantListCLI() const
    {
    return iParticipantCLI;    
    }    


// ---------------------------------------------------------------------------
// CBubbleHeader::ResetParent
//
//  Puts everything in initial state
// ---------------------------------------------------------------------------
//
void CBubbleHeader::ResetParent()
    {
    iPhoneCallState = CBubbleManager::ENone;
    iNumberType = CBubbleManager::ENotSet;
    iCallFlags = 0; //normal
    delete iThumbnailIconBitmap;
    iThumbnailIconBitmap = NULL;
    
    if ( iCOImageDataOwnership & EBubbleThumbnailImageOwned )
        {
        delete iThumbnailBitmap;    
        delete iThumbnailBitmapMask;
        }
    
    iThumbnailBitmap = NULL;
    iThumbnailBitmapMask = NULL;

    if ( iCOImageDataOwnership & EBubbleCallImageOwned )
        {
        delete iCOImageBitmap;
        delete iCOImageBitmapMask;            
        }
        
    iCOImageBitmap = NULL;
    iCOImageBitmapMask = NULL;
    delete iCOText; 
    iCOText = NULL;
    delete iCOFileName;
    iCOFileName = NULL;
    
    iCOImageDataOwnership = 0;
    iCOImageType = ENotDefined;

    if ( iText )
        {
        delete iText;
        iText = NULL;
        }
    
    iCLI.Zero();
    iTimerCost.Zero();
    iCNAP.Zero();
    iInternalFlags = 0; //normal
    iParticipantCLI = CBubbleManager::EParticipantListCLIText;
    }

// End of Fíle
