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
* Description: Interface to HbDocumentLoader (DocML files).
*
*/

#include <QtCore>
#include <hbstyle.h>
#include <hbdocumentloader.h>
#include "bubblewidgetmanager.h"
#include "bubblecontainerwidget.h"
#include "bubbleimagewidget.h"
#include "bubbleheadingwidget.h"
#include "bubbleexpandedhandler.h"
#include "bubblecollapsedhandler.h"
#include "bubbleconferencehandler.h"
#include "bubbleparticipantlistitem.h"
#include "bubblebuttonstyle.h"
#include "hbpushbutton.h"
#include "hbtextitem.h"

const char *BUBBLE_DOCUMENT_CONTENT = "content";

// document loader
class BubbleDocumentLoader : public HbDocumentLoader
{
public:
    BubbleDocumentLoader(const QString& stylePluginName);
    virtual QObject *createObject(const QString& type,
                                  const QString &name);
private:
    QString mStylePluginName;
};


BubbleWidgetManager::BubbleWidgetManager(int style, QObject* parent)
    : QObject(parent), mStyleBaseId(style),mOrientation(Qt::Vertical)
{
    // .docml mappings
    mFileNames.insert(SingleCallView,":/xml/bubble_layout_1.docml");
    mFileNames.insert(TwoCallsView,  ":/xml/bubble_layout_2.docml");
    mFileNames.insert(ThreeCallsView,":/xml/bubble_layout_3.docml");
    mFileNames.insert(ConferenceView,":/xml/bubble_layout_4.docml");
    mFileNames.insert(MutedOverlay,  ":/xml/bubble_layout_5.docml");

    mOrientationName.insert(Qt::Vertical,   "portrait");
    mOrientationName.insert(Qt::Horizontal, "landscape");

    mWidgetPrefix.insert(ExpandedBubble, "eb:");
    mWidgetPrefix.insert(CollapsedBubble,    "cb:");
    mWidgetPrefix.insert(CollapsedBubble2,    "cb2:");
    mContainerName.insert(ExpandedBubble, "expandedBubble");
    mContainerName.insert(CollapsedBubble,   "collapsedBubble");
    mContainerName.insert(CollapsedBubble2,   "collapsedBubble2");
    mWidgetName.insert(Heading,        "bubbleHeading");
    mWidgetName.insert(Image,          "callImage");
    mWidgetName.insert(LeftButton,     "leftButton");
    mWidgetName.insert(CenterButton,   "centerButton");
    mWidgetName.insert(RightButton,    "rightButton");
    mWidgetName.insert(ConferenceTimer,"callTimer");
    mWidgetName.insert(ParticipantList,"participantList");

    mButtonStyle.insert(LeftButton, new BubbleButtonStyle());
    mButtonStyle.insert(CenterButton, new BubbleButtonStyle());
    mButtonStyle.insert(RightButton, new BubbleButtonStyle());
    mButtonStyle.insert(ParticipantListButton, new BubbleButtonStyle()); // not needed??
}

BubbleWidgetManager::~BubbleWidgetManager()
{
    qDeleteAll(mDocumentLoaders);
    mDocumentLoaders.clear();

    QList<int> keys = mDocumentHandlers.keys();
    foreach (int key, keys) {
        qDeleteAll(*mDocumentHandlers[key]);
    }
    qDeleteAll(mDocumentHandlers);
    mDocumentHandlers.clear();

    qDeleteAll(mButtonStyle);
}

QGraphicsWidget* BubbleWidgetManager::view(View view)
{
    if (mDocumentContent.contains(view)) {
        // already loaded
        return mDocumentContent[view];
    } else {
        // load from .docml
        return loadDocument(view,mOrientation);
    }
}

void BubbleWidgetManager::releaseView(View view)
{
    Q_UNUSED(view)
}

QGraphicsWidget* BubbleWidgetManager::loadDocument(
    View view,
    Qt::Orientation orientation)
{
    BubbleDocumentLoader* loader =
        new BubbleDocumentLoader(mStylePluginName);
    bool ok;

    loader->load(mFileNames[view],&ok);
    Q_ASSERT(ok);
    loader->load(mFileNames[view],mOrientationName[orientation],&ok);
    Q_ASSERT(ok);

    mDocumentLoaders.insert(view,loader);

    QGraphicsWidget* widget =
        mDocumentLoaders[view]->findWidget(BUBBLE_DOCUMENT_CONTENT);

    Q_ASSERT(widget);
    widget->setVisible(false);
    mDocumentContent.insert(view,widget);

    applyCustomStyles(view);

    if (view!=MutedOverlay) {
        createDocumentHandlers(view);
    }

    return widget;
}

