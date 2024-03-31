// Copyright (c) 2018 Jangoo US - https://www.jangoo.us

#ifndef VIDEO_INTERFACE_ALGORITHM_PROXY_H
#define VIDEO_INTERFACE_ALGORITHM_PROXY_H

#include <atomic>
#include <cstdint>
#include <mutex>
#include <thread>

#include "FeatureApplicationBase.hpp"
#include "BufferListener.hpp"
#include "CommunicationDefs.hpp" 
#include "CommonMessageHandler.hpp"
#include "RADSInterSystemMessages.hpp"
#include "RADSInterApplicationTypes.hpp"
#include "RADSStdFloat.hpp"
#include "FunctionBuffer.hpp"
#include "Timer.hpp"
#include "RADSInternalMessages.hpp"
#include "ISPFrameInfoSharedMemory.hpp"

#ifdef QNX
#include "VideoBufferManager.hpp"
#ifdef DEVELOPMENT
// Allow Hostsim for QNX development builds if configured
#include "BufferManager.hpp"
#endif // DEVELOPMENT
#else
// LINUX x86-Builds
#include "BufferManager.hpp"
#endif // QNX

namespace RADS
{

class VideoInterfaceAlgorithmProxy;

struct VideoInterfaceInputAggregate_t : public CommonMessageBase_t
{
    // received from consumers
    // RADS::ImageSupport::ImageReadComplete_t imageReadACK{};
    RADS::ImageSupport::ImageFrame_t imgFrame {};

    // received video frame data
    std::vector<uint8_t> imageData{};

    // request to register/unregister video consumer
    RADS::VideoConsumerRegister_t videoConsumerRegistry{};

    // Message ID 16 - Camera Support Data
    RADSInterSystemMessages::CameraSupport_t cameraSupportData{};

    // status of all apps
    RADS::SystemApplicationStatus_t systemApplicationStatus{};

    VideoInterfaceInputAggregate_t()
        : CommonMessageBase_t{}
    {
    }
};

/// The VideoInterfaceTask class provides a derived class to run timed
/// VideoInterfaceAlgorithmProxy callbacks
class VideoInterfaceTask : public RADS::Task
{
  public:
    VideoInterfaceAlgorithmProxy* mOwnerPtr = nullptr; /// pointer to algorithm to call logging

    /// Constructor that sets mOwnerPtr
    /// @param[in] videoInterfaceTaskOwner *this from the VideoInterfaceAlgorithmProxy that owns this task
    /// @return Returns nothing
    /// @exception none
    explicit VideoInterfaceTask(VideoInterfaceAlgorithmProxy& videoInterfaceTaskOwner);

