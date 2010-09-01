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
* Description:  Easy dialing listbox list item drawer.
*
*/


#ifndef _EASYDIALINGLISTBOXDRAWER_H
#define _EASYDIALINGLISTBOXDRAWER_H


// INCLUDE FILES
#include <aknlists.h>

// FORWARD DECLARATIONS

class CEasyDialingListBoxData;

// CLASS DECLARATIONS

/*
 * Easy dialing list item drawer.
 */

class CEasyDialingListBoxItemDrawer : public CFormattedCellListBoxItemDrawer
    {
public:
    
    /**
    * Constructor.
    * @param aTextListBoxModel Listbox mode.
    * @param aFont  Font parameter required by super class constructor.
    * @param aFormattedCellData List box data pointer.
    */
    CEasyDialingListBoxItemDrawer(
            MTextListBoxModel* aTextListBoxModel, 
            const CFont* aFont, 
            CFormattedCellListBoxData* aFormattedCellData);
   
    /**
    * Returns pointer to item data.
    */
    CEasyDialingListBoxData* EasyDialingCellData() const;

    /**
    * Draws item.
    * @param    aItemIndex  Index of the item.
    * @param    aItemRectPos    Position of the item.
    * @param    aItemIsSelected ETrue if item is selected, EFalse otherwise.
    * @param    aItemIsCurrent  ETrue if this item is the current item.
    * @param    aViewIsEmphasized ETrue if item is emphasized.
    * @param    aViewIsDimmed   ETrue if item is dimmed.
    */
    void DrawItem(
            TInt aItemIndex, 
            TPoint aItemRectPos, 
            TBool aItemIsSelected, 
            TBool aItemIsCurrent, 
            TBool aViewIsEmphasized, 
            TBool aViewIsDimmed) const;

    /**
    * Draws data content of the item. Not just text but
    * but any content.
    * @param    aItemIndex  Index of the item.
    * @param    aItemTextRect   Rectangle of the item.
    * @param    aItemIsCurrent  ETrue is the item is currently selected item.
    * @param    aViewIsEmphasized   ETrue if the item is emphasized.
    * @param    aItemIsSelected ETrue if item is selected.
    */
    void DrawItemText( 
            TInt aItemIndex,
            const TRect& aItemTextRect,
            TBool aItemIsCurrent,
            TBool aViewIsEmphasized,
            TBool aItemIsSelected ) const;
    
    /**
    * Finds proper listbox colors from theme and makes them active.
    */
    void SetColors();

protected:
    
    /**
    * Draws background of list item.
    * @param    aItemTextRect   Item rectangle.
    */
    void DrawBackgroundAndSeparatorLines( const TRect& aItemTextRect ) const;
    
protected:
    
    /** The matching text colour. */
    TRgb iMatchingText;

    /** The matching text background colour. */
    TRgb iMatchingBack;    
    };


#endif // _EASYDIALINGLISTBOXDRAWER_H
