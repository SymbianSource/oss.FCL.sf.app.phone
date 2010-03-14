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
* Description:  Handles the updating dialer, toolbar buttons.
*
*/


#ifndef CPHONEDIALERCONTROLLER_H
#define CPHONEDIALERCONTROLLER_H

#include <e32base.h>
#include "mphonedialercontroller.h"
#include "mphonenumberentrychangedhandler.h"

class CPhoneBubbleWrapper;
class CCoeEnv;
class MAknsSkinInstance;
class TPhoneCommandParam;
class CPhoneEasyDialingController;

/**
 *  Dialer controller, provides toolbar button data for the normal mode dialer
 *
 *  @lib PhoneUIView.lib
 *  @since S60 v5.0
 */
class CPhoneDialerController : 
    public CBase,
    public MPhoneDialerController
    {
public: // constructor and destructor
    /**
     * Two-phased constructor.
     * @param aBubbleWrapper    Handle to bubble wrapper. Ownership not transferred.
     *                          May be NULL, but then emergency call state can't be identified
     * @param aCoeEnv           Handle to coeenv. Ownership not transferred.
     */
    static CPhoneDialerController* NewL( CPhoneBubbleWrapper* aBubbleWrapper,
                                         CCoeEnv& aCoeEnv );
    /**
     * Destructor.
     */
    IMPORT_C virtual ~CPhoneDialerController();
    
protected: // constructors
    
    /**
     * Constructor.
     * 
     * @param aBubbleWrapper 
     * @param aCoeEnv 
     */
    IMPORT_C CPhoneDialerController( CPhoneBubbleWrapper* aBubbleWrapper,
                                     CCoeEnv& aCoeEnv );
    /**
     * Symbian 2nd phase constructor.
     * 
     */
    IMPORT_C void ConstructL();
    
public: // new methods
    
    /**
     * Sets state of restricted dialer.
     *
     * @since S60 v5.0
     * @param aRestricted
     */      
    void SetRestrictedDialer( TBool aRestricted );    
    
     /**
     * Sets state of service code flag.
     *
     * @since S60 v5.0
     * @param aCommandParam
     */
    void SetServiceCodeFlag( TPhoneCommandParam* aCommandParam );

    /**
     * Sets handle to the Easy Dialing controller.
     * 
     * @since S60 v5.2
     * @param aController   Easy Dialing controller. Ownership not transferred.
     */
    void SetEasyDialingController( CPhoneEasyDialingController* aController );
    
public: // From MPhoneDialerController

    /**
     * @see MPhoneDialerController
     */
    IMPORT_C TInt CbaResourceId() const;

    /**
     * @see MPhoneDialerController
     */
    IMPORT_C TInt MenuResourceId() const;

    /**
     * @see MPhoneDialerController
     */
    IMPORT_C const TDesC& NumberEntryPromptTextL();

    /**
     * @see MPhoneDialerController
     */

    /**
     * @see MPhoneDialerController
     */
    IMPORT_C TInt GetButtonData( TButtonIndex aIndex, RPointerArray<CButtonData>& aData ) const;
    
    /**
     * @see MPhoneDialerController
     */
    IMPORT_C void SetNumberEntryIsEmpty( TBool aEmpty );
    
    /**
     * @see MPhoneDialerController
     */
    IMPORT_C TInt ButtonState( TButtonIndex aIndex ) const;
    
    /**
     * @see MPhoneDialerController
     */
    IMPORT_C TBool ButtonDimmed( TButtonIndex aIndex ) const;
    
    /**
     * @see MPhoneDialerController
     */
    IMPORT_C TBool EasyDialingAllowed() const;

public: // data types
    /** 
     * Initialization data for one toolbar button state.
     */
    struct TLocalButtonData
        {
        TInt iCommandId;
        TInt iSkinItemIdMinor;
        TInt iBitmapIndex;
        TInt iBitmapMaskIndex;
        TInt iTooltipResourceId;
        };
    
protected: // methods
    
    /**
     * Check is there an emergency call ongoing.
     * @return  ETrue   if there's an active emergency call.
     *          EFalse  otherwise.
     */
    TBool EmergencyCallActive() const;
    
    /**
     * Create button data object from given source data.
     * @param   aSourceData Defines, where the data should be loaded.
     * @param   aSkin       Skin instance. Ownership not transferred.
     * @param   aMifFile    Full path to the MIF file holding the fallback graphics.
     * @return  New CButtonData instance. Ownership is transferred.
     */
    CButtonData* CreateButtonDataL( const TLocalButtonData& aSourceData, 
            MAknsSkinInstance* aSkin, const TDesC& aMifFile ) const;
    
protected: // data
    /**
     * CoeEnv 
     */
    CCoeEnv& iCoeEnv;
    
    /**
     * Handle to the Bubblewrapper. May be NULL. Not owned.
     */
    CPhoneBubbleWrapper* iBubbleWrapper;
    
    /**
     * Handle to Easy Dialing controller. May be NULL. Not owned. 
     */
    CPhoneEasyDialingController* iEasyDialingController;
    
    /**
     * Used to check is the Dialer in the restricted mode.
     */
    TBool iRestrictedDialer;
    
    /**
     * Used to check is the service code flag set, meaning that number entry
     * contains a service code rather than phone number.
     */
    TBool iServiceCodeFlag;
    
    /**
     * Store availability of phone number (etc) in the number entry.
     */
    TBool iNumberAvailable;
    };

#endif // CPHONEDIALERCONTROLLER_H
