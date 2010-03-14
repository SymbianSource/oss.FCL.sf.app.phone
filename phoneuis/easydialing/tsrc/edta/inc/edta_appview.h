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

#ifndef __EDTA_APPVIEW_H__
#define __EDTA_APPVIEW_H__

// INCLUDES
#include "edta_screentextbuffer.h"
#include <coecntrl.h>
#include <MPsResultsObserver.h>
#include <mccaconnection.h>
#include <dialingextensioninterface.h>


// FORWARD DECLARATIONS
class CPsClientData;
class CPbkContactEngine;
class CPsQuery;
class CVPbkContactManager;
class CVPbkContactStoreUriArray;
class MVPbkContactLink;
class CPSRequestHandler;
class TCCAFactory;
class MCCAParameter;
class CDialingExtensionInterface;
class CDummyListener;



// CLASS DECLARATION
class CEdtaAppView : public CCoeControl, public MPsResultsObserver, public MCCAObserver, public MDialingExtensionObserver
    {
    public: // New methods

        static CEdtaAppView* NewL( const TRect& aRect );
        static CEdtaAppView* NewLC( const TRect& aRect );
        virtual ~CEdtaAppView();
         
        TInt CountComponentControls() const;
        CCoeControl* ComponentControl(TInt aIndex) const;
        
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
        
        CEdtaScreenTextBuffer* GetScreenBuffer();
        
        CDialingExtensionInterface* GetEasyDialer();
        
        void ShowMatchingContactsL(const TDesC& aSearchString);

        TBool IsPCSServerLoaded();
        void CheckPCSServerL();
        
        void ShowNameOrderL();
        
        void LoadPluginL();
        
        void LaunchNthContactL(TInt aIx);
        
        void SelectAndLaunchContactL();
        
        void DoMiscellaneousTestsL();
        
    public:  // Functions from base classes

        virtual void SizeChanged();

    private: // Constructors

        void ConstructL(const TRect& aRect);
        
        CEdtaAppView();
        
        void Draw( const TRect& aRect ) const;
        
        void InitContactDataStoreUrisL();
        void InitPredictiveContactSearchL();
         
    private:    

        //MPsResultsObserver functions
        virtual void HandlePsResultsUpdate(RPointerArray<CPsClientData>& aResults, RPointerArray<CPsPattern>& aSeqs);
        virtual void HandlePsError(TInt aErrorCode);
        virtual void CachingStatus(TCachingStatus& aStatus, TInt& aError);
       
        /**
         * Leaving function for Predictive search callback. Call TRAPPED in HandlePsResultsUpdate.
         */
        void HandlePredictiveSearchResultL(RPointerArray<CPsClientData>& aResults, RPointerArray<CPsPattern>& aSeqs);
       
        //MCCAObserver functions
        void CCASimpleNotifyL( TNotifyType aType, TInt aReason );
    
        void HandleDialingExtensionEvent( MDialingExtensionObserver::TEvent aEvent );

    private:
        
        CEdtaScreenTextBuffer* iScreenBuffer;
        
        CPSRequestHandler* iPredictiveContactSearchHandler;
        
        CVPbkContactManager* iContactManager;
        
        CVPbkContactStoreUriArray* iContactStoreUriArray;
        
        RPointerArray<TDesC> iContactDataStores;
        
        RPointerArray<TDesC> iContactNames;
        
        RPointerArray<MVPbkContactLink> iMatchingContactLinks;
        
        CPsQuery* iPredictiveSearchQuery;
        
        MCCAConnection* iContactLauncher;
        
        /*
         * Pointer to easy dialing plugin. This may be null, if no plugin present.
         * Null-check is a must before use.
         */
        CDialingExtensionInterface* iEasyDialer;
        
        CDummyListener* iDummyListener;

    };

#endif // __EDTA_APPVIEW_H__

// End of File

