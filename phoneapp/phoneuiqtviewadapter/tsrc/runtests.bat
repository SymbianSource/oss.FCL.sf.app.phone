@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description: 
@rem

echo OFF

del \epoc32\winscw\c\data\ut_phoneuiqtbuttonscontroller.log
\epoc32\release\winscw\udeb\ut_phoneuiqtbuttonscontroller.exe -o c:\data\ut_phoneuiqtbuttonscontroller.log
type \epoc32\winscw\c\data\ut_phoneuiqtbuttonscontroller.log

del \epoc32\winscw\c\data\ut_phoneuiqtviewadapter.log
\epoc32\release\winscw\udeb\ut_phoneuiqtviewadapter.exe -o c:\data\ut_phoneuiqtviewadapter.log
type \epoc32\winscw\c\data\ut_phoneuiqtviewadapter.log

del \epoc32\winscw\c\data\ut_phonebubblewrapper.log
\epoc32\release\winscw\udeb\ut_phonebubblewrapper.exe -o c:\data\ut_phonebubblewrapper.log
type \epoc32\winscw\c\data\ut_phonebubblewrapper.log

del \epoc32\winscw\c\data\ut_phoneresourceadapter.log
\epoc32\release\winscw\udeb\ut_phoneresourceadapter.exe -o c:\data\ut_phoneresourceadapter.log
type \epoc32\winscw\c\data\ut_phoneresourceadapter.log

del \epoc32\winscw\c\data\ut_telephonyservice.log
\epoc32\release\winscw\udeb\ut_telephonyservice.exe -o c:\data\ut_telephonyservice.log
type \epoc32\winscw\c\data\ut_telephonyservice.log

del \epoc32\winscw\c\data\ut_phonevisibilityhandler.log
\epoc32\release\winscw\udeb\ut_phonevisibilityhandler.exe -o c:\data\ut_phonevisibilityhandler.log
type \epoc32\winscw\c\data\ut_phonevisibilityhandler.log
