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
* Description:  CBubbleManager class provides API for phone to draw current calls
*                on the screen. 
*
*/



#ifndef BUBBLEMANAGER_H
#define BUBBLEMANAGER_H

//  INCLUDES
#include    <coemain.h>
#include    <bmbubbleplace.h>
#include    <bldvariant.hrh>
#include    <mphcntmatch.h>

// CONSTANTS
_LIT( KBubbleTextSeparator ,"\t");
const TUint8 KBubbleShortLabelMaxLength = 25;
const TUint8 KBubbleLongLabelMaxLength = 35;
const TUint8 KBubbleLabelMaxLength = 
    KBubbleShortLabelMaxLength + KBubbleLongLabelMaxLength;
//const TUint8 KBubbleMaxCharsInNE = 48;
const TUint8 KBubbleCallMaxAmount = 7;
const TUint8 KBubblePlaceMaxAmount = 20;
const TUint8 KBubblePlaceMaxAmountTouch = 23;
const TUint8 KBubblePlaceMaxVisible = 4;
const TUint8 KBubbleMaxCallsInConf = 5;
const TInt KBubbleConferenceId = -1; // differs from 0-6 which 
                                     //are reserved for calls.
const TInt KBubbleInvalidId = -2;
const TInt KBubbleNoHighlightRow = 0; // 1-5 are conference rows

// FORWARD DECLARATIONS
class CBubbleOutlook;
class CBubbleImageManager;
class CBubbleCallHeader;
class CBubbleHeader;
class CBubbleConfHeader;
class CBubbleOutlookNumberEntry;
class CBubbleOutlookTopRight;
class CEikImage;
class CBubbleResourceManager;
class CPNGNumberGrouping;
class CBubbleCallObjectManager;
class CBubbleVideoController;
class MBubbleVideoPlaybackObserver;
class CBubbleTouchPane;
class MBubbleTouchPaneInterface;
class CTelBubbleCustomElement;
class CBubbleCustomManager;
class CBubbleMainPaneControl;

// CLASS DECLARATION

