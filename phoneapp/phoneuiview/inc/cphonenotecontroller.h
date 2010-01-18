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
* Description:  PhoneUI Note Controller
*
*/

/*
*/
#ifndef CPHONENOTECONTROLLER_H
#define CPHONENOTECONTROLLER_H


//  INCLUDES
#include "cphoneprogressnote.h"
#include "mphonetimer.h"

// FORWARD DECLARATIONS
class TPhoneCommandParam;
class CPhoneTimer;
class CEikonEnv;
class CPhoneNote;
class CPhoneUIDisabledNote;

class CPhoneNoteController : public CBase, public MPhoneTimer
    {
    public:

        /**
        * Two-phased constructor.
        */
        static CPhoneNoteController* NewL( CEikonEnv& aEikEnv );

        /**
        * Destructor.
        */
        virtual ~CPhoneNoteController();   

        void CreateNoteL( TPhoneCommandParam* aCommandParam );

        void DestroyNote();
        
        void DestroyDTMFNote();
        
        void DestroyGlobalNoteL();

        void CreateGlobalNoteL( TPhoneCommandParam* aCommandParam );
        
        void CreateProgressNoteL( TPhoneCommandParam* aCommandParam );
        
        void DestroyProgressNote();
        
        /**
        * This function is called after on timeout.
        */
        void HandleTimeOutL();
        
        /**
        * Writes information is note visible or not
        * to the given command parameter.
        * @param aCommandParam - boolean param
        */
        void IsNoteVisible( TPhoneCommandParam* aCommandParam );        

        /**
        * Writes information is progress note visible or not
        * to the given command parameter.
        * @param aCommandParam - boolean param
        */
        void IsProgressNoteVisible( TPhoneCommandParam* aCommandParam );   
        
        /**
        * Writes information is note or not
        * to the given command parameter.
        * @param aCommandParam - boolean param
        */
        void IsNoteActive( TPhoneCommandParam* aCommandParam );  
        
        /**
        * Returns true if active note is dismissable by key event, else false.
        * @param aCommandParam - boolean param
        */
        void IsActiveNoteDissmissableByKeyEvent( TPhoneCommandParam* aCommandParam );

        /**
        * Updates softkeys to note 
        * @param aCommandParam - integer param for resolved resource id
        */
        void UpdateNoteSoftkeysL( TPhoneCommandParam* aCommandParam );
        
    private:
        
        /**
        * C++ default constructor.
        */
        CPhoneNoteController( CEikonEnv& aEikEnv );

        /**
        * Updates information text to ongoing note
        * @param aCommandParam - note param
        */
        void UpdateNoteTextL( TPhoneCommandParam* aCommandParam );    

    private:  // Data

        // Actual note
        CPhoneNote* iNote;
        
        CPhoneProgressNote* iProgressNote;
        
        // Owned timer instance.
        CPhoneTimer* iProgressUpdateTimer;
        
        // Interval of progress updates.
        TInt iInterval;

        // Current progress.
        TInt iProgressBarCount;

        // Time to wait until next attempt.
        TInt iWaitTime;
        
        TTime iStartTime;
        
        /**
        * ID of global note.
        */
        TInt iGlobalNoteId;
        
        // Eikon environment delivered from CPhoneViewController
        CEikonEnv& iEikEnv;
        
        // Note's text
        TDesC* iNoteText;
        
    };

#endif      // CPHONENOTECONTROLLER_H   
            
// End of File
