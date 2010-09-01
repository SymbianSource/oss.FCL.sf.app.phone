/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies)
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
* Description:  Ctelbubbleimagescaler declaration
*
*/

#ifndef TELBUBBLEIMAGESCALER_H
#define TELBUBBLEIMAGESCALER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CFbsBitmap;
class CBitmapScaler;

/**
 *  Interface to observe scaling status
 */
class MTelBubbleImageScalerObserver
{
public:
    /**
     * Callback to indicate scaling completion
     * 
     * @param aError Error occured during scaling
     * @param aBitmap Scaled bitmap
     */
    virtual void ImageScalingComplete
                (TInt aError, CFbsBitmap* aBitmap) = 0;
    };

/**
 *  Asynchronous bitmap scaler.
 *  Uses CBitmapScaler to do scaling
 */
class CTelBubbleImageScaler : public CActive
{
public:
    // Cancel and destroy
    ~CTelBubbleImageScaler();

    /** Two phase constructor */
    static CTelBubbleImageScaler* NewL(MTelBubbleImageScalerObserver& aObserver);

    /** Two phase constructor */
    static CTelBubbleImageScaler* NewLC(MTelBubbleImageScalerObserver& aObserver);
    
    /**
     *  Scaling status
     */
    enum TState
        {
        EScalingIdle = 0, EScalingStarted, EScalingDone
        };

public:
    /**
     * Starts bitmap scaling Asynchronously 
     * @param aSrc Bitmap that needs to be scaled
     * @param aTarget Scaled bitmap
     * @return None.
     */
    void StartScaleL(CFbsBitmap* aSrc, CFbsBitmap* aTarget);
    
    /**
     * Sets the state of Scaler 
     * @param aState State to be set
     * @return None.
     */
    void SetState(TState aState);
    
    /**
     * Returns the current state of Scaler 
     * @param None
     * @return TInt State of the scaler
     */   
    TInt GetState();

private:
    /* constructor */
    CTelBubbleImageScaler(MTelBubbleImageScalerObserver& aObserver);

    /* Second-phase constructor */
    void ConstructL();

private: // From CActive
    /*
     * Handle completion
     */
    void RunL();

    /*
     * Cancels active request
     */
    void DoCancel();

    /*
     * Override to handle leaves from RunL(). Default implementation causes
     * the active scheduler to panic.
     */
    TInt RunError(TInt aError);

private:
    /* 
     * Scaler observer instance 
     */
    MTelBubbleImageScalerObserver& iObserver;
    
    /*
     * Own: Bitmap scaler
     */
    CBitmapScaler* iScaler;
    
    /* 
     * Own: The bitmap that is scaled.
     */
    CFbsBitmap* iScaledBitmap;

    /*
     * Scaler state
     */
    TInt iState;
    };

#endif // TELBUBBLEIMAGESCALER_H

// End of File

