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
* Description:  Animated icon.
*
*/

#include <QLocalServer>
#include <QLocalSocket>
#include <QMessageBox>
#include <QLabel>
#include <QStatusBar>
#include <QDomDocument>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bubbledata.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mServer(new QLocalServer(this)),
      mClientConnection(0),
      mStatusLabel(new QLabel(this)),
      mBubbleId("1")
{
    ui->setupUi(this);
    statusBar()->addWidget(mStatusLabel,1);

    connect( ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(bubbleChanged()) );
    connect( ui->lineEdit, SIGNAL(editingFinished()), this, SLOT(updateName()) );
    connect( ui->lineEdit_2, SIGNAL(editingFinished()), this, SLOT(updateNumber()) );
    connect( ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(updateState()) );
    connect( ui->checkBox, SIGNAL(stateChanged(int)) , this, SLOT(updateDivert(int)) );
    connect( ui->checkBox_2, SIGNAL(stateChanged(int)) , this, SLOT(updateMute(int)) );

    mDataModel = new BubbleData(this);

    initUi();
}

MainWindow::~MainWindow()
{
    delete ui;
    mServer->close();
    delete mServer;
}


bool MainWindow::start()
{
    bool success = false;
    if (!mServer->listen("bubbletestserver")) {
        QMessageBox::critical( this,
                               tr("Bubble Tester"),
                               tr("Unable to start the server: %1.").arg(mServer->errorString()));
        return success;
    }

    mStatusLabel->setText(tr("Server running"));

    connect( mServer, SIGNAL(newConnection()), this, SLOT(clientConnected()));
    success = true;
    return success;
}


void MainWindow::clientConnected()
{
    if (mClientConnection) {
        return;
    }

    // get the next pending connection
    mClientConnection = mServer->nextPendingConnection();

    mStatusLabel->setText(tr("Connected"));

    connect(mClientConnection, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
}

void MainWindow::clientDisconnected()
{
    mClientConnection = 0;
    connect((QLocalSocket*)sender(), SIGNAL(disconnected()), (QLocalSocket*)sender(), SLOT(deleteLater()));
    mStatusLabel->setText(tr("Disconnected"));
}

void MainWindow::sendData()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << mDataModel->document().toString();

    if (mClientConnection) {
        mClientConnection->write(block);
    }
}

void MainWindow::initUi()
{
    ui->lineEdit->setText(mDataModel->dataField(mBubbleId,"name"));
    ui->lineEdit_2->setText(mDataModel->dataField(mBubbleId,"number"));

    QString state = mDataModel->dataField(mBubbleId,"state");
    int index = ui->comboBox_2->findText(state);
    ui->comboBox_2->setCurrentIndex(index);

    QString divert = mDataModel->dataField(mBubbleId,"divert");
    ui->checkBox->setChecked((divert=="On"));

    QString mute = mDataModel->dataField("mute");
    ui->checkBox_2->setChecked((mute=="On"));
}

void MainWindow::bubbleChanged()
{
    mBubbleId = ui->comboBox->currentText();
    initUi();
}

void MainWindow::updateName()
{
    mDataModel->setDataField(mBubbleId,"name",ui->lineEdit->text());
    sendData();
}

void MainWindow::updateNumber()
{
    mDataModel->setDataField(mBubbleId,"number",ui->lineEdit_2->text());
    sendData();
}

void MainWindow::updateState()
{
    mDataModel->setDataField(mBubbleId,"state",ui->comboBox_2->currentText());
    sendData();
}

void MainWindow::updateDivert(int state)
{
    if (!state) {
        mDataModel->setDataField(mBubbleId,"divert","Off");
    } else {
        mDataModel->setDataField(mBubbleId,"divert","On");
    }

    sendData();
}

void MainWindow::updateMute(int state)
{
    if (!state) {
        mDataModel->setDataField("mute","Off");
    } else {
        mDataModel->setDataField("mute","On");
    }

    sendData();
}