void BubbleWidgetManager::applyCustomStyles(View view)
{
    setButtonStyle(LeftButton, widget(view,ExpandedBubble,LeftButton));
    setButtonStyle(CenterButton, widget(view,ExpandedBubble,CenterButton));
    setButtonStyle(RightButton, widget(view,ExpandedBubble,RightButton));
}

void BubbleWidgetManager::setButtonStyle(Widget widget, QGraphicsWidget* button)
{
    HbPushButton* b;
    b = qobject_cast<HbPushButton*>(button);
    if (b) {
        Q_ASSERT(mButtonStyle.contains(widget));
        b->setStyle(mButtonStyle.value(widget));
        HbTextItem* label =
        static_cast<HbTextItem*>(b->primitive(HbStyle::P_PushButton_text));
        if (label) {
            label->setTextColor(Qt::white);
        }
    }
}

void BubbleWidgetManager::createDocumentHandlers(View view)
{
    Q_ASSERT(mDocumentLoaders.contains(view));

    QList<BubbleHandler*>* handlers = new QList<BubbleHandler*>;

    if (mDocumentLoaders[view]->findWidget(mContainerName[ExpandedBubble]) ) {
        if (view==ConferenceView) {
            BubbleConferenceHandler* handler =
                new BubbleConferenceHandler(*this,view,ExpandedBubble);
            handlers->append(handler);
        } else {
            BubbleExpandedHandler* handler =
                new BubbleExpandedHandler(*this,view,ExpandedBubble);
            handlers->append(handler);
        }
    }

    if (mDocumentLoaders[view]->findWidget(mContainerName[CollapsedBubble]) ) {
        BubbleCollapsedHandler* handler =
            new BubbleCollapsedHandler(*this,view,CollapsedBubble);
        handlers->append(handler);
    }

    if (mDocumentLoaders[view]->findWidget(mContainerName[CollapsedBubble2]) ) {
        BubbleCollapsedHandler* handler =
            new BubbleCollapsedHandler(*this,view,CollapsedBubble2);
        handlers->append(handler);
    }


    mDocumentHandlers.insert(view,handlers);
}

QGraphicsWidget* BubbleWidgetManager::container(
    View view,
    Container container) const
{
    QGraphicsWidget* w=0;

    if (mDocumentLoaders.contains(view)) {
        w = mDocumentLoaders.value(view)->findWidget(
            mContainerName[container]);
    }

    return w;
}


QGraphicsWidget* BubbleWidgetManager::widget(
    View view,
    Container container,
    Widget widget ) const
{
    QGraphicsWidget* w=0;

    if (mDocumentLoaders.contains(view)) {
        QString name;
        name.append(mWidgetPrefix.value(container));
        name.append(mWidgetName.value(widget));
        w = mDocumentLoaders.value(view)->findWidget(name);
    }

    return w;
}

QList<BubbleHandler*>* BubbleWidgetManager::handlers(
    QGraphicsWidget* widget) const
{
    int doc = mDocumentContent.key(widget);
    return mDocumentHandlers[doc];
}

void BubbleWidgetManager::handleOrientationChange(
    Qt::Orientation orientation)
{
    if (mOrientation!=orientation) {
        bool ok;
        QList<int> docs = mDocumentLoaders.keys();
        foreach (int doc, docs) {
            mDocumentLoaders[doc]->load(
                mFileNames[doc],mOrientationName.value(orientation),&ok);
            Q_ASSERT(ok);
        }
        mOrientation = orientation;
    }
}

QGraphicsWidget* BubbleWidgetManager::createParticipantListItem()
{
    return new BubbleParticipantListItem(mStylePluginName,
                                         *mButtonStyle.value(ParticipantListButton));
}

void BubbleWidgetManager::setStylePluginName(const QString& name)
{
    mStylePluginName = name;
}

// custom widget loading
BubbleDocumentLoader::BubbleDocumentLoader(const QString& stylePluginName)
    : mStylePluginName(stylePluginName)
{
}

QObject *BubbleDocumentLoader::createObject(
    const QString& type,
    const QString &name)
{
    if ( type == BubbleContainerWidget::staticMetaObject.className() ) {
        QObject *object = new BubbleContainerWidget(mStylePluginName);
        object->setObjectName(name);
        return object;
    }

    if ( type == BubbleImageWidget::staticMetaObject.className() ) {
        QObject *object = new BubbleImageWidget();
        object->setObjectName(name);
        return object;
    }

    if ( type == BubbleHeadingWidget::staticMetaObject.className() ) {
        QObject *object = new BubbleHeadingWidget(mStylePluginName);
        object->setObjectName(name);
        return object;
    }

    return HbDocumentLoader::createObject(type, name);
}

