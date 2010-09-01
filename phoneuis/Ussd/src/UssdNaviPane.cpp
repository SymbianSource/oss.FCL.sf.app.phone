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
* Description:  Declares navi pane control for application.
*
*/


// INCLUDE FILES
#include    <gsmuset.h>                 // CCnvCharacterSetConverter
#include    <aknenv.h>                  // CAknEnv
#include    <aknnavide.h>               // CAknNavigationDecorator
#include    <aknEditStateIndicator.h>
#include    <AknIndicatorContainer.h>
#include    <aknnavilabel.h>
#include    "UssdComms.h" 
#include    "UssdAppUi.h"
#include    "UssdNaviPane.h"
#include    "UssdLogger.h"
 
#include <AknUtils.h>
// CONSTANTS

// Unicode char codes for GSM 03.38 7 bit ext table characters 
static const TUint KUssdEdEuroSymbol = 0x20ac;
static const TUint KUssdEdLeftSquareBracket = 0x5b;
static const TUint KUssdEdReverseSolidus = 0x5c;
static const TUint KUssdEdRightSquareBracket = 0x5d;
static const TUint KUssdEdCircumflexAccent = 0x5e;
static const TUint KUssdEdLeftCurlyBracket = 0x7b;
static const TUint KUssdEdVerticalLine = 0x7c;
static const TUint KUssdEdRightCurlyBracket = 0x7d;
static const TUint KUssdEdTilde = 0x7e;
static const TUint KUssdDownwardsArrowLeft = 0x21B2;
static const TUint KUssdDownwardsArrowRight = 0x21B3;
static const TUint KUssdCharDwithLine = 0xD0;
static const TUint KUssdCharBigRhoo = 0xDE; 
static const TUint KUssdCharSmallDelta = 0xF0;
static const TUint KUssdCharSmallFii = 0xFE;
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUssdNaviPane::CUssdNaviPane
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CUssdNaviPane::CUssdNaviPane( CUssdAppUi& aAppUi ): iAppUi( aAppUi )
    {
    }