  protected:
    /// Callback function that is called whenever the timer expires
    /// @return Returns void
    /// @exception none
    virtual void run() override; // NOLINT - satisfy MISRA for 'virtual' keyword
};

class VideoInterfaceAlgorithmProxy : public FeatureApplicationBase<VideoInterfaceInputAggregate_t>,
                                     public BufferListener
{

    bool               mIsInitialized{false};
    bool               mLiveVideo{};
    uint32_t           mRestartGlobalRecoveryTrials{};
    float64_t          mCameraFramesRate_fps{};
    uint32_t           mVideoInterfaceLogTimeInterval_ms{};
    RADS::Timer         mVideoInterfaceLoggingTimer; // Used to time logging interval
    VideoInterfaceTask mVideoInterfaceLoggingTask{*this};

    // default to RVC. Future work should switch cameras at runtime.
    // CameraIds_e mSelectedCameraId = CameraIds_e::AR0143REAR;

    // default to UNKNOWN until we receive CameraSupport_t packets
    //  used for live video only
    // CameraStatus_e mCurrentCameraStatus = CameraStatus_e::UNKNOWN; ///< Stores the latest value of rvccameraonstatus
    // CameraStatus_e mPrevCameraStatus    = CameraStatus_e::UNKNOWN; ///< Stores the previous value of rvccameraonstatus

    uint32_t mNumberOfRegisteredConsumers{0U}; ///< Stores the number of registered consumers
    uint32_t mPrevNumberOfRegisteredConsumers{0U};
    ///< Stores the number of registered consumers prior to register/unregister call

    // default to IDLE until consumers register
    // used for live video only
    // VisionOperationMode_e    mVisionOperationMode = VisionOperationMode_e::STOPPED_IDLE_NO_CONSUMERS;
    // VisionPipelineController mVisionPipelineController; ///< Controller to start/stop vision pipeline based on camera
                                                        ///< status and consumer status

    std::mutex mProxyMutex;
    bool               mCaptureFrame = false;

    std::thread                          mRunThread;
    std::thread                          mRunThreadCommand;


// // polyspace +1 MISRA-CPP:16-2-1 [Justified:Low] "Code separation for QNX and LINUX is necessary"
// #ifdef QNX
//     VideoBufferManager mVideoBufferManager; // live video buffer management (PRODUCTION or DEVELOPMENT)

// #if DEVELOPMENT
//     BufferManager mBufferManager; // Hostsim on QNX DEVELOPMENT if configured
// #endif

// #else
//     // LINUX X86-Builds (recorded video buffer management)
//     BufferManager mBufferManager;
// #endif // #ifdef QNX

    // until camera support message implements signal for frames rate(day or night), this will be used
    // to track the estimated frames rate (fps) and sent out to video consumers as part of ImageReady_t message
    float32_t mEstimatedFramesRate_fps{DAY_CAMERA_FRAMES_RATE_FPS}; // default to 30 fps

    // polyspace +1 MISRA-CPP:9-3-3 [Not a defect:Low] "mutex used"
    // VisionOperationMode_e getVisionOperationMode();
    // VisionOperationMode_e setVisionOperationMode(VisionOperationMode_e visionMode);

    /// Sets the mCurrentCameraStatus
    /// <br>Errors: None
    /// @param[in] cameraStatus CameraStatus_e camera status to be set
    /// @return CameraStatus_e The previous camera status before it was set
    /// @exception None
    // CameraStatus_e setCameraStatus(CameraStatus_e cameraStatus);

    /// Gets the mCurrentCameraStatus
    /// <br>Errors: None
    /// @param[in] None
    /// @return CameraStatus_e current value of mCurrentCameraStatus
    /// @exception None
    // virtual CameraStatus_e getCameraStatus() override; // NOLINT -- redundant virtual warns

    /// Checks whether conditions for starting vision pipeline are available
    /// and returns true if vision pipeline can started.
    /// <br>Errors: None
    /// @param[in] None
    /// @return bool true if conditions to start vision pipeline exist
    /// @exception None
    bool isValidConditionToStartVisionPipeline() const;

    /// Checks whether trigger condition for starting/stopping vision pipeline
    /// has changed and returns true either rvcCameraOnStatus or number of registered
    /// consumers has changed
    /// <br>Errors: None
    /// @param[in] None
    /// @return bool true if any of the trigger conditions have changed
    /// @exception None
    bool isVisionPipelineStartStopConditionChanged() const;

    void recordCameraFramesRate_fps(float64_t fps);
    // void informConsumers(VisionOperationMode_e newVisionOperationMode) const;
    void sendRegisterAgainMessage() const;
    void logVideoInterfaceStatus() const; // VideoInterface logging thread

    /// Process VIDEO_CONSUMER_REGISTER message sent by consumer application. Uses this message to start/stop vision
    /// pipeline based on whether at least consumer is registered <br>Errors: None
    /// @param[in] VideoConsumerRegister_t Data structure containing name of application, whether to register or
    /// unregister
    /// @return None
    /// @exception None
    void processReceivedMessage(const RADS::VideoConsumerRegister_t& videoConsumerRegistry);

    /// Process CAMERA_SUPPORT_DATA message sent by consumer application. rvcCameraOnStatus signal within the camera
    /// support data is used to start stop vision pipeline on whether camera is running or sleep. <br>Errors: None
    /// @param[in] CameraSupport_t Data structure contains all surround camera calibration values
    /// @return None
    /// @exception None
    void processReceivedMessage(const RADSInterSystemMessages::CameraSupport_t& currentCameraSupportData);

    /// Process APPLICATION_STATUS message sent by Supervisor. This message is only sent on any application status
    /// change. This is used by VideoInterface to get consumer's health status. VideoInterface de/registers consumers if
    /// actual VIDEO_CONSUMER_REGISTER message was missed. <br>Errors: None
    /// @param[in] SystemApplicationStatus_t List of all RADS2.0 application with their current status.
    /// @return None
    /// @exception None
    void processReceivedMessage(const RADS::SystemApplicationStatus_t& systemApplicationStatus);


    /// Process IMAGE_FRAME_CAPTURE_RESULTS message sent by consumer application. Uses this message to start/stop vision
    /// pipeline based on whether at least consumer is registered <br>Errors: None
    /// @param[in] std::vector<uint8_t>& Data structure containing name of application, whether to register or
    /// unregister
    /// @return None
    /// @exception None
    void processReceivedMessage(const std::vector<uint8_t>& imageData);

    bool inRange(const float32_t value, const float32_t low, const float32_t high) const
    {
        return (value >= low) && (value <= high);
    }

  protected:
    BufferManager* mBufferManager{nullptr}; // Base class pointer to choose between live/recorded

    // Set Buffer Manager reference
    void setBufferManager(BufferManager* inputBufferManager);

  public:
    void startImageCapturing();
    void readSharedCommand();
    void updateCameraStopStatus();

    ISPFrameInfoSharedMemory sharedMemory {"IMAGE_FRAME"};

    // 'virtual' is needed for MISRA Rule 10–3–2.
    virtual void initialize() override; // NOLINT -- redundant virtual warns

    virtual uint32_t featureAppRun( // NOLINT -- redundant virtual warns
        const VideoInterfaceInputAggregate_t& inputAggregateRef,
        FeatureAppOutput&                       frontEndOutputRef) override;

    // virtual void stopProxy(); // NOLINT -- redundant virtual warns


    // Developers should strive to make their app sensitive to being stopped if there is a requirement to
    // save state between invocations of their containing application.  If an app runs longer than a preset
    // time, determined by the Launcher, it will be summarily stopped without any gentle shutdown.
    //
    virtual void stopFeatureApp() override;









#if defined(QNX)
    /// Notify consumers by sending ImageReady_t message
    /// @return void
    /// @param[in] ISPFrameInfo_t&
    virtual void bufferReady(const ISPFrameInfo_t& ispFrameInfo) override; // NOLINT -- redundant 'virtual' warns
#endif

    // Notify consumers by sending ImageReady_t message
    // 'virtual' is needed for MISRA Rule 10–3–2
    virtual void bufferReady(size_t bufferIndex) override; // NOLINT -- redundant 'virtual' warns

    // VisionListener implementation
    // virtual void updateVisionOperationMode( // NOLINT -- redundant virtual warns
    //     VisionOperationMode_e newVisionOperationMode) override;
    // virtual void
    //     updateVisionErrorStatus(VisionErrorStatus_e visionErrorStatus) override; // NOLINT -- redundant virtual warns

    // virtual float64_t getCameraFramesRate_ms() override; // NOLINT -- redundant virtual warns
    // virtual void      visionPipelineStarted() override;  // NOLINT -- redundant virtual warns
    // virtual void      visionPipelineStopped() override;  // NOLINT -- redundant virtual warns

    // These variables used for logging purpose only
    static std::mutex mMutexCameraFramesRate;
    static float64_t  mCameraFramesRate_ms;
    //

    bool mIsConsumerRegistered{false}; ///< Stores the value set by setVideoInterfaceConsumerRegistered. Used in logic
                                       ///< for sleep periods

    constexpr static float32_t DAY_CAMERA_FRAMES_RATE_FPS   = 30.0F;
    constexpr static float32_t NIGHT_CAMERA_FRAMES_RATE_FPS = 15.0F;

    // This should be a temporary solution to find out frames rate (15 or 30) fps and send it out in every ImageReady_t
    // message. It is based on timing measurements of inter-arrivals frames (live video, or Hostsim). The best solution
    // should be based on CameraSupportData signals once implemented

    float32_t estimateFramesRate_fps(); // return either 15 or 30 fps

    constexpr static float32_t FRAMES_RATE_TOLERANCE = 0.1F; // used to estimate frames rate with 10% tolerance
    static_assert(FRAMES_RATE_TOLERANCE != 1.0F, "FRAMES_RATE_TOLERANCE should be less than 100%");

    // will be used to approximate frames rates (in ms) within this tolerance into frames rate (either 15 or 30 fps)
    constexpr static float32_t DAY_FRAMES_RATE_LOW_TOLERANCE_MS =
        (1000.0F / (DAY_CAMERA_FRAMES_RATE_FPS * (1.0 + FRAMES_RATE_TOLERANCE)));
    constexpr static float32_t DAY_FRAMES_RATE_HIGH_TOLERANCE_MS =
        (1000.0F / (DAY_CAMERA_FRAMES_RATE_FPS * (1.0 - FRAMES_RATE_TOLERANCE)));

    constexpr static float32_t NIGHT_FRAMES_RATE_LOW_TOLERANCE_MS =
        (1000.0F / (NIGHT_CAMERA_FRAMES_RATE_FPS * (1.0 + FRAMES_RATE_TOLERANCE)));
    constexpr static float32_t NIGHT_FRAMES_RATE_HIGH_TOLERANCE_MS =
        (1000.0F / (NIGHT_CAMERA_FRAMES_RATE_FPS * (1.0 - FRAMES_RATE_TOLERANCE)));

    friend class VideoInterfaceAlgorithmProxyTest;
    friend class VideoInterfaceTask;
};
} // namespace RADS

#endif // VIDEO_INTERFACE_ALGORITHM_PROXY_H
