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

#ifndef BUBBLEUTILS_H_
#define BUBBLEUTILS_H_

#include <QString>

class BubbleHeader;
class BubbleCallIndicator;
class HbIconItem;
class BubbleStyleOption;
class HbPushButton;
class HbAction;

class BubbleUtils
    {
public:
    static void setCallHeaderTexts3Lines(
        const BubbleHeader& header,
        BubbleStyleOption& option );

    static void setCallHeaderTexts2Lines(
        const BubbleHeader& header,
        BubbleStyleOption& option );

    static bool compareHeaders(
        const BubbleHeader* header1,
        const BubbleHeader* header2 );

    static QString stylePluginNameWithPath(const QString& pluginName);

    static void setButtonStyleForAction(HbPushButton& button,
                                        const HbAction& action);
    };

#endif /* BUBBLEUTILS_H_ */
