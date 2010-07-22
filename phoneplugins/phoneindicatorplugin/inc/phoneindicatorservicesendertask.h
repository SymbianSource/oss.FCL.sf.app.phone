/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef PHONEINDICATORSERVICESENDERTASK_H_
#define PHONEINDICATORSERVICESENDERTASK_H_

#include <QObject>
#include <QRunnable>
#include "phoneindicators.h"

class PhoneIndicatorServiceSenderTask : public QRunnable
{
public:
    PhoneIndicatorServiceSenderTask(Interaction interaction);
    ~PhoneIndicatorServiceSenderTask(){}
    
    void run();
private:
    Interaction     m_interaction;
};

#endif /* PHONEINDICATORSERVICESENDERTASK_H_ */

