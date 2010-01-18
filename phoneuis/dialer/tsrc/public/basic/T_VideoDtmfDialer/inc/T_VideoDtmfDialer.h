/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Module/API tester for CVideoDtmfDialer domain API 
*
*/



#ifndef T_VIDEODTMFDIALER_H
#define T_VIDEODTMFDIALER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <aknapp.h>

// MACROS
_LIT( KT_VideoDtmfDialerLogPath, "\\logs\\testframework\\T_VideoDtmfDialer\\" ); 
_LIT( KT_VideoDtmfDialerLogFile, "T_VideoDtmfDialer.txt" ); 
_LIT( KT_VideoDtmfDialerLogFileWithTitle, "T_VideoDtmfDialer_[%S].txt" );

// FORWARD DECLARATIONS
class CDialer;
class CT_VideoDtmfDialer;
class CCoeControl;
class CEikLabel; 
class CVideoDTMFDialer; 
class MNumberEntry;

// CLASS DECLARATION

/**
*  CT_VideoDtmfDialer test class for STIF Test Framework TestScripter.
*/
NONSHARABLE_CLASS(CT_VideoDtmfDialer) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CT_VideoDtmfDialer* NewL(CTestModuleIf& aTestModuleIf);

        /**
        * Destructor.
        */
        virtual ~CT_VideoDtmfDialer();

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL(CStifItemParser& aItem);

    private:

        /**
        * C++ default constructor.
        */
        CT_VideoDtmfDialer( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

    private: // Ui helper methods
    
        /** 
        * Helper functions for UI handling are listed below
        */ 
        /**
        * Set toolbar visibility
        */        
        void MakeToolbarVisible(TBool aVisible);

        /**
        * Set status panel visibility
        */        
        void MakeStatusPaneVisible(TBool aVisible);

    private: // Test methods are listed below
    
        /**
        * Create view 
        */
        virtual TInt CreateViewL(CStifItemParser& aItem);

        /**
        * Delete view 
        */
        virtual TInt DeleteViewL(CStifItemParser& aItem);

        /**
        * Create CDialer
        */
        virtual TInt CreateDialerL(CStifItemParser& aItem);

        /**
        * Show CDialer
        */
        virtual TInt ShowDialerL(CStifItemParser& aItem);

        /**
        * Delete CDialer
        */
        virtual TInt DeleteDialerL(CStifItemParser& aItem);

        /**
        * Create CVideoDtmfDialer
        */
        virtual TInt CreateVideoDtmfDialerL(CStifItemParser& aItem);

        /**
        * Show CVideoDtmfDialer
        */
        virtual TInt ShowVideoDtmfDialerL(CStifItemParser& aItem);

        /**
        * Delete CVideoDtmfDialer
        */
        virtual TInt DeleteVideoDtmfDialerL(CStifItemParser& aItem);

    private: // Data        
        
        // Pointer to video dtmf dialer
		CVideoDTMFDialer* iVideoDtmfDialer; 
		// Pointer to video dtmf dialer control 
    	CCoeControl* iVideoDtmfDialerControl;
    	// Pointer to video control label
		CEikLabel* iVideoControl;
    	// Pointer to dialer
	    CDialer* iDialer;
    	// Interface to Dialer's number entry functionality   
	    MNumberEntry* iNumberEntry;    	
    };

#endif      // T_VIDEODTMFDIALER_H

// End of File
