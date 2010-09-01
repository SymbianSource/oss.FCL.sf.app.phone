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
* Description:  gsvmbxplugin implementation.
*
*/

#ifndef GSVMBXPLUGIN_H
#define GSVMBXPLUGIN_H

// User includes

// System includes
#include <gsplugininterface.h>
#include <aknview.h>
#include <ConeResLoader.h>
#include <eikclb.h>
#include <AknServerApp.h>

// Classes referenced
class CAknNavigationDecorator;
class CAknViewAppUi;
class CAknNullService;

// Constants
const TUid KGSVmbxPluginUID = { 0x2000B4D6 };
const TUid KGsVmbxAppUid    = { 0x100058F5 };

_LIT( KGSVmbxApp, "z:\\sys\\bin\\vm.exe" );
_LIT( KVmbxPluginResourceFileName, "z:gsvmbxpluginrsc.rsc" );
_LIT( KGSVmbxPluginIconDirAndName, "z:gsvmbxplugin.mif"); // Use KDC_BITMAP_DIR

// CLASS DECLARATION

//For embedding VMBX in CS
class TAppInfo
    {
    public:
        TAppInfo( TUid aUid, const TDesC& aFile )
            : iUid( aUid ), iFile( aFile )
            {}
        TUid iUid;
        TFileName iFile;
    };


/**
* CGSVmbxPlugin.
*
* This class handles launching of the Voice Mailbox application from the 
* General Settings. The plugin is a type of EGSItemTypeSettingDialog and 
* therefore the GS FW will call HandleSelection() instead of DoActivate().
* No CAknView functionality is supported even though the base class is 
* CAknView derived via CGSPluginInterface.
*
*/
class CGSVmbxPlugin : public CGSPluginInterface,
                        public MAknServerAppExitObserver // Embedding
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSVmbxPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSVmbxPlugin();

    public: // From CAknView

        /**
        * See base class.
        */
        TUid Id() const;

    public: // From CGSPluginInterface

        /**
        * See base class.
        */
        void GetCaptionL( TDes& aCaption ) const;

        /**
        * See base class.
        */
        TInt PluginProviderCategory() const;

        /**
        * See base class.
        */
        TGSListboxItemTypes ItemType();

        /**
        * See base class.
        */
        void GetValue( const TGSPluginValueKeys aKey,
                       TDes& aValue );

        /**
        * See base class.
        */
        void HandleSelection( const TGSSelectionTypes aSelectionType );
        
        /**
        * See base class.
        */
        CGulIcon* CreateIconL( const TUid aIconType );

    protected: // New

        /**
        * C++ default constructor.
        */
        CGSVmbxPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

    protected: // From CAknView

        /**
        * This implementation is empty because this class, being just a dialog,
        * does not implement the CAknView functionality.
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );

        /**
        * This implementation is empty because this class, being just a dialog,
        * does not implement the CAknView functionality.
        */
        void DoDeactivate();

    private: // New

        /**
        * Opens localized resource file.
        */
        void OpenLocalizedResourceFileL(
            const TDesC& aResourceFileName,
            RConeResourceLoader& aResourceLoader );
        
        /**
        * Launches Voice Mailbox application.
        */
        void LaunchVoiceMailBoxAppL();
        
        /**
        * Launches application as embedded.
        */
        void EmbedAppL( const TAppInfo& aApp );
        
    protected: // Data

        // Resource loader.
        RConeResourceLoader iResources;
        
        // AVKON NULL service. Own.
        CAknNullService* iNullService;        
    };

#endif // GSVMBXPLUGIN_H
// End of File
