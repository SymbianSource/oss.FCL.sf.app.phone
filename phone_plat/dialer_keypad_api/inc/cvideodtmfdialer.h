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
* Description:  CVideoDTMFDialer class provides dialer keypad 
*                functionalities
*
*/



#ifndef CVIDEODTMFDIALER_H
#define CVIDEODTMFDIALER_H

//  INCLUDES
#include <coecntrl.h>
#include <coemain.h>
// CONSTANTS

// FORWARD DECLARATIONS
class CDialerKeyPadContainer;
class CDialerVideoContainer;
class CEikonEnv;

// CLASS DECLARATION

/**
*  CVideoDTMFDialer class.
*
*  @lib dialer.lib
*  @since Series60_5.0
*
*  Description:
*
*   In Video Telephony dialer offers touch keypad for enter as DTMFs 
*   and client's CCoeControl is used to show video and keypad. 
*   Video DTMF dialer does not implement menu or command buttons, 
*   only keypad which presses are forwarded to host application as key events.  
*
*   From host application dialer requires:
*   - View/container as CCoeControl
*   - App UI HandleCommand, DynInitMenuBar/Pane and CBA handling 
*   - HandleKeyEventL to handle numbers/characters from touch pane input.
*
* 
*  Usage:

    // Create 'video window'.
    CCoeControl* videoControl = new( ELeave ) CEikLabel;
    videoControl->SetBufferReserveLengthL( 10 );
    videoControl->SetTextL( _L("Video Here") ) ;

    // Create dialer.
    CVideoDTMFDialer* videoDialer = CVideoDTMFDialer::NewL( *this, *videoControl, aDialerArea );
    // Delete dialer
    delete dialer;
    
*
*/
NONSHARABLE_CLASS(CVideoDTMFDialer) : public CCoeControl
    {
    public:  // Constructors and destructor
    
        /**
        * Two phase constructor
        * @param aContainer Parent container
        * @param aVideoWindow Control viewing video
        * @param aRect Area to use for dialer
        * @return New instance or Dialer
        */
        IMPORT_C static CVideoDTMFDialer* NewL( const CCoeControl& aContainer, 
            CCoeControl& aVideoWindow,
            const TRect& aRect );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CVideoDTMFDialer();

    private:  // Functions from CCoeControl
        
        /**
        * @see CCoeControl
        */
        void SizeChanged();
        
        /**
        * @see CCoeControl
        */
        void PositionChanged();

        /**
        * @see CCoeControl
        */
        TInt CountComponentControls() const;
        
        /**
        * @see CCoeControl
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;
        
        /**
        * @see CCoeControl
        */
        void Draw( const TRect& aRect ) const;

        /**
        * @see CCoeControl
        */
        void HandleResourceChange( TInt aType );
        
        /**
        * @see CCoeControl
        */
        void HandlePointerEventL( const TPointerEvent& aPointerEvent );
        
    private:

        /**
        * Constructor
        */
        CVideoDTMFDialer( );

        /**
        * 2nd phase constructor.
        * @param aContainer Parent container
        * @param aVideoWindow Control containing the video
        * @param aRect area to use for dialer
        */
        void ConstructL( const CCoeControl& aContainer , 
            CCoeControl& aVideoWindow,
            const TRect& aRect );
        
        /**
        * Load resource file.
        */
        void LoadResourceL();
        
        /**
        * Unload resource file.
        */
        void UnLoadResources();
 
        /**
        * @see CCoeControl::ComponentControl()
        * @param aIndex control index
        */
        CCoeControl* ComponentControlForVideoMode( const TInt aIndex ) const;
        
    private:    // Data
           
        // Keypad container  - owned
        CDialerKeyPadContainer* iKeypadArea;
        
        // Video container - owned
        CDialerVideoContainer* iVideoWindow;
        
        // Parent container
        const CCoeControl* iParentControl;

        // Reference to CEikonEnv
        CEikonEnv& iEikEnv;
        
        // Resource 
        TInt iResourceOffset;
        
        // For future use.        
        TBool* iReserved1;
        TBool* iReserved2;
    };

#endif      // CVIDEODTMFDIALER_H

// End of File
