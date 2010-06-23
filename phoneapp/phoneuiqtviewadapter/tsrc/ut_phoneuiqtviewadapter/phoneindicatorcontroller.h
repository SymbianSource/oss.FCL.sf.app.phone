/*
 * phoneindicatorcontroller.h
 *
 *  Created on: May 4, 2010
 *      Author: maposio
 */

#ifndef PHONEINDICATORCONTROLLER_H_
#define PHONEINDICATORCONTROLLER_H_

#include <QObject>

extern bool m_setActiveCallCalled;
extern bool m_clearActiveCallCalled;
extern bool m_enableCallIndCalled;
extern bool m_disableCallIndCalled;

class PhoneIndicatorController : public QObject
    {
    Q_OBJECT
public:
    PhoneIndicatorController(QObject *parent);
    virtual ~PhoneIndicatorController();
    
    void setActiveCallData();
    void clearActiveCallData();
    void enableActiveCallIndicator();
    void disableActiveCallIndicator();
    
    };

#endif /* PHONEINDICATORCONTROLLER_H_ */
