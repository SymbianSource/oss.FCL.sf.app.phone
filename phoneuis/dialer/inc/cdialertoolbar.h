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
* Description:  Updates toolbar.
 *
*/


#ifndef __CDIALERTOOLBAR_H__
#define __CDIALERTOOLBAR_H__

#ifdef RD_SCALABLE_UI_V2

#include <akntoolbar.h>
#include <akntoolbarobserver.h>
#include <coecobs.h>    // MCoeControlObserver
#include <dialer.hrh>

//Forward declarations
class CAknToolbar;
class CAknButton;
class CAknAppUi;
class CCoeControl;
class CDialerNumberEntry;

NONSHARABLE_CLASS( CDialerToolBar ) : public CBase, 
    public MCoeControlObserver
    {
    public:
    
        /**
        * Two phase constructor
        * @param CAknAppUi reference
        * @param CDialerNumberEntry reference
        * @return New instance
        */    
        static CDialerToolBar* NewL( CAknAppUi* aAppUi, 
                                     CDialerNumberEntry& aNumberEntry );
        static CDialerToolBar* NewLC( CAknAppUi* aAppUi, 
                                      CDialerNumberEntry& aNumberEntry );
    
        /**
        * Destructor.
        */    
        virtual ~CDialerToolBar();

    public: // New functions
        
        void UpdateToolbar( TDialerCommandId aCommand );
        
        void DetermineCurrentToolbarButtons();


    public: // from MCoeControlObserver
    
        void HandleControlEventL( CCoeControl* aControl,TCoeEvent aEventType );

    private:
        
        /**
        * Constructor
        */        
        CDialerToolBar( CDialerNumberEntry& aNumberEntry );
    
        /**
        * 2nd phase constructor.
        */
        void ConstructL( CAknAppUi* aAppUi );
        
        CAknButton* CreateButtonL( TInt aNormalIconId,
                               TInt aNormalMaskId,
                               const TDesC& aText, 
                               const TAknsItemID& aSkinIconId,
                               TInt aCommand = EDialedCmdNone );

        
        void CreateButtonSetNumberEntryEmpty();
        
        void CreateButtonSetNumberEntryNotEmpty();

    private: // data

        CAknToolbar* iToolbar;
        CAknAppUi* iAppUi;
        TBool iNumberEntryEmpty;
        CDialerNumberEntry& iNumberEntry;

    };

#endif // RD_SCALABLE_UI_V2

#endif // CDIALERTOOLBAR


// End of File