// -----------------------------------------------------------------------------
// CUssdNaviPane::NewL
//
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
CUssdNaviPane* CUssdNaviPane::NewL( CUssdAppUi& aAppUi )
    {
    CUssdNaviPane* self = new ( ELeave ) CUssdNaviPane( aAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CUssdNaviPane::ConstructL
//
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUssdNaviPane::ConstructL()
    {
    _LOGSTRING( "CUssdNaviPane::ConstructL =>" )
    
    CreateExtraNaviLabelL();

    User::LeaveIfError( iPeninputServer.Connect() );
    iCharacterSetConverter = CCnvCharacterSetConverter::NewL();

    iCharacterSetConverter->PrepareToConvertToOrFromL(
        KCharacterSetIdentifierSms7Bit,
        iCoeEnv->FsSession() );

    iCharacterSetConverter->SetDowngradeForExoticLineTerminatingCharacters(
        CCnvCharacterSetConverter::EDowngradeExoticLineTerminatingCharactersToJustLineFeed );
    _LOGSTRING( "CUssdNaviPane::ConstructL <=" )
    }

// -----------------------------------------------------------------------------
// CUssdNaviPane::~CUssdNaviPane
// 
// Destructor
// -----------------------------------------------------------------------------
//
CUssdNaviPane::~CUssdNaviPane()
    {
    _LOGSTRING( "CUssdNaviPane::~CUssdNaviPane =>" )
    iPeninputServer.Close();
    
    if ( iNaviPane && iNaviDecorator )
        {
        iNaviPane->Pop( iNaviDecorator );
        delete iNaviDecorator;
        iNaviDecorator = NULL;
        iNaviPane = NULL;
        }
    
    delete iCharacterSetConverter;
    iCharacterSetConverter = NULL;

    iNaviLabel = NULL;
    _LOGSTRING( "CUssdNaviPane::~CUssdNaviPane <=" )
    }
  
// ----------------------------------------------------
// CUssdNaviPane::CreateExtraNaviLabelL
// Creates an extra navi label.
// (other items were commented in a header).
// ----------------------------------------------------
//
void CUssdNaviPane::CreateExtraNaviLabelL()
    {
      
    CEikStatusPane* sp = iAppUi.StatusPane();
    iNaviPane = static_cast<CAknNavigationControlContainer*>( 
        sp->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );     
    
    iNaviDecorator = iNaviPane->CreateNavigationLabelL( 
        KNullDesC );
    iNaviLabel = static_cast<CAknNaviLabel*>( 
        iNaviDecorator->DecoratedControl() );
    iNaviPane->PushL( *iNaviDecorator ) ;
    
    }

// ---------------------------------------------------------
// CUssdNaviPane::UpdateMsgLengthL
// Update the message length information to navipane
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CUssdNaviPane::UpdateMsgLengthL( const TDesC& aInputString, 
                                             CCoeControl* aControl )
    {
    _LOGSTRING2( "CUssdNaviPane::UpdateMsgLengthL =>, aInputString=%S",
    &aInputString )
    TInt maxLen(0);
    const TInt txtLen = aInputString.Length();
 
    HBufC* digitBuffer = HBufC::NewLC( EAknMaxIntegerDigits ); 
    TPtr bufPtr( digitBuffer->Des() );
 
    if ( NeedsToBeSentAsUnicodeL( aInputString ) )
        {
        bufPtr.AppendNum( KUssdEditorMaxLenght/2 - txtLen );
        maxLen=KUssdEditorMaxLenght/2;
        }
    else
        {
        TInt extendedCount = CountExtendedTableChars( aInputString );
        TInt totalLen = KUssdEditorMaxLenght - ( txtLen + extendedCount );
        if ( totalLen < 0 ) // last was alphabet extended when one left
            {
            GenerateBackSpaceKeyL( aControl );
            extendedCount--;
            totalLen = totalLen+2;
            }
        bufPtr.AppendNum( totalLen );
        maxLen=KUssdEditorMaxLenght-extendedCount;
        }
    
   AknTextUtils::LanguageSpecificNumberConversion( bufPtr );

    // Giving msg length string to indicator container
    MAknEditingStateIndicator* stateIndicator = 
        iAvkonEnv->EditingStateIndicator();
    if ( stateIndicator )
        {
        CAknIndicatorContainer* iIndicatorContainerSMS = 
            stateIndicator->IndicatorContainer();
        if ( iIndicatorContainerSMS )
            {
            iIndicatorContainerSMS->SetIndicatorState( 
                TUid::Uid( EAknNaviPaneEditorIndicatorMessageLength ), 
                EAknIndicatorStateOn );
            iIndicatorContainerSMS->SetIndicatorValueL(
                TUid::Uid( EAknNaviPaneEditorIndicatorMessageLength ), 
                *digitBuffer );
            }
        }
    
    iNaviLabel->SetTextL( *digitBuffer );
    _LOGSTRING(
    "CUssdNaviPane::UpdateMsgLengthL iPeninputServer.UpdateAppInfo" )
    iPeninputServer.UpdateAppInfo( *digitBuffer, EAppIndicatorMsg );
    CleanupStack::PopAndDestroy( digitBuffer );
    _LOGSTRING2( "CUssdNaviPane::UpdateMsgLengthL <=, maxLen=%d",
    maxLen )
    
    return maxLen;
    }
 

// ---------------------------------------------------------
// CUssdNaviPane::NeedsToBeSentAsUnicodeL
// Checks if there are any unicode characters in the message
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CUssdNaviPane::NeedsToBeSentAsUnicodeL( const TDesC& aInputString ) const
    {
    _LOGSTRING2(
    "CUssdNaviPane::NeedsToBeSentAsUnicodeL =>, aInputString=%S",
    &aInputString )
    
    TBool needsToBeSentAsUnicode = EFalse;
      
    for ( TPtrC remainderOfInputString( aInputString ); 
          remainderOfInputString.Length()>0 ; )
        {
        TBuf8<KUssdEditorMaxLenght> notUsedForeignParam;
        TInt numberOfUnconvertibleCharacters = 0;
        const TInt returnValue = 
            iCharacterSetConverter->ConvertFromUnicode( 
                notUsedForeignParam,
                remainderOfInputString, 
                numberOfUnconvertibleCharacters );
        if ( ( returnValue < 0 ) || 
             ( numberOfUnconvertibleCharacters > 0 )) 
            // if there was an error in trying to do the conversion, or if 
            // there was an unconvertible character (e.g. a Chinese character)
            {
            TUint uChar;
            for ( TInt i = 0;i < remainderOfInputString.Length();i++ )
                {
                uChar = TUint( remainderOfInputString[ i ] );
                if ( uChar ==  KUssdDownwardsArrowLeft || 
                     uChar == KUssdDownwardsArrowRight ||
                     uChar == KUssdCharDwithLine ||
                     uChar == KUssdCharBigRhoo ||
                     uChar == KUssdCharSmallDelta ||
                     uChar == KUssdCharSmallFii )
                    {
                    numberOfUnconvertibleCharacters--;
                    }  
                }
                   
            if ( returnValue < 0 || numberOfUnconvertibleCharacters > 0  ) 
                {
                // if there was an error in trying to do the conversion, or if there was an
                // unconvertible character (e.g. a Chinese character)
                needsToBeSentAsUnicode = ETrue;
                break;
                }      
            }
        
        remainderOfInputString.Set( 
            remainderOfInputString.Right( returnValue ) );
        }
    _LOGSTRING2(
    "CUssdNaviPane::NeedsToBeSentAsUnicodeL <=, needsToBeSentAsUnicode=%d",
    needsToBeSentAsUnicode )
    return needsToBeSentAsUnicode;
    }

// ---------------------------------------------------------
// CUssdNaviPane::CountExtendedTableChars
// Counts if there are 7bit extended table characters in message
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CUssdNaviPane::CountExtendedTableChars( const TDesC& aInputString ) const
    {
    const TInt total = aInputString.Length();
    TInt extChars = 0;
    
    TUint uChar;
    for ( TInt i=0; i < total; i++ )
        {
        uChar = TUint(aInputString[i]);
        if (( uChar == KUssdEdLeftSquareBracket) || 
            ( uChar == KUssdEdReverseSolidus ) || 
            ( uChar == KUssdEdRightSquareBracket ) || 
            ( uChar == KUssdEdCircumflexAccent ) || 
            ( uChar == KUssdEdLeftCurlyBracket ) || 
            ( uChar == KUssdEdVerticalLine ) || 
            ( uChar == KUssdEdRightCurlyBracket ) || 
            ( uChar == KUssdEdTilde ) || 
            ( uChar == KUssdEdEuroSymbol )) 
            {
            extChars = extChars + 1;
            }
        }
    return extChars;
    }

// ---------------------------------------------------------
// CUssdNaviPane::GenerateBackSpaceKeyL
// Generates a backspace key press event.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CUssdNaviPane::GenerateBackSpaceKeyL( CCoeControl* aControl )
    {
    TKeyEvent keyEvent;
    keyEvent.iCode = EKeyBackspace;
    keyEvent.iScanCode = EStdKeyBackspace;
    keyEvent.iRepeats = 1;
    keyEvent.iModifiers = 0;
    TEventCode type = EEventKey;

    if ( aControl )
        {
        aControl->OfferKeyEventL( keyEvent, type );
        }
    }

// End of File  
