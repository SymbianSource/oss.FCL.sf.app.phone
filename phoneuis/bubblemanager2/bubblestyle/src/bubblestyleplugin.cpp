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
* Description:  Bubble style plugin.
*
*/

#include "bubblestyleplugin.h"
#include "bubblestyleoption.h"
#include "bubbleprimitives.h"
#include "bubbleanimiconitem.h"

#include <hbstyle.h>
#include <hbiconitem.h>
#include <hbtextitem.h>
#include <hbframeitem.h>
#include <hbframedrawer.h>
#include <hbfontspec.h>
#include <hbdeviceprofile.h>
#include <hbcolorscheme.h>
#include <hbstringutil.h>

static const int BUBBLE_ICON_ANIM_INTERVAL = 500; // ms

BubbleStylePlugin::BubbleStylePlugin()
{
    mCliFont = new HbFontSpec(HbFontSpec::Primary);
    mTextFont = new HbFontSpec(HbFontSpec::Secondary);
    mTimerFont = new HbFontSpec(HbFontSpec::Secondary);

    HbDeviceProfile profile;
    mCliFont->setTextHeight(4*HbDeviceProfile::current().unitValue());
    mTextFont->setTextHeight(4*HbDeviceProfile::current().unitValue());
    mTimerFont->setTextHeight(4*HbDeviceProfile::current().unitValue());
}

BubbleStylePlugin::~BubbleStylePlugin()
{
    delete mCliFont;
    delete mTextFont;
    delete mTimerFont;
}

int BubbleStylePlugin::primitiveCount() const
{
    return BP_Bubble_primitive_count;
}

QGraphicsItem* BubbleStylePlugin::createPrimitive(
    HbStyle::Primitive primitive, 
    QGraphicsItem *parent) const
{
    QGraphicsItem* item = 0;
    
    switch(primitive) {
    case BP_Bubble_frame: {
        HbFrameItem* frame = new HbFrameItem(parent);
        frame->setZValue(-1.0);
        HbFrameDrawer *drawer = new HbFrameDrawer();
        frame->setFrameDrawer( drawer ); // takes ownership
        item = frame;
        }
        break;
    case BP_Text1_text:
    case BP_Text2_text:
    case BP_Text3_text: {
        HbTextItem* text = new HbTextItem(parent);
        item = text;
        }
        break;    
    case BP_CallStatus_icon: {
        BubbleAnimIconItem* anim =
            new BubbleAnimIconItem(BUBBLE_ICON_ANIM_INTERVAL,parent);
        item = anim;
        }
        break;
    case BP_NumberType_icon: {
        HbIconItem* icon = new HbIconItem(parent);
        item = icon;
        }
        break;
    case BP_Ciphering_icon: {
        HbIconItem* icon = new HbIconItem(parent);
        item = icon;
        }
        break;
    default:
        break;
    } // switch

    return item;    
}

void BubbleStylePlugin::updatePrimitive(
    QGraphicsItem *item,
    HbStyle::Primitive primitive, 
    const QStyleOption *option ) const
{
#ifndef Q_OS_SYMBIAN
    // qtcast didn't work with mingw
    const BubbleStyleOption *opt = static_cast<const BubbleStyleOption*>(option);
#else
    const BubbleStyleOption *opt = qstyleoption_cast<const BubbleStyleOption*>(option);
#endif
    if ( !opt ) {
        return;
    }    
    
    switch(primitive) {
    case BP_Bubble_frame: {
        HbFrameItem *frame = static_cast<HbFrameItem*>(item);
        frame->frameDrawer().setFrameType( HbFrameDrawer::NinePieces );
        frame->frameDrawer().setFrameGraphicsName("qtg_fr_list_normal");
        }
        break;                

    case BP_Text1_text: {
        HbTextItem* textItem = static_cast<HbTextItem*>(item);
        setText(textItem, opt->mText1, opt->mText1Clip);
        setFont(textItem, primitive, opt);
        }
        break;    
    
    case BP_Text2_text: {
        HbTextItem* textItem = static_cast<HbTextItem*>(item);
        setText(textItem, opt->mText2, opt->mText2Clip);
        setFont(textItem, primitive, opt);
        }
        break;    

    case BP_Text3_text: {
        HbTextItem* textItem = static_cast<HbTextItem*>(item);
        setText(textItem, opt->mText3, opt->mText3Clip);
        setFont(textItem, primitive, opt);
        }
        break;    
    
    case BP_CallStatus_icon: {
        setCallStatusIcons(static_cast<BubbleAnimIconItem*>(item), opt);
        }
        break;
    
    case BP_NumberType_icon: {
        HbIconItem* icon = static_cast<HbIconItem*>(item);
        if ( (( opt->mCallState == BubbleManagerIF::Incoming ) ||
              ( opt->mCallState == BubbleManagerIF::Waiting )) &&
             ( opt->mCallFlags & BubbleManagerIF::Diverted ) ) {
            icon->setIconName(":/qgn_indi_call_diverted.svg");
            setIconColor(icon);
            icon->show();
            } else {
            icon->setIconName(QString());
            icon->hide();
            }
        }
        break;

    case BP_Ciphering_icon: {
        HbIconItem* icon = static_cast<HbIconItem*>(item);
        if ( opt->mCallFlags & BubbleManagerIF::NoCiphering ) {
            icon->setIconName("qtg_mono_ciphering_off");
            setIconColor(icon);
            icon->show();
            } else {
            icon->setIconName(QString());
            icon->hide();
            }
        }
        break;

    default:
        break;
    } // switch
}

