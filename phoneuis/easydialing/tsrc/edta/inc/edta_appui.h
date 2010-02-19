/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Easy dialing test application.
*
*/

#ifndef __EDTA_APPUI_H__
#define __EDTA_APPUI_H__

// INCLUDES
#include <aknappui.h>
#include "edta_screentextbuffer.h"

// FORWARD DECLARATIONS
class CEdtaAppView;
class CAiwServiceHandler;
class CRepository;

// CLASS DECLARATION
/**
* CEdtaAppUi application UI class.
* Interacts with the user through the UI and request message processing
* from the handler class
*/
class CEdtaAppUi : public CAknAppUi
    {
    public: // Constructors and destructor

        void ConstructL();
        CEdtaAppUi();
        virtual ~CEdtaAppUi();

        TBool CheckPluginLoadedAndShowNote();

    private:  // Functions from base classes

        void HandleCommandL( TInt aCommand );
		void HandleStatusPaneSizeChange();
		void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);

    protected:
        
    private: // Data

        CEdtaAppView* iAppView;
        
        // Contains query text
        TBuf<KEdtaMaxLineWidth> iQueryText;

        // OWN: aiw service handler
        CAiwServiceHandler* iServiceHandler;
        
        // own
        CRepository* iRepository;
    };

#endif // __EDTA_APPUI_H__


