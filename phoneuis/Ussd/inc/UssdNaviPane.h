/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declares container control for application.
*
*/


#ifndef USSDNAVIPANE_H
#define USSDNAVIPANE_H


// INCLUDES
#include <coecntrl.h>
    

// FORWARD DECLARATIONS
class CUssdAppUi;
class CAknNaviLabel;
class CAknNavigationDecorator;
class CAknNavigationControlContainer;
class CCnvCharacterSetConverter;


// CLASS DECLARATION

/**
*  CUssdNaviPane
*  Declares navi pane control for application.
*  @since 2.8.
*
*/
class CUssdNaviPane : public CCoeControl
    {
    public: // Constructors and destructor

        /**
        * Constructor.
        * @param aAppUi A reference to AppUI for container.
        */
        CUssdNaviPane( CUssdAppUi& aAppUi );

        /**
        * Two-phased constructor.
        * @param aAppUi A reference to AppUI for container.
        */
        static CUssdNaviPane*  NewL( CUssdAppUi& aAppUi );
        
        /**
        * 2nd phase constructor
        *  
        */
        void ConstructL();

        /**
        * Destructor.
        */
        ~CUssdNaviPane();

    public: // New functions

        /**
        * Update the message length information to navipane
        * @param aInputString Reference to a string to be checked
        * @param aControl for handling backspace key event
        * @return Updated value of maximum size of string
        */
        TInt UpdateMsgLengthL( 
            const TDesC& aInputString,
            CCoeControl* aControl );

    public: // Functions from base classes

        
    private: // Functions from base classes

 
    private: //new functions
                
        /**
        * Creates an extra navi label. It is needed so that the msg length
        * won't dissapear when the focus is on the picture of a picture msg.
        */
        void CreateExtraNaviLabelL();
        
        /**
        * Checks if there are any unicode characters in the message
        * @param aInputString Reference to a string to be checked
        * @return ETrue whether we have unicode chars otherwise EFalse
        */
        TBool NeedsToBeSentAsUnicodeL( const TDesC& aInputString ) const;
        
        /**
        * Counts if there are 7bit extended table characters in message 
        * (see more info from ETSI 03.38)
        * @param aInputString Reference to chars to be checked
        * @return amount of extended table chars
        */
        TInt CountExtendedTableChars( const TDesC& aInputString ) const;

        /**
        * Generates a backspace key press event.
        * @param aControl Reference to control class
        */
        void GenerateBackSpaceKeyL(CCoeControl* aControl);
       
         
    private: //data

        // Application UI.
        CUssdAppUi&                         iAppUi;

         // Char converter.
        CCnvCharacterSetConverter*          iCharacterSetConverter;

        // The navipane decorator.
        CAknNavigationDecorator*            iNaviDecorator;

        // The navipane label.
        CAknNaviLabel*                      iNaviLabel;

        // The navipane control container.
        CAknNavigationControlContainer*     iNaviPane;
        
        

    };

#endif // USSDNAVIPANE_H

// End of File
