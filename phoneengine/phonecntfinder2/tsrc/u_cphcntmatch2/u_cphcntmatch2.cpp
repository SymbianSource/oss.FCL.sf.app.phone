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

#include <QtGui>
#include <QtTest/QtTest>
#include <QDebug>
#include <e32cmn.h> //KNullDesC
#include "qcontact.h"
#include "qcontactdetail.h"
#include "qcontactname.h"
#include "qcontactphonenumber.h"
#include "cphcntmatch2.h"
#include "cphcntcontactidimpl2.h"


//---- test class

class U_CPhCntMatch2: public QObject
{
Q_OBJECT

public:
    U_CPhCntMatch2();
    ~U_CPhCntMatch2();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testFirstNameEmpty();
    void testFirstNameNotEmpty();
    void testLastNameEmpty();
    void testLastNameNotEmpty();
    void testNumberEmpty();
    void testNumberNotEmpty();
    //void testSeveralFirstNames();
    //void testEmptyList();
    void testPhoneNumberOverride();
    void testRelease();
    void testCli();
    void testContactId();
    void testNumberType();
    void testCompanyName();
    void testPersonalRingingTone();
    void testBelongsToGroups();
    void testTextToSpeechText();
    void testCallText();
    void testCallImage();
    void testHasThumbnailImage();
    void testAllDtmfNumbers();
    
private:
    
    CPhCntMatch2* mMatch; // SUT 
    MPhCntMatch* mMatchIf;
    QContact mContact;
    QContactDetail mDetail;
  
};

//--- testing

U_CPhCntMatch2::U_CPhCntMatch2()
    {
    }

U_CPhCntMatch2::~U_CPhCntMatch2()
    {

    }

void U_CPhCntMatch2::initTestCase()
    {
    TRAP_IGNORE( mMatch = CPhCntMatch2::NewL(mContact) );
    mMatchIf = mMatch;
    
    }

void U_CPhCntMatch2::cleanupTestCase()
    {
    delete mMatch;
    }

void U_CPhCntMatch2::testFirstNameEmpty()
    {
    QVariant name(""); 
    mDetail.setValue(QContactName::FieldFirst,name);
    bool err = mContact.saveDetail(&mDetail);
    Q_ASSERT(mMatchIf->FirstName().Length() == 0);
    // reset mDetail
    mDetail.removeValue(QContactName::FieldFirst);
    }

void U_CPhCntMatch2::testFirstNameNotEmpty()
    {
     _LIT(KFirstName, "firstname");
     QString name((QChar*)KFirstName().Ptr(),KFirstName().Length());
     mDetail.setValue(QContactName::FieldFirst,name);
     bool err = mContact.saveDetail(&mDetail);
     delete mMatch;
     TRAP_IGNORE( mMatch = CPhCntMatch2::NewL(mContact) );
     mMatchIf = mMatch;
     qDebug()<<"U_CPhCntMatch2 : mContact.details().count() " << mContact.details().count();
     qDebug()<<"U_CPhCntMatch2 : mContact.details().at(0).value() " << mContact.details().at(0).value("");
     HBufC* buf = NULL;
     buf = mMatchIf->FirstName().AllocL();
     QString qBuf((QChar*)buf->Ptr(),buf->Length());
     qDebug()<<"mMatchIf->FirstName() returned...: " << qBuf;
     Q_ASSERT( *buf == KFirstName()); 
     delete buf;
     // reset mDetail
     mDetail.removeValue(QContactName::FieldFirst);
    }

void U_CPhCntMatch2::testLastNameEmpty()
    {
     QVariant name(""); 
     mDetail.setValue(QContactName::FieldLast,name);
     bool err = mContact.saveDetail(&mDetail);
     delete mMatch;
     TRAP_IGNORE( mMatch = CPhCntMatch2::NewL(mContact) );
     mMatchIf = mMatch;
     Q_ASSERT(mMatchIf->LastName().Length() == 0);
     // reset mDetail
     mDetail.removeValue(QContactName::FieldLast);
    }


void U_CPhCntMatch2::testLastNameNotEmpty()
    {
    _LIT(KLastName, "lastname");
    QString name((QChar*)KLastName().Ptr(),KLastName().Length());
    mDetail.setValue(QContactName::FieldLast,name);
    bool err = mContact.saveDetail(&mDetail);
    delete mMatch;
    TRAP_IGNORE( mMatch = CPhCntMatch2::NewL(mContact) );
    mMatchIf = mMatch;
    HBufC* buf = NULL;
    buf = mMatchIf->LastName().AllocL();
    QString qBuf((QChar*)buf->Ptr(),buf->Length());
    Q_ASSERT(*buf == KLastName());
    delete buf;
    qDebug()<<"testLastNameNotEmpty() -- exit ";
    // reset mDetail
    mDetail.removeValue(QContactName::FieldLast);
    }

void U_CPhCntMatch2::testNumberEmpty()
    {
    QVariant number(""); 
    mDetail.setValue(QContactPhoneNumber::FieldNumber,number);
    bool err = mContact.saveDetail(&mDetail);
    delete mMatch;
    TRAP_IGNORE( mMatch = CPhCntMatch2::NewL(mContact) );
    mMatchIf = mMatch;
    Q_ASSERT(mMatchIf->Number().Length() == 0);
    // reset mDetail
    mDetail.removeValue(QContactPhoneNumber::FieldNumber);
    }
    
