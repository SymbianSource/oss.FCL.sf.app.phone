/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  It provides voice/video mailbox related functionality.
*
*/


#ifndef CPHONEVMBXGHANDLER_H
#define CPHONEVMBXGHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <vmnumber.h>
 
// CLASS DECLARATION
class CPhApEVmbxMonitor; 
/**
*  Engine for voice/video mailbox
*
*/
class CPhoneVmbxHandler :public CBase
    {
    public:  // Constructors and destructor
     
        static CPhoneVmbxHandler* NewL(); 
             
        
        /**
        * Destructor.
        */
        virtual ~CPhoneVmbxHandler();

    public: // New functions
 
        /**
        * Checks value of CR key KTelVideoMbxKey. 
        * @return True if key 2 is assigned for videomailbox.
        */
        TBool IsStraightCallToVideoMailBox() const;  
        
        /**
        * Checks value of CR key KVideoMbxSupport. 
        * @return ETrue if Videomailbox configured.
        */        
        TBool IsVideoMailBoxConfigured() const;
  
        /**
        * Perform speed dialling, either by speed dialling or one key dialling
        * @param aSpdLocation the speed dialling location
        * @param aNumber The number stored in the location
        * @return error code
        */
        TInt GetStraightSpeedDialNumber( TInt aSpdLocation, 
            TDes& aNumber );
    
        /**
        * Get type of mailbox
        */
        TInt GetMailboxType();
        
        /**
        * Launches the mailbox.
        *
        * @param aMailboxType Voice, Video or Voip mailbox.               
        * @param aNumber phonenumber.        
        * @return error code.
		* @since 3.2
        */            
        TInt LaunchMbx( TInt aMailboxType, TDes& aNumber ); 
        
        
       /**
        * Launches the mailbox type query.
        *        
        * @return select mail box type.
        * @param serviceId Contains the selected serviceId is on return
        * @since 3.2
        */ 
        TInt SelectMailboxTypeL( TUint& aServiceId );
        
        /**
         * Cancels the mailbox query.
         *        
         * @since 5.0
         */         
        void CancelVmbx();

    private:

        /**
        * C++ constructor.
        */
        CPhoneVmbxHandler();
    
        /**
        * Symbian OS constructor.
        */
        void ConstructL();
        
    private: // new functions
    
        /**
        * Voice mailbox.
        *      
        * @param aNumber phonenumber.           
        * @return error code.
		* @since 3.2
        */           
        TInt CSVmbxL( TDes& aNumber );

        /**
        * VoIP mailbox.
        *
        * @param aNumber phonenumber.        
        * @return error code.
		* @since 3.2
        */                        
        TInt IPVmbxL( TDes& aNumber ); 
                    
        /**
        * Video mailbox.
        *
        * @param aNumber phonenumber.        
        * @return error code.
		* @since 3.2
        */                        
        TInt VideoVmbxL( TDes& aNumber );   
                               
        /**
        * Returns current networkMode.
        *
        * @return Network mode.
		* @since 3.2
        */                                 
        TInt NetworkMode();                       

        /**
        * Is CSVideoTelephony feature enabled.
        *
        * @return ETrue is feature enabled otherwise EFalse.
		* @since 3.2
        */ 
        TBool IsCsVideoTelFeature();
        
        /**
        * Check if location is videombx '2' and videombx is configured.
        *
        * @param speeddialing location
        * @return ETrue is location is '2' and videoMbx configured
		* otherwise EFalse.
		* @since 3.2
        */ 
        TBool IsVideoMbxLocationAndConfigured( TInt aSpdLocation ) const;
        
         /**
        * Construct vmbx connection
        */
        void CreateVmbxL();
        
    private:    // Data
          
        //Indicates if CSVideoTelephony feature is enabled.
        TBool iCsVideoTelFeature;
        
        // Mailbox
        RVmbxNumber* iVmbx;
         
        // Type of mailbox 
        TInt iMailboxType;

        // ServiceId
        TUint iServiceId;
    };

#endif      // CPHONEVMBXGHANDLER_H   
            
// End of File
