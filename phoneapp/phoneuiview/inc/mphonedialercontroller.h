/*
* Copyright (c) 2007 - 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Definition of MPhoneDialerController class.
*
*/

#ifndef MPHONEDIALERCONTROLLER_H_
#define MPHONEDIALERCONTROLLER_H_

#include <e32cmn.h>
#include <gulicon.h>


/**
 *  Abstract dialer controller interface.
 *  Dialer controller provides data to be shown on dialer UI
 *  (e.g. prompt text and toolbar button contents), but is
 *  agnostic to implementation details and layout of the
 *  dialer UI components.
 *
 *  @lib PhoneUiView.lib
 *  @since S60 v5.0
 */
class MPhoneDialerController
    {
public: // data types
    
    /** 
     * Indices to refer to visual button slots in the dialer toolbar. 
     * The index order in not necessarily the same as the visual
     * order of the buttons. The index names correspond to the button
     * functions in the normal dialer mode.
     * @since S60 v5.2
     */
    enum TButtonIndex
        {
        EClearButton = 0,   //< Button normally used for backspce
        ECallButton,        //< Button normally used for "call" and "log"
        EPhonebookButton,   //< Button normally used for "Phonebook" and "Add to Phonebook"
        
        ENumberOfButtons    //< Total number of buttons in the toolbar.
        };
    
    /**
     * Struct-like class encapsulating data of one toolbar button function.
     * @since S60 v5.2
     */
    class CButtonData : public CBase
        {
    public: // data
        TInt iCommandId;
        CGulIcon* iIcon; // OWN
        HBufC* iTooltip; // OWN
    public: // constructors & destructor
        inline CButtonData() {}
        inline CButtonData( TInt aCommandId, CGulIcon* aIcon, HBufC* aTooltip ) :
            iCommandId( aCommandId ), iIcon( aIcon ), iTooltip( aTooltip ) {}
        inline ~CButtonData() { delete iIcon; delete iTooltip; }
        };
    
public:
    
    /**
    * Returns Cba resource id
    * @return Resource Id of the softkeys
    * @since S60 v5.1
    */
    virtual TInt CbaResourceId() const = 0;
    
    /**
    * Returns Menu resource id
    * @return Id of the menu resource
    * @since S60 v5.1
    */
    virtual TInt MenuResourceId() const = 0;
    
    /**
    * Returns number entry prompt text
    * @return Reference to text to be shown on number entry
    * @since S60 v5.1
    */
    virtual const TDesC& NumberEntryPromptTextL() = 0;

    /**
     * Get contents for given toolbar button. One button may have zero or more
     * different functions. Which one is set active, depends on the return value
     * of the member function ButtonState().
     * @param   aIndex  Index of the button for which data is queried.
     * @param   aData   On return, contains all the possible states for the button.
     * @return  KErrNone if action was succesful. 
     *          System wide error code if something went wrong.
     * @since S60 v5.2
     */
    virtual TInt GetButtonData( TButtonIndex aIndex, RPointerArray<CButtonData>& aData ) const = 0;
    
    /**
     * Function used to tell controller if number entry is empty or not.
     * In many cases this has an influence on the status of some or all toolbar buttons.
     * @param   aEmpty  ETrue if number entry is empty, EFalse othwerwise.
     * @since S60 v5.2
     */
    virtual void SetNumberEntryIsEmpty( TBool aEmpty ) = 0;
    
    /**
     * Gets current state of the given button.
     * @param   aIndex  Index of the button for which state is queried.
     * @return  Index of the button state which should be active. 
     *          KErrNotFound if the given button has no states.
     * @since S60 v5.2
     */
    virtual TInt ButtonState( TButtonIndex aIndex ) const = 0;
    
    /**
     * Gets dimming status of the given button
     * @param   aIndex  Index of the button for which dimming status is queried.
     * @return  ETrue   if the given button should be dimmed.
     *          EFalse  otherwise.
     * @since S60 v5.2
     */
    virtual TBool ButtonDimmed( TButtonIndex aIndex ) const = 0;
    
    /**
     * Gets value indicating whether using Easy Dialing should be allowed in
     * the current Dialer mode.
     * @return  ETrue   if Easy Dialing can be enabled.
     *          EFalse  if Easy Dialing must not be enabled.
     * @since S60 v5.2
     */
    virtual TBool EasyDialingAllowed() const = 0;
    };

#endif /*MDIALERCONTROLLER_H_*/