void BubbleStylePlugin::setText(
    HbTextItem* item,
    const QString& text,
    Qt::TextElideMode clip) const
{
    if (clip == Qt::ElideLeft) {
        // convert phonenumber to phone ui language
        QString converted = HbStringUtil::convertDigits(text);
        item->setText(converted);
    } else {
        item->setText( text );
    }
    item->setElideMode( clip );
}

void BubbleStylePlugin::setFont(
    HbTextItem* item,
    int primitive,
    const BubbleStyleOption *option) const
{
    int primitiveLineNumber = 0;
    if (primitive==BP_Text1_text) {
        primitiveLineNumber = 1;
    } else if (primitive==BP_Text2_text) {
        primitiveLineNumber = 2;
    } else if (primitive==BP_Text3_text) {
        primitiveLineNumber = 3;
    }

    QFont font = mTextFont->font();
    if (primitiveLineNumber==option->mCliLineNumber) {
        font = mCliFont->font();
    } else if (primitiveLineNumber==option->mTimerLineNumber) {
        font = mTimerFont->font();
    }

    item->setFont(font);

    // set color (to be done via css when possible)
    QColor color;
    color = HbColorScheme::color("list_item_title_normal");
    if (color.isValid()) {
        item->setTextColor(color);
    } else {
        item->setTextColor(Qt::black);
    }
}

void BubbleStylePlugin::setIconColor(HbIconItem* icon) const
{
    // to be done via css when possible
    QColor color;
    color = HbColorScheme::color("list_item_title_normal");
    if (color.isValid()) {
        icon->setColor(color);
    }
}

QString BubbleStylePlugin::layoutPath() const
{
   QString path(":/");
   return path;
}

void BubbleStylePlugin::setCallStatusIcons(BubbleAnimIconItem* anim, const BubbleStyleOption *opt) const
{
    anim->reset();
    switch(opt->mCallState) {
    case BubbleManagerIF::Incoming:
    case BubbleManagerIF::Alerting:
    case BubbleManagerIF::Waiting:
        if ( opt->mCallFlags & BubbleManagerIF::VoIPCall ) {
            anim->appendIcon("qtg_large_voip_received_call");
            anim->appendIcon("qtg_large_voip_dialled_call");
        } else if ( opt->mCallFlags & BubbleManagerIF::Video ) {
            anim->appendIcon("qtg_large_video_received_call");
            anim->appendIcon("qtg_large_video_dialled_call");
        } else {
            anim->appendIcon("qtg_large_active_call");
            anim->appendIcon("qtg_large_waiting_call");
        }
        break;
    case BubbleManagerIF::Outgoing:
    case BubbleManagerIF::Active:
        if ( opt->mCallFlags & BubbleManagerIF::VoIPCall ) {
            anim->appendIcon("qtg_large_voip_received_call");
        } else if ( opt->mCallFlags & BubbleManagerIF::Video ) {
            anim->appendIcon("qtg_large_video_received_call");
        } else {
            anim->appendIcon("qtg_large_active_call");
        }            
        break;
    case BubbleManagerIF::OnHold:
        if ( opt->mCallFlags & BubbleManagerIF::VoIPCall ) {
            anim->appendIcon("qtg_large_voip_dialled_call");
        } else if ( opt->mCallFlags & BubbleManagerIF::Video ) {
            anim->appendIcon("qtg_large_video_dialled_call");
        } else {
            anim->appendIcon("qtg_large_waiting_call");
        }                      
        break;
    case BubbleManagerIF::Disconnected:
    case BubbleManagerIF::AlertToDisconnected:
        if ( opt->mCallFlags & BubbleManagerIF::VoIPCall ) {
            anim->appendIcon("qtg_large_voip_missed_call");
        } else if ( opt->mCallFlags & BubbleManagerIF::Video ) {
            anim->appendIcon("qtg_large_video_missed_call");
        } else {
            anim->appendIcon("qtg_large_end_call");
        }           
        break;
    default:
        break;
    }
    
}

Q_EXPORT_PLUGIN2(bubblestyleplugin, BubbleStylePlugin)
