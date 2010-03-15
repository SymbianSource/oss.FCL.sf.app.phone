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
* Description:  Easy dialing listbox data.
*
*/


#ifndef _EASYDIALINGLISTBOXDATA_H
#define _EASYDIALINGLISTBOXDATA_H


// INCLUDE FILES
#include <aknlists.h>

// CONSTANTS

/*
 * Matching highlight separator character is used in listbox model to separate
 * matching and non-matching charaters. 0x1F is chosen for this because
 * it is a control character "INFORMATION SEPARATOR ONE" of ASCII set. 
 * User of the listbox must use this character in creating list box item 
 * strings.
 */
// FORWARD DECLARATIONS
class CEasyDialingListBox;
class CEasyDialingContactDataManager;

// CLASS DECLARATIONS

/*
 * Easy dialing listbox data class.
 */

class CEasyDialingListBoxData : public CFormattedCellListBoxData
    {
public:
    
    class TExtendedColors : public CFormattedCellListBoxData::TColors 
        {
        public:
            
            TExtendedColors();
    
            /** The matching text colour. */
            TRgb iMatchingText;
    
            /** The matching background colour. */
            TRgb iMatchingBack;
    
        };
    
    
    /**
    * Constructor.
    */
    CEasyDialingListBoxData();
       
    /**
     * Destructor
     */
    ~CEasyDialingListBoxData();
       
    /**
    * Second phase constructor of the class.
    * @return   Pointer to the instance. 
    */
    static CEasyDialingListBoxData* NewL();
     
    /**
    * Draws selected item.
    */
    void DrawData(
        const TListItemProperties& aItemProperties, 
        CWindowGc& aGc,
        const TDesC* aText,
        const TRect& aRect,
        TBool aHighlight,
        const TExtendedColors& aColors ) const;
     
    /**
    * Draws selected item highlight.
    * @param    aGc         Graphics context.
    * @param    aItemRect   Item rectangle.
    */
    void DrawHighlight( CWindowGc &aGc, const TRect &aItemRect ) const;
    
    /**
    * Sets the contact data manager. Must be set before using the listbox.
    * @param    aContactDataManager Contact data manager.
    */
    void SetContactDataManager( CEasyDialingContactDataManager* aContactDataManager );

    /**
    * Sets a pointer to listbox class. Called from listbox constructor.
    * @param    aControl    Pointer to easy dialing listbox cotrol.
    */
    void SetEDLBXControl( CEasyDialingListBox* aControl );
    
    /**
     * Handle change in list item size
     */
    void HandleItemSizeChange();
    
protected:
        
    void ConstructLD();
   
private:
    
    /**
    * Determines fonts for contact name and company name.
    * The font sizes are determined based on list box item height.
    * This function must be called once before any drawing routines are 
    * done.
    * @param    aItemHeight List box item height
    */
    void ObtainFonts( TInt aItemHeight );
    
    /**
     * Update the color bitmap used for fallback graphics for the Action Menu arrow according to 
     * size changes in listbox control.
     * @param   aSize   New size.
     */
    void UpdateColorBitmapL( const TSize& aSize );
    
    /**
    * Draws the data for one list item.
    * @param    aProperties List item property structure.
    * @param    aGc Graphics context.
    * @param    aText   String containing  list item data information.
    * @param    aItemRect   Item rectangle.
    * @param    aHighlight  ETrue if the item has highlight, EFalse otherwise.
    * @param    aColors Structure containing used colors.
    */
    void DrawDataFormatted( 
        TListItemProperties aProperties,
        CWindowGc& aGc,
        const TDesC* aText,
        const TRect& aItemRect,
        TBool aHighlight,
        const TExtendedColors& aColors ) const;

    /**
     * Draws the Contact Thumbnail Icon if any else draws the dummy contact thumbnail.
     * @param   aGc Graphics context.
     * @param   aBoundingBox    Rectangle containing bounds for contact thumbnail.
     * @param   aDescId Descriptor containing contact id.
     */
    TBool ContactThumbnailDrawing(
            CWindowGc& aGc,
            TRect aBoundingBox,
            TPtrC aDescId) const;
    
    /**
     * Draws the Action Menu.
     * @param   aGc Graphics context.
     * @param   aActionMenuArrowRect    Rectangle where arrow is to be drawn.
     */ 
    void DrawArrowIcon( CWindowGc& aGc, TRect aActionMenuArrowRect ) const;
    
    /**
     * Draws the Favourite Icon. Takes the name rect as a parameter. Name rect
     * must be unmirrored, because the mirroring is done as the last thing
     * before drawing.
     * @param   aGc Graphics context.
     * @param   aNameRectUnMirrored Name rect, where fav star is contained
     * @param   aEffectiveRect The whole listbox item rect. Used for mirroring.
     */
    void DrawFavouriteIcon(
            CWindowGc& aGc, 
            TRect aNameRectUnMirrored,
            TRect aEffectiveRect) const;
    
private:
    
    /** Font to be used showing contact name. Not owned. */
    CFont* iContactNameFont;
    
    /** Font to be used showing company name. Not owned. */ 
    CFont* iCompanyNameFont;
    
    /** Handle for loading and storing the thumbnails. Not owned.*/ 
    CEasyDialingContactDataManager* iContactDataManager;
        
    /** Action Menu LeftPointing Arrow for mirrored layout. Owned. */
    CGulIcon* iArrowPointingRight;
    
    /** Action Menu RightPointing Arrow. Owned. */
    CGulIcon* iArrowPointingLeft;

    /** Action icon color bitmap. Owned. */
    CFbsBitmap* iColorBitmap;

    /** Default image for contacts with no own thumbnail. Owned. */
    CGulIcon* iDummyThumbnail;
    
    /** Favourite icon. Owned. */
    CGulIcon* iFavouriteIcon;
    
    /** Pointer to listbox. Not owned. */
    CEasyDialingListBox* iControl;
    };


#endif // _EASYDIALINGLISTBOXDATA_H

