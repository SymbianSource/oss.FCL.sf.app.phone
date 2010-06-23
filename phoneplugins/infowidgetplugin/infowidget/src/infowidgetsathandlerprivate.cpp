/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:
 *
 */

#include <fbs.h>
#include "infowidgetsathandlerprivate.h"
#include "infowidgetsathandler.h"
#include "infowidgetlogging.h"

/*!
  \class InfoWidgetSatHandlerPrivate
  \brief Private implementation of InfoWidgetSatHandler  
*/

/*!
   InfoWidgetSatHandlerPrivate::InfoWidgetSatHandlerPrivate   
 */
InfoWidgetSatHandlerPrivate::InfoWidgetSatHandlerPrivate(
        InfoWidgetSatHandler *iwSatHandler, RSatService& satService)
    : CActive(CActive::EPriorityStandard), 
	  q_ptr(iwSatHandler),
      m_satService(satService)
{
    DPRINT << ": IN";
    CActiveScheduler::Add(this);
    DPRINT << ": OUT";
}

/*!
   InfoWidgetSatHandlerPrivate::~InfoWidgetSatHandlerPrivate           
*/
InfoWidgetSatHandlerPrivate::~InfoWidgetSatHandlerPrivate()
{
    DPRINT << ": IN";
    Cancel();
    DPRINT << ": OUT";
}

/*!
   InfoWidgetSatHandlerPrivate::connect
 */
bool InfoWidgetSatHandlerPrivate::connect()
{
    TInt err( KErrNone );
    bool connected(false); 
    
    TRAP(err, m_satSession.ConnectL());
    if (KErrNone != err) {
        DPRINT << ": Exception occured while connecting SatSession:" << err;
    }
    else{
        TRAP(err, m_satService.OpenL(m_satSession));
        if (KErrNone != err) {
            DCRITICAL << ": Exception occured while opening SatService:" << err;
            DCRITICAL << ": closing SAT session"; 
            m_satSession.Close(); 
        } else {
            connected = true; 
        }
    }
    
    return connected;
}

/*! 
   InfoWidgetSatHandlerPrivate::startObserving
    
   To get initial content if any and start listening    
 */
void InfoWidgetSatHandlerPrivate::startObserving()
{
    //not observing yet but
    getIdleModeData();
    // in case there were content, there is need to send response to SAT
    if(!m_idleTxt.isEmpty()){
        q_ptr->handleIdleModeTxtMessage( m_idleResult );
    }
    // Start observing for changes
    activate();
}

/*!
   InfoWidgetSatHandlerPrivate::disconnect   
 */
int InfoWidgetSatHandlerPrivate::disconnect()
{
    DPRINT << ": IN";
    m_idleTxt = "";
    Cancel();
    m_satService.NotifySetupIdleModeTextChangeCancel(); 
    m_satService.Close();
    m_satSession.Close();
    
    DPRINT << ": OUT";
    return 0;//TODO:Fixme
}

/*!
   InfoWidgetSatHandlerPrivate::getIdleModeData   
 */
void InfoWidgetSatHandlerPrivate::getIdleModeData()
{
    HBufC* string( NULL );
    TUint8 recordId( NULL );
    RSatService::TSatIconQualifier iconQualifier( 
        RSatService::ESatIconNoIcon );
    
    // Get setup idle mode text, icon qualifier and record id.
    TInt result( KErrNotFound  );
    TRAP(result, m_satService.GetSetupIdleModeTextL( string, iconQualifier, recordId )); 
    if (0 != result) {
        DPRINT << ": Exception occured while GetSetupIdleModeTextL :" << result;
        m_idleResult = RSatService::ESATIdleMeUnableToProcessCmd;
    }
    else{
        DPRINT << "string->Length() : " << string->Length();
        if(RSatService::ESatIconSelfExplanatory != iconQualifier){ 
            m_idleTxt = QString((QChar*)string->Des().Ptr(), string->Length());
        }else{
            m_idleTxt = "";
        }
        //determine result
        if(RSatService::ESatIconNoIcon != iconQualifier
                && !m_idleTxt.isEmpty()){
            //icon requested but we don't have icon support
            m_idleResult = RSatService::ESATIdleSuccessRequestedIconNotDisplayed;
        }else if(RSatService::ESatIconNoIcon == iconQualifier
                && !m_idleTxt.isEmpty()){
            m_idleResult = RSatService::ESATIdleSuccess;
        }else{ //got empty string, could be permanen problem as well?
            //m_idleResult = RSatService::ESATIdleCmdBeyondMeCapabilities
            m_idleResult = RSatService::ESATIdleMeUnableToProcessCmd;
        }
    }
    delete string;
    q_ptr->setSatDisplayText(m_idleTxt);//empty in case of selexplanatory icon
}

/*!
   InfoWidgetSatHandlerPrivate::RunL   
 */
void InfoWidgetSatHandlerPrivate::RunL()
{ 
    DPRINT << ": IN";
    if ( KErrNone == iStatus.Int() )
        {   
        getIdleModeData();
        q_ptr->handleIdleModeTxtMessage( m_idleResult );
    } else {
        q_ptr->handleSatError(1,iStatus.Int());
    }
    if ( !IsActive() )
        {
        activate();
        }       
    DPRINT << ": OUT";
}

/*!
   InfoWidgetSatHandlerPrivate::DoCancel   
 */
void InfoWidgetSatHandlerPrivate::DoCancel()
{
    DPRINT << ": IN";
    m_satService.NotifySetupIdleModeTextChangeCancel(); 
    DPRINT << ": OUT";
}

/*!
   InfoWidgetSatHandlerPrivate::activate   
 */
void InfoWidgetSatHandlerPrivate::activate()
{
    DPRINT << ": IN";
    Cancel();   
    TInt error = m_satService.NotifySetupIdleModeTextChange( iStatus );
    
    if ( KErrNone == error )
        {
        SetActive();
        }
    DPRINT << ": OUT";
}

// End of File. 
    

