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
* Description:  Custom style for push buttons.
*
*/

#ifndef BUBBLEBUTTONSTYLE_H_
#define BUBBLEBUTTONSTYLE_H_

#include <hbstyle.h>

class BubbleButtonStyle : public HbStyle
{
public:
    enum ButtonColor
    {
        Default,
        Green,
        Red,
    };

public:
    explicit BubbleButtonStyle();
    virtual ~BubbleButtonStyle();

    void setButtonColor(ButtonColor color);

    void updatePrimitive(
        QGraphicsItem *item,
        HbStyle::Primitive primitive,
        const QStyleOption *option ) const;

protected:
    ButtonColor mButtonColor;
};

#endif /* BUBBLEBUTTONSTYLE_H_ */
