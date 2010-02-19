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
* Description:  This is a class for starting services
*
*/

#ifndef __EDCONTACTORSERVICE_H__
#define __EDCONTACTORSERVICE_H__

// INCLUDES
#include <e32std.h>
#include <f32file.h>
#include <VPbkFieldTypeSelectorFactory.h>

//FORWARD DECLARATIONS

class CEDContactorServiceOperator;
class MEDContactorObserver;


/**
 *  CEDContactorService
 *
 *  Class for starting services of CCA contactor service.
 *  @code
 *      HBufC* fullName = GetFullNameLC();
 *      CVPbkContactLinkArray* linkArray = CVPbkContactLinkArray::NewLC();
 *      linkArray->AppendL(iContactLink);
 *      HBufC8* streamedArray = linkArray->PackLC();
 *
 *      //Set control flags.
 *      TUint flags;
 *      flags |= CEDContactorService::TCSParameter::EEnableDefaults;
 *
 *      CEDContactorService::TCSParameter params(VPbkFieldTypeSelectorFactory::EVoiceCallSelector, *streamedArray, flags, *fullName);
 *      CEDContactorService::ExecuteServiceL(params);
 *
 *      CleanupStack::PopAndDestroy(3); //linkArray, streamedArray, fullName
 *
 *  @endcode
 *
 *  @lib CEDContactorService
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(CEDContactorService) : public CBase
    {
public:

    /**
     * Parameter class for the service.
     *
     * @since S60 v5.0
     */
    class TCSParameter
        {
        public:

        /** Flags for controlling CCA Contactor Service */
        enum TControlFlags
            {
            /**
             * Enable defaults. If this is set, AIW popup query will automatically search possible
             * default settings and if found one, it doesn't show selection query at all.
             */
            EEnableDefaults  = 0x0001,
            
            /**
             * Execute service as forced. If this is set, then voice call will be
             * executed as forced cs voice call. Doesn't affect other services currently.
             */
            EForcedService = 0x0002
            };

        /*
         * Constructor
         */
        inline TCSParameter(
                VPbkFieldTypeSelectorFactory::TVPbkContactActionTypeSelector aCommTypeSelector,
                TDesC8& aContactLinkArray, TUint aControlFlag, TDesC& aFullName) :
                iCommTypeSelector(aCommTypeSelector),
                iContactLinkArray(aContactLinkArray),
                iControlFlag(aControlFlag),
                iFullName(aFullName)
            {
            };

         /*
         * Enumeration for inticating which communication method is wanted to use.
         */
        VPbkFieldTypeSelectorFactory::TVPbkContactActionTypeSelector iCommTypeSelector;

        /*
         * Streamed contact link array of contact who will be communicated at.
         */
        TDesC8& iContactLinkArray;

        /**
         * Flag for controlling behaviour of service. Currently only set of default 
         * and forced service supported.
         */
        TUint iControlFlag;

        /*
         * Some communication methods requires contacts name order to use services.
         * (for example mms/sms editor)
         */
        TDesC& iFullName;
        };

    /**
     *
     */
    IMPORT_C static CEDContactorService* NewL( MEDContactorObserver* aObserver );

    /**
     * Destructor.
     */
    virtual ~CEDContactorService();

    /**
     * Start communication service.
     *
     * If the contactlink contains a multiple contact fields for selected communication
     * method, the executed service will launch Phbk AIW selection popup. User have to select
     * wanted field to be used for communication.
     * The communication method is defined by TVPbkCommTypeSelector.
     * After selection, communication is automatically started by the service.
     *
     * @see TVPbkCommTypeSelector from VPbkFieldTypeSelectorFactory.h.
     * Leaves KErrArgument if given parameter doesn't have valid values.
     *
     * @since S60 5.0
     * @param aParameter: The Launch Parameter.
     */
    IMPORT_C void ExecuteServiceL(const TCSParameter& aParameter);

    /**
     * Cancels an ongoing service execution.
     */
    IMPORT_C void CancelService();
    
    /**
     * Is contactor service busy.
     */
    IMPORT_C TBool IsBusy() const;

private:
    /**
     * Constructor.
     */
    CEDContactorService( MEDContactorObserver* aObserver );

    /**
     *
     */
    void ConstructL();

private:
    // data

    /**
     *
     */
    CEDContactorServiceOperator* iOperator;
    
    /**
     * ETrue, if command is being handled at the moment
     */
    TBool iCommandIsBeingHandled;
    
    TInt iResourceFileOffset;
    
    MEDContactorObserver* iObserver;
    };

#endif      //__EDCONTACTORSERVICE_H__

// End of File
