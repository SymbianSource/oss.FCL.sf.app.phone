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
* Description:  View for setting call image.
*
*/


#ifndef PSLNCALLIMAGEPLUGIN_H
#define PSLNCALLIMAGEPLUGIN_H

// INCLUDES
#include <aknsettingpage.h>
#include <pslnfwbaseview.h>
#include <ConeResLoader.h>
#include <pslnfwplugininterface.h>
#include <centralrepository.h> 
#include <mpslnfwmsklabelobserver.h>

//CONSTANTS
const TUid KPslnCallImagePluginUid = { 0x102818EF };

// FORWARD DECLARATIONS
class CAknViewAppUi;
class CPslnCallImagePluginContainer;
class CPslnCallImagePluginDRM;

// CLASS DEFINITION
/**
*  CPslnCallImagePlugin view class for call image settings
*  @since Series 60_3.2
* 
*/
class CPslnCallImagePlugin : public CPslnFWBaseView,
                             public MPslnFWMSKObserver
    {            
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return Psln AI skinning view.
        */
        static CPslnCallImagePlugin* NewL( TAny* aAppUi );
        
        /**
        * Destructor
        */
        ~CPslnCallImagePlugin();
        
    public: // From CPslnFWnInterface

        /**
        * @see CPslnFWPluginInterface.
        */
        void GetCaptionL( TDes& aCaption ) const;
        

        /**
        * @see CPslnFWPluginInterface.
        */
        void GetTabTextL( TDes& aCaption ) const;
        
        /**
        * @see CPslnFWPluginInterface.
        */
        CGulIcon* CreateIconL();

        /**
        * @see CPslnFWPluginInterface.
        */
        void GetLocationTypeAndIndex( 
            TPslnFWLocationType& aType, 
            TInt& aIndex ) const;
                
    public: // from AknView
        
        /**
        * Returns view id.
        * @return TUid
        */
        TUid Id() const;

        /**
        * Handles commands.
        * @param aCommand Command to be handled.
        * 
        */
        void HandleCommandL( TInt aCommand );

    public: // new
        
        /**
        * Get CPslnCallImagePlugin's container.
        * @return pointer to container.
        */
        CPslnCallImagePluginContainer* Container();
        
        /**
        * From MPslnFWMSKObserver.
        * Checks if the MSK label needs to be adjusted.
        */    
        void CheckMiddleSoftkeyLabelL();
    
    protected: // From CAknView 
    
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
        void DoDeactivate();

        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
    
    protected:

        /**
        * C++ default constructor.
        */
        CPslnCallImagePlugin( CAknViewAppUi* aAppUi );

        /**
        * Symbian OS default constructor.
        * 
        */
        void ConstructL();

    private: // from CPslnFWBaseView
        
        /**
        * @see CPslnFWBaseView.
        */
        void NewContainerL();

        /**
        * @see CPslnFWBaseView.
        */
        void HandleListBoxSelectionL();
        
        /**
        * @see CPslnFWBaseView.
        */
        void SetTitlePaneL( TInt& aResourceId );
        
    private:
    
    	void HandleImageErrorsL( TInt aError );
    
    	void SetCallImageL();

        TInt SetCallImagePath( const TDesC& aImagePath );
        
        void SetCallImageSettingL( const TInt aValue );
        
        TInt SetPathL( const TDesC& aPath );
        
        void RemoveCommandFromMSK();
                
    private:
	    
        // Resource loader.
        RConeResourceLoader iResourceLoader;
        
        // Resource loader for common psln resources.
	    RConeResourceLoader iResourceLoaderCommon;  
        
        // Resource loader for Psln app resources.
        RConeResourceLoader iResourceLoaderPsln;
                
        // Application UI pointer (not owned)
        CAknViewAppUi* iAppUi;
        
        // Call Image Plugin 
        CPslnCallImagePluginDRM* iVerifier;
    };

#endif //PSLNCALLIMAGEPLUGIN_H

// End of File
