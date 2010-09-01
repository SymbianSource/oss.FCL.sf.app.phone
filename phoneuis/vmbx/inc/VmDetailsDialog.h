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
* Description:  Details dialog for last message.
*
*/


#ifndef CVMDETAILSDIALOG_H
#define CVMDETAILSDIALOG_H

//  INCLUDES
#include <AknDialog.h>
#include <centralrepository.h>

// FORWARD DECLARATIONS
class CPNGNumberGrouping;
class CVmDetDlgPropertyObserver;

// CLASS DECLARATION

/**
*  Details dialog for last message.
*
*  Usage:
*  CVmDetailsDialog* dlg = CVmDetailsDialog::NewL();
*  dlg->ExecuteLD( R_VMBX_DETAIL_DIALOG );
*
*  @lib 
*  @since 2.7
*/
class CVmDetailsDialog : public CAknDialog
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CVmDetailsDialog* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CVmDetailsDialog();
        
        // Called when the key value is changed.
        void HandlePropertyNotificationEventL( const TDesC& aValue );
                
    protected:  // New functions
        
        /**
        * Creates the listbox items
        * @param aItemArray Array to fill.
        */
        void PopulateListboxItemsL( CDesCArray& aItemArray );

        /**
        * Adds text line to the listbox.
        * @param aItemArray Array to append onto.
        * @param aHeading Text for the first (heading) column.
        * @param aData Text for the second (body/data) column.
        * @param aItemTextRect list box rectangle where the text is drawn
        * @param aUseMultipleLines ETrue if the text is wrapped into several
        *                                lines (if needed) in the listbox.
        *                          EFalse if the text is truncated into one
        *                                 line in the listbox (default).
        */
        static void AppendLineL( 
                        CDesCArray& aItemArray, 
                        const TDesC& aHeading, 
                        const TDesC& aData, 
                        TRect aItemTextRect,
                        TBool aUseMultipleLines = EFalse );

        /**
        * Sets the old title pane text.
        */
        void SetOldTitleL();

        /**
        * Format date string to UI style. Initial format must 
        * be DD/MM/YYYY. If not, this leaves with KErrCorrupt.
        * @param aDateString String to be modified.
        */
        static void FormatDateStringL( TDes& aDateString );

        /**
        * Format time string to UI style. Initial format must 
        * be HH:MM:SS. If not, this leaves with KErrCorrupt.
        * @param aTimeString String to be modified.
        */
        static void FormatTimeStringL( TDes& aTimeString );


    protected:  // Functions from CAknDialog
        
        /**
        * Called after the dialog is constructed and before it is launched.
        */
        void PreLayoutDynInitL();

        /**
        * Called when a key is pressed.
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                                     TEventCode aType );

        /**
        * Called when a softkey is pressed.
        */
        TBool OkToExitL( TInt aButtonId );

    private:

        /**
        * C++ default constructor.
        */
        CVmDetailsDialog();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data
        // Old title pane text
        HBufC* iOldTitlePaneText;

        // Central Repository
        CRepository* iSession;

        // Does the number grouping.
        CPNGNumberGrouping* iNumberGrouping;
        
        // Observer class        
        CVmDetDlgPropertyObserver* iObserver;
    
        // Starts observing for key value changes.
        void StartObservingL();
    };

#endif      // CVMDETAILSDIALOG_H   
            
// End of File
