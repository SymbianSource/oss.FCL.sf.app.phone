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
* Description:  Base class for all dialer containers
*
*/



#ifndef CDIALERCONTAINERBASE_H
#define CDIALERCONTAINERBASE_H

//  INCLUDES
#include <coecntrl.h>
#include <coemain.h>
#include <bldvariant.hrh>

#include "dialercommon.h"

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CAknButton;
class TAknsItemID;
class CGulIcon;

/**
*  CDialerContainerBase container class
*
*  @lib dialer.lib
*  @since S60 v5.0
*/
NONSHARABLE_CLASS(CDialerContainerBase) : public CCoeControl, 
                                          public MCoeControlObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Destructor.
        */
        virtual ~CDialerContainerBase();
            

    protected:
        
        // Constructor
        CDialerContainerBase( 
            CCoeControl& aContainer );

        /**
        * Base constructor.
        */
        void BaseConstructL( );

        /**
        * Set variety currently in use
        */
        virtual void SetVariety( ) = 0;
        
        /**
        * Set layout
        */
        virtual void SetLayout( ) = 0;   

        
    protected:  // Functions from CCoeControl
        
        /**
        * @see CCoeControl
        */
        TTypeUid::Ptr MopSupplyObject( TTypeUid aId );
        
        /**
        * @see CCoeControl
        */
        virtual void SizeChanged();
        
        /**
        * @see CCoeControl
        */
        virtual void PositionChanged();

        /**
        * @see CCoeControl
        */
        void HandleResourceChange( TInt aType );
             
        
    protected: // From MCoeControlObserver

        /**
        * @see MCoeControlObserver
        */
        virtual void HandleControlEventL( CCoeControl* /*aControl*/, TCoeEvent /*aEventType*/ ) {};

    protected:    // Data

        // Variety set according to conditions of number etc of buttons.
        // See Variety method of each implementing class.
        TInt iVariety;

        // Parent container
        CCoeControl& iParentControl;


    };

#endif      // CDIALERCONTAINERBASE_H

// End of File
