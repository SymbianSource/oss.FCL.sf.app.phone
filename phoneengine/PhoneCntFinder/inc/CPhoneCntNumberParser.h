/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class loads contact model phone number parser DLL.
*
*/


#ifndef CCntNumberParser_H
#define CCntNumberParser_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class CContactPhoneNumberParser;

// CLASS DECLARATION

/**
 * Template classes to call ResetAndDestroy on array classes
 * automatically, when ::PopAndDestroy'ing from the cleanup stack.
 */
template <class T>
class CleanupResetAndDestroy
    {
public:
    inline static void PushL(T& aRef);
private:
    static void ResetAndDestroy(TAny *aPtr);
    };
template <class T>
inline void CleanupResetAndDestroyL(T& aRef);

// Template class CleanupClose
template <class T>
inline void CleanupResetAndDestroy<T>::PushL(T& aRef)
    {CleanupStack::PushL(TCleanupItem(&ResetAndDestroy, &aRef));}
template <class T>
void CleanupResetAndDestroy<T>::ResetAndDestroy(TAny *aPtr)
    {static_cast<T*>(aPtr)->ResetAndDestroy();}
template <class T>
inline void CleanupResetAndDestroyPushL(T& aRef)
    {CleanupResetAndDestroy<T>::PushL(aRef);}


/**
*  CCntNumberParser
*
*  This class loads contact model phone number parser DLL.
*
*  @lib PhoneCntFinder
*  @since 2.8
*/
NONSHARABLE_CLASS( CCntNumberParser ) : public CBase
    {
    public: // Constructor

        /**
        * Two-phased constructor.
        * @return New instance
        */
        static CCntNumberParser* NewL();

        /**
        * Destructor.
        */
        ~CCntNumberParser();

        /**
        * Gets a pointer of CContactPhoneNumberParser instance.
        *
        * @return the pointer of the instance.
        */
        CContactPhoneNumberParser* CntPhoneNumberParser() const;

    private:

        /**
        * C++ constructor.
        */
        CCntNumberParser();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

    private: // Data

        // Parser itself
        CContactPhoneNumberParser* iParser;
    };


#endif //CCntNumberParser_H

// End of File
