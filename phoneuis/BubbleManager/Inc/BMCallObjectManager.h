/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Controls call object loading.
*
*/



#ifndef CBUBBLECALLOBJECTMANAGER_H
#define CBUBBLECALLOBJECTMANAGER_H

//  INCLUDES
#include <gdi.h>
#include <gulicon.h>
#include <avkon.hrh>

#include "BMBubbleManager.h"
#include "BMMediaReaderObserver.h"

// CONSTANTS

// DATA TYPES

// FORWARD DECLARATIONS
class CIdle;
class MBubbleMediaReader;
class CImageManager;

// CLASS DECLARATION

/**
*  Controls call object loading.
*
*  @lib BubbleManager.lib
*  @since Series 60 3.0
*/
class CBubbleCallObjectManager : public CBase,
                                 public MBubbleMediaReaderObserver
    {
    public:
        class TImageLoadingParams
            {
            public:
                // Source fills the main pane.
                TSize iPreferredSize;
                // Source image is smaller than main pane.
                TSize iTinyImageSize;
                // Size used in incall bubble clips.
                TSize iThumbnailSize;
                // Display mode
                TDisplayMode iDisplayMode;
            };

    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor. 
        * @param aBubbleManager Bubble manager reference.
        * @return New instance
        */
        static CBubbleCallObjectManager* NewL( 
            CBubbleManager& aBubbleManager );
        
        /**
        * Destructor.
        */
        virtual ~CBubbleCallObjectManager();

    public: // New functions

        // Call object loading

        /**
        * Start loading image from given file. 
        * @param aBubbleId Bubble id which image belongs.
        * @param aFileName Image file name.
        * @param aParams Image loading parameters.
        */            
        void LoadImageFromFile(
            const CBubbleManager::TBubbleId& aBubbleId,
            const TDesC& aFileName,
            const TImageLoadingParams& aParams );
            
        /**
        * Start loading image from given text. 
        * @param aBubbleId Bubble id which image belongs.
        * @param aText Text for image generation.
        * @param aParams Image loading parameters.
        */            
        void LoadImageFromText(
            const CBubbleManager::TBubbleId& aBubbleId,
            const TDesC& aText,
            const TImageLoadingParams& aParams );            
            
        /**
        * Cancel call object loading.
        * @param aBubbleId Bubble id.
        */
        void CancelCallObjectLoading( 
                    const CBubbleManager::TBubbleId& aBubbleId );
                    
        /**
        * Sets image loading to suspended state. Loading can
        * be continued by calling Resume().
        */
        void Suspend();
        
        /**
        * Resumes image loading that has been set to suspended
        * state by calling Suspend(). 
        */
        TInt Resume();
        
        /**
        * Load call theme image. 
        * @param aImageManager Image manager.
        * @param aSize Image size.
        */            
        void LoadCallThemeImage(
            CBubbleImageManager& aImageManager,    
            const TSize& aSize );
        
        /**
        * Release call theme image. 
        */
        void ReleaseCallThemeImage();
        
        /**
        * Resizes call theme image.  
        * @param aSize New image size.
        */
        void ResizeCallThemeImage( const TSize& aSize );
                
        /**
        * Returns call theme icon.
        * @return Call theme image icon. 
        */
        CGulIcon* CallThemeImageIcon();
        
    private: // Constructors
        
        /**
        * C++ default constructor.
        */
        CBubbleCallObjectManager( CBubbleManager& aBubbleManager );
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( );
        
    private: // From MBubbleMediaReaderObserver        
        void FrameBufferDataChanged();
        void ReaderError( TInt aError );                            
        
    private:
        void Reset();
        void ReadFromFileL( const TDesC& aFileName,
                            const TSize& aPreferredImageSize,
                            const TSize& aTinyImageSize ); 
        static TInt IdleProcessorCallback( TAny* aThis );
        void DoLoadImageInIdleL();
        void DoDeleteReaderInIdle();        
        void DoHandleFrameBufferDataL();
        void DoResumeL();
        TBool LoadThumbnailVersion();
        void AllocateCallThemeImageIconL( CBubbleImageManager& aImageManager );
        void DoInitializeIcons();
    
    private: // Private type definition               
        enum TLoadingState
            {
            EBMLoaderReady,
            EBMLoadingImage,
            EBMLoadingThumbnail,
            EBMLoadingText,
            EBMAnimating,
            EBMDeleting,
            EBMSuspended,
            EBMInitializingIcon
            };

    private: // Data
        // Bubble manager reference
        CBubbleManager& iBubbleManager;
        
        // Loading state
        TLoadingState iLoadingState;
        TInt iBubbleId;
        TImageLoadingParams iImageLoadParams;
        MBubbleMediaReader* iMediaReader; // Owned
        CIdle* iIdleProcessor; // Owned
        HBufC* iCallText; // Owned
        HBufC* iCallImage; // Owned
        CGulIcon* iCallThemeImageIcon; // Owned
        TSize     iCallThemeImageSize;
    };

#endif // CBUBBLECALLOBJECTMANAGER_H   
            
// End of File
