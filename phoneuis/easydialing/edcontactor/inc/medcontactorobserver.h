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
* Description:  Class implementing the abstract contactor observer class.
*
*/

#ifndef MEDCONTACTOROBSERVER_H_
#define MEDCONTACTOROBSERVER_H_

/**
 * MEDContactorObserver
 * 
 * Observer class for contactor module.
 * 
 * 
 * 
 */

class MEDContactorObserver 
    {
    
public:
    
    /*
     * Enum for communication events.
     */
    typedef enum TEvent_enum
        {
        /*
         * ECommunicationStarted
         * Used when communication was normally started.
         */
        ECommunicationStarted = 1,
        
        /*
         * ECommunicationCancelled
         * Used when
         * 1. User cancelled communication when choosing phone number, email, etc.
         * 2. When there was no appropriate field in the contact.
         */
        ECommunicationCancelled
        
        } TEvent;
    
    virtual void InformContactorEvent( TEvent aEvent ) = 0;
    };



#endif /* MEDCONTACTOROBSERVER_H_ */
