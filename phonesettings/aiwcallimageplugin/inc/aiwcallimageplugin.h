/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides ECOM interface implementation for aiwcallimageplugin.
*
*/


#ifndef AIWCALLIMAGEPLUGIN_H
#define AIWCALLIMAGEPLUGIN_H


//  INCLUDES
#include    <AiwServiceIfMenu.h>
#include    <AiwCommon.h>


// FORWARD DECLARATIONS
class MAiwNotifyCallback;


// CLASS DECLARATION

/**
*  AiwCallImagePlugin implementation.
*
*  @since 3.2
*/
class CAiwCallImagePlugin : public CAiwServiceIfMenu
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * 
        * @return An instance of CaUiPlugin.
        */
        static CAiwCallImagePlugin* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CAiwCallImagePlugin();

    public: // Functions from base classes

        /**
        * @see CAiwServiceIfBase::InitialiseL.
        */
        void InitialiseL(
            MAiwNotifyCallback& aFrameworkCallback,
            const RCriteriaArray& aInterest );

        /**
        * @see CAiwServiceIfBase::HandleServiceCmdL.
        */
        void HandleServiceCmdL(
            const TInt& aCmdId,
            const CAiwGenericParamList& aInParamList,
            CAiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            const MAiwNotifyCallback* aCallback = NULL );

        /**
        * @see CAiwServiceIfMenu::InitializeMenuPaneL.
        */
        void InitializeMenuPaneL(
        CAiwMenuPane& aMenuPane,
            TInt aIndex,
            TInt aCascadeId,
            const CAiwGenericParamList& aInParamList );

        /**
        * @see CAiwServiceIfMenu::HandleMenuCmdL.
        */
        void HandleMenuCmdL(
            TInt aMenuCmdId,
            const CAiwGenericParamList& aInParamList,
            CAiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            const MAiwNotifyCallback* aCallback = NULL );

    private:

        /**
        * C++ default constructor.
        */
        CAiwCallImagePlugin();
   
    private:
    
        /**
        * Shows confirmation note.
        */
        void ShowNoteL( TInt aError );
        
        /**
        * Shows drm error note.
        */
        void ShowDRMErrorNoteL();
        
        /**
        * Sets call image path to Central Repository.
        */
        TInt SetCallImagePathL( const TAiwGenericParam* aPathParam );
        
        /**
        * Reads AIW parameters.
        */
        TPtrC GetAiwParamAsDescriptor( 
            const CAiwGenericParamList& aParamList,
		    TGenericParamId aParamType );
	    
	    /**
        * Checks if call image plugin menu option is to be loaded.
        */
	    TBool IsMimeTypeSupportedL( const TDesC& aMimeTypeString );
    };

#endif  // AIWCALLIMAGEPLUGIN_H

// End of file
