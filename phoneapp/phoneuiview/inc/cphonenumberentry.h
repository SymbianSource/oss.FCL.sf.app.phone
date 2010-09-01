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
* Description: Definition of number entry handler.
*
*/

#ifndef CPHONENUMBERENTRY_H
#define CPHONENUMBERENTRY_H

// Includes
#include <e32base.h>

#include "mphonenumberentry.h"
#include "mphonenumberentrychangedhandler.h"

// Forward declarations
class CBubbleManager;
class MNumberEntry;
class CCoeControl;
class TPhoneCommandParam;

/**
 *  Access to number entry.
 *
 *  @lib PhoneUIView.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CPhoneNumberEntry ): public CBase, public MPhoneNumberEntry,
    public MPhoneNumberEntryChangedHandler
    {

public:


    static CPhoneNumberEntry* NewL( CBubbleManager& aBubbleManager );

    virtual ~CPhoneNumberEntry();
    
    // From MPhoneNumberEntry
    
    void EnableTactileFeedback( const TBool aEnable );
    
    TBool IsNumberEntryUsed() const;
    
    TInt CountNumberEntryCharacters();
    
    void SetNumberEntry( MNumberEntry* aNumberEntry );
    
    void SetNumberEntryContent( const TDesC& aContent );
    
    void SetNumberEntryPromptText( const TDesC& aPromptText ); 
    
    // from base class MPhoneNumberEntryChangedHandler
    
    /**
     * From MPhoneNumberEntryChangedHandler
     * 
     * Handles number entry state change. 
     * Checks has the content of the number entry been modifed.
     * Informs the via call back functionality the party interested about 
     * the change. (at the moment phone state machine) 
     * 
     * @since S60 5.0
     */
    void HandleNumberEntryChanged( );
    
    /**
     * From MPhoneNumberEntryChangedHandler
     * 
     * Sets the call back function that is used to get notification when the 
     * content of the Number Entry has changed. (Used by the the phone 
     * statemachine.)  
     * 
     * @since S60 5.0
     */
    void SetNumberEntryChangedCallBack( TPhoneCommandParam* aCommandParam );
   
private:

    CPhoneNumberEntry( CBubbleManager& aBubbleManager );
    
    void ConstructL();
    
    /**
     * Compares previous number entry content to current.
     * If it is different updates the previous buffer. 
     * 
     * @return ETrue if nubmer entry content has changed
     */
    TBool CheckNumberEntryContent();
    
    /**
     * Updates the previous number entry buffer. 
     * 
     * @return ETrue if nubmer entry content was changed
     */
    TBool UpdatePreviousNumberEntryContent( CCoeControl* aEditor );
    
    /**
     * Returns pointer to phonenumbereditor of the Dialer number entry , 
     * if it in use
     * 
     * @return pointer to number entry editor
     */
    CCoeControl* DialerNumberEntryEditor() const;
    
    /**
     * Returns pointer to phonenumbereditor of the Bubblemanager number entry , 
     * if it in use
     * 
     * @return pointer to number entry editor
     */
    CCoeControl* BubbleNumberEntryEditor() const;

private:
    
    CBubbleManager& iBubbleManager;
    
    /**
     * Pointer to MNumberEntry, dialercontroller sets this to 
     * point to touch dialer.
     * Not own.
     */
    MNumberEntry* iDialerNumberEntry;
    
    // Owned contents.
    HBufC* iNumberEntryContents;
    
    /**
     * Is used to check is touch dialer enabled.
     */
    TBool iUseDialer;
    
    /*
    * Call back function to inform that the content of 
    * Number Entry has changed.
    */
    TCallBack iNEChangedCallBack;
    
    /**
     * Keeps track of the content of the number entry
     * Own.
     */
    HBufC* iPreviousNumberEntryContent;     
    
    };


#endif // CPHONENUMBERENTRY_H
