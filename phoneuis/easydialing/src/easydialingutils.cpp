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

const TText KGraphicReplaceCharacter = ' ';

// Zero Widthcharacters
const TUint KZWSCharacter = 0x200B;
const TUint KZWNJCharacter = 0x200C;
const TUint KZWJCharacter = 0x200D;

inline TBool ZWSCharacter( const TChar aChar );

void ReplaceNonGraphicCharacters( TDes& aText, TText aChar );
 
// -----------------------------------------------------------------------------
// CreateContactStringLC
//
// -----------------------------------------------------------------------------
//
HBufC* EasyDialingUtils::CreateContactStringLC(
        const TDesC& aFirstName,
        const TDesC& aLastName,
        const TDesC& aCompanyName,
        CEasyDialingContactDataManager::TNameOrder aNameOrder )
    {
    // first strip control chars from the names
    HBufC* firstName = aFirstName.AllocLC();
    TPtr fPtr = firstName->Des();
    ReplaceNonGraphicCharacters( fPtr, KGraphicReplaceCharacter );
    AknTextUtils::StripCharacters( fPtr, KAknStripListControlChars );

    HBufC* lastName = aLastName.AllocLC();
    TPtr lPtr = lastName->Des();
    ReplaceNonGraphicCharacters( lPtr, KGraphicReplaceCharacter );
    AknTextUtils::StripCharacters( lPtr, KAknStripListControlChars );

    HBufC* companyName = aCompanyName.AllocLC();
    TPtr cPtr = companyName->Des();
    ReplaceNonGraphicCharacters( cPtr, KGraphicReplaceCharacter );
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
        if( aNameOrder == CEasyDialingContactDataManager::EFirstnameLastname )
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

    // If firstname and last name are missing, put company name into the first field.
    if ( ptr.Length() == 0 )
        {
        ptr.Append( *companyName );
        }
    else
        {
        // Otherwise add company name into the second field using tab as a separator.
        ptr.Append( KListFieldSeparatorChar );
        ptr.Append( *companyName );
        }

    CleanupStack::Pop( string );
    CleanupStack::PopAndDestroy( 3, firstName );
    CleanupStack::PushL( string );
    return string;
    }

/*
 * ==============================================================================
 * 
 * Local functions
 * 
 * ==============================================================================
 */

/**
 * Checkif the given char is a zero width character:
 * ZeroWidth Space, Zero Width Non-Joiner
 * orZero Width Joiner character
 * @paramaChar Given character
 *@return ETrue if aChar is zero width character.
 *        EFalse if aChar is not specified zero width character.
 */

inline TBool ZWSCharacter( const TChar aChar )
   {
   const TChar zwsChar( KZWSCharacter );
   const TChar zwnjChar( KZWNJCharacter );
   const TChar zwjChar( KZWJCharacter );
   return ( aChar == zwsChar ) || ( aChar == zwnjChar ) ||
          ( aChar == zwjChar );
}

/**
 * Replaces all non-graphic characters (!TChar::IsGraph()) in
 * a string with the specified character.
 **/

void ReplaceNonGraphicCharacters( TDes& aText, TText aChar )
   {
   const TInt len = aText.Length();
   for ( TInt i = 0; i <len; ++i )
      {
      if ( !TChar( aText[i] ).IsGraph() && !ZWSCharacter( aText[i] ) )
          {
          //If non-graphic char is specified in ZWSCharacter,
          //it will not be replaced. Otherwise replace non-graphic
          //character with aChar.
          aText[i] = aChar;
          }
      }
    }
