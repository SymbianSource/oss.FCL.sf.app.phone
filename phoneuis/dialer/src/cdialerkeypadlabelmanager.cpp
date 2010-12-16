/*
* Copyright (c) 2009, 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Helper class to handle virtual keypad text labels
*
*/

#include <AknUtils.h>
#include <bautils.h>
#include <barsread.h>
#include <avkon.rsg>
#include <dialer.rsg>
#include <dialerkeypad.rsg>
#include <e32const.h>
#include <e32lang.h>
#include <PtiEngine.h>
#include <AknFepInternalCRKeys.h>
#include <AvkonInternalCRKeys.h>
#include <bacntf.h> 

#include "cdialerkeypadlabelmanager.h"
#include "dialercommon.h"

// CONSTANTS
const TInt KKeyCount( 12 );

_LIT( KDialerButton1NumLabel, "1" );
_LIT( KDialerButton2NumLabel, "2" );
_LIT( KDialerButton3NumLabel, "3" );
_LIT( KDialerButton4NumLabel, "4" );
_LIT( KDialerButton5NumLabel, "5" );
_LIT( KDialerButton6NumLabel, "6" );
_LIT( KDialerButton7NumLabel, "7" );
_LIT( KDialerButton8NumLabel, "8" );
_LIT( KDialerButton9NumLabel, "9" );
_LIT( KDialerButton10NumLabel, "*+" );
_LIT( KDialerButton11NumLabel, "0" );
_LIT( KDialerButton12NumLabel, "#" );

_LIT( KDialerKeypadResourceFile, "z:\\resource\\dialerkeypad" );
_LIT( KDialerKeypadResourceFileExt, ".rsc" );
_LIT( KDialerKeypadVariantExtSeparator, "_" );

_LIT( KScriptLatin, "latin" );
_LIT( KScriptCyrillic, "cyrillic" );
_LIT( KScriptGreek, "greek" );
_LIT( KScriptHebrew, "hebrew" );
_LIT( KScriptArabic, "arabic" );
_LIT( KScriptFarsi, "farsi" );
_LIT( KScriptUrdu, "urdu" );
_LIT( KScriptThai, "thai" );
_LIT( KScriptDevanagari, "devanagari" );
_LIT( KScriptChnPinyin, "chn_pinyin" );
_LIT( KScriptChnZhuyin, "chn_zhuyin" );
_LIT( KScriptChnStroke, "chn_stroke" );
_LIT( KScriptChnCangjie, "chn_cangjie" );

enum TScript
    {
    EScriptLatin = 0,
    EScriptCyrillic,
    EScriptGreek,
    EScriptHebrew,
    EScriptArabic,
    EScriptFarsi,
    EScriptUrdu,
    EScriptThai,
    EScriptDevanagari,
    EScriptChinese, // generic value for all Chinese input modes
    EScriptChnPinyin,
    EScriptChnZhuyin,
    EScriptChnStroke,
    EScriptChnCangjie
    };

struct TLanguageScriptMapping
    {
    TLanguage iLang;
    TScript iScript;
    };

// Following table maps non-Latin languages to corresponding scripts.
// Rest of the languages are asumed to use Latin script.
// The data of the table has been extracted from the header 
// \sf\mw\inputmethods\textinput\peninputgenericitut\inc\peninputgenericitutdata.h
const TLanguageScriptMapping KScriptMap[] =
    {
        {ELangRussian, EScriptCyrillic},
        {ELangBulgarian, EScriptCyrillic},
        {ELangUkrainian, EScriptCyrillic},
        
        {ELangGreek, EScriptGreek},
        {ELangCyprusGreek, EScriptGreek},
        
        {ELangHebrew, EScriptHebrew},
        
        {ELangArabic, EScriptArabic},
        
        {ELangFarsi, EScriptFarsi},
        
        {ELangUrdu, EScriptUrdu},
        
        {ELangThai, EScriptThai},
        
        {ELangHindi, EScriptDevanagari},
        {ELangMarathi, EScriptDevanagari},
        
        {ELangTaiwanChinese, EScriptChinese},
        {ELangHongKongChinese, EScriptChinese},
        {ELangPrcChinese, EScriptChinese}
    };
const TInt KScriptMapCount = sizeof( KScriptMap ) / sizeof( TLanguageScriptMapping );

// Values for Chinese adaptive search modes, as documented in AvkonInternalCRKeys.h
enum TChinesePrcMode
    {
    EChnPrcPinyin = 0,
    EChnPrcStroke = 1
    };
