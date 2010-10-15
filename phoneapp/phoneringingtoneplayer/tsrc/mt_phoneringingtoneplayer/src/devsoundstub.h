
#ifndef DEVSOUNDSTUB_H
#define DEVSOUNDSTUB_H

// Define tone & sample players, so the original headers won't get included afterwards.
#define __MDAAUDIOTONEPLAYER_H__

#define __MDAAUDIOSAMPLEPLAYER_H


#include <e32base.h>
#include <e32std.h>


// Priority from devsound
enum TMdaPriority
     {
    EMdaPriorityMin = -100,
    EMdaPriorityNormal = 0,
    EMdaPriorityMax = 100
    };


// Preference from devsound 
enum TMdaPriorityPreference    
    {
    EMdaPriorityPreferenceNone =    0,
    EMdaPriorityPreferenceTime =    1,
    EMdaPriorityPreferenceQuality = 2,
    EMdaPriorityPreferenceTimeAndQuality = 3
    };

// Mda server stub definition.
class CMdaServer : public CBase
    {
public:
    static CMdaServer* NewL();
    ~CMdaServer();
    };

// Function call test data. These objects should be compared to define the test result. Devsound stub registers function call data in these objects.
class CTestFunctionCallData : public CBase
    {
public:
    static CTestFunctionCallData* NewL( TInt aFunctionIndex, HBufC* aString, TInt aInt1, TInt aInt2 );
    static CTestFunctionCallData* NewL( TInt aFunctionIndex, TInt aInt1, TInt aInt2 );
    static CTestFunctionCallData* NewL( TInt aFunctionIndex, TInt aInt1 = 0 );
    static CTestFunctionCallData* NewL( TInt aFunctionIndex, TInt aInt1, TTimeIntervalMicroSeconds aSeconds );
    static CTestFunctionCallData* NewL( TInt aFunctionIndex, TTimeIntervalMicroSeconds aSeconds );
    static CTestFunctionCallData* NewL( TInt aFunctionIndex, HBufC8* aString );
    
    ~CTestFunctionCallData();

    CTestFunctionCallData( TInt aFunctionIndex, HBufC* aPtr1, TInt aInt1, TInt aInt2, TTimeIntervalMicroSeconds aSeconds );
    
    CTestFunctionCallData( TInt aFunctionIndex, HBufC8* aString );
    
    HBufC* DesC1();
    HBufC8* DesC2();
    TInt Int1();
    TInt Int2();
    TTimeIntervalMicroSeconds Seconds();
    
    TInt FunctionIndex();
    
    TBool operator ==( CTestFunctionCallData& aData );

private:

    TInt iFunctionIndex;
    HBufC* iP1;
    TInt iP2;
    TInt iP3;
    TTimeIntervalMicroSeconds iP4;
    HBufC8* iP5;

    };


// M-interface from devsound for callbacks.
class MMdaAudioToneObserver
    {
public:
    virtual void MatoPrepareComplete(TInt aError) = 0;
    virtual void MatoPlayComplete(TInt aError) = 0;
    };


// M-interface from devsound for callbacks.
class MMdaAudioPlayerCallback
  {
public:
    virtual void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration) = 0;
    virtual void MapcPlayComplete(TInt aError) = 0;
    };


// Audio player utility stub. All imperative function calls are stored into iTestData static pointer array.
class CMdaAudioPlayerUtility : public CActive
    {
public:
    
    // function call index. 
    enum TFunctionCall
        {
        ENewFilePlayerL,
        ENewL,
        ESetVolume,
        ESetRepeats,
        ESetVolumeRamp,
        EMaxVolume,
        EGetVolume,
        EOpenDesL,
        EPlay,
        EStop
        };
    
    static CMdaAudioPlayerUtility* NewFilePlayerL(const TDesC& aFileName,
        MMdaAudioPlayerCallback& aCallback,
        TInt aPriority = EMdaPriorityNormal,
        TInt aPref = EMdaPriorityPreferenceTimeAndQuality,
        CMdaServer* aServer = NULL );
    static CMdaAudioPlayerUtility* NewL( MMdaAudioPlayerCallback& aCallback,
        TInt aPriority = EMdaPriorityNormal,
        TInt aPref = EMdaPriorityPreferenceTimeAndQuality );
    CMdaAudioPlayerUtility(MMdaAudioPlayerCallback& aObserver, TInt aPriority, TInt aPref, CMdaServer* aServer );
    ~CMdaAudioPlayerUtility();
    TInt SetVolume(TInt aVolume );
    void SetRepeats(TInt aRepeatNumberOfTimes, const TTimeIntervalMicroSeconds& aTrailingSilence);
    void SetVolumeRamp(const TTimeIntervalMicroSeconds& aRampDuration);
    TInt MaxVolume();
    TInt GetVolume(TInt& aVolume);
    void OpenDesL(const TDesC8& aDescriptor);
    
    void DoCancel();
    void RunL();
    
    void Play();
    void Stop();
    void Close();
    
    
    // Static access to test data. If used, stub object must be allocated from the same memory space as the client.
    static RPointerArray< CTestFunctionCallData >& TestData();
    static void DelayAudioLoading();
    static void SimulateErrorInAudioLoading();
    static void ResetTestData();

private:

    void ConstructL( const TDesC& aFileName );

    void ConstructL( const TDesC8& aFileName );
private:

    MMdaAudioPlayerCallback& iObserver;
    TInt iVolume;
    TInt iMaxVolume;
    TInt iPriority;
    TInt iPref;
    CMdaServer* iServer;
    HBufC* iFile;
    HBufC8* iFile8;

    RTimer iTimer;

    static RPointerArray< CTestFunctionCallData > iStaticTestData;
    static bool iDelayedAudioLoading;
    static bool iErrorInAudioLoading;

    };



