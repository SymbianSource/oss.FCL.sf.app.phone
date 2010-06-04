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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui
{
    class MainWindow;
}

class QLocalServer;
class QLocalSocket;
class QLabel;
class BubbleData;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool start();

private slots:
    void clientConnected();
    void clientDisconnected();

    void bubbleChanged();
    void updateName();
    void updateNumber();
    void updateState();
    void updateDivert(int);
    void updateMute(int);

    void sendData();

private:
    void initUi();

private:
    Ui::MainWindow *ui;

    QLocalServer *mServer;
    QLocalSocket *mClientConnection;
    QLabel       *mStatusLabel;
    BubbleData   *mDataModel;
    QString      mBubbleId;
};

#endif // MAINWINDOW_H
