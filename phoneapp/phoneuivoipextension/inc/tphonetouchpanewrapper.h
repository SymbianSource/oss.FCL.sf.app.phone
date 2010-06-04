/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Wrapper class to touch pane interface.
*
*/


#ifndef TPHONETOUCHPANEWRAPPER_H
#define TPHONETOUCHPANEWRAPPER_H

// INCLUDES
#include <e32base.h>
#include <teltouchpaneextension.h>

/**
 * Wrapper class to touch pane interface.
 *
 * @since S60 5.1
 */
NONSHARABLE_CLASS( TPhTouchPaneWrapper ): public MTelTouchPaneExtension
    {

public:
    TPhTouchPaneWrapper();
     
    /**
     * Returns number of buttons in touch pane.
     *
     * @since S60 v5.1
     * @return Button count.
     */
    TInt NumberOfButtonsInPane()  const;

    /**
     * Returns command id assinged to button in given position.
     * If button is toggling, currently active command is 
     * returned.
     *
     * @since S60 v5.1
     * @param aButtonIndex Button index starting 0.
     * @return Command id assigned to given position.
     */
    TInt ButtonCommandId( TInt aButtonIndex )  const;

    /**
     * Sets button dimmed.
     *
     * @since S60 v5.1
     * @param aCommand Command id.
     * @param aDimmed  ETrue to set dimmed, EFalse to undim.
     */
    void SetButtonDimmed( TInt aCommand, TBool aDimmed );

private:
    
    };


#endif TPHONETOUCHPANEWRAPPER_H

//End of file
