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
 * Description:
 *
 */

#ifndef CPPLUGINCOMMON_H
#define CPPLUGINCOMMON_H

#include <qvariant.h>
#include "cptelephonyutilsdefs.h"

class XQSettingsManager;

using namespace CpTelephonyUtils;

/*!
    \class Tools
    \brief The static functionality.
 */
class CPTELEPHONYUTILS_EXPORT Tools : public QObject
{
    Q_OBJECT

public:
    /*!
      Is VoIP currently supported
      */
    static bool voipSupported();

    /*!
      Is video currently supported
      */
    static bool videoSupported();

    /*!
      Service code conversion
      */
    static BasicServiceGroups convertEtelMobileServiceCode(int serviceCode);

    /*!
      Error code text conversion
      */
    static bool errorCodeTextMapping(const int errorcode, QString &errorText);

};

/*!
    \class SettingsWrapper
    \brief Wraps central repository and P&S dependency.
 */
class CPTELEPHONYUTILS_EXPORT CpSettingsWrapper : public QObject
{
    Q_OBJECT
public:
    CpSettingsWrapper(QObject *parent = NULL);
    ~CpSettingsWrapper();
public:
    /*!
      Show call duration setting
      */
    bool showCallDuration();
    int setShowCallDuration(bool value);

    /*!
      Soft reject text setting
      */
    void readSoftRejectText( QString &text, bool &userDefined );
    int writeSoftRejectText(const QString &text, bool userDefined );

    /*!
      Number grouping support
      */
    bool numberGroupingSupported() const;
    
    /*!
     Call waiting distiquish not provisioned support
     */
    bool isFeatureCallWaitingDistiquishNotProvisionedEnabled();
    
    /*!
     Checks if phone is in offline mode or not.
     Return true if phone is in offline mode.
     Return false if phone is not in offline mode.
     */
    bool isPhoneOffline() const;

private:
    /*!
      Read cenrep value. 
      */
    QVariant readCenrepValue( const long int uid, const unsigned long int key) const;
    
    /*!
      Read cenrep string. 
      */
    QString readCenrepString( const long int uid, const unsigned long int key) const;
    
    /*!
      Write cenrep value or string. 
      */
    int writeCenrepValue( const long int uid, const unsigned long int key,
        const QVariant &settingsKeyValue ) const;

private: // Data
    // Own
    XQSettingsManager* m_Settings;
};

#endif // CPPLUGINCOMMON_H