/**
*  CBubbleManager container class
*
*  @lib bubblemanager.lib
*  @since Series60_1.0
*/
NONSHARABLE_CLASS(CBubbleManager) : public CCoeControl
    {
    public:     // Data
        typedef TInt TBubbleId;
        typedef TBuf<KBubbleLabelMaxLength> TBubbleLabelString;
        typedef TUint8 TRowNumber; // 1-5 allowed (for conf)
        
 
        // Different call states
        enum TPhoneCallState // Set the order for drawing too
            { 
            ENone           = 0, // Default on creation
            EOnHold,
            EDisconnected,
            EActive,
            EIncoming,
            EWaiting,
            EAlertToDisconnected,
            EOutgoing,
            EAlerting,
            };
        
        // Call Flags
        enum TPhoneCallTypeFlags 
            {
            ENormal         = 0x00,     // Default
            ENoCiphering    = 0x08,     // open lock icon
            ELine2          = 0x10,     // is call on line 2 
            EMuted          = 0x20,     // not implemented for a single call
            EDiverted       = 0x40,     // is call diverted
            EVideo          = 0x80,     // video call
            EVoIPCall       = 0x100     // VoIP call.  
            };
        
        // Number types
        enum TPhoneNumberType
            {
            ENotSet    = 0, // Default
            EHome,
            EMobile,
            EWork,
            EPhone,
            EFaxNumber,
            EPager,
            ECar,
            EAssistant
            };
        
        // Where the text is clipped.
        enum TPhoneClippingDirection
            {
            ELeft,  // Default on numbers
            ERight  // Default on texts/names
            };

        enum TPhoneCallAnimationTypes
            {
            EAnimVoiceCall = 0,
            EAnimVideoCall,
            EAnimVoIPCall
            };
            
        // Popup bubble type
        enum TPhonePopupBubbleType
            {
            EVideoRingingToneQCIF,
            EVideoRingingToneSubQCIF    
            };
            
        // Video play
        enum TBubbleVideoPlayMode
            {
            EPlayInLoop,
            EPlayOnce,
            EPlayAscending    
            };
            
        // Conference participant CLI
        enum TBubbleParticipantListCLI
            {
            EParticipantListCLIText,
            EParticipantListCNAP    
            };                        

    public:  // Constructors and destructor
        
        /**
        * Two phase constructor
        * @param aContainer Parent container
        * @param aRect Area to use
        * @return New instance
        */
        IMPORT_C static CBubbleManager* NewL( const CCoeControl& aContainer , 
                                              const TRect& aRect );
        
        /**
        * Two phase constructor
        * @param aContainer Parent container
        * @param aRect Area to use
        * @param aBigNEInIdle Bigger number entry used in idle state.
        * @return New instance
        */
        IMPORT_C static CBubbleManager* NewL( const CCoeControl& aContainer , 
                                              const TRect& aRect,
                                              TBool aBigNEInIdle );
        
        /**
        * Destructor.
        */
        IMPORT_C ~CBubbleManager();

    public: // New functions
        
        // ====================================================
        // Must surround all the changes:
        // ====================================================
        //
        
        /**
        * Before making any changes to bubble headers, call this function so
        * manager can prapare for them properly.
        */
        IMPORT_C void StartChanges();
        
        /**
        * After the changes to bubble headers call this function so manager 
        * can prepare bubbles to right places and check the drawing order. 
        * Memory for arrays has been allocated beforehand.
        */
        IMPORT_C void EndChanges();
        
        
        // ====================================================
        // For number entry:
        // ====================================================
        //
        
        /**
        * Sets number entry active.
        */
        IMPORT_C void CreateNumberEntry();
        
        /**
        * Return pointer to number entry
        * @return The NE.
        */
        IMPORT_C CCoeControl* GetNumberEntry( ) const;
        
        /**
        * Query, is number entry used.
        * @return ETrue is NE is used.
        */
        IMPORT_C TBool IsNumberEntryUsed( ) const;
        
        /**
        * This disables NE drawing but doesn't empty the component.
        * @param aVisibility NE visible or not.
        */
        IMPORT_C void SetNumberEntryVisible( 
            const TBool& aVisibility = ETrue );
        
        /**
        * Set Number Entry text.
        * @param aDesC The text.
        */
        IMPORT_C void SetTextToNumberEntry( const TDesC& aDesC );
        
        /**
        * Get Number Entry Text.
        * @param aDesC The text.
        */
        IMPORT_C void GetTextFromNumberEntry( TDes& aDesC );
        
        /**
        * Removes NE and empties the buffer.
        */
        IMPORT_C void RemoveNumberEntry( );
        
        
        // ====================================================
        // For call headers and some for conference call
        // ====================================================
        //
        
        /**
        * Takes a new call header in use.
        * @return Bubble idenfication number
        */
        IMPORT_C TBubbleId CreateCallHeader();
        
        /**
        * Removes call header from use
        * @param aBubbleId Indentifies call header
        */
        IMPORT_C void RemoveCallHeader( const TBubbleId& aBubbleId );
        
        /**
        * Sets call state to header.
        * @param aBubbleId Identifies header
        * @param aState Call state ( e.g. incoming,.. ), for conf also 
        *               (active/on hold/ none)
        */
        IMPORT_C void SetState( const TBubbleId& aBubbleId, 
            const TPhoneCallState& aState );
        
        /**
        * Sets text label to header. For conf also.
        * @param aBubbleId Identifies header
        * @param aText Text to be seen in bubble ( e.g. 'on hold' )
        * @param aClipDirection Text clipping end
        */
        IMPORT_C void SetLabel( 
            const TBubbleId& aBubbleId, 
            const TDesC& aText, 
            const TPhoneClippingDirection& aClipDirection = ERight );
        
        /**
        * Sets text label to header. For conf also.
        * @param aBubbleId Identifies header
        * @param aLongText Text to be seen in bubble ( e.g. 'disconnected' )
        * @param aShortText Text to be seen in bubble ( e.g. 'disconn' )
        * @param aClipDirection Text clipping end
        */
        IMPORT_C void SetLabel( 
            const TBubbleId& aBubbleId, 
            const TDesC& aLongText, 
            const TDesC& aShortText, 
            const TPhoneClippingDirection& aClipDirection = ERight );
        
        /**
        * Sets caller's line identification ( name or number) to header. 
        * For conf also.
        * @param aBubbleId Identifies header
        * @param aCLIText Caller's CLI ( e.g. 'Mother' )
        * @param aClipDirection Text clipping end
        */
        IMPORT_C void SetCLI( const TBubbleId& aBubbleId, 
                              const TDesC& aCLIText, 
                              const TPhoneClippingDirection& aClipDirection );
        
        /**
        * Updates caller's line identification ( name or number) to header. 
        * For conf also.
        * @param aBubbleId Identifies header
        * @param aCLIText Caller's CLI ( e.g. 'Daddy' )
        * @param aClipDirection Text clipping end
        */
        IMPORT_C void UpdateCLI( 
            const TBubbleId& aBubbleId, 
            const TDesC& aCLIText, 
            const TPhoneClippingDirection& aClipDirection );
        
        
        /**
        * Sets call time or cost text to header. For conf also.
        * @param aBubbleId Identifies header
        * @param aCallTime Timer or cost text ( e.g. '00:12:34' or '£01.23' ). 
        *                  If this parameter is too long 
        *                  (> KBubbleLabelMaxLength) it discards the end.
        */
        IMPORT_C void SetCallTime( 
            const TBubbleId& aBubbleId, 
            const TDesC& aCallTime );
        
        /**
        * Updates call time or cost text to header.  For conf also.
        * @param aBubbleId Identifies header
        * @param aCallTime Timer or cost text ( e.g. '00:12:34' or '£01.23' ). 
        *                  If this parameter is too long 
        *                  (> KBubbleLabelMaxLength) it discards the end.
        */
        IMPORT_C void UpdateCallTime( 
            const TBubbleId& aBubbleId, 
            const TDesC& aCallTime ); 
        
        /**
        * Attach a thumbnail image to header. Bitmap parameters can be NULL.
        * Ownership is not transferred.
        * @param aBubbleId Identifies header
        * @param aTnBitmap A Pointer to wanted thumbnail bitmap.
        * @param aTnBitmapMask A Pointer to thumbnail bitmap mask.
        * @param aDataOwnershipTransferred Set ETrue if ownership is transferred.
        */
        IMPORT_C void SetThumbnail( const TBubbleId& aBubbleId, 
                                    CFbsBitmap* aTnBitmap, 
                                    CFbsBitmap* aTnBitmapMask,
                                    TBool aDataOwnershipTransferred 
                                          = EFalse ); 

        /**
        * Attach a call object image to header.
        * @param aBubbleId Identifies header
        * @param aImageFile.
        */
        IMPORT_C void SetCallObjectImage( const TBubbleId& aBubbleId,
                                          const TDesC& aImageFileName );            
                                            
        /**
        * Attach a call object image to header.
        * Ownership is not transferred.
        * @param aBubbleId Identifies header
        * @param aCOBitmap A Pointer to wanted call object image bitmap.
        * @param aCOBitmapMask A Pointer to call object bitmap mask.
        * @param aDataOwnershipTransferred Set ETrue if ownership is transferred.
        */
        IMPORT_C void SetCallObjectImage( const TBubbleId& aBubbleId, 
                                          CFbsBitmap* aCOBitmap, 
                                          CFbsBitmap* aCOBitmapMask = NULL,
                                          TBool aDataOwnershipTransferred 
                                                = EFalse  );

        /**
        * Attach a call object image to header. Image is taken from
        * active theme.   
        * @param aBubbleId Identifies header.
        */
        IMPORT_C void SetCallObjectFromTheme( const TBubbleId& aBubbleId );
                                    
        /**
        * Attach a call object text to header.
        * Ownership is not transferred.
        * @param aBubbleId Identifies header
        * @param aCOText Pointer to buffer holding the text. 
        */
        IMPORT_C void SetCallObjectText( const TBubbleId& aBubbleId, 
                                         const TDesC& aCOText );                                                                        
                                            
        
        /**
        * Sets call flags to header.
        * @param aBubbleId Identifies header
        * @param aFlags Call flags
        */
        IMPORT_C void SetCallFlags( 
            const TBubbleId& aBubbleId, 
            const TUint32& aFlags ); 
        
        /**
        * Sets call flags to header.
        * @param aBubbleId Identifies header
        * @param aFlag One call flag
        * @param aSet ETrue sets flag and EFalse clears flag
        */
        IMPORT_C void SetCallFlag( const TBubbleId& aBubbleId, 
                                   const TPhoneCallTypeFlags& aFlag, 
                                   const TBool& aSet );
        
        /**
        * Sets number type.
        * @param aBubbleId Identifies header
        * @param aNumType Number type enumeration ( e.g. mobile )
        */
        IMPORT_C void SetNumberType( const TBubbleId& aBubbleId, 
                                     const TPhoneNumberType& aNumType );

        /**
        * Sets number type.
        * @param aBubbleId Identifies header
        * @param aNumType Number type id.
        */
        IMPORT_C void SetPbkNumberType( const TBubbleId& aBubbleId, 
                                        const TInt aNumType );

        IMPORT_C void SetPhCntNumberType( const TBubbleId& aBubbleId,
                                     const MPhCntMatch::TNumberType aNumType );
        
        /**
        * Updates video call header text for animation
        * @param aBubbleId Identifies header
        * @param aHeaderText video call header animation text 
        */
        IMPORT_C void UpdateCallHeaderText( 
            const TBubbleId& aBubbleId, 
            const TDesC& aLongText, 
            const TDesC& aShortText, 
            const TPhoneClippingDirection& aClipDirection );

        // ====================================================
        // For all bubbles
        // ====================================================
        //
        
        /**
        * Sets phone muted/unmuted.
        * @param aIsMuted ETrue if phone is muted.
        */
        IMPORT_C void SetPhoneMuted( const TBool& aIsMuted = ETrue );
        
        
        // ====================================================
        // For conference call
        // ====================================================
        //
        
        /**
        * Creates a conference call based upon two calls. Bubbles must be 
        * created first.
        * @param aBubble1 Identifies first header to set in conference call.
        * @param aBubble2 Identifies second header to set in conference call
        * @return Conference bubble idenfication.
        */
        IMPORT_C TBubbleId CreateConference( const TBubbleId& aBubble1, 
                                             const TBubbleId& aBubble2 );
        
        /**
        * Splits conference call into invidual two calls. Call headers stays 
        * in use. Headers' state will not be changed.
        */
        IMPORT_C void RemoveConference();
        
        /**
        * Adds new call to conference call.
        * @param aBubbleId Identifies header
        */
        IMPORT_C void AddRowToConference( const TBubbleId& aBubbleId );
        
        /**
        * Takes specified call out of conference. Use RemoveConference if 
        * conference has only two calls in it. Header's state is same as 
        * before adding it to conference (if not changed inside the conf).
        * @param aBubbleId Identifies header
        */
        IMPORT_C void RemoveRowFromConference( const TBubbleId& aBubbleId );
        
        /**
        * Counts calls in conference call.
        * @return Call amount in conference.
        */
        IMPORT_C TUint8 ConferenceRowCount( ) const;
        
        /**
        * Sets highlight to specified line in conference.
        * @param aRowNumber Line number in conference bubble. 
        *                   0 means no highlight.
        */
        IMPORT_C void SetSelectionInConference( const TRowNumber& aRowNumber );
        
        /**
        * Sets highlight to specified bubble id in conference.
        * @param aBubbleId Id for wanted selection. 
        *                  KBubbleInvalidId means no hihgtlight. 
        */
        IMPORT_C void SetSelectionIdInConference( const TBubbleId& aBubbleId );
        
        /**
        * Gets highlighted item in conference.
        * @return Row number of the call. 0 if no highlight
        */
        IMPORT_C TRowNumber SelectionInConference() const;
        
        /**
        * Gets highlighted item in conference.
        * @return Id of the call. Returns KBubbleInvalidId if no highlight
        */
        IMPORT_C TBubbleId SelectionIdInConference() const;
        
        /**
        * Moves highligh one up if possible
        */
        IMPORT_C void MoveHighlightOneUpInConference();
        
        /**
        * Moves highligh one down if possible
        */
        IMPORT_C void MoveHighlightOneDownInConference();
        
        /**
        * Use this function to expand or shrink conference bubble.
        * @param aIsExpanded ETrue=expanded, EFalse=shrink.
        */
        IMPORT_C void SetExpandedConferenceCallHeader( 
            const TBool& aIsExpanded = ETrue );
        
        /**
        * Query: is conference expanded?
        * @return ETrue, if conference is expanded
        */
        IMPORT_C TBool IsConferenceExpanded( ) const;

        // ====================================================
        // Use only if __SERIES60_PHONE_CNAP if defined:
        // ====================================================
        //
        /**
        * Sets CNAP information to the call header. CLI is used for the CNAP 
        * name got from the network. In addition, the phone number is stored 
        * here to show it also.
        * @since 1.2
        * @param aBubbleId Identifies header
        * @param aCNAPText Caller's CNAP information.
        * @param aClipDirection Text clipping end
        */
        IMPORT_C void SetCNAP( 
            const TBubbleId& aBubbleId, 
            const TDesC& aCNAPText, 
            const TPhoneClippingDirection& aClipDirection = ELeft );

        
        /**
        * Set number entry's editor mode.
        * @param aMode for number editor input mode.
        * @return Input mode of the editor.
        * @since 3.0
        */
        IMPORT_C TInt ChangeEditorMode( TBool aDefaultMode = EFalse );
        
        /**
        * Get number entry's editor mode.
        * @return Input mode of the editor.
        * @since 3.0
        */
        IMPORT_C TInt GetEditorMode() const;

        /**
        * Resets number entry editor to default values.
        * @since 3.0
        */
        IMPORT_C void ResetEditorToDefaultValues();

        /**
        * Starts initialization of most essential icons. This method can 
        * be used to ensure correct LAF in OOM situation (to be called
        * before OOM).
        */
        IMPORT_C void PrepareIcons();
        
        /**
        * Preloads/Releases call theme image.  
        * @param aPreload ETrue to preload the image.
        *                 EFalse to release the image.
        */
        IMPORT_C void PreloadCallThemeImage( TBool aPreload );
        
        /**
        * Number of headers shown on the screen.
        * @return Shown header count.
        */
        IMPORT_C TUint8 ShownHeaderCount() const;
        
        /**
        * Client should call this method when the background context
        * changes after EndChanges() call. This method updates animation
        * frames that are created by drawing part of application view 
        * off-screen (updates here cannot be based on redraw events). 
        */
        IMPORT_C void HandleBackgroundImageChange();

        /**
        * Sets video ringing tone. Video is played when the next call 
        * enters incoming state and stopped when the call exits the
        * incoming call state.
        * @param aFileName File name with path.
        * @param aPlayMode Play mode.
        * @param aVolumeLevel Volume level (0-10).
        * @param aUseArbitraryScaling Set ETrue for arbitrary video scaling.
        * @param aObserver Playback observer.
        */
        IMPORT_C void SetVideoRingTone(
            const TDesC& aFileName,
            TBubbleVideoPlayMode aPlayMode,
            TInt aVolumeLevel,
            TBool aUseArbitraryScaling,
            MBubbleVideoPlaybackObserver* aObserver );
        
        /**
        * Stops video playback. Call bubble for video ringtone stays
        * active.   
        */
        IMPORT_C void StopVideoRingTone();
        
        /**
        * Mutes video playback.
        */
        IMPORT_C void MuteVideoRingTone();
        
        /**
        * Cancels video ringing tone. Video playback is stopped and call
        * bubble layout is changed to normal. This is meant to be used in 
        * error situations, where client needs to abort video state and
        * play default ringtone instead.    
        */
        IMPORT_C void CancelVideoRingTone();
        
        /**
        * Set CLI used in participant list (text or phonenumber).
        * @param aBubbleId Bubble identifier.
        * @param aParticipantCLI CLI type.
        * @since S60 5.0   
        */        
        IMPORT_C void SetParticipantListCLI(
            const TBubbleId& aBubbleId,
            TBubbleParticipantListCLI aParticipantCLI );
        
        /**
        * Returns touchpane interface.
        * @return Pointer to touchpane interface.
        */
        IMPORT_C MBubbleTouchPaneInterface* TouchPane();
        
        /**
        * Sets touchpane visibility.
        * @param Set ETrue if touchpane is shown otherwise EFalse.
        */
        IMPORT_C void SetTouchPaneVisible( TBool aVisible );
        
        /**
        * Adds custom ui element to call bubble. Onwership of the element
        * is not transferred, caller shall remove the element before deleting
        * the instance.
        * @since S60 5.0
        * @param aBubbleId Bubble identifier.
        * @param aElement  Custom element.
        * @param aPriority Priotity 0(high) - 255(low). 
        *                  Used when multiple instances in same position.
        */
        IMPORT_C void AddCustomElement( const TBubbleId& aBubbleId,
                                        CTelBubbleCustomElement* aElement,
                                        TInt aPriority );
        
        /**
        * Removes custom ui element from call bubble.
        * @since S60 5.0
        * @param aBubbleId Bubble identifier.
        * @param aElement Custom element.
        */
        IMPORT_C void RemoveCustomElement( const TBubbleId& aBubbleId,
                                           CTelBubbleCustomElement* aElement );

    public: // non-imported functions

        /**
        * Focus changed
        * @param aDrawNow Needs redraw?
        */
        void FocusChanged( TDrawNow aDrawNow );

        /**
        * For getting image manager
        * @return Reference
        */
        CBubbleImageManager& ImageManager();

        /**
        * For getting resource manager
        * @return Reference
        */
        CBubbleResourceManager& ResourceManager();
        
        /**
        * For getting customization manager
        * @return Reference
        */
        CBubbleCustomManager& CustomManager();

        /**
        * Uses number grouping to change the phone number
        * @since 1.2
        * @param aNumber original number
        * @return grouped number
        **/
        const TDesC& NumberGroup( const TDesC& aNumber );
        
        /**
        * To test if this call state is such that end key
        * press terminates the call.
        * end key is pressed.
        * @return ETrue if call is dropped by end key. Otherwise EFalse.
        */
        TBool IsCallDroppedByEndKey( const TPhoneCallState& aState ) const;
        
        /**
        * To get number of ongoing calls.
        * @return Active call count.
        */
        TUint8 ActiveCallCount() const;
        
        /**
        * For getting video controller.
        * @return Video controller reference.
        */
        CBubbleVideoController& VideoController() const;
        
        /**
        * To get call state of the bubble.
        * @return Call state.
        */
        TPhoneCallState CallState( const TBubbleId& aBubbleId );
        
        /**
        * Get size for call object image for incall situations. 
        * @param aIncall Set ETrue for incall states.
        * @param aBigBubble Set ETrue for big call bubble.
        * @return The Size.
        */
        TSize CallObjectImageIncallSize( TBool aBigBubble = EFalse );
        
        /**
        * Returns touch feature status.
        * @return ETrue if touch call handling enabled, otherwise EFalse.
        */
        TBool IsTouchCallHandling() const;
        
        /**
        * Returns main pane image rect
        */
        const TRect MainPaneImageRect( 
            TBool aTinyImage = EFalse,
            TBool aThreeLinesShown = EFalse ) const;
        
    private:  // Functions from base classes
        
        /**
        * From CCoeControl
        */
        void SizeChanged();
        
        /**
        * From CCoeControl
        */
        void PositionChanged();

        /**
        * From CCoeControl
        */
        TInt CountComponentControls() const;
        
        /**
        * From CCoeControl
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;
        
        /**
        * From CCoeControl
        */
        void Draw( const TRect& aRect ) const;

        /**
        * From CCoeControl
        */
        void HandleResourceChange( TInt aType );
        
        /**
        * From CCoeControl
        */
        void MakeVisible( TBool aVisible );

    private:

        /**
        * 2nd phase constructor.
        */
        void ConstructL( const CCoeControl& aContainer, 
            const TRect& aRect,
            TBool aBigNEInIdle );

        /**
        * Activates current place. Place's outlook will read info from header.
        */
        void SetActivePlace( const CBubblePlace::TPhoneBubblePlace& aPlace , 
                             CBubbleHeader& aHeader );
        
        /**
        * Finds header from call headers array.
        */
        TBool FindHeader( 
            const TBubbleId& aBubbleID, 
            CBubbleHeader*& aHeader ) const;
        
        /*
        * Find header from active headers array
        */
        TBool FindActiveHeader( 
            const TBubbleId& aBubbleID, 
            CBubbleHeader*& aHeader ) const;
        
        /**
        * Builds up expanded conference call
        */
        void BuildExpandedConference();
        
        /**
        * Buils bubbles without NE
        */
        void BuildBubbles();
        
        /**
        * Builds bubbles with NE
        */
        void BuildBubblesWithNE();
        
        /**
        * Builds bubbles for touch call handling.
        */
        void BuildBubblesForTouch();
        
        /**
        * Uses index to navigate through active headers. 
        */
        void FindNextDrawableHeader( TUint8& aIndex );
        
        /*
        * Draw lines in expanded conference again.
        */
        void UpdateConferenceRows( TRowNumber aRow1, TRowNumber aRow2 );

        /**
        * Finds header by call state.
        */            
        TBool FindHeader( const TPhoneCallState& aCallState,
                          CBubbleHeader*& aHeader ) const;

        /**
        * Builds main pane image from call object data.
        */
        void BuildMainPaneImage();
        
        /**
        * Reloads call object image.
        */
        void ReloadCallObjectImage();
        
        /**
        * DoLayoutCall1 (legacy)
        */
        void DoLayoutCall1Bubbles();
        
        /**
        * DoLayoutCall2 (call object bubbles)
        */
        void DoLayoutCall2Bubbles();
        
        /**
        * DoLayoutCall4 (touch)
        */
        void DoLayoutCall4Bubbles( const TRect& aWindowPane );
        
        /**
        * Start animations
        */
        void StartAnimations();
        
        /**
        * Creates video player and starts video playback.
        */
        void CreateVideoPlayerAndPlay();
        
        /**
        * Stops video playback and deletes video player.
        */
        void StopAndDeleteVideoPlayer();
        
        /**
        * Checks if initializing call exist. Returns ETrue if
        * initializing call exist otherwise EFalse.
        */                             
        TBool InitializingCall() const;                         

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        // Is phone muted?
        TBool iIsMuted;
        // Muted icon (component control).
        CEikImage* iMutedImage;
        
        // Counter for Start-/EndChanges()
        TUint8 iIsReadyToDraw;
        
        // Array for the call infos
        CArrayPtrFlat<CBubbleCallHeader>* iCallHeaders;
        // Array for currently used calls
        CArrayPtrFlat<CBubbleHeader>* iActiveHeaders;
        // Info object for conference call.
        CBubbleConfHeader* iConfHeader;
        
        // Array for bubble places
        CArrayPtrFlat<CBubblePlace>* iBubblePlaces;
        // Shown bubbles (component control)
        CArrayPtrFlat<CBubblePlace>* iActivePlaces;
        
        // Number Entry (component control)
        CBubbleOutlookNumberEntry* iNumberEntry;
        
        // For getting the images.
        CBubbleImageManager* iImageManager;
        // For component pool
        CBubbleResourceManager* iResourceManager;
        // For customizable UI controls
        CBubbleCustomManager* iCustomManager;

        // Parent container
        const CCoeControl* iParentControl;

        // Pointer to the label buffer
        HBufC* iLabelBuf;

        // Handles number grouping
        CPNGNumberGrouping* iNumberGrouping;

        // Call object manager
        CBubbleCallObjectManager* iCallObjectManager;
        
        // Main pane control
        CBubbleMainPaneControl* iMainPaneControl;
        
        // Flag indicating change in call object
        TBool iCallObjectChanged;
        
        // Video control.
        CBubbleVideoController* iVideoController;
        enum TVideoFlags
            {
            EBMVideoSetAsRingTone = 1,
            EBMVideoBubbleActive = 2,
            EBMVideoPlayingVideo = 4,
            };
        TInt iVideoFlags;
        
        // Configuration flags
        enum TConfigurationFlags
            {
            EBMCallObjectDisplay = 1,
            EBMBigNEInIdleState  = 2,
            EBMTouchCallhandling = 4
            };
        TInt iConfigFlags;
        
        // Touchpane object.
        CBubbleTouchPane* iTouchPane;        
        
        // Touchpane visibility.
        TBool iTouchPaneSetVisible;
        
        // Layout variety that depend's on image size.
        TInt iBottomImageVariety;
        
        TBool* iReserved2;
    };

#endif      // BUBBLEMANAGER_H

// End of File
