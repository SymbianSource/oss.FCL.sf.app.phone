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
* Description:  Easy dialing test application.
*
*/



#include <cntdb.h> 
#include <cntitem.h> 
#include <cntfldst.h> 
#include <cpbkcontactitem.h>
#include <cpbkcontactengine.h>
#include "edta_debugprint.h"


/*
 * Number of contacts in contactdata array.
 */
const TInt nbr_of_contacts = 15;

/*
 * Contact data contains contact data as text strings.
 * Each contact takes six line:
 * - first name
 * - last name
 * - cell number
 * - home number
 * - company name
 * - e-mail address
 */
const char* contactdata[] = 
{
    "Dummy",
    "Contact",
    "+1056439939",
    "035558756",
    "Ericsson",
    "dummy.contact@ericsson.sw",
    
    "Jaakko",
    "Helanti",
    "+358405287761",
    "034447611",
    "Nokia",
    "jaakko.helanti@nokia.com",
    
    "Stan",
    "Laurel",
    "+0104438839992",
    "+0104438839000",
    "MGM",
    "stan.laurel@heaven.com",
    
    "Oliver",
    "Hardy",
    "+01078398927",
    "+10364537882",
    "MGM",
    "oliver.hardy@heaven.com",
    
    "Harold",
    "Lloyd",
    "+102843947",
    "+10290503958",
    "Universal",
    "harold.lloyd@universal.com",
    
    "Tauno",
    "Palo",
    "+358405557631",
    "014445466",
    "Suomi Filmi",
    "tauno.palo@gmail.com",
    
    "Tea",
    "Ista",
    "+358475554590",
    "018723195",
    "Helsingin Kaupungin Teatteri",
    "tea.ista@cultureclub.com",

    "Ansa",
    "Ikonen",
    "+358471134099",
    "012388192",
    "Suomi Filmi",
    "ansa.ikonen@gmail.com",

    "Bertil",
    "Nystrom",
    "+35866377334",
    "074500998",
    "Rovaniemen Viuluveistämö",
    "bertil.nystrom@rvv.com",
        
    "",
    "Onlylastname",
    "+3585556743",
    "+35835552727",
    "",
    "",
    
    "Onlyfirstname",
    "",
    "+358405559874",
    "035553747",
    "",
    "",
       
    "",
    "Two-Part Lastname",
    "+358405557432",
    "",
    "",
    "",
    
    "",
    "",
    "+35866377334",
    "074500998",
    "Just Companyname",
    "john.doe@justcompanyname.com",
        
    "One",
    "Number",
    "",
    "075553266",
    "Idaho",
    "",
    
    "No",
    "Number",
    "",
    "",
    "Ericsson",
    "no.number@ericsson.com",    
};


HBufC* CreateFieldStringL(TInt aIx)
    {
    const char* str = contactdata[aIx];
    TInt len = 0;
    
    while (str[len] != '\0')
        {
        len++;
        }
    
    if ( len == 0 )
        {
        return NULL;
        }
    
    HBufC *field = HBufC::NewL( len );
    TPtr des = field->Des();
    des.SetLength( len );
    
    for (TInt i = 0; i < len; i++)
        {
        des[i] = (TChar) str[i];
        }
    
    return field;
    }


void AddFieldStringL( CPbkContactItem* aContact, TInt aIx, TInt aFieldId )
    {
    HBufC* fieldName = CreateFieldStringL( aIx );
    
    if ( fieldName ) 
        {
        CleanupStack::PushL( fieldName );
        aContact->FindField( aFieldId )->TextStorage()->SetTextL( *fieldName );
        CleanupStack::PopAndDestroy(fieldName); 
        }
    }


void AddContactL(CPbkContactEngine* engine, TInt aIx)
    {
    CPbkContactItem* contact = engine->CreateEmptyContactL();
    CleanupStack::PushL( contact );
    
    AddFieldStringL( contact, aIx * 6 + 0, EPbkFieldIdFirstName );
    AddFieldStringL( contact, aIx * 6 + 1, EPbkFieldIdLastName );
    AddFieldStringL( contact, aIx * 6 + 2, EPbkFieldIdPhoneNumberMobile );
    AddFieldStringL( contact, aIx * 6 + 3, EPbkFieldIdPhoneNumberGeneral );
    AddFieldStringL( contact, aIx * 6 + 4, EPbkFieldIdCompanyName );
    AddFieldStringL( contact, aIx * 6 + 5, EPbkFieldIdEmailAddress );
        
    engine->AddNewContactL( *contact );
    
    CleanupStack::PopAndDestroy( contact );
    }



// ---------------------------------------------------------
// CreateTestContactDatabaseL
//
// Creates a test contact database if it hasn't been created yet.
// The existence of test base is done through testing if there
// is a contact matching with word "Dummy". Test data base has
// a contact for Mr Dummy Contact.
// This is the primary function of this file. 
// ---------------------------------------------------------
//
TInt CreateTestContactDatabaseL()
    {
    DebugPrintF(_L("CreateTestContactDatabaseL: Contacts creation started.."));

    CPbkContactEngine* engine = CPbkContactEngine::NewL();
    CleanupStack::PushL( engine );
    
    _LIT(KDummyContact, "Dummy");
        
    CContactIdArray* findings = engine->FindLC( KDummyContact );
    
    // If dummy contact is found, the test database has already been created. 
    if (findings->Count() > 0)
        {
        CleanupStack::PopAndDestroy( findings );
        CleanupStack::PopAndDestroy( engine );
        DebugPrintF(_L("CreateTestContactDatabaseL: Database already set up"));
        return 0;
        }
    
    CleanupStack::PopAndDestroy( findings );

    for (TInt i = 0; i < nbr_of_contacts; i++ )
        {
        AddContactL( engine, i );
        }
    
    CleanupStack::PopAndDestroy(engine);

    DebugPrintF(_L("CreateTestContactDatabaseL: Contacts creation completed, number of contacts: %d"), nbr_of_contacts );

    return nbr_of_contacts;
    }












