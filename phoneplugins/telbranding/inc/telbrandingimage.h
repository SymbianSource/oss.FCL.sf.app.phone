/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Branding plug-in specific image.
*
*/

#ifndef C_CTELBRANDINGIMAGE_H
#define C_CTELBRANDINGIMAGE_H

#include <telbubbleimage.h>

/**
 *  Used to handle CTelBrandingImage visibility.
 *  Only one image is shown at one time according priority
 *
 *  @since S60 S60 v5.0
 */
NONSHARABLE_CLASS( MTelBrandingPriorityHandler )
    {
public:
    virtual void SetVisibilityAllowed( TBool aAllowed ) = 0;
    
    };

/**
 *  Branding plug-in specific image.
 *  Uses MTelBrandingPriorityHandler to handle visibility.
 *  Only one image is shown at one time according priority
 *
 *  @since S60 S60 v5.0
 */
NONSHARABLE_CLASS( CTelBrandingImage ): 
    public CTelBubbleImage,
    public MTelBrandingPriorityHandler
    {
public:
    /**
    * Constructors.
    */
    static CTelBrandingImage* NewL();
    static CTelBrandingImage* NewLC();

    /**
    * Destructor.
    */
    virtual ~CTelBrandingImage();

public: // From CCoeControl
    void MakeVisible( TBool aVisible );
    
public: // From MTelBrandingPriorityHandler
    void SetVisibilityAllowed( TBool aAllowed );
    
public: // New functions
    void SetLowerPriorityBranding( MTelBrandingPriorityHandler* aHandler );
    
private:
    CTelBrandingImage();

    void ConstructL();

private: // data
    // Specifies that is visiblity allowed
    TBool iVisibilityAllowed;
    
    // Specifies lower priority image
    MTelBrandingPriorityHandler* iLowerPriority;
    };

#endif // C_CTELBRANDINGIMAGE_H
