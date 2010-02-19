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
* Description:  Easy dialing listbox.
*
*/


#ifndef _EASYDIALINGLISTBOX_H
#define _EASYDIALINGLISTBOX_H

// INCLUDE FILES
#include "easydialinglistboxview.h"
#include "easydialinglistboxitemdrawer.h"
#include "easydialinglistboxdata.h"
#include <aknlists.h>
#include <aknlongtapdetector.h>

// CONSTANTS

// Easy dialing specific listbox events
const TInt KEasyDialingContactSelected( 0x1001 );
const TInt KEasyDialingContactLongTapped( 0x1002 );

// FORWARD DECLARATIONS

class CEasyDialingListBoxItemDrawer;
class CAknsFrameBackgroundControlContext;
class CEasyDialingContactDataManager;
class CAknLongTapDetector;
class CAknPointerEventSuppressor;

// CLASS DECLARATIONS

/*
 * ==============================================================================
 * 
 * CEasyDialingListBox
 * 
 * ==============================================================================
 */

class CEasyDialingListBox : public CEikFormattedCellListBox, 
                            public MAknLongTapDetectorCallBack
    {   
public:
    
    /**
    * Constructor.
    */
    CEasyDialingListBox();
    
    /**
    * Destructor.
    */
    ~CEasyDialingListBox();
    
    /**
    * Second phase constructor.
    * @param    aFlags  Listbox flags.
    * @param    aContactDataManager Pointer to contact data manager
    */
    void ConstructL( TInt aFlags, 
            CEasyDialingContactDataManager* aContactDataManager );

    /**
    * From CEikFormattedCellListBox.
    */
    virtual CListBoxView* MakeViewClassInstanceL();
    
    /**
    * From CEikFormattedCellListBox.
    */
    CEasyDialingListBoxItemDrawer* ItemDrawer() const;
    
    /**
    * From CEikFormattedCellListBox.
    */
    void Draw(const TRect& aRect) const;
    
    /**
    * From CEikFormattedCellListBox.
    */
    void HandleResourceChange(TInt aType);
    
    /**
    * From CCoeControl
    */
    void MakeVisible(TBool aVisible);
    
    /**
    * From CCoeControl
    */
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);
    
    /**
     * The the rectangle within which the listbox must always fit.
     * @param   aMaxRect   The largest allowed rect size, given in
	 *                     screen relative coordinate system.
     */
    void SetMaxRect( TRect aMaxRect );
    
    /**
     * Adjusts the control rect according to amount of visible contacts. 
     * @param   aNumberOfNames  Number of contact to which to adjust.
     */
    void SetRectToNumberOfItems( TInt aNumberOfNames );
    
    
    /**
     * Puts list box control into focus, and moved the focus highlight
     * to the appropriate item.
     * @param   aKeyEvent   key event
     * @param   aType       key event type
     * @return  Whether the key was handled or not.
     */
    TKeyResponse SetFocusedWithKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

    
    /**
    * Gets the height of the listbox based on the number of items.
    * @param    aNum    Number of items.
    */
    TInt GetHeightBasedOnNumberOfItems( TInt aNum ) const;
    
    /**
     * Gets the contact index of the currently selected contact item.
     * Contact index points to contact data object in 
     * CEasyDialingContactDataManager. 
     * @return  Contact data manager index.
     */
    TInt CurrentContactDataIndex();
    
    /**
     * Return contact link of current item packaged in HBufC object. 
     * @return  Contact link.
     */
    HBufC8* CurrentContactLinkLC();
    
public:
    /**
     * From MAknLongTapDetectorCallBack
     */
    void HandleLongTapEventL( const TPoint& aPenEventLocation, const TPoint& aPenEventScreenLocation );
    
protected:
    
    /**
    * From CEikFormattedCellListBox.
    */
    virtual void FocusChanged(TDrawNow aDrawNow);
        
    /**
    * From CEikFormattedCellListBox.
    */
    virtual void CreateItemDrawerL();
    
    /**
    * From CEikFormattedCellListBox.
    */
    virtual void SizeChanged();
        
    /**
    * From CCoeControl
    */
    TTypeUid::Ptr MopSupplyObject(TTypeUid aId);
    
private:
    
    /** Background control context. Owned. */
    CAknsFrameBackgroundControlContext* iBGContext;
        
    /** Pointer to a contact data manager; Not Own. */
    CEasyDialingContactDataManager* iContactDataManager;
    
    /** Number of matching contacts. */
    TInt iNumberOfNames;
    
    /** Maximum size rectangle. */
    TRect iMaxRect;
    
    /** Detector for Long Tap event; Owned. */
    CAknLongTapDetector* iLongTapDetector;
    
    /** Flag set when long pointer press has been handled after latest pointer down event.*/
    TBool iPointerLongPressHandled;	
    };
    
#endif // _EASYDIALINGLISTBOX_H



