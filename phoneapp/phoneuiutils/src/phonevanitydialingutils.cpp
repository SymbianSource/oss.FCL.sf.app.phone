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
*     Helper class for converting vanity dialing numbers
*
*/

// INCLUDE FILES
#include "phonevanitydialingutils.h"

// CONSTANTS
const TInt KCharA = 'A';
const TInt KCharB = 'B';
const TInt KCharC = 'C';
const TInt KCharD = 'D';
const TInt KCharE = 'E';
const TInt KCharF = 'F';
const TInt KCharG = 'G';
const TInt KCharH = 'H';
const TInt KCharI = 'I';
const TInt KCharJ = 'J';
const TInt KCharK = 'K';
const TInt KCharL = 'L';
const TInt KCharM = 'M';
const TInt KCharN = 'N';
const TInt KCharO = 'O';
const TInt KCharP = 'P';
const TInt KCharQ = 'Q';
const TInt KCharR = 'R';
const TInt KCharS = 'S';
const TInt KCharT = 'T';
const TInt KCharU = 'U';
const TInt KCharV = 'V';
const TInt KCharW = 'W';
const TInt KCharX = 'X';
const TInt KCharY = 'Y';
const TInt KCharZ = 'Z';

const TInt KChar2 = '2';
const TInt KChar3 = '3';
const TInt KChar4 = '4';
const TInt KChar5 = '5';
const TInt KChar6 = '6';
const TInt KChar7 = '7';
const TInt KChar8 = '8';
const TInt KChar9 = '9';

_LIT( KPhoneVanityValidFirstChars, "0123456789+" );
_LIT( KPhoneVanityValidChars, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*#+pw" );

// ---------------------------------------------------------
// CouldBeVanityNumber
//
// Vanity dialing numbers start with a number, and after that they
// contain only upper case letters (A-Z) and numbers
// for example: 555PIZZA, +3583ONE2THREE
// ---------------------------------------------------------
//
TBool CouldBeVanityNumber( TDesC& aDes )
    {
    if ( !aDes.Length() )
        {
        return EFalse;
        }
        
    TBool possibleVanityNumber = ETrue;
    
    // check first character
    TPtrC validFirstChar( KPhoneVanityValidFirstChars );
    if ( validFirstChar.Locate( aDes[0] ) == KErrNotFound )
        {
        possibleVanityNumber = EFalse;
        }
    
    // check rest of the string
    TInt i = 1;
    TPtrC validChar( KPhoneVanityValidChars );
    while ( possibleVanityNumber && i < aDes.Length() )
        {
        if ( validChar.Locate( aDes[i] ) == KErrNotFound )
            {
            possibleVanityNumber = EFalse;
            }
        i++;
        }
    
    return possibleVanityNumber;
    }

// ---------------------------------------------------------
// PhoneVanityDialingUtils::DoVanityNumberConversion
//
// ---------------------------------------------------------
//
EXPORT_C void PhoneVanityDialingUtils::DoVanityNumberConversion( TDes& aDes )
    {
    if ( CouldBeVanityNumber( aDes ) )
        {
        for ( TInt i = 0; i < aDes.Length(); i++ )
            {
            switch ( aDes[i] )
                {
                case KCharA:
                case KCharB:
                case KCharC:
                    aDes[i] = KChar2;
                    break;
                case KCharD:
                case KCharE:
                case KCharF:
                    aDes[i] = KChar3;
                    break;
                case KCharG:
                case KCharH:
                case KCharI:
                    aDes[i] = KChar4;
                    break;
                case KCharJ:
                case KCharK:
                case KCharL:
                    aDes[i] = KChar5;
                    break;
                case KCharM:
                case KCharN:
                case KCharO:
                    aDes[i] = KChar6;
                    break;
                case KCharP:
                case KCharQ:
                case KCharR:
                case KCharS:
                    aDes[i] = KChar7;
                    break;
                case KCharT:
                case KCharU:
                case KCharV:
                    aDes[i] = KChar8;
                    break;
                case KCharW:
                case KCharX:
                case KCharY:
                case KCharZ:
                    aDes[i] = KChar9;
                    break;
                default:
                    break;
                }
            }
        }
    }

// End of File
