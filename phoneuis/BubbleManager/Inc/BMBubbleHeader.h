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
* Description: 
*     Abstract base class for headers
*
*/


#ifndef BUBBLEHEADER_H
#define BUBBLEHEADER_H

// INCLUDES
#include  "BMBubbleManager.h"
   
// CLASS DECLARATION

/**
*  CBubbleHeader class
*
*  Abstract base class for headers.
*
*  @lib bubblemanager
*  @since 1.0
*/

class CBubbleHeader : public CBase
    {
    public:
        // Different call image types
        enum TBMCallObjectImageType
            {
            ENotDefined,
            EThemeImage,
            EGalleryImage,
            ETextBasedImage    
            };

    public: // Constructors and destructor    
        /**
        * Destructor.
        */
        virtual ~CBubbleHeader();

    public: // Member functions

        /**
        * To put everything in initial state.
        */
        virtual void Reset( ) = 0;

        /**
        * Next functions are for CBubbleManager to set current state
        */

        /**
        * Set header in use/unuse.
        * @param aIsUsed ETrue if header is take into use.
        */
        virtual void SetIsUsed( const TBool& aIsUsed ) = 0;

        /**
        * Set header to conference.
        * @param aIsInConference ETrue if set in conference call.
        *                        Panics if this is a conf header.
        */
        void SetIsInConference( const TBool& aIsInConference );

        /**
        * Set thumbnail image. Ownership is not tranferred.
        * @param aThumbnailBitmap The bitmap. NULL means no thumbnail.
        */
        void SetTnBitmap( CFbsBitmap* aThumbnailBitmap );

        /**
        * Set thumbnail mask. Ownership is not tranferred.
        * @param aThumbnailBitmapMask The bitmap. NULL means no mask.
        */
        void SetTnBitmapMask( CFbsBitmap* aThumbnailBitmapMask );
        
        /**
        * Set thumbnail ownership. By default thumbnail bitmaps (image and mask) 
        * are owned externally.
        * @param aOwnership ETrue if ownership is transferred to header.
        */
        void SetTnDataOwnership( TBool aOwnership );

        /**
        * Set call object image. Ownership is not tranferred.
        * @param aCOImageBitmap Pointer to bitmap. NULL means no image.
        */
        void SetCallObjectImage( CFbsBitmap* aCOImageBitmap );
        
        /**
        * Set call object image mask. Ownership is not tranferred.
        * @param aCOImageBitmapMask Pointer to mask. NULL means no mask.
        */
        void SetCallObjectImageMask( CFbsBitmap* aCOImageBitmapMask );

        /**
        * Set call object file name. Ownership is tranferred.
        * @param aCOImageFileName Pointer to buffer holding the file name.
        */
        void SetCallObjectFileName( const HBufC* aCOFileName );
        
        /**
        * Set call object text. Ownership is  tranferred.
        * @param aCOText Pointer to buffer holding the text.
        */
        void SetCallObjectText( const HBufC* aCOText );
        
        /**
        * Set call object image data ownership. By default image
        * data (image and mask) is owned externally.
        * @param aOwnership ETrue if ownership is transferred to header.
        */
        void SetCallObjectImageDataOwnership( TBool aOwnership );
        
        
        /**
        * Set type of assigned call image.
        * @param aType The type.
        */
        void SetCallObjectImageType( 
            TBMCallObjectImageType aType );
        
        /**
        * Switches call image to thumbnail version, if available. 
        * Bigger version is released. 
        */
        void SwitchToThumbnailImage();

        /**
        * Set call state.
        * @param aPhoneCallState The state.
        */
        void SetCallState( 
            const CBubbleManager::TPhoneCallState& aPhoneCallState );

        /**
        * Set number type.
        * @param aNumberType The number type enum (see CBubbleManaher.h).
        */
        void SetNumberType( 
            const CBubbleManager::TPhoneNumberType& aNumberType );

        /**
        * Set call flags.
        * @param aCallFlags Call flags (see CBubbleManaher.h).
        */
        void SetCallFlags( const TUint32& aCallFlags ); 

        /**
        * Set current flag on.
        * @param aFlag The flag.
        */
        void SetCallFlag( const CBubbleManager::TPhoneCallTypeFlags& aFlag );

        /**
        * Remove flag
        * @param aFlag The flag.
        */
        void RemoveCallFlag( const CBubbleManager::TPhoneCallTypeFlags& aFlag );

        /**
        * Set bubble id for header.
        * @param aBubbleId The id.
        */
        void SetBubbleId( const CBubbleManager::TBubbleId& aBubbleId );

        /**
        * Set text label to header.
        * @param aText The label text.
        * @param aClipDirection Text clipping direction.
        */
        void SetText( 
            const TDesC16& aText,
            const CBubbleManager::TPhoneClippingDirection& aClipDirection );

        /**
        * Set caller line identification to header.
        * @param aCLI The CLI.
        * @param aClipDirection Text clipping direction.
        */
        void SetCLI( 
            const CBubbleManager::TBubbleLabelString& aCLI, 
            const CBubbleManager::TPhoneClippingDirection& aClipDirection );

        /**
        * Set timer/cost value to header.
        * @param aTimerCost Timer or cost value.
        */
        void SetTimerCost( 
            const CBubbleManager::TBubbleLabelString& aTimerCost );

        /**
        * Set caller name presentation to header.
        * @param aCNAP The CNAP.
        * @param aClipDirection Text clipping direction.
        * @since Series60 2.6
        */
        void SetCNAP( 
            const CBubbleManager::TBubbleLabelString& aCNAP, 
            const CBubbleManager::TPhoneClippingDirection& aClipDirection );
            
        /**
        * Set CLI used in conference call participant list.
        * @since S60 5.0.
        * @param aParticipantCLI Participant list CLI type.
        */
        void SetParticipantListCLI(
            CBubbleManager::TBubbleParticipantListCLI aParticipantCLI );            

        /**
        * Next functions are for CBubbleOutlook:s to fetch information
        */

        /**
        * Is header in use?
        * @return ETrue if in use.
        */
        virtual TBool IsUsed( ) const = 0;

        /**
        * Is header in conference call?
        * @return ETrue if header is in conf.
        */
        TBool IsInConference( ) const;

        /**
        * Is this header conference header?
        * @return ETrue if this in conference header.
        */
        virtual TBool IsConference( ) const = 0;
        
        /**
        * Get the thumbnail icon bitmap.
        * @return The icon bitmap.
        */
        CFbsBitmap* TnIconBitmap( ) const;

        /**
        * Get bitmap.
        * @return The bitmap.
        */
        CFbsBitmap* TnBitmap( ) const;

        /**
        * Get bitmap mask.
        * @return The mask.
        */
        CFbsBitmap* TnBitmapMask( ) const;

        /**
        * Get call object image bitmap.
        * @return The bitmap.
        */
        CFbsBitmap* CallObjectImage( ) const;

        /**
        * Get call object image mask.
        * @return The mask.
        */
        CFbsBitmap* CallObjectImageMask( ) const;
        
        /**
        * Get call object file name.
        * @return The file name.
        */
        const TDesC& CallObjectFileName( ) const;

        /**
        * Get call object text.
        * @return The text.
        */
        const TDesC& CallObjectText( ) const;
        
        /**
        * Get call image type. This is valid only, if call image
        * bitmap exists.
        * @return The type.
        */
        TBMCallObjectImageType CallObjectImageType() const;

        /**
        * Get call state.
        * @return The state.
        */
        CBubbleManager::TPhoneCallState CallState( ) const;
        
        /**
        * Get previous call state.
        * @return The previous state.
        */
        CBubbleManager::TPhoneCallState PreviousCallState( ) const;

        /**
        * Get number type.
        * @return The number type.
        */
        CBubbleManager::TPhoneNumberType NumberType( ) const;

        /**
        * Get call flags.
        * @return The flags.
        */
        TUint32 CallFlags( ) const;     

        /**
        * Get bubble id.
        * @return The id.
        */
        CBubbleManager::TBubbleId BubbleId( ) const;

        /**
        * Get label text.
        * @return The text.
        */
        const TDesC16& Text( ) const;

        /**
        * Get label's clipping direction.
        * @return The clip dir.
        */
        CBubbleManager::TPhoneClippingDirection TextClipDirection( ) const;

        /**
        * Get caller line identificatoin.
        * @return The CLI.
        */
        const CBubbleManager::TBubbleLabelString& CLI( ) const;

        /**
        * Get CLI's clip direction.
        * @return The dir.
        */
        CBubbleManager::TPhoneClippingDirection CLIClipDirection( ) const;

        /**
        * Get timer/cost text.
        * @return The text.
        */
        const CBubbleManager::TBubbleLabelString& TimerCost( ) const;

        /**
        * Get caller name presentation.
        * @since Series60 2.6
        * @return The CNAP.
        */
        const CBubbleManager::TBubbleLabelString& CNAP( ) const;

        /**
        * Get CNAP's clip direction.
        * @since Series60 2.6
        * @return The dir.
        */
        CBubbleManager::TPhoneClippingDirection CNAPClipDirection( ) const;
        
        /**
        * Get CLI used in conference call participant list.
        * @since S60 5.0.
        * @return aParticipantCLI CLI type.
        */
        CBubbleManager::TBubbleParticipantListCLI ParticipantListCLI() const;

    protected:

        /**
        * Base's 1st phase constructor.
        */
        CBubbleHeader();

        /**
        * Base's 2nd phase constructor.
        */
        void ConstructL(const CBubbleManager::TBubbleId& aBubbleId);
        
        /**
        * For resetting the values.
        */
        void ResetParent();

    private: //data
        // Call state
        CBubbleManager::TPhoneCallState iPhoneCallState;
        // Previous call state
        CBubbleManager::TPhoneCallState iPreviousCallState;
        // Number tyoe
        CBubbleManager::TPhoneNumberType iNumberType;
        // Call flags
        TUint32 iCallFlags; 
        // Internal flags
        TUint8 iInternalFlags;

        // Thumbnail icon bitmap
        CFbsBitmap* iThumbnailIconBitmap;

        // Thumbnail image
        CFbsBitmap* iThumbnailBitmap;
        // TN mask
        CFbsBitmap* iThumbnailBitmapMask;
        // Identifier
        CBubbleManager::TBubbleId iBubbleId;
        // Label text
        HBufC16* iText;
        // CLI text
        CBubbleManager::TBubbleLabelString iCLI;
        // Timer/cost value
        CBubbleManager::TBubbleLabelString iTimerCost;
        // Cnap text
        CBubbleManager::TBubbleLabelString iCNAP; 
        // Call object image
        CFbsBitmap* iCOImageBitmap;
        // Call object image mask
        CFbsBitmap* iCOImageBitmapMask;
        // Call object file name
        const HBufC* iCOFileName;
        // Call object text
        const HBufC* iCOText;
        // Call object image data ownership
        enum TBMCOImageDataOwnership
            {
            EBubbleCallImageOwned = 1,
            EBubbleThumbnailImageOwned = 2    
            };
        TBool iCOImageDataOwnership;
        // Call object image type
        TBMCallObjectImageType iCOImageType;
        // CLI for participant list.
        CBubbleManager::TBubbleParticipantListCLI iParticipantCLI;
    private:
        friend class CT_CBubbleHeader;

    };

#endif //BUBBLEHEADER_H

// End of File
