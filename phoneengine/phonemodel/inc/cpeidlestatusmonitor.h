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
* Description:  This file contains the header file for CPEIdleStatusMonitor class
*
*/


#ifndef CPEIdleStatusMonitor_H
#define CPEIdleStatusMonitor_H

//INCLUDES
#include <e32base.h>
#include <e32property.h>

// FORWARD DECLARATIONS
class CPEPhoneModel;

// CLASS DECLARATION

/**
*  CPEIdleStatusMonitor
*  Monitors the idle status P&S key for changes
*
*  @since S60_5.0
*/
NONSHARABLE_CLASS( CPEIdleStatusMonitor ) : public CActive
    {
    public:
        /**
        * Two-phased constructor.
        */
        static CPEIdleStatusMonitor* NewL( 
            CPEPhoneModel& aOwner );
            
        /**
        * Destructor.
        */
        virtual ~CPEIdleStatusMonitor();     
               
    protected:
        /**
        * C++ default constructor.
        */    
        CPEIdleStatusMonitor( CPEPhoneModel& aOwner );

        /**
        * Second-phase constructor.
        */
        void ConstructL();    
        
    private: // From CActive

        /**
        * Implements cancellation of an outstanding request.
        */
        void DoCancel();

        /**
        * Handles request completion event.
        */
        void RunL();
        
    private:

        // MPEPhoneModelInternal owns this object.
        CPEPhoneModel& iOwner;
        // The interface to Publish And Subscribe.
        RProperty iProperty;
    };

#endif  //  CPEIdleStatusMonitor_H
