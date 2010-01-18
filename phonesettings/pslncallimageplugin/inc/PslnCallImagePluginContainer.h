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
* Description:  Container for Call Image settings view.
*
*/


#ifndef PSLNCALLIMAGEPLUGINCONTAINER_H
#define PSLNCALLIMAGEPLUGINCONTAINER_H

// INCLUDES
#include <pslnfwbasecontainer.h>
#include <centralrepository.h> 

// CONSTANTS
// Central repository values for key KThemesCallImageSupport
enum TPslnCRCallImageValues
    {
    EPlsnCRCallImageNone = 0,
    EPlsnCRUserDefinedImage,
    EPlsnCRThemeImage
    };
    
// Setting values, order must be same than in R_PSLN_CI_VIEW_LBX
enum TPslnCallImageSetting
    {
    EPlsnCallImageThemeImage,
    EPlsnCallImageNone,
    EPlsnCallImageUserDefinedImage
    };

// FORWARD DECLARATION
// CLASS DECLARATION

/**
*  CPslnCallImagePluginContainer container class.
*  @since Series 60_3.1
* 
*/
class CPslnCallImagePluginContainer : 
    public CPslnFWBaseContainer
    {
    public: // Constructors and destructor
        
        /**
        * Symbian OS constructor.
        * @param aRect Listbox's rect.
        * 
        */
        void ConstructL( const TRect& aRect );

        /**
        * Destructor.
        */
        ~CPslnCallImagePluginContainer();

    public: //new

        /**
        * Updates listbox's item's value.
        * @param aItemId specific item ID (ignored).
        */
        void UpdateListBoxL();
                
        /**
        * Gets currently active index of given property.
        * @return index of currently active property, or error code.
        */
        TInt CurrentSelectionIndexL();

    protected:
    
        void ConstructListBoxL( TInt aResLbxId );

    private: // new
    
        void CreateListBoxItemsL();
        
        /**
        * From CCoeControl. Gets help context.
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;
    
    private: // data
    
        // Listbox array for items.
        CDesCArray* iItemArray;
        
        // List of items from resources.
        CDesCArrayFlat* iItems;
        
        // One setting item.
        HBufC* iItemBuf; 
    };

#endif //PSLNCALLIMAGEPLUGINCONTAINER_H

// End of File
