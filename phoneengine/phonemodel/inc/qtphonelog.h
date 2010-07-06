/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides macros for logging
*
*/



#ifndef __QTPHONELOG_H__
#define __QTPHONELOG_H__

//  INCLUDES
#include <e32def.h>
#include <QDebug>

// Q_FUNC_INFO does not print function signature in Symbian..

#ifdef _DEBUG
#define PHONE_DEBUG(A)                   qDebug() <<A;
#define PHONE_DEBUG2(A, B)               qDebug() <<A <<B
#define PHONE_DEBUG3(A, B, C)            qDebug() <<A <<B <<C
#define PHONE_DEBUG4(A, B, C, D)         qDebug() <<A <<B <<C <<D

#ifdef Q_OS_SYMBIAN
#define PHONE_TRACE                      qDebug() <<__PRETTY_FUNCTION__;
#define PHONE_TRACE1(A)                  qDebug() <<__PRETTY_FUNCTION__ <<A;
#define PHONE_TRACE2(A, B)               qDebug() <<__PRETTY_FUNCTION__ <<A <<B
#define PHONE_TRACE3(A, B, C)            qDebug() <<__PRETTY_FUNCTION__ <<A <<B <<C
#define PHONE_TRACE4(A, B, C, D)         qDebug() <<__PRETTY_FUNCTION__ <<A <<B <<C <<D
#else //cannot use __PRETTY_FUNCTION__ in win32
#define PHONE_TRACE
#define PHONE_TRACE1(A)                  PHONE_DEBUG(A)
#define PHONE_TRACE2(A, B)               PHONE_DEBUG(A, B)
#define PHONE_TRACE3(A, B, C)            PHONE_DEBUG(A, B, C)
#define PHONE_TRACE4(A, B, C, D)         PHONE_DEBUG(A, B, C, D)
#endif //Q_OS_SYMBIAN

#else
#define PHONE_DEBUG(A)
#define PHONE_DEBUG2(A, B)
#define PHONE_DEBUG3(A, B, C)
#define PHONE_DEBUG4(A, B, C, D)

#define PHONE_TRACE
#define PHONE_TRACE1(A)
#define PHONE_TRACE2(A, B)
#define PHONE_TRACE3(A, B, C)
#define PHONE_TRACE4(A, B, C, D)
#endif //_DEBUG

#endif

