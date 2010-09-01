/*
* Copyright (c) 2009, 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Helper class to handle virtual keypad text labels
*
*/

#ifndef CDIALERKEYPADLABELMANAGER_H
#define CDIALERKEYPADLABELMANAGER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <badesca.h>
#include "cdialercenrepwrapper.h"


// FORWARD DECLARATIONS
class CCoeEnv;
class CPtiEngine;
class CEnvironmentChangeNotifier;


// CALLBACK INTERFACE
NONSHARABLE_CLASS( MDialerKeyPadLabelManagerCallback )
    {
public:
    /**
     * This event is given when change in system settings has
     * caused key label texts to change. All the TPtrC descriptors
     * previously returned by CDialerKeyPadLabelManager become
     * dangling references when this happens and they must not
     * be used after this method is called. 
     */
    virtual void KeyLabelsChanged() = 0;
    };


// CLASS DECLARATION

/**
 *  CDialerKeyPadLabelManager
 * 
 */
NONSHARABLE_CLASS( CDialerKeyPadLabelManager ) : public CBase, public MDialerCenRepObserver
    {
public: // Constructors and destructor
    
    /**
     * Destructor.
     */
    ~CDialerKeyPadLabelManager();

    /**
     * Two-phased constructor.
     */
    static CDialerKeyPadLabelManager* NewL( CCoeEnv& aCoeEnv, 
            MDialerKeyPadLabelManagerCallback& aObserver );

    /**
     * Two-phased constructor.
     */
    static CDialerKeyPadLabelManager* NewLC( CCoeEnv& aCoeEnv, 
            MDialerKeyPadLabelManagerCallback& aObserver );

private: // consturctors

    /**
     * Constructor for performing 1st stage construction
     */
    CDialerKeyPadLabelManager( CCoeEnv& aCoeEnv, 
            MDialerKeyPadLabelManagerCallback& aObserver );

    /**
     * Symbian default constructor for performing 2nd stage construction
     */
    void ConstructL();

public: // methods
    
    /**
     * Get number label for the button.
     * @return   Reference to the number text in current global number mode. 
     *           The reference is valid until this label manager is deleted or 
     *           global number mode is changed. Callback KeyLabelsChanged()
     *           is given to observer in the latter case.
     */        
    TPtrC ButtonNumLabel( TInt aIndex ) const;
    
    /**
     * Get first row alphabet label for the button
     * @return  Reference to descriptor containing text for the first
     *          row of alphabets to be shown on the key. The reference is
     *          valid until this label manager is deleted or input language
     *          is changed. Callback KeyLabelsChanged()
     *          is given to observer in the latter case.        
     */        
    TPtrC ButtonFirstAlphaLabel( TInt aIndex ) const;
    
    /**
     * Get second row alphabet label for the button
     * @return  Reference to descriptor containing text for the first
     *          row of alphabets to be shown on the key. The reference is
     *          valid until this label manager is deleted or input language
     *          is changed. Callback KeyLabelsChanged()
     *           is given to observer in the latter case.
     */        
    TPtrC ButtonSecondAlphaLabel( TInt aIndex ) const;

public: // from MDialerCenRepObserver
    
    /**
     * Handle change in one of the observed repositories.
     */
    void HandleRepositoryChange( TUid aRepository, TUint32 aId );
    
private: // methods
    
    /**
     * Handle environment change event. We are interested about digit
     * type changes in locale data.
     */
    void HandleEnvChange();
    
    /**
     * Static wrapper for HandleEnvChange() function to be passed in TCallBack object.
     */
    static TInt EnvChangeCallback( TAny* aSelfPtr );
    
    /**
     * Update button text labels to match current input and UI language.
     */
    void UpdateTextLabelsL();
    
    /**
     * Update button number labels to use current global number mode.
     */
    void UpdateNumLabelsL();
    
    /**
     * Get the full path of the keypad resource file to be used in current situation.
     * @param   aFileName   On return, contains the file name. Caller is responsible
     *                      of supplying large enough buffer. Maximum path length
     *                      in Symbian is 256 characters.
     */
    void GetResourceFileNameL( TDes& aFileName );
    
    /**
     * Returns code of the script to be used in current situation.
     * It depends on the current input language, current UI language, and all
     * available input languages on the device.
     */
    TInt LabelScriptToUseL() const;
    
    /**
     * Returns code of the Chinese script to be used in key labels.
     * This depends on the currently active Chinese adaptive search mode.
     * This should be used only when current input language is Chinese.
     */
    TInt ChineseScriptToUseL() const;
    
private: // data
    
    /** Control environment to use for resource management */
    CCoeEnv& iCoeEnv;
    
    /** Observer is notified when keypad labels are changed */
    MDialerKeyPadLabelManagerCallback& iObserver;
    
    /** PtiEngine instance is needed to find out available input languages */
    CPtiEngine* iPtiEngine;
    
    /** AknFep repository stores the current input language */
    CDialerCenRepWrapper* iAknFepRepository;
    
    /** Avkon repository stores the current Chinese matching mode */
    CDialerCenRepWrapper* iAvkonRepository;
    
    /** Helper to observe locale changes. Locale data contains digit type used for number labels. */
    CEnvironmentChangeNotifier* iEnvChangeNotifier;
    
    /** 
     * Array of number labels. Indexing matches that of the 
     * CDialerKeypadContainer::iButtons array. Owned. 
     */
    CDesCArray* iKeypadNumLabelArray;
    
    /** 
     * Array of first row alphabet labels. Indexing matches that of the 
     * CDialerKeypadContainer::iButtons array. Owned. 
     */
    CDesCArray* iKeypadFirstLabelArray;
    
    /** 
     * Array of second row alphabet labels. Indexing matches that of the 
     * CDialerKeypadContainer::iButtons array. Owned. 
     */
    CDesCArray* iKeypadSecondLabelArray;
    
    /** 
     * Offset value of currently loaded keypad resource. 
     * 0 if there's no loaded resource. 
     */
    TInt iResourceOffset;
    
    /** Script ID of the current keypad labels */
    TInt iLabelScriptInUse;
    
    /** Digit type currently used for keypad number labels */
    TDigitType iDigitTypeInUse;
    };

#endif // CDIALERKEYPADLABELMANAGER_H

