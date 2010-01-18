/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class handles all bitmaps
*
*/



#ifndef BUBBLEIMAGEMANAGER_H
#define BUBBLEIMAGEMANAGER_H

// INCLUDES

#include    <e32base.h>
#include    <bldvariant.hrh>
#include    <AknsItemID.h>
#include    <AknsConstants.h>
#include    <AknsItemData.h>

// CONSTANTS
const TInt KBMMaxFileNameLength = 100;

// FORWARD DECLARATIONS
class CBubbleSkinBitmap;
class CEikImage;
class CFbsBitmap;

// DATA TYPES

// Do NOT change order or numbering!!
enum TBMIcons
    {
    ///////////////////////////////////////////////////////////////////////////
    // Cached icons
    ///////////////////////////////////////////////////////////////////////////
    EQgn_indi_call_active = 0,
    EQgn_indi_call_active_mask,
    EQgn_indi_call_active_2,
    EQgn_indi_call_active_2_mask,
    EQgn_indi_call_disconn,
    EQgn_indi_call_disconn_mask,
    EQgn_indi_call_held,
    EQgn_indi_call_held_mask,
    EQgn_indi_call_video_callsta_1,
    EQgn_indi_call_video_callsta_1_mask,
    EQgn_indi_call_video_callsta_2,
    EQgn_indi_call_video_callsta_2_mask,
    EQgn_indi_call_video_callsta_disconn,
    EQgn_indi_call_video_callsta_disconn_mask,
    EQgn_indi_call_voip_active,
    EQgn_indi_call_voip_active_mask,
    EQgn_indi_call_voip_active_2,
    EQgn_indi_call_voip_active_2_mask,
    EQgn_indi_call_voip_held,
    EQgn_indi_call_voip_held_mask,
    EQgn_indi_call_voip_callsta_disconn,
    EQgn_indi_call_voip_callsta_disconn_mask,
    // Animated call icons
    EQgn_indi_call_waiting_1,
    EQgn_indi_call_waiting_1_mask,
    EQgn_indi_call_waiting_2,
    EQgn_indi_call_waiting_2_mask,
    EQgn_indi_call_waiting_disconn,
    EQgn_indi_call_waiting_disconn_mask,
    EQgn_indi_call_video_1,
    EQgn_indi_call_video_1_mask,
    EQgn_indi_call_video_2,
    EQgn_indi_call_video_2_mask,
    EQgn_indi_call_video_disconn,
    EQgn_indi_call_video_disconn_mask,
    EQgn_indi_call_voip_waiting_1,
    EQgn_indi_call_voip_waiting_1_mask,
    EQgn_indi_call_voip_waiting_2,
    EQgn_indi_call_voip_waiting_2_mask,
    EQgn_indi_call_voip_disconn,
    EQgn_indi_call_voip_disconn_mask,    
    // Background for NE + single call (not cached, if frame icons in use)
    EQgn_graf_call_rec_big,
    EQgn_graf_call_rec_big_mask, 
    EQgn_graf_call_rec_big_disconn,
    EQgn_graf_call_rec_big_disconn_mask,
    EQgn_graf_call_first_one_active,
    EQgn_graf_call_first_one_active_mask,
    EQgn_graf_call_first_one_disconn,
    EQgn_graf_call_first_one_disconn_mask,
    ///////////////////////////////////////////////////////////////////////////
    // Non-cached icons
    ///////////////////////////////////////////////////////////////////////////
    EQgn_graf_call_first_one_held,
    EQgn_graf_call_first_one_held_mask,
    EQgn_indi_call_active_cyph_off,
    EQgn_indi_call_active_cyph_off_mask,
    EQgn_indi_call_disconn_cyph_off,
    EQgn_indi_call_disconn_cyph_off_mask,
    EQgn_indi_call_held_cyph_off,
    EQgn_indi_call_held_cyph_off_mask,
    EQgn_indi_call_cyphering_off,
    EQgn_indi_call_cyphering_off_mask,
    EQgn_indi_call_line2,
    EQgn_indi_call_line2_mask,
    EQgn_indi_call_active_emergency,
    EQgn_indi_call_active_emergency_mask,
    EQgn_indi_call_waiting_cyph_off_1,
    EQgn_indi_call_waiting_cyph_off_1_mask,
    EQgn_indi_call_waiting_disconn_cyph_off,
    EQgn_indi_call_waiting_disconn_cyph_off_mask,
    EQgn_graf_call_first_one_active_emergency,
    EQgn_graf_call_first_one_active_emergency_mask,
    EQgn_graf_call_two_active_emergency,
    EQgn_graf_call_two_active_emergency_mask,
    EQgn_graf_call_rec_big_right,
    EQgn_graf_call_rec_big_right_mask,
    EQgn_graf_call_rec_big_right_disconn,
    EQgn_graf_call_rec_big_right_disconn_mask,
    EQgn_graf_call_conf_five,
    EQgn_graf_call_conf_five_mask,
    EQgn_graf_call_conf_four,
    EQgn_graf_call_conf_four_mask,
    EQgn_graf_call_conf_three,
    EQgn_graf_call_conf_three_mask,
    EQgn_graf_call_conf_two,
    EQgn_graf_call_conf_two_mask,
    EQgn_graf_call_first_three_active,
    EQgn_graf_call_first_three_active_mask,
    EQgn_graf_call_first_three_disconn,
    EQgn_graf_call_first_three_disconn_mask,
    EQgn_graf_call_first_three_held,
    EQgn_graf_call_first_three_held_mask,
    EQgn_graf_call_first_two_active,
    EQgn_graf_call_first_two_active_mask,
    EQgn_graf_call_first_two_disconn,
    EQgn_graf_call_first_two_disconn_mask,
    EQgn_graf_call_first_two_held,
    EQgn_graf_call_first_two_held_mask,
    EQgn_graf_call_first_wait_active,
    EQgn_graf_call_first_wait_active_mask,
    EQgn_graf_call_first_wait_disconn,
    EQgn_graf_call_first_wait_disconn_mask,
    EQgn_graf_call_hidden_held,
    EQgn_graf_call_hidden_held_mask,
    EQgn_graf_call_rec_big_left,
    EQgn_graf_call_rec_big_left_mask,
    EQgn_graf_call_rec_big_left_disconn,
    EQgn_graf_call_rec_big_left_disconn_mask,
    EQgn_graf_call_rec_small_left,
    EQgn_graf_call_rec_small_left_mask,
    EQgn_graf_call_rec_small_right,
    EQgn_graf_call_rec_small_right_mask,
    EQgn_graf_call_rec_small_right_disconn,
    EQgn_graf_call_rec_small_right_disconn_mask,
    EQgn_graf_call_rec_bigger,
    EQgn_graf_call_rec_bigger_mask,
    EQgn_graf_call_second_three_active,
    EQgn_graf_call_second_three_active_mask,
    EQgn_graf_call_second_three_disconn,
    EQgn_graf_call_second_three_disconn_mask,
    EQgn_graf_call_second_three_held,
    EQgn_graf_call_second_three_held_mask,
    EQgn_graf_call_second_two_active,
    EQgn_graf_call_second_two_active_mask,
    EQgn_graf_call_second_two_disconn,
    EQgn_graf_call_second_two_disconn_mask,
    EQgn_graf_call_second_two_held,
    EQgn_graf_call_second_two_held_mask,
    EQgn_indi_call_active_conf,
    EQgn_indi_call_active_conf_mask,
    EQgn_indi_call_disconn_conf,
    EQgn_indi_call_disconn_conf_mask,
    EQgn_indi_call_diverted,
    EQgn_indi_call_diverted_mask,
    EQgn_indi_call_held_conf,
    EQgn_indi_call_held_conf_mask,
    EQgn_indi_call_muted,
    EQgn_indi_call_muted_mask,
    EQgn_prop_nrtyp_mobile_div,
    EQgn_prop_nrtyp_mobile_div_mask,
    EQgn_prop_nrtyp_phone_div,
    EQgn_prop_nrtyp_phone_div_mask,
    EQgn_indi_call_active_conf_cyph_off,
    EQgn_indi_call_active_conf_cyph_off_mask,
    EQgn_indi_call_active_cyph_off_2,
    EQgn_indi_call_active_cyph_off_2_mask,
    EQgn_indi_call_disconn_conf_cyph_off,
    EQgn_indi_call_disconn_conf_cyph_off_mask,
    EQgn_indi_call_held_conf_cyph_off,
    EQgn_indi_call_held_conf_cyph_off_mask,
    EQgn_indi_call_waiting_cyph_off_2,
    EQgn_indi_call_waiting_cyph_off_2_mask,
    EQgn_prop_nrtyp_home_div,
    EQgn_prop_nrtyp_home_div_mask,
    EQgn_prop_nrtyp_work_div,
    EQgn_prop_nrtyp_work_div_mask,
    EQgn_graf_call_image_1,
    EQgn_graf_call_image_1_mask,
    EQgn_indi_call4_image_mask_prt, 
    EQgn_indi_call4_image_mask_lsc, 
    // From avkon.mbm
    EQgn_prop_nrtyp_fax,
    EQgn_prop_nrtyp_fax_mask,
    EQgn_prop_nrtyp_home,
    EQgn_prop_nrtyp_home_mask,
    EQgn_prop_nrtyp_mobile,
    EQgn_prop_nrtyp_mobile_mask,
    EQgn_prop_nrtyp_pager,
    EQgn_prop_nrtyp_pager_mask,
    EQgn_prop_nrtyp_phone,
    EQgn_prop_nrtyp_phone_mask,
    EQgn_prop_nrtyp_work,
    EQgn_prop_nrtyp_work_mask,
    EQgn_prop_nrtyp_voip,
    EQgn_prop_nrtyp_voip_mask,
    EQgn_prop_nrtyp_car,
    EQgn_prop_nrtyp_car_mask,
    EQgn_prop_nrtyp_assistant,
    EQgn_prop_nrtyp_assistant_mask
    };



