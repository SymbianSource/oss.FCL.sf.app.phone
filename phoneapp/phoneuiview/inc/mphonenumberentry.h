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
* Description:  Abstract number entry interface. 
*
*/

#ifndef M_PHONENUMBERENTRY_H
#define M_PHONENUMBERENTRY_H

// Forward declarations
class MNumberEntry;

/**
 *  Abstract number entry interface.
 *
 *
 *  @lib PhoneUiView.lib
 *  @since S60 v5.0
 */
class MPhoneNumberEntry
    {

public:

    /**
    * Enables or disables tactile feedback
    *
    * @since S60 S60 v5.0
    */
    virtual void  EnableTactileFeedback( const TBool aEnable ) = 0;
    
    /**
    * Indicates state whether numberentry is used or not
    *
    * @since S60 S60 v5.0
    */    
    virtual TBool IsNumberEntryUsed() const = 0;
    
    /**
    * Returns count of characters in numberentry
    *
    * @since S60 S60 v5.0
    */    
    virtual TInt CountNumberEntryCharacters() = 0;
    
    /**
    * Sets dialer's numberentry pointer 
    *
    * @since S60 S60 v5.0
    */      
    virtual void SetNumberEntry( MNumberEntry* aNumberEntry ) = 0;
    
    /**
    * Sets content for numberentry
    *
    * @since S60 S60 v5.0
    */
    virtual void SetNumberEntryContent( const TDesC& aContent ) = 0;
    
    /**
    * Sets prompt text for numberentry
    *
    * @since S60 S60 v5.0
    */    
    virtual void SetNumberEntryPromptText( const TDesC& aPromptText ) = 0; 
    };


#endif // M_PHONENUMBERENTRY_H
