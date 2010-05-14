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
* Description:  Unit tests for DialService.
*
*/

#include <QtTest/QtTest>

#include "mpekeysequencerecognitionif.h"
#include "keysequencerecognitionservice.h"

class UT_KeySequenceRecognitionService 
    : 
    public QObject, public MPEKeySequenceRecognitionIF
{
    Q_OBJECT
    
public:
    UT_KeySequenceRecognitionService();
    virtual ~UT_KeySequenceRecognitionService();

public:
    TBool ExecuteKeySequenceL(const TDesC16& aSequence);
    
private slots:
    void init();
    void cleanup();
    void t_executeKeySequence();
    void t_notRecognizedKeySequence();
    void t_leaveDuringExecuteKeySequence();

private:
    KeySequenceRecognitionService *m_RecognitionService;
    TBool m_keySequenceExecutionResult;
    TPtrC16 m_keySequencePtr;
    int m_leaveSimulationCode;
};

UT_KeySequenceRecognitionService::UT_KeySequenceRecognitionService()
    :
    m_RecognitionService(NULL),
    m_keySequenceExecutionResult(ETrue), 
    m_leaveSimulationCode(KErrNone)
{
}

UT_KeySequenceRecognitionService::~UT_KeySequenceRecognitionService()
{
    delete m_RecognitionService;
}

TBool UT_KeySequenceRecognitionService::ExecuteKeySequenceL(
    const TDesC16& aSequence)
{
    Q_UNUSED(aSequence)
    User::LeaveIfError(m_leaveSimulationCode);
    
    m_keySequencePtr.Set(aSequence.Ptr());
    return m_keySequenceExecutionResult;
}

void UT_KeySequenceRecognitionService::init()
{
    m_RecognitionService = new KeySequenceRecognitionService(*this);
    m_keySequenceExecutionResult = ETrue;
    m_keySequencePtr.Set(NULL, 0);
    m_leaveSimulationCode = KErrNone;
}

void UT_KeySequenceRecognitionService::cleanup()
{
    delete m_RecognitionService;
    m_RecognitionService = NULL;
}

void UT_KeySequenceRecognitionService::t_executeKeySequence()
{
    QString sequence("*#06#");
    bool result = m_RecognitionService->executeKeySequence(sequence);
    QCOMPARE(result, true);
    QVERIFY(
        m_keySequencePtr == TPtrC16(reinterpret_cast<const TUint16*>(sequence.utf16())));
}

void UT_KeySequenceRecognitionService::t_notRecognizedKeySequence()
{
    m_keySequenceExecutionResult = EFalse;
    QString sequence("*#abc#");
    bool result = m_RecognitionService->executeKeySequence(sequence);
    QCOMPARE(result, false);
}

void UT_KeySequenceRecognitionService::t_leaveDuringExecuteKeySequence()
{
    m_leaveSimulationCode = KErrArgument;
    QString sequence("abc");
    bool result = m_RecognitionService->executeKeySequence(sequence);
    QCOMPARE(result, false);
}

QTEST_MAIN(UT_KeySequenceRecognitionService)
#include "unit_tests.moc"
