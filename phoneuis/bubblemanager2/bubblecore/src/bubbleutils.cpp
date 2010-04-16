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
* Description:  Utility methods.
*
*/

#include <hbiconitem.h>
#include "bubblemanager2.h"
#include "bubbleutils.h"
#include "bubbleheader.h"
#include "bubblestyleoption.h"
#include "bubblebuttonstyle.h"
#include "hbpushbutton.h"
#include "hbaction.h"

void BubbleUtils::setCallHeaderTexts3Lines(
    const BubbleHeader& header,
    BubbleStyleOption& option )
{
    switch( header.callState() ) {
    case BubbleManager::Incoming:
    case BubbleManager::Waiting:
    case BubbleManager::AlertToDisconnected:
    {
        if ( header.secondaryCli().length() ) {
            option.mText1.append( header.cli() );
            option.mText1Clip = header.cliClipDirection();
            option.mText2.append( header.secondaryCli() );
            option.mText2Clip = header.secondaryCliClipDirection();
            option.mText3.append( header.text() );
            option.mText3Clip = header.textClipDirection();
            option.mCliLineNumber = 1;
        } else {
            option.mText1.append( header.cli() );
            option.mText1Clip = header.cliClipDirection();
            option.mText2.append( header.text() );
            option.mText2Clip = header.textClipDirection();
            option.mCliLineNumber = 1;
        }
        break;
    }
    
    case BubbleManager::Outgoing:
    {
        if ( header.secondaryCli().length() ) {
            option.mText1.append( header.text() );
            option.mText1Clip = header.textClipDirection();
            option.mText2.append( header.cli() );
            option.mText2Clip = header.cliClipDirection();
            option.mText3.append( header.secondaryCli() );
            option.mText3Clip = header.secondaryCliClipDirection();
            option.mCliLineNumber = 2;
        } else {
            option.mText1.append( header.text() );
            option.mText1Clip = header.textClipDirection();
            option.mText2.append( header.cli() );
            option.mText2Clip = header.cliClipDirection();
            option.mCliLineNumber = 2;
        }
        break;
    }

    case BubbleManager::Active:
    case BubbleManager::Alerting:
    {
        if ( header.secondaryCli().length() ) {
            option.mText1.append( header.cli() );
            option.mText1Clip = header.cliClipDirection();
            option.mText2.append( header.secondaryCli() );
            option.mText2Clip = header.secondaryCliClipDirection();
            option.mText3.append( header.timerCost() );
            option.mText3Clip = Qt::ElideRight;
            option.mCliLineNumber = 1;
            option.mTimerLineNumber = 3;
        } else {
            option.mText1.append( header.cli() );
            option.mText1Clip = header.cliClipDirection();
            option.mText2.append( header.timerCost() );
            option.mText2Clip = Qt::ElideRight;
            option.mCliLineNumber = 1;
            option.mTimerLineNumber = 2;
        }
        break;
    }
    
    case BubbleManager::OnHold:
    case BubbleManager::Disconnected:
    {
        if ( header.secondaryCli().length() ) {
            option.mText1.append( header.cli() );
            option.mText1Clip = header.cliClipDirection();
            option.mText2.append( header.secondaryCli() );
            option.mText2Clip = header.secondaryCliClipDirection();
            option.mText3.append( header.text() );
            option.mText3Clip = header.textClipDirection();
            option.mCliLineNumber = 1;
        } else {
            option.mText1.append( header.cli() );
            option.mText1Clip = header.cliClipDirection();
            option.mText2.append( header.text() );
            option.mText2Clip = header.textClipDirection();
            option.mCliLineNumber = 1;
        }
        break;
    }

    default:
        // do nothing
        break;
    } // switch
}

void BubbleUtils::setCallHeaderTexts2Lines(
    const BubbleHeader& header,
    BubbleStyleOption& option )
{
    switch( header.callState() ) {
    case BubbleManager::Active:
        option.mText1.append( header.cli() );
        option.mText1Clip = header.cliClipDirection();
        option.mText2.append( header.timerCost() );
        option.mText2Clip = Qt::ElideRight;
        option.mCliLineNumber = 1;
        option.mTimerLineNumber = 2;
        break;


    case BubbleManager::OnHold:
    case BubbleManager::Disconnected:
        option.mText1.append( header.cli() );
        option.mText1Clip = header.cliClipDirection();
        option.mText2.append( header.text() );
        option.mText2Clip = header.textClipDirection();
        option.mCliLineNumber = 1;
        break;

    case BubbleManager::Waiting:
    case BubbleManager::AlertToDisconnected:
        option.mText1.append( header.cli() );
        option.mText1Clip = header.cliClipDirection();
        option.mText2.append( header.text() );
        option.mText2Clip = header.textClipDirection();
        option.mCliLineNumber = 1;
        break;

    case BubbleManager::Outgoing:
        option.mText1.append(header.text());
        option.mText1Clip = header.textClipDirection();
        option.mText2.append(header.cli());
        option.mText2Clip = header.cliClipDirection();
        option.mCliLineNumber = 2;
        break;

    default:
        // do nothing
        break;
    } // switch
}

bool BubbleUtils::compareHeaders(
    const BubbleHeader* header1,
    const BubbleHeader* header2 )
{
    if ( header1->callState() < header2->callState() ) {
        return false;
    } else {
        return true;
    }
}

QString BubbleUtils::stylePluginNameWithPath(const QString& pluginName)
{
    QString nameWithPath;

#if defined(Q_OS_SYMBIAN)
    nameWithPath.append("z:/resource/qt/plugins/phone/");
    nameWithPath.append(pluginName);
#elif defined(Q_OS_WIN)
    nameWithPath.append("c:/hb/bin/");
    nameWithPath.append(pluginName);
#else // Q_OS_UNIX
    nameWithPath.append("/home/lib/");
    nameWithPath.append(pluginName);
#endif

    return nameWithPath;
}

void BubbleUtils::setButtonStyleForAction(
    HbPushButton& button,
    const HbAction& action)
{
    BubbleButtonStyle* style =
        static_cast<BubbleButtonStyle*>(button.style());

    if (action.softKeyRole()==QAction::PositiveSoftKey) {
        style->setButtonColor(BubbleButtonStyle::Green);
    } else if (action.softKeyRole()==QAction::NegativeSoftKey) {
        style->setButtonColor(BubbleButtonStyle::Red);
    } else {
        style->setButtonColor(BubbleButtonStyle::Default);
    }

}

