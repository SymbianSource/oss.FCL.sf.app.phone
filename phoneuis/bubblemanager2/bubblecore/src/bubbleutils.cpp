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
#include <hbaction.h>
#include <hbtextitem.h>
#include <hbstringutil.h>
#include <hbiconanimator.h>
#include <hbcolorscheme.h>

#include "bubblemanager2.h"
#include "bubbleutils.h"
#include "bubbleheader.h"
#include "bubblebutton.h"

void BubbleUtils::setCallHeaderTexts3Lines(
    const BubbleHeader& header,
    HbTextItem& textLine1,
    HbTextItem& textLine2,
    HbTextItem& textLine3,
    int& cliLineNumber,
    int& timerLineNumber)
{
    switch( header.callState() ) {
    case BubbleManager::Incoming:
    case BubbleManager::Waiting:
    case BubbleManager::AlertToDisconnected:
    {
        if ( header.secondaryCli().length() ) {
            setText(textLine1, header.cli(), header.cliClipDirection());
            setText(textLine2, header.secondaryCli(),
                    header.secondaryCliClipDirection());
            setText(textLine3, header.text(), header.textClipDirection());
            cliLineNumber = 1;
        } else {
            setText(textLine1,header.cli(), header.cliClipDirection());
            setText(textLine2,header.text(), header.textClipDirection());
            cliLineNumber = 1;
        }
        break;
    }

    case BubbleManager::Outgoing:
    {
        if ( header.secondaryCli().length() ) {
            setText(textLine1, header.text(), header.textClipDirection());
            setText(textLine2, header.cli(), header.cliClipDirection());
            setText(textLine3, header.secondaryCli(),
                    header.secondaryCliClipDirection());
            cliLineNumber = 2;
        } else {
            setText(textLine1, header.text(), header.textClipDirection());
            setText(textLine2, header.cli(), header.cliClipDirection());
            cliLineNumber = 2;
        }
        break;
    }

    case BubbleManager::Active:
    case BubbleManager::Alerting:
    {
        if ( header.secondaryCli().length() ) {
            setText(textLine1, header.cli(), header.cliClipDirection());
            setText(textLine2, header.secondaryCli(),
                    header.secondaryCliClipDirection());
            setText(textLine3, header.timerCost(), Qt::ElideRight);
            cliLineNumber = 1;
            timerLineNumber = 3;
        } else {
            setText(textLine1, header.cli(), header.cliClipDirection());
            setText(textLine2, header.timerCost(), Qt::ElideRight);
            cliLineNumber = 1;
            timerLineNumber = 2;
        }
        break;
    }

    case BubbleManager::OnHold:
    case BubbleManager::Disconnected:
    {
        if ( header.secondaryCli().length() ) {
            setText(textLine1, header.cli(), header.cliClipDirection());
            setText(textLine2, header.secondaryCli(),
                    header.secondaryCliClipDirection());
            setText(textLine3, header.text(), header.textClipDirection());
            cliLineNumber = 1;
        } else {
            setText(textLine1, header.cli(), header.cliClipDirection());
            setText(textLine2, header.text(), header.textClipDirection());
            cliLineNumber = 1;
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
    HbTextItem& textLine1,
    HbTextItem& textLine2,
    int& cliLineNumber,
    int& timerLineNumber)
{
    switch( header.callState() ) {
    case BubbleManager::Active:
        setText(textLine1, header.cli(), header.cliClipDirection());
        setText(textLine2, header.timerCost(), Qt::ElideRight);
        cliLineNumber = 1;
        timerLineNumber = 2;
        break;


    case BubbleManager::OnHold:
    case BubbleManager::Disconnected:
        setText(textLine1, header.cli(), header.cliClipDirection());
        setText(textLine2, header.text(), header.textClipDirection());
        cliLineNumber = 1;
        break;

    case BubbleManager::Waiting:
    case BubbleManager::AlertToDisconnected:
        setText(textLine1, header.cli(), header.cliClipDirection());
        setText(textLine2, header.text(), header.textClipDirection());
        cliLineNumber = 1;
        break;

    case BubbleManager::Outgoing:
        setText(textLine1, header.text(), header.textClipDirection());
        setText(textLine2, header.cli(), header.cliClipDirection());
        cliLineNumber = 2;
        break;

    default:
        // do nothing
        break;
    } // switch
}

void BubbleUtils::setText(
    HbTextItem& item,
    const QString& text,
    Qt::TextElideMode clip)
{
    if (clip == Qt::ElideLeft) {
        // convert phonenumber to phone ui language
        QString converted = HbStringUtil::convertDigits(text);
        item.setText(converted);
    } else {
        item.setText( text );
    }
    item.setElideMode( clip );
}

bool BubbleUtils::compareHeaders(
    const BubbleHeader* header1,
    const BubbleHeader* header2 )
{
    // sort according to call states, but keep conference header
    // last, it shown at foreground only when expanded.
    if ( (header1->callState() < header2->callState()) ||
         header1->isConference() ) {
        return false;
    } else {
        return true;
    }
}

void BubbleUtils::setButtonStyleForAction(
    BubbleButton& button,
    const HbAction& action)
{
    if (action.softKeyRole()==QAction::PositiveSoftKey) {
        button.setButtonType(BubbleButton::GreenButton);
    } else if (action.softKeyRole()==QAction::NegativeSoftKey) {
        button.setButtonType(BubbleButton::RedButton);
    } else {
        button.setButtonType(BubbleButton::DefaultButton);
    }

    button.updatePrimitives();
}

void BubbleUtils::setCallStatusIcon(
    int callState,
    int callFlags,
    HbIconItem& icon)
{
    bool showIcon = true;

    switch(callState) {
    case BubbleManagerIF::Incoming:
    case BubbleManagerIF::Alerting:
    case BubbleManagerIF::Waiting:
        // from bubble_icon_anim.axml
        if ( callFlags & BubbleManagerIF::VoIPCall ) {
            icon.setIconName("voip_call_waiting_anim");
        } else if ( callFlags & BubbleManagerIF::Video ) {
            icon.setIconName("video_call_waiting_anim");
        } else {
            icon.setIconName("voice_call_waiting_anim");
        }
        break;
    case BubbleManagerIF::Outgoing:
    case BubbleManagerIF::Active:
        if ( callFlags & BubbleManagerIF::VoIPCall ) {
            icon.setIconName("qtg_large_voip_call_active");
        } else if ( callFlags & BubbleManagerIF::Video ) {
            icon.setIconName("qtg_large_video_call_active");
        } else {
            icon.setIconName("qtg_large_active_call");
        }
        break;
    case BubbleManagerIF::OnHold:
        if ( callFlags & BubbleManagerIF::VoIPCall ) {
            icon.setIconName("qtg_large_voip_call_waiting");
        } else if ( callFlags & BubbleManagerIF::Video ) {
            icon.setIconName("qtg_large_video_call_waiting");
        } else {
            icon.setIconName("qtg_large_waiting_call");
        }
        break;
    case BubbleManagerIF::Disconnected:
    case BubbleManagerIF::AlertToDisconnected:
        if ( callFlags & BubbleManagerIF::VoIPCall ) {
            icon.setIconName("qtg_large_end_call");
        } else if ( callFlags & BubbleManagerIF::Video ) {
            icon.setIconName("qtg_large_end_call");
        } else {
            icon.setIconName("qtg_large_end_call");
        }
        break;
    default:
        icon.setIcon(HbIcon());
        showIcon = false;
        break;
    }

    if (showIcon) {
        icon.show();
    }
}

void BubbleUtils::setNumberTypeIcon(
    int callState,
    int callFlags,
    HbIconItem& icon)
{
    if ( (( callState == BubbleManagerIF::Incoming ) ||
          ( callState == BubbleManagerIF::Waiting )) &&
         (callFlags & BubbleManagerIF::Diverted) ) {
        icon.setIcon( HbIcon(":/qtg_mono_call_diverted.svg") );

        // temporary, diverted icon is not yet in theme and
        // because of that css coloring doesn't work
        QColor color;
        color = HbColorScheme::color("qtc_list_item_title_normal");
        if (color.isValid()) {
            icon.setColor(Qt::white);
        }

        icon.show();
    } else {
        icon.setIcon( HbIcon() );
        icon.hide();
    }
}

void BubbleUtils::setCipheringIcon(
    int callState,
    int callFlags,
    HbIconItem& icon)
{
    Q_UNUSED(callState)

    if (callFlags & BubbleManagerIF::NoCiphering) {
        icon.setIcon( HbIcon("qtg_mono_ciphering_off") );
        icon.show();
    } else {
        icon.setIcon( HbIcon() );
        icon.hide();
    }
}