void U_CPhCntMatch2::testNumberNotEmpty()
    {
    _LIT(KNumber, "05012345678");
    QString number((QChar*)KNumber().Ptr(),KNumber().Length());
    mDetail.setValue(QContactPhoneNumber::FieldNumber,number);
    bool err = mContact.saveDetail(&mDetail);
    delete mMatch;
    TRAP_IGNORE( mMatch = CPhCntMatch2::NewL(mContact) );
    mMatchIf = mMatch;
    HBufC* buf = NULL;
    buf = mMatchIf->Number().AllocL();
    QString qBuf((QChar*)buf->Ptr(),buf->Length());
    Q_ASSERT(*buf == KNumber());
    delete buf;
    // reset mDetail
    mDetail.removeValue(QContactPhoneNumber::FieldNumber);
    }
/*
void U_CPhCntMatch2::testSeveralFirstNames() //in case pb returns bad variants
    {
    mDetail.setValue("Abba");
    mList.clear();
    mList << mDetail;
    mList << mDetail;
    mList << mDetail;
    mContact.setDetails(mList);
    delete mMatch;
    TRAP_IGNORE( mMatch = CPhCntMatch2::NewL(mContact) );
    mMatchIf = mMatch;
    Q_ASSERT(mMatchIf->FirstName().Length() == 0);
    }
    
void U_CPhCntMatch2::testEmptyList()
    {
    QList<QContactDetail> list;
    mContact.setDetails(list);
    delete mMatch;
    TRAP_IGNORE( mMatch = CPhCntMatch2::NewL(mContact) );
    mMatchIf = mMatch;
    Q_ASSERT(mMatchIf->FirstName().Length() == 0);
    }
*/
void U_CPhCntMatch2::testPhoneNumberOverride()
    {
    _LIT(KNumber, "05012345678");
    _LIT(KNumberOverride, "888888888");
    QString number((QChar*)KNumber().Ptr(),KNumber().Length());
    mDetail.setValue(QContactPhoneNumber::FieldNumber,number);
    bool err = mContact.saveDetail(&mDetail);
    delete mMatch;
    TRAP_IGNORE( mMatch = CPhCntMatch2::NewL(mContact) );
    mMatch->SetNumberL(KNumberOverride());
    mMatchIf = mMatch;
    HBufC* buf = NULL;
    buf = mMatchIf->Number().AllocL();
    QString qBuf((QChar*)buf->Ptr(),buf->Length());
    Q_ASSERT(*buf == KNumberOverride());
    delete buf;
    // reset mDetail
    mDetail.removeValue(QContactPhoneNumber::FieldNumber);
    }

void U_CPhCntMatch2::testRelease()
    {
    _LIT(KNumber, "1234");
    QVariant number("1234"); 
    mDetail.setValue(QContactPhoneNumber::FieldNumber,number);
    bool err = mContact.saveDetail(&mDetail);
    delete mMatch;
    TRAP_IGNORE( mMatch = CPhCntMatch2::NewL(mContact) );
    mMatchIf = mMatch;
    mMatchIf->Release();
    Q_ASSERT(mMatchIf->Number() == KNumber());
    // reset mDetail
    mDetail.removeValue(QContactPhoneNumber::FieldNumber);
    }

void U_CPhCntMatch2::testCli()
    {
    HBufC* buf = NULL;
    Q_ASSERT(mMatchIf->Cli(buf) == MPhCntMatch::ECliNumber );
    }

void U_CPhCntMatch2::testContactId()
    {
    CPhCntContactId* id = NULL;
    id = mMatchIf->ContactId();
    Q_ASSERT(id->ContactId()== mContact.localId());
    }

void U_CPhCntMatch2::testNumberType()
    {
    Q_ASSERT(mMatchIf->NumberType() == MPhCntMatch::EMobileNumber);
    }

void U_CPhCntMatch2::testCompanyName()
    {
    Q_ASSERT(mMatchIf->CompanyName().Length() == 0);
    }

void U_CPhCntMatch2::testPersonalRingingTone()
    {
    Q_ASSERT(mMatchIf->PersonalRingingTone().Length() == 0);
    }

void U_CPhCntMatch2::testBelongsToGroups()
    {
    CArrayFix<TContactItemId>* groups = NULL;
    Q_ASSERT(mMatchIf->BelongsToGroups(*groups) == false);
    }

void U_CPhCntMatch2::testTextToSpeechText()
    {
    Q_ASSERT(mMatchIf->TextToSpeechTextL() == NULL);
    }

void U_CPhCntMatch2::testCallText()
    {
    Q_ASSERT(mMatchIf->CallText().Length() == 0);
    }

void U_CPhCntMatch2::testCallImage()
    {
    Q_ASSERT(mMatchIf->CallImage().Length() == 0);
    }

void U_CPhCntMatch2::testHasThumbnailImage()
    {
    Q_ASSERT(mMatchIf->HasThumbnailImage()== false);
    }

void U_CPhCntMatch2::testAllDtmfNumbers()
    {
    //dummy array for now
    Q_ASSERT(mMatchIf->AllDtmfNumbers().Count() == 0);
    }


QTEST_MAIN(U_CPhCntMatch2)
#include "u_cphcntmatch2.moc"