// The state of the audio tone utility.
 
 enum TMdaAudioToneUtilityState
     {
     /** The audio tone utility has not been prepared to play a tone.
     */
     EMdaAudioToneUtilityNotReady=0,
     /** The audio tone utility has been prepared to play a tone and is ready.
     */
     EMdaAudioToneUtilityPrepared,
     /** The audio tone utility is currently playing a tone.
     */
     EMdaAudioToneUtilityPlaying,
     /** The audio tone utility is currently paused.
     This state is only reached if underlying DevSound implementations supports resume.
     */
     EMdaAudioToneUtilityPaused
     };

// Audio tone player utility stub. All imperative function calls are stored into iTestData static pointer array.
class CMdaAudioToneUtility : public CActive
    {
public:
    
    
    // function call index. 
    enum TFunctionCall
        {
        ENewL,
        EMaxVolume,
        EVolume,
        ESetVolume,
        ESetRepeats,
        ESetVolumeRamp,
        EFixedSequenceCount,
        EFixedSequenceName,
        EPrepareToPlayTone,
        EPrepareToPlayDTMFString,
        EPrepareToPlayDesSequence,
        EPrepareToPlayFileSequence,
        EPrepareToPlayFixedSequence,
        ECancelPrepare,
        ECancelPlay,
        EPlay,
        EStop
        };  
        
    static CMdaAudioToneUtility* NewL( MMdaAudioToneObserver& aObserver, 
        CMdaServer* aServer = NULL,
        TInt aPriority = EMdaPriorityNormal, 
        TInt aPref = EMdaPriorityPreferenceTimeAndQuality );
    
    
    CMdaAudioToneUtility( MMdaAudioToneObserver& aObserver, 
                          TInt aPriority, TInt aPref, CMdaServer* aServer );
    ~CMdaAudioToneUtility();
    virtual TInt MaxVolume();
    virtual TInt Volume();
    virtual void SetVolume(TInt aVolume );
    virtual void SetRepeats(TInt aRepeatNumberOfTimes, 
                            const TTimeIntervalMicroSeconds& aTrailingSilence );
    virtual void SetVolumeRamp(const TTimeIntervalMicroSeconds& aRampDuration );
    virtual TInt FixedSequenceCount();
    virtual const TDesC& FixedSequenceName(TInt aSequenceNumber );
    virtual void PrepareToPlayTone(TInt aFrequency, 
                                   const TTimeIntervalMicroSeconds& aDuration );
    virtual void PrepareToPlayDTMFString(const TDesC& aDTMF);
    virtual void PrepareToPlayDesSequence(const TDesC8& aSequence);
    virtual void PrepareToPlayFileSequence(const TDesC& aFileName);
    virtual void PrepareToPlayFixedSequence(TInt aSequenceNumber);
    virtual void CancelPrepare();
    virtual void Play();
    virtual TMdaAudioToneUtilityState State();
    virtual void CancelPlay();

    // Static access to test data. If used, stub object must be allocated from the same memory space as the client.
    static RPointerArray< CTestFunctionCallData >& TestData();
    static void DelayAudioLoading();
    static void SimulateErrorInAudioLoading();
    static void ResetTestData();
    
    
    void DoCancel();
    
    void RunL();
    
    void Stop();
   

private:
    
    void ConstructL();
    
private:
    
    MMdaAudioToneObserver& iObserver;
    TInt iVolume;
    TInt iMaxVolume;
    TInt iPriority;
    TInt iPref;
    CMdaServer* iServer;
    HBufC8* iFile8;

    
    RTimer iTimer;

    
    static RPointerArray< CTestFunctionCallData > iStaticTestData;
    static bool iDelayedAudioLoading;
    static bool iErrorInAudioLoading;
    
    };

#endif // DEVSOUNDSTUB_H

// End of file
