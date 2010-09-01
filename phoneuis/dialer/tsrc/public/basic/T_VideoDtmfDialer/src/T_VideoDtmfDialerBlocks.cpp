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
* Description:  Implementation of CT_VideoDtmfDialer class, API testing.
*
*/

// INCLUDE FILES 
#include <eiklabel.h> 
#include <AknAppUi.h>
#include <akntoolbar.h> 

#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "T_VideoDtmfDialer.h"
#include "T_VideoDtmfDialerAppView.h"

#include <cdialer.h>
#include <cvideodtmfdialer.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialer::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CT_VideoDtmfDialer::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialer::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CT_VideoDtmfDialer::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        ENTRY( "CreateView", CT_VideoDtmfDialer::CreateViewL ),
        ENTRY( "DeleteView", CT_VideoDtmfDialer::DeleteViewL ),
        ENTRY( "CreateDialer", CT_VideoDtmfDialer::CreateDialerL),
        ENTRY( "DeleteDialer", CT_VideoDtmfDialer::DeleteDialerL),
        ENTRY( "ShowDialer", CT_VideoDtmfDialer::ShowDialerL),
        ENTRY( "CreateVideoDtmfDialer", CT_VideoDtmfDialer::CreateVideoDtmfDialerL ),
        ENTRY( "ShowVideoDtmfDialer", CT_VideoDtmfDialer::ShowVideoDtmfDialerL),
        ENTRY( "DeleteVideoDtmfDialer", CT_VideoDtmfDialer::DeleteVideoDtmfDialerL ),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialer::CreateViewL()
// -----------------------------------------------------------------------------
//
TInt CT_VideoDtmfDialer::CreateViewL(CStifItemParser& /*aItem*/)
	{
	CCoeControl* videoDtmfDialerControl = CT_VideoDtmfDialerAppView::NewL(
		CEikonEnv::Static()->EikAppUi()->ClientRect() );	

	CleanupStack::PushL( videoDtmfDialerControl ); 
	CCoeEnv::Static()->AppUi()->AddToStackL( videoDtmfDialerControl );
	CleanupStack::Pop( videoDtmfDialerControl ); 

    // Ownership is transferred 
    iVideoDtmfDialerControl = videoDtmfDialerControl; 
    iVideoDtmfDialerControl->MakeVisible(ETrue);

    return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialer::DeleteViewL()
// -----------------------------------------------------------------------------
//
TInt CT_VideoDtmfDialer::DeleteViewL(CStifItemParser& /*aItem*/)
	{
	if( iVideoDtmfDialerControl )
		{
	    CCoeEnv::Static()->AppUi()->RemoveFromStack( iVideoDtmfDialerControl );
	    delete iVideoDtmfDialerControl;
	    iVideoDtmfDialerControl = NULL; 
		}

    return KErrNone;		
	}

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialer::CreateDialerL()
// -----------------------------------------------------------------------------
//
TInt CT_VideoDtmfDialer::CreateDialerL(CStifItemParser& /*aItem*/)
    {
    if( iVideoDtmfDialerControl )
	    {
	    iDialer = CDialer::NewL( *iVideoDtmfDialerControl, 
	    	CEikonEnv::Static()->EikAppUi()->ClientRect(),
	    	NULL );
	    iNumberEntry = iDialer->NumberEntry();

	    return KErrNone;
	    }
	else
		{
		return KErrGeneral; 	
		}	
    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialer::ShowDialerL()
// -----------------------------------------------------------------------------
//
TInt CT_VideoDtmfDialer::ShowDialerL(CStifItemParser& /*aItem*/)
    {
    if( iNumberEntry )
        {
        iNumberEntry->CreateNumberEntry();

        static_cast<CT_VideoDtmfDialerAppView*>(iVideoDtmfDialerControl)->SetControl( iDialer );

        return KErrNone;
        }
    else
        {
        return KErrGeneral; 
        }
    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialer::DeleteDialerL()
// -----------------------------------------------------------------------------
//
TInt CT_VideoDtmfDialer::DeleteDialerL(CStifItemParser& /*aItem*/)
    {
    delete iDialer; 
    iDialer = NULL;     

    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CT_VideoDtmfDialer::CreateVideoDtmfDialerL()
// -----------------------------------------------------------------------------
//
TInt CT_VideoDtmfDialer::CreateVideoDtmfDialerL(CStifItemParser& /*aItem*/)
    {
    if( iVideoDtmfDialerControl )
    	{
	    iVideoControl = new( ELeave ) CEikLabel;
	    iVideoControl->SetBufferReserveLengthL( 10 );
	    iVideoControl->SetTextL( _L("Video Here") ) ;
	    iVideoControl->SetAlignment( EHCenterVCenter );

	    iVideoDtmfDialer = CVideoDTMFDialer::NewL( 
	    	*iVideoDtmfDialerControl, 
           	*iVideoControl, 
           	CEikonEnv::Static()->EikAppUi()->ClientRect() );
           
		return KErrNone;		                                           
    	}
    else
    	{
    	return KErrGeneral; 	
    	}	    
    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialer::ShowVideoDtmfDialerL()
// -----------------------------------------------------------------------------
//
TInt CT_VideoDtmfDialer::ShowVideoDtmfDialerL(CStifItemParser& /*aItem*/)
    {
    if( iVideoDtmfDialerControl )
    	{
    	static_cast<CT_VideoDtmfDialerAppView*>(iVideoDtmfDialerControl)->SetControl( 
    	    iVideoDtmfDialer );
           
		return KErrNone;		                                           
    	}
    else
    	{
    	return KErrGeneral; 	
    	}	    
    }
    
// CT_VideoDtmfDialer::DeleteVideoDtmfDialerL()
// -----------------------------------------------------------------------------
//
TInt CT_VideoDtmfDialer::DeleteVideoDtmfDialerL(CStifItemParser& /*aItem*/)
    {
	delete iVideoControl; 
	iVideoControl = NULL; 
    delete iVideoDtmfDialer;                   
    iVideoDtmfDialer = NULL;
    
    return KErrNone;
    }
	

// End of file

