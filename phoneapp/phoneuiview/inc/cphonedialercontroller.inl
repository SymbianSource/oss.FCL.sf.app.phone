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
* Description:  private, CPhoneDialerController
*
*/

#ifndef CPHONEDIALERCONTROLLER_INL_
#define CPHONEDIALERCONTROLLER_INL_

#include <AknsUtils.h>
#include <phoneui.mbg>

// ---------------------------------------------------------------------------
// SkinId
// ---------------------------------------------------------------------------
//    
inline TAknsItemID CPhoneDialerController::SkinId( TInt aIconIndex )
    {
    TAknsItemID skinId;
    
    switch ( aIconIndex )
        {
        case EMbmPhoneuiQgn_indi_dialer_call_send:
            skinId = KAknsIIDQgnIndiDialerCallSend;
            break;
        case EMbmPhoneuiQgn_indi_button_send_dtmf:
            skinId = KAknsIIDQgnIndiButtonSendDtmf;
            break;
        case EMbmPhoneuiQgn_indi_dialer_contacts:
            skinId = KAknsIIDQgnIndiDialerContacts;
            break;
        case EMbmPhoneuiQgn_indi_dialer_add_contacts:
            skinId = KAknsIIDQgnIndiDialerAddContacts;
            break;
        case EMbmPhoneuiQgn_indi_dialer_clear:
            skinId = KAknsIIDQgnIndiDialerClear;
            break;                        
        default:
            skinId = KAknsIIDNone;        
            break;            
        }
    return skinId;   
    }

#endif /*CPHONEDIALERCONTROLLER_INL_*/
