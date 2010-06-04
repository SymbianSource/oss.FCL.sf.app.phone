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
#include <QDebug>
#include <smcmockclassincludes.h>
#include "psetcalldivertingwrapper.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PSetCallDivertingWrapper::PSetCallDivertingWrapper
// -----------------------------------------------------------------------------
//
PSetCallDivertingWrapper::PSetCallDivertingWrapper( 
        CPsetContainer & psetContainer,
        QObject * parent ) : QObject( parent )
    {
    Q_UNUSED(psetContainer);
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapper::~PSetCallDivertingWrapper
// -----------------------------------------------------------------------------
//
PSetCallDivertingWrapper::~PSetCallDivertingWrapper(  )
    {
    
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapper::setCallDiverting
// -----------------------------------------------------------------------------
//
int PSetCallDivertingWrapper::setCallDiverting( 
        PSCallDivertingCommand & aSetting,
        int aBasicServiceGroup )
    {
    SMC_MOCK_METHOD2( int, PSCallDivertingCommand &, aSetting, 
        int, aBasicServiceGroup )
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapper::getCallDivertingStatus
// -----------------------------------------------------------------------------
//
void PSetCallDivertingWrapper::getCallDivertingStatus( 
        const PsServiceGroup aServiceGroup,
        const PsCallDivertingCondition aCondition,
        int aBsc )
    {
    int iDummy = aServiceGroup;
    SMC_MOCK_METHOD3( void, int, iDummy, 
        const PsCallDivertingCondition, aCondition, 
        int, aBsc )
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapper::cancelProcess
// -----------------------------------------------------------------------------
//
void PSetCallDivertingWrapper::cancelProcess(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapper::getDefaultNumbers
// -----------------------------------------------------------------------------
//
void PSetCallDivertingWrapper::getDefaultNumbers( 
        QStringList & aDefNumbers )
    {
    static int i =0;
    if(i == 0){
        QStringList list;
        list << "0401234567" << "0501234567" << "0451234567";
        aDefNumbers = list;
        i=1;
    }else{
        i=0;
    }
    SMC_MOCK_METHOD1( void, QStringList &, aDefNumbers )
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapper::setNewDefaultNumber
// -----------------------------------------------------------------------------
//
void PSetCallDivertingWrapper::setNewDefaultNumber( 
        QString aNumber )
    {
    SMC_MOCK_METHOD1( void, QString, aNumber )
    }

// -----------------------------------------------------------------------------
// PSetCallDivertingWrapper::getVoiceMailBoxNumber
// -----------------------------------------------------------------------------
//
void PSetCallDivertingWrapper::getVoiceMailBoxNumber(QString &number)
{
    SMC_MOCK_METHOD1( void, QString&, number )
}