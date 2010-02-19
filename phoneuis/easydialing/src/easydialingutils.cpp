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
* Description:  Helper class for easydialing.
*
*/

#include <e32std.h>
#include <AknUtils.h>

#include "easydialingutils.h"

const TText KNameSeparatorChar = ' ';
const TText KListFieldSeparatorChar = '\t';

// -----------------------------------------------------------------------------
// CreateContactStringLC
//
// -----------------------------------------------------------------------------
//
HBufC* EasyDialingUtils::CreateContactStringLC(
        const TDesC& aFirstName,
        const TDesC& aLastName,
        const TDesC& aCompanyName,
        CPbkContactEngine::TPbkNameOrder aNameOrder )
    {
    // first strip control chars from the names
    HBufC* firstName = aFirstName.AllocLC();
    TPtr fPtr = firstName->Des();
    AknTextUtils::StripCharacters( fPtr, KAknStripListControlChars );

    HBufC* lastName = aLastName.AllocLC();
    TPtr lPtr = lastName->Des();
    AknTextUtils::StripCharacters( lPtr, KAknStripListControlChars );

    HBufC* companyName = aCompanyName.AllocLC();
    TPtr cPtr = companyName->Des();
    AknTextUtils::StripCharacters( cPtr, KAknStripListControlChars );

    // Calculate string lenght. If first name and last name are missing, company name is duplicated.
    // Add company name lenght twice to make sure there is enough space.
    TInt stringLength = firstName->Length() + lastName->Length() + 2 * companyName->Length() + 2;
    HBufC* string = HBufC::NewLC( stringLength );
    TPtr ptr = string->Des();
    
    // If there is no first name, the last name will be used.
    if ( firstName->Length() == 0 )
        {
        ptr.Copy( *lastName );
        }
    
    // If there is no last name, the first name will be used.
    else if ( lastName->Length() == 0 )
        {
        ptr.Copy( *firstName );
        }
    
    // If there are both first and last name, the name string is composed of them both.
    else 
        {
        if( aNameOrder == CPbkContactEngine::EPbkNameOrderFirstNameLastName )
            {
            ptr.Copy( *firstName );
            ptr.Append( KNameSeparatorChar );
            ptr.Append( *lastName );
            }
        else
            {
            ptr.Copy( *lastName );
            ptr.Append( KNameSeparatorChar );
            ptr.Append( *firstName );
            }        
        } 
    
    // If firstname and last name are missing, duplicate company name into name field.
    if ( ptr.Length() == 0 )
        {
        ptr.Append( *companyName );
        }
    
    // Append company name using tab as a separator.
    ptr.Append( KListFieldSeparatorChar );
    ptr.Append( *companyName );
    
    CleanupStack::Pop( string );
    CleanupStack::PopAndDestroy( 3, firstName );
    CleanupStack::PushL( string );
    return string;
    }
