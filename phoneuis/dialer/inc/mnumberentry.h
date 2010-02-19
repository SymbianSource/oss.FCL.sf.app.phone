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
* Description:  MNumberEntry class provides an bubble manager like
*                API to Dialer for Phone.
*
*/



#ifndef MNUMBERENTRY_H
#define MNUMBERENTRY_H

//  INCLUDES
#include <e32def.h>
#include <e32cmn.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CCoeControl;
class CEikMenuPane; // easydialing

// CLASS DECLARATION

/**
*  MNumberEntryObserver class
*
*  @lib dialer.lib
*  @since Series60 5.0
*/
class MNumberEntryObserver
    {
    public:
      
    /**
     * Indication that numberentry state changed
     *
     * @since S60 v5.0
     */
    virtual void NumberEntryStateChanged( TBool aHidePromptText ) = 0;         

    };
    
/**
*  MNumberEntry container class
*
*  @lib dialer.lib
*  @since Series60 5.0
*/
class MNumberEntry
    {
    public: // New functions

        /**
        * Sets number entry active.
        */
        virtual void CreateNumberEntry() = 0;
        
        /**
        * Return pointer to number entry
        * @return The NE.
        */
        virtual CCoeControl* GetNumberEntry( ) const = 0;
        
        /**
        * Query, is number entry used.
        * @return ETrue is NE is used.
        */
        virtual TBool IsNumberEntryUsed( ) const = 0;
        
        /**
        * This disables NE drawing but doesn't empty the component.
        * @param aVisibility NE visible or not.
        */
        virtual void SetNumberEntryVisible( 
            const TBool& aVisibility = ETrue ) = 0;
        
        /**
        * Set Number Entry text.
        * @param aDesC The text.
        */
        virtual void SetTextToNumberEntry( const TDesC& aDesC ) = 0;
        
        /**
        * Get Number Entry Text.
        * @param aDesC The text.
        */
        virtual void GetTextFromNumberEntry( TDes& aDesC ) = 0;
        
        /**
        * Removes NE and empties the buffer.
        */
        virtual void RemoveNumberEntry( ) = 0;
        
        /**
        * Set number entry's editor mode.
        * @param aMode for number editor input mode.
        * @return Input mode of the editor.
        */
        virtual TInt ChangeEditorMode( TBool aDefaultMode = EFalse ) = 0;
        
        /**
        * Open VKB
        */
        virtual void OpenVkbL() = 0;
        
        /**
        * Get number entry's editor mode.
        * @return Input mode of the editor.
        */
        virtual TInt GetEditorMode() const = 0;

        /**
        * Resets number entry editor to default values.
        */
        virtual void ResetEditorToDefaultValues() = 0;
        
         /**
         * Sets number editor observer. 
         * 
         * @param aObserver Observer.
         */    
        virtual void SetNumberEntryObserver( 
            MNumberEntryObserver& aObserver ) = 0;    
            
         /**
         * Sets prompt text to numberentry. 
         * 
         * @param aPromptText.
         */    
        virtual void SetNumberEntryPromptText( const TDesC& aPromptText ) = 0;       
        
        /**
        * Enable or disable tactile feedback. 
        * 
        * @param aEnable.
        */          
        virtual void EnableTactileFeedback( const TBool aEnable ) = 0;
    };

#endif      // MNUMBERENTRY_H

// End of File
