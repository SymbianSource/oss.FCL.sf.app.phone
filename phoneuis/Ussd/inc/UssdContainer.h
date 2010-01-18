/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef USSDCONTAINER_H
#define USSDCONTAINER_H


// INCLUDES
#include <coecntrl.h>
#include <bldvariant.hrh>
// Include file for MEikEdwinObserver
#include <eikedwob.h>

// FORWARD DECLARATIONS
class CEikEdwin;
class CUssdEditorLines;
class CUssdAppUi;
class CAknsBasicBackgroundControlContext;
class CUssdNaviPane;


// CLASS DECLARATION

/**
*  CUssdContainer
*  Declares container control for application.
*
*  @since 1.0
*/
class CUssdContainer : public CCoeControl,
                       public MEikEdwinObserver
    {
    public: // Constructors and destructor

        /**
        * Constructor.
        * @param aAppUi A reference to AppUI for container.
        */
        CUssdContainer( CUssdAppUi& aAppUi );


        /**
        * 2nd phase constructor
        * @param aRect Frame rectangle for container.
        */
        void ConstructL( const TRect& aRect );

        /**
        * Destructor.
        */
        ~CUssdContainer();

    public: // New functions

        /**
        * Gives editor.
        * @return Reference to editor
        */
        CEikEdwin& Editor();

    public: // Functions from base classes

        /**
        * From CoeControl,GetHelpContext.
        * Does nothing if help is not defined.
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;

        /**
        * From CoeControl, FocusChanged
        */
        void FocusChanged( TDrawNow aDrawNow );

        /**
        * Update the message length information to navipane
        */
        void UpdateNavipaneMsgLengthL();

    private: // Functions from base classes

        /**
        * From CoeControl, SizeChanged.
        */
        void SizeChanged();

        /**
        * From CoeControl, CountComponentControls.
        */
        TInt CountComponentControls() const;

        /**
        * From CCoeControl, ComponentControl.
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
        * From CCoeControl, Draw.
        */
        void Draw( const TRect& aRect ) const;

        /**
        * From CCoeControl, OfferKeyEventL.
        */
        TKeyResponse OfferKeyEventL(
            const TKeyEvent& aKeyEvent, TEventCode aType );

        /**
        * From CCoeControl, HandleResourceChange
        */
        void HandleResourceChange( TInt aType );

        /**
        * From CCoeControl, MopSupplyObject
        */
        TTypeUid::Ptr MopSupplyObject( TTypeUid aId );

        /**
        * Functions from MEikEdwinObserver, Handle editor event.
        */
        void HandleEdwinEventL( CEikEdwin* aEdwin, TEdwinEvent aEventType );

    private: //new functions

        // Creates edwin
        CEikEdwin* CreateEdwinL();

    private: //data

        CUssdAppUi& iAppUi;       // Application UI
        CEikEdwin* iEditor;       // The edit window.
        CUssdEditorLines* iLines; // Application ine drawer

        CAknsBasicBackgroundControlContext* iBgContext; // Skinning context.
        // The navipane control handler
        CUssdNaviPane*               iNaviPaneHandler;
    };

#endif // USSDCONTAINER_H

// End of File