// CLASS DECLARATION

/**
*  CBubbleImageManager class
*  This class handles all bitmaps
*
*  @lib bubblemanager
*  @since Series60_1.0
*/
class CBubbleImageManager : public CBase
    {

    public: // Constructors and destructor

        /**
        * Two phase constructor
        * @param aFrameIcons ETrue if 9-piece bubbles are used.
        * @return New instance
        */
        static CBubbleImageManager* NewL( TBool aFrameIcons );

        /**
        * Destructor.
        */
        ~CBubbleImageManager();

    public: // New functions

        /**
        * Set given bitmap to CEikImage object.
        * @param aImage Image which the bitmap is set to.
        * @param aPicture Bitmap enum.
        */
        void SetBitmapToImage( CEikImage*& aImage , 
                               const TBMIcons& aPicture );

        /**
        * Set given bitmap to CEikImage object.
        * @param aImage Image which the bitmap is set to.
        * @param aPicture Bitmap enum.
        * @param aPictureMask Bitmap mask enum.
        */
        void SetBitmapToImage( CEikImage*& aImage , 
                               const TBMIcons& aPicture, 
                               const TBMIcons& aPictureMask );

        /**
        * Clears skinned bitmaps. Call only between StartChanges() and
        * EndChanges()!!!
        * @since 2.0
        */
        void ClearSkinsCache();
        
        /**
        * This method prepares cached bitmaps. SVG icons cannot be
        * used before they are initialized. Using this method ensures 
        * validity of most essentials icons in OOM situation.  
        * @since 3.1
        */
        void PrepareCachedBitmaps();

    protected:
        /**
        * Constructor.
        */
        CBubbleImageManager( TBool aFrameIcons );

        // From CTimer
        void RunL();

        // Set bitmap+mask to given image
        void SetBitmapToImage( CEikImage& aImage , 
                               const TBMIcons& aPicture, 
                               const TBMIcons& aPictureMask );

        // Maps enumeration to mbm file
        void MapEnumToMbmFile( const TBMIcons aBmIcon,
                               TInt& aMbm,
                               TDes& aFileName ) const;

        // Maps emumeration to skin id
        void MapEnumToSkin( const TBMIcons aBmIcon, 
                            TAknsItemID& aItemId, 
                            TInt& aColorGroupId ) const;

        // Finds skin for given enumeration
        TBool GetSkin( CEikImage&     aImage, 
                       const TBMIcons aBmIcon, 
                       const TBMIcons aBmIconMask );

        // Loads new skin image into cache
        void LoadAndAppendSkinL( const TBMIcons aBmIcon,
                                 const TBMIcons aBmIconMask,
                                 CFbsBitmap*&   aBitmap, 
                                 CFbsBitmap*&   aMask );
                                 
        // Initialize bitmaps
        TBool DoInitializeBitmaps();
        
        // Callback function for CIdle
        static TInt InitializeBitmaps( TAny* aThis );
        
        // Load cached bitmaps
        void LoadCachedBitmapsL();
    
    private:
        // Returns id for last icon kept in cache
        inline TInt LastCachedIcon();

    private: // private functions
        /**
        * Symbian OS 2nd phase constructor.
        */
        void ConstructL();

    private: //data

        // MBM file name
        TBuf<KBMMaxFileNameLength> iMbmFileBuffer;

        // Cached skin bitmaps
        RPointerArray<CBubbleSkinBitmap> iSkinnedBitmaps;
        // Ordering function pointer.
        TLinearOrder<CBubbleSkinBitmap> iOrder;

        // Compares two different skin images
        static TInt CompareSkinnedData(
            const CBubbleSkinBitmap& aSkinnedBitmap1, 
            const CBubbleSkinBitmap& aSkinnedBitmap2 );
            
        // Bitmap initialization
        CIdle* iBitmapInitializer;
        TInt   iBitmapInitIndex;
        
        // 9-piece background icons used 
        TBool iFrameIcons;

    private:
        friend class CT_CBubbleImageManager;
    };

#endif //BUBBLEIMAGEMANAGER_H

// End of File
