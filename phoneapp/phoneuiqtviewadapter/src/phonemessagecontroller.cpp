/*!
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
* Description:  Handles message editor.
*/

#include "phonemessagecontroller.h"
#include "tphonecmdparamsfidata.h"
#include "phoneconstants.h"
#include "cphonecenrepproxy.h"
#include "cphonepubsubproxy.h"
#include <settingsinternalcrkeys.h>
#include <coreapplicationuisdomainpskeys.h>
#include <hbglobal.h>
#include <xqservicerequest.h>


PhoneMessageController::PhoneMessageController(QObject *parent) : 
    QObject(parent),
    mService(0)
{

}

PhoneMessageController::~PhoneMessageController()
{
    delete mService;
    mService = 0;
}

void PhoneMessageController::openSoftRejectMessageEditor(
        TPhoneCommandParam *commandParam)
{
    TPhoneCmdParamSfiData* sfiParam =
        static_cast<TPhoneCmdParamSfiData*>( commandParam );
    
    QString toField;
    QString alias;
    QString messageBody;
    
    
    // Get message body
    TInt softRejectDefaultInUseValue = 0;
    const TInt err = CPhoneCenRepProxy::Instance()->GetInt(
        KCRUidTelephonySettings,
        KSettingsSoftRejectDefaultInUse,
        softRejectDefaultInUseValue );
    
    if (softRejectDefaultInUseValue) {
        HBufC* softRejectTxt = NULL;
        TRAP_IGNORE( softRejectTxt = HBufC::NewL( KPhoneSMSLength ) );
        
        if (softRejectTxt) {
            TPtr string( softRejectTxt->Des() );
        
            // Default txt used or not
            CPhoneCenRepProxy::Instance()->GetString(
                KCRUidTelephonySettings,
                KSettingsSoftRejectText,
                string );
            
            messageBody = QString::fromUtf16(string.Ptr(), string.Length());
            delete softRejectTxt;
        }
    } else {
        messageBody = hbTrId("txt_phone_setlabel_soft_reject_val_default_text");
    }
    
    // store the remote caller's info
    alias = QString::fromUtf16(sfiParam->Name().Ptr(), 
            sfiParam->Name().Length());
    
    toField = QString::fromUtf16(sfiParam->Number().Ptr(), 
            sfiParam->Number().Length());

    
    // open message editor
    openEditor(toField, alias, messageBody );
}

void PhoneMessageController::openEditor(
        QString toField, QString name, QString messageBody)
{
    delete mService;
    mService = 0;
    mService = new XQServiceRequest("com.nokia.services.hbserviceprovider.conversationview",
                                    "send(QString,QString,QString)", false);
    
    QList<QVariant> arguments;
    arguments.append(QVariant(toField));
    arguments.append(QVariant(name));
    arguments.append(QVariant(messageBody));
    mService->setArguments(arguments);
    
    QVariant retValue;
    mService->send(retValue);
}



