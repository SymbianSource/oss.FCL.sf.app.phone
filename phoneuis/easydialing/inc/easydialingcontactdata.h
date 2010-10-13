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
* Description:  Easy dialing contact data manager. 
*
*/


#ifndef __EASYDIALINGCONTACTDATA_H__
#define __EASYDIALINGCONTACTDATA_H__

class CFbsBitmap;
class MVPbkContactLink;

/**
 *  Container for easy dialing contact data for one contact.
 */
class CEasyDialingContactData : public CBase
    {
public:
    
    /**
    * Constructor.
    */
    CEasyDialingContactData(MVPbkContactLink* aContactLink);
    
    /**
     * Destructor.
     */
    ~CEasyDialingContactData();
    

    /**
     * Gets a pointer to contact link.
     * @return  Pointer to contact link object.
     */
    MVPbkContactLink* ContactLink();
    
    
    /**
     * Get's contact's thumbnail bitmap.
     * @return  Pointer to bitmap object.
     */
    CFbsBitmap* Thumbnail();
    
    
    /**
     * Setting function for contact thumbnail.
     * @param   aThumnail   Thumbnail to set.
     */
    void SetThumbnail(CFbsBitmap* aThumbnail);
    
    
    /**
     * Get favourite status of the contact.
     * @return  ETrue, if contact is a favourite, EFalse otherwise.
     */
    TBool Fav();
    
    
    /**
     * Sets favourite status of the contact.
     * @param   aFav    Boolean value containing the status.
     */
    void SetFav(TBool aFav);
    
    
    /**
     * Returns the loading status of the contact.
     * @return  ETrue, if contact data loading has completed, EFalse otherwise.
     */
    TBool IsLoaded();
    
    
    /**
     * Sets loading status to complete.
     */
    void LoadingComplete();

    
    /**
     * Deletes the thumbnail image of the contact.
     */
    void DeleteThumbnail();
    
    
    /**
     * Return ETrue if voice call is available.
     */
    TBool VoiceCallAvailable();
    
    
    /**
     * Sets voice call availability status.
     * @param   aVailable   Availability of voice call.
     */
    void SetVoiceCallAvailable( TBool aVailable );
    
    
    /**
     * Return ETrue if video call is available.
     */
    TBool VideoCallAvailable();
    
    
    /**
     * Sets video call availability status.
     * @param   aVailable   Availability of video call.
     */
    void SetVideoCallAvailable( TBool aVailable );
    
    
    /**
     * Return ETrue if uni-editor (SMS, MMS, e-mail) is available.
     */
    TBool UniEditorAvailable();
    
    
    /**
     * Sets uni-editor availability status.
     * @param   aVailable   Availability of uni-editor.
     */
    void SetUniEditorAvailable( TBool aVailable );
    
private:
    
    /** Contact link. Owned. */
    MVPbkContactLink* iContactLink;
    
    /** Thumbnail bitmap. Owned. */
    CFbsBitmap* iThumbnail;

    /** ETrue if this is a favourite contact. */
    TBool iFav;
       
    /** ETrue is asynchronous loading of contact data is complete.  */
    TBool iLoaded;
       
    /** ETrue if voice call is a possible action with this contact. */
    TBool iVoiceCallAvailable;
    
    /** ETrue if video call is a possible action with this contact. */
    TBool iVideoCallAvailable;

    /** ETrue if SMS or MMS is a possible action with this contact. */
    TBool iUniEditorAvailable;
    };

#endif //__EASYDIALINGCONTACTDATA_H__
