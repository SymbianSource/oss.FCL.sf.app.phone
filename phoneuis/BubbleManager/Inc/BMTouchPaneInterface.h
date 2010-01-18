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
* Description:  Interface to add bubble touch pane buttons.
*
*/


#ifndef M_BUBBLETOUCHPANEINTERFACE_H
#define M_BUBBLETOUCHPANEINTERFACE_H

#include <e32std.h>

class CGulIcon;
class TAknsItemID;

/**
 *  Interface for obtaining icon from skin, the icon is shown in touch pane 
 *  button. The client using MBubbleTouchPaneInterface must implement this
 *  to support skinning.
 */
class MBubbleTouchPaneIconProvider
    {
public:        
    /**
     * Gets icon for command.
     *
     * @since S60 v5.0
     * @param aCommandId The command id.
     * @return Icon instance. NULL if icon doesn't exist for command.
     */
    virtual CGulIcon* GetIconForCommandL( TInt aCommandId ) = 0;

    /**
    * Gets button for command.
    *
    * @since TB 9.2
    * @param aCommandId The command id.
    * @return frameId. Normal if special button doesn't exist for command.
    */
    virtual TAknsItemID GetButtonForCommandL( TInt aCommandId ) = 0;

    };

/**
 *  Interface to add bubble touch pane buttons.
 *
 *
 *  @lib bubblemanager.lib
 *  @since S60 v5.0
 */
class MBubbleTouchPaneInterface
    {
public:
    /**
     * Constructs touch pane buttons from resource.
     *
     * @since S60 v5.0
     * @param aResourceId Resource id. 0 = empty set.
     * @return Error code.
     */
    virtual TInt SetButtonSet( TInt aResourceId ) = 0;
    
    /**
     * Sets toggling button to the state, which triggers
     * the given command, when pressed.
     *
     * @since S60 v5.0
     * @param aCommand Command id.
     */
    virtual void SetButtonState( TInt aCommand ) = 0;
    
    /**
     * Sets button dimmed.
     *
     * Toggling buttons should be set to a state, where aCommand
     * is the active command before applying this method.
     *
     * @since S60 v5.0
     * @param aCommand Command id.
     * @param aDimmed  ETrue to set dimmed, EFalse to undim.
     */
    virtual void SetButtonDimmed( TInt aCommand, TBool aDimmed ) = 0;
    
    /**
     * Returns number of buttons in touch pane.
     *
     * @since S60 v5.0
     * @return Button count.
     */
    virtual TInt NumberOfButtonsInPane()  const = 0;
    
    /**
     * Returns command id assinged to button in given position.
     * If button is toggling, currently active command is 
     * returned.
     *
     * @since S60 v5.0
     * @param aButtonIndex Button index starting 0.
     * @return Command id assigned to given position.
     */
    virtual TInt ButtonCommandId( TInt aButtonIndex )  const = 0;
    
    /**
     * Checks if button has state that generates given command.
     *
     * @since S60 v5.0
     * @param aButtonIndex Button index starting 0.
     * @param aCommand Command id.
     * @return ETrue if button has this state, otherwise EFalse.
     */
    virtual TBool ButtonHasState( TInt aButtonIndex, TInt aCommand )  const = 0;
    
    /**
     * Replaces button in given position. 
     *
     * @since S60 v5.0
     * @param aButtonIndex Button to be replaced.
     * @param aResourceId Resource definiton of replacing button.
     * @return Error code.
     */
    virtual TInt ReplaceButton( TInt aButtonIndex,
                                TInt aResourceId ) = 0;
                                
    /**
     * Set icon provider.
     *
     * @since S60 v5.0
     * @param aIconProvider Provider interface.
     */
    virtual void SetIconProvider(
        MBubbleTouchPaneIconProvider* aIconProvider ) = 0;                                
    };

#endif // M_BUBBLETOUCHPANEINTERFACE_H