enum TChineseTwMode
    {
    EChnTwZhuyin = 0,
    EChnTwStroke = 1
    };
enum TChineseHkMode
    {
    EChnHkStroke = 0,
    EChnHkCangjie = 1
    };


// LOCAL FUNCTION DECLARATIONS
static TScript ScriptForLang( TLanguage aLang );
static const TDesC& ScriptName( TScript aScriptCode );
static TBool ScriptCanSubstituteLatin( TScript aScript );


// MEMBER FUNCTION DEFINITIONS

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::CDialerKeyPadLabelManager
//
// ---------------------------------------------------------------------------
//
CDialerKeyPadLabelManager::CDialerKeyPadLabelManager( CCoeEnv& aCoeEnv, 
        MDialerKeyPadLabelManagerCallback& aObserver )
    : iCoeEnv( aCoeEnv ), iObserver( aObserver )
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::~CDialerKeyPadLabelManager
//
// ---------------------------------------------------------------------------
//
CDialerKeyPadLabelManager::~CDialerKeyPadLabelManager()
    {
    delete iPtiEngine;
    
    delete iAknFepRepository;
    delete iAvkonRepository;
    
    if ( iEnvChangeNotifier )
        {
        iEnvChangeNotifier->Cancel();
        delete iEnvChangeNotifier;
        }
    
    delete iKeypadNumLabelArray;
    delete iKeypadFirstLabelArray;
    delete iKeypadSecondLabelArray;
    
    if ( iResourceOffset )
        {
        iCoeEnv.DeleteResourceFile( iResourceOffset );
        }
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::NewLC
//
// ---------------------------------------------------------------------------
//
CDialerKeyPadLabelManager* CDialerKeyPadLabelManager::NewLC( CCoeEnv& aCoeEnv, 
        MDialerKeyPadLabelManagerCallback& aObserver )
    {
    CDialerKeyPadLabelManager* self =
            new (ELeave) CDialerKeyPadLabelManager( aCoeEnv, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::NewL
//
// ---------------------------------------------------------------------------
//
CDialerKeyPadLabelManager* CDialerKeyPadLabelManager::NewL( CCoeEnv& aCoeEnv, 
        MDialerKeyPadLabelManagerCallback& aObserver )
    {
    CDialerKeyPadLabelManager* self = 
            CDialerKeyPadLabelManager::NewLC( aCoeEnv, aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::ConstructL
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadLabelManager::ConstructL()
    {
    iPtiEngine = CPtiEngine::NewL();
    
    iAknFepRepository = CDialerCenRepWrapper::NewL( *this, KCRUidAknFep );
    iAvkonRepository = CDialerCenRepWrapper::NewL( *this, KCRUidAvkon );
    
    TCallBack envChangeCallback( EnvChangeCallback, this );
    iEnvChangeNotifier = CEnvironmentChangeNotifier::NewL( 
            CActive::EPriorityStandard, envChangeCallback );
    iEnvChangeNotifier->Start();
    
    iKeypadNumLabelArray = new ( ELeave ) CDesCArrayFlat( KKeyCount );
    iKeypadFirstLabelArray = new ( ELeave ) CDesCArrayFlat( KKeyCount );
    iKeypadSecondLabelArray = new ( ELeave ) CDesCArrayFlat( KKeyCount );
    
    UpdateNumLabelsL();
    UpdateTextLabelsL();
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::ButtonNumLabel
//
// ---------------------------------------------------------------------------
//
TPtrC CDialerKeyPadLabelManager::ButtonNumLabel( TInt aIndex ) const
    {
    __ASSERT_DEBUG( aIndex < iKeypadNumLabelArray->Count(), DialerPanic(EDialerPanicLabelNotFound) );

    TPtrC label( KNullDesC );
    if ( aIndex < iKeypadNumLabelArray->Count() )
        {
        label.Set( (*iKeypadNumLabelArray)[ aIndex ] );
        }
    return label;
    }   

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::ButtonFirstAlphaLabel
//
// ---------------------------------------------------------------------------
//
TPtrC CDialerKeyPadLabelManager::ButtonFirstAlphaLabel( TInt aIndex ) const
    {
    __ASSERT_DEBUG( aIndex < iKeypadFirstLabelArray->Count(), DialerPanic(EDialerPanicLabelNotFound) );

    TPtrC label( KNullDesC );
    if ( aIndex < iKeypadFirstLabelArray->Count() )
        {
        label.Set( (*iKeypadFirstLabelArray)[ aIndex ] );
        }
    return label;
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::ButtonSecondAlphaLabel
//
// ---------------------------------------------------------------------------
//
TPtrC CDialerKeyPadLabelManager::ButtonSecondAlphaLabel( TInt aIndex ) const
    {
    __ASSERT_DEBUG( aIndex < iKeypadSecondLabelArray->Count(), DialerPanic(EDialerPanicLabelNotFound) );

    TPtrC label( KNullDesC );
    
    if ( ( aIndex < iKeypadSecondLabelArray->Count() ) &&
            ShowSecondLabel() )
        {
        label.Set( (*iKeypadSecondLabelArray)[ aIndex ] );
        }
    return label;
    }   

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::UpdateTextLabelsL
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadLabelManager::HandleRepositoryChange( 
        TUid aRepository, TUint32 aId )
    {
    if ( (aRepository == KCRUidAknFep && aId == KAknFepInputTxtLang) ||
         (aRepository == KCRUidAvkon && aId == KAknAdaptiveSearchChinesePRC) ||
         (aRepository == KCRUidAvkon && aId == KAknAdaptiveSearchChineseTW) ||
         (aRepository == KCRUidAvkon && aId == KAknAdaptiveSearchChineseHongkong) )
        {
        // Check if alpabet labels need to be changed
        TInt scriptToUse( EScriptLatin );
        TRAP_IGNORE( scriptToUse = LabelScriptToUseL() );
        if ( iLabelScriptInUse != scriptToUse )
            {
            TRAP_IGNORE( UpdateTextLabelsL() );
            iObserver.KeyLabelsChanged();
            }
        }
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::HandleEnvChange
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadLabelManager::HandleEnvChange()
    {
    if ( iEnvChangeNotifier->Change() & EChangesLocale )
        {
        TLocale locale;
        if ( locale.DigitType() != iDigitTypeInUse )
            {
            TRAP_IGNORE( UpdateNumLabelsL() );
            iObserver.KeyLabelsChanged();
            }
        }
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::EnvChangeCallback
//
// ---------------------------------------------------------------------------
//
TInt CDialerKeyPadLabelManager::EnvChangeCallback( TAny* aSelfPtr )
    {
    CDialerKeyPadLabelManager* self =
            static_cast< CDialerKeyPadLabelManager* >( aSelfPtr );
    self->HandleEnvChange();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::UpdateTextLabelsL
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadLabelManager::UpdateTextLabelsL()
    {
    iKeypadFirstLabelArray->Reset();
    iKeypadSecondLabelArray->Reset();
    
    // Load the correct keypad resource file
    TFileName resourceName;
    GetResourceFileNameL( resourceName );
    iResourceOffset = iCoeEnv.AddResourceFileL( resourceName );

    // Read keypad labels from the resource
    TResourceReader reader;
    iCoeEnv.CreateResourceReaderLC( reader, R_DIALER_KEYPAD );
    
    TInt16 count = reader.ReadInt16();

    for ( TInt i = 0 ; i < count ; ++i )
        {
        TPtrC firstRow = reader.ReadTPtrC();
        iKeypadFirstLabelArray->AppendL( firstRow );
        TPtrC secondRow = reader.ReadTPtrC();
        iKeypadSecondLabelArray->AppendL( secondRow );
        }

    CleanupStack::PopAndDestroy(); // reader
    
    // Free resource as it's not usually needed anymore. Ensure that destructor 
    // won't free it again.
    iCoeEnv.DeleteResourceFile( iResourceOffset );
    iResourceOffset = 0;
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::UpdateNumLabelsL
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadLabelManager::UpdateNumLabelsL()
    {
    // Save digit type used. This is to allow us update number labels
    // only when digit type is changed and not on all locale changes.
    TLocale locale;
    iDigitTypeInUse = locale.DigitType();
    
    iKeypadNumLabelArray->Reset();
    
    // Fill number labels array
    iKeypadNumLabelArray->AppendL( KDialerButton1NumLabel );
    iKeypadNumLabelArray->AppendL( KDialerButton2NumLabel );
    iKeypadNumLabelArray->AppendL( KDialerButton3NumLabel );
    iKeypadNumLabelArray->AppendL( KDialerButton4NumLabel );
    iKeypadNumLabelArray->AppendL( KDialerButton5NumLabel );
    iKeypadNumLabelArray->AppendL( KDialerButton6NumLabel );
    iKeypadNumLabelArray->AppendL( KDialerButton7NumLabel );
    iKeypadNumLabelArray->AppendL( KDialerButton8NumLabel );
    iKeypadNumLabelArray->AppendL( KDialerButton9NumLabel );
    iKeypadNumLabelArray->AppendL( KDialerButton10NumLabel );
    iKeypadNumLabelArray->AppendL( KDialerButton11NumLabel );
    iKeypadNumLabelArray->AppendL( KDialerButton12NumLabel );
    
    // Convert num labels to current global number mode
    for ( TInt i = 0 ; i < iKeypadNumLabelArray->Count() ; ++i )
        {
        // A bit of magic is needed to get a modifiable pointer
        // descriptor which points to string within the array.
        TPtrC roPtr = (*iKeypadNumLabelArray)[i];
        TPtr ptr( const_cast<TUint16*>( roPtr.Ptr() ), roPtr.Length(), roPtr.Length() );
        
        AknTextUtils::LanguageSpecificNumberConversion( ptr );
        }
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::GetResourceFileNameL
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadLabelManager::GetResourceFileNameL( TDes& aFileName )
    {
    TFileName resName;

    iLabelScriptInUse = LabelScriptToUseL();
    // Try to get variated resource name if necessary
    if ( iLabelScriptInUse != EScriptLatin )
        {
        const TDesC& scriptName = ScriptName( static_cast<TScript>(iLabelScriptInUse) );
        resName.Copy( KDialerKeypadResourceFile );
        resName.Append( KDialerKeypadVariantExtSeparator );
        resName.Append( scriptName );
        resName.Append( KDialerKeypadResourceFileExt );
        
        BaflUtils::NearestLanguageFile( iCoeEnv.FsSession(), resName );
        if ( !BaflUtils::FileExists( iCoeEnv.FsSession(), resName ) )
            {
            // resource not found, fall back to Latin
            iLabelScriptInUse = EScriptLatin;
            }
        }
    
    // Get the default resource name if Latin script was selected or
    // resource file for the selected script was not found.
    if ( iLabelScriptInUse == EScriptLatin )
        {
        resName.Copy( KDialerKeypadResourceFile );
        resName.Append( KDialerKeypadResourceFileExt );
        BaflUtils::NearestLanguageFile( iCoeEnv.FsSession(), resName );
        }
    
    aFileName.Copy( resName );
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::LabelScriptToUseL
//
// ---------------------------------------------------------------------------
//
TInt CDialerKeyPadLabelManager::LabelScriptToUseL() const
    {
    TInt lang( ELangEnglish );
    
    // Get current input language
    TInt err = iAknFepRepository->Get( KAknFepInputTxtLang, lang );
    __ASSERT_DEBUG( !err, DialerPanic(err) );
            
    TInt script = ScriptForLang( static_cast<TLanguage>(lang) );
    
    // Distinct logic is used to select labels when current input language
    // is any version of Chinese. PCS uses different algorith for Chinese.
    if ( script == EScriptChinese )
        {
        script = ChineseScriptToUseL();
        }
    // If current input language is written in Latin script, then we check
    // if some other script of the device could also be shown on the labels.
    // Most variated labels include Latin characters as well.
    else if ( script == EScriptLatin )
        {
        RArray<TInt> availableLanguages;
        CleanupClosePushL( availableLanguages );
        iPtiEngine->GetAvailableLanguagesL( availableLanguages );
        
        // Scirpt of the current UI language is preferred option for 
        // alternative script. Ensure it's considered first.
        TLanguage uiLang = User::Language();
        TInt pos = availableLanguages.Find( uiLang );
        if ( pos != KErrNotFound )
            {
            availableLanguages.Remove( pos );
            availableLanguages.InsertL( uiLang, 0 );
            }
        
        TInt langCount = availableLanguages.Count();
        for ( TInt i = 0 ; i < langCount ; ++i )
            {
            TLanguage altLang = static_cast<TLanguage>( availableLanguages[i] );
            TScript altScript = ScriptForLang( altLang );
            if ( altScript != EScriptLatin && ScriptCanSubstituteLatin(altScript) )
                {
                // Suitable alternative script found. Use its labels
                // instead of showing only Latin alphabets.
                script = altScript;
                i = langCount; // to break out
                }
            }
        
        CleanupStack::PopAndDestroy( &availableLanguages );
        }
    
    return script;
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadLabelManager::ChineseScriptToUseL
//
// ---------------------------------------------------------------------------
//
TInt CDialerKeyPadLabelManager::ChineseScriptToUseL() const
    {
    TInt script( EScriptLatin );
    
    TInt lang( ELangPrcChinese );
    TInt chnSearchMode( 0 );
    
    TInt err = iAknFepRepository->Get( KAknFepInputTxtLang, lang );
    __ASSERT_DEBUG( !err, DialerPanic(EDialerPanicChineseScriptError) );
    
    switch ( lang )
        {
        case ELangPrcChinese:
            {
            err = iAvkonRepository->Get( KAknAdaptiveSearchChinesePRC, chnSearchMode );
            script = ( chnSearchMode == EChnPrcPinyin ? EScriptChnPinyin : EScriptChnStroke );
            }
            break;
        case ELangTaiwanChinese:
            {
            err = iAvkonRepository->Get( KAknAdaptiveSearchChineseTW, chnSearchMode );
            script = ( chnSearchMode == EChnTwZhuyin ? EScriptChnZhuyin : EScriptChnStroke );
            }
            break;
        case ELangHongKongChinese:
            {
            err = iAvkonRepository->Get( KAknAdaptiveSearchChineseHongkong, chnSearchMode );
            script = ( chnSearchMode == EChnHkStroke ? EScriptChnStroke : EScriptChnCangjie );
            }
            break;
        default:
            {
            err = KErrUnknown;
            }
            break;
        }
    __ASSERT_DEBUG( !err, DialerPanic(EDialerPanicChineseScriptError) );
    
    return script;
    }

// ---------------------------------------------------------------------------
// ShowSecondLabel
//
// ---------------------------------------------------------------------------
//
TBool CDialerKeyPadLabelManager::ShowSecondLabel() const
    {
    TLanguage lang = User::Language();
    TBool showSecondLabel;
    
    switch( lang )
        {
        case ELangLatvian:
        case ELangLithuanian:
        case ELangEstonian:
        case ELangCroatian:
        case ELangHungarian:  // Flow trough
            showSecondLabel = EFalse;
            break;
        default:
            showSecondLabel = ETrue;
            break;
        }
    
    return showSecondLabel;
    }


// LOCAL FUNCTION DEFINITIONS

// ---------------------------------------------------------------------------
// ScriptForLang
//
// ---------------------------------------------------------------------------
//
static TScript ScriptForLang( TLanguage aLang )
    {
    TScript script( EScriptLatin );
    for ( TInt i = 0 ; i < KScriptMapCount ; ++i )
        {
        if ( aLang == KScriptMap[i].iLang )
            {
            script = KScriptMap[i].iScript;
            i = KScriptMapCount; // to break out
            }
        }
    return script;
    }

// ---------------------------------------------------------------------------
// ScriptName
//
// ---------------------------------------------------------------------------
//
static const TDesC& ScriptName( TScript aScriptCode )
    {
    // Convert script ID to script name
    switch ( aScriptCode )
        {
        case EScriptLatin:
            return KScriptLatin;
        case EScriptCyrillic:
            return KScriptCyrillic;
        case EScriptGreek:
            return KScriptGreek;
        case EScriptHebrew:
            return KScriptHebrew;
        case EScriptArabic:
            return KScriptArabic;
        case EScriptFarsi:
            return KScriptFarsi;
        case EScriptUrdu:
            return KScriptUrdu;
        case EScriptThai:
            return KScriptThai;
        case EScriptDevanagari:
            return KScriptDevanagari;
        case EScriptChnPinyin:
            return KScriptChnPinyin;
        case EScriptChnZhuyin:
            return KScriptChnZhuyin;
        case EScriptChnStroke:
            return KScriptChnStroke;
        case EScriptChnCangjie:
            return KScriptChnCangjie;
        default:
            __ASSERT_DEBUG( EFalse, DialerPanic(EDialerPanicUnknownScript) );
        }
    return KNullDesC;
    }

// ---------------------------------------------------------------------------
// ScriptCanSubstituteLatin
// Return ETrue if labels of given script can be used instead of Latin labels
// when current input language is written with Latin script. This requires
// that the labels include also Latin characters and that labels don't contain
// any characters which would only make sense in some specific input mode.
// ---------------------------------------------------------------------------
//
static TBool ScriptCanSubstituteLatin( TScript aScript )
    {
    // It would be better to figure this out from the resources but that would
    // require opening and parsing several resource files.
    return !( aScript == EScriptArabic || 
              aScript == EScriptUrdu || 
              aScript == EScriptFarsi ||
              aScript == EScriptChinese ||
              aScript == EScriptChnPinyin ||
              aScript == EScriptChnZhuyin ||
              aScript == EScriptChnStroke ||
              aScript == EScriptChnCangjie );
    }

// end of file

