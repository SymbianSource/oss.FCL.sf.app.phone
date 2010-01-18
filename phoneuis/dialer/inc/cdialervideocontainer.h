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
* Description:  CDialerVideoContainer class handles positioning 
*                and viewing the control.
*
*/



#ifndef CDIALERVIDEOCONTAINER_H
#define CDIALERVIDEOCONTAINER_H

//  INCLUDES

//#include <fbs.h>
#include <coecntrl.h>
#include <coemain.h>

#include "cdialercontainerbase.h"

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  CDialerVideoContainer container class
*
*  @lib dialer.lib
*  @since S60 v5.0
*/
NONSHARABLE_CLASS(CDialerVideoContainer) : 
    public CDialerContainerBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two phase constructor
        * @param aContainer Parent container
        * @param aVideoWindow
        * @return New instance
        */
        static CDialerVideoContainer* NewL( 
            const CCoeControl& aContainer,
            CCoeControl& aVideoWindow );
        
        /**
        * Destructor.
        */
        virtual ~CDialerVideoContainer();

    private:  // Functions from base classes

        /**
        * From CCoeControl
        */
        TInt CountComponentControls() const;
        
        /**
        * From CCoeControl
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
        * From CCoeControl
        */
        void Draw( const TRect& /*aRect */) const;
    
    public:        
        /**
        * @see CCoeControl
        */
        void SetFocus( TBool aFocus, 
                       TDrawNow aDrawNow=ENoDrawNow );        
    
    private: // From CDialerContainerBase
   

        /**
        * @see CDialerContainerBase
        */
        void SetVariety( );
        
        /**
        * @see CDialerContainerBase
        */
        void SetLayout( );
        
        
    private:

        /**
        * 2nd phase constructor.
        */
        void ConstructL();
            
        /**
        * Constructor
        * @param aContainer Parent container
        * @param aVideoWindow
        * @param aCommandHandler
        */
       CDialerVideoContainer(
            const CCoeControl& aContainer,
            CCoeControl& aVideoWindow );

    private:    // Data

        // Video window
        CCoeControl& iVideoWindow;
    };

#endif      // CDIALERVIDEOCONTAINER_H

// End of File
