// Copyright (c) 2022 Jangoo US - https://www.jangoo.us

#ifndef VIDEO_BUFFER_MANAGER_H
#define VIDEO_BUFFER_MANAGER_H

#include <array>
#include <cassert>
#include <cstdint>
#include <thread>
#include <chrono>
#include <mutex>
#include <string>

#include "BufferListener.hpp"
#include "BufferManager.hpp"
#include "BufferWriter.hpp"
#include "BufferManagerException.hpp"
// #include "RADSInterApplicationTypes.hpp"
// #include "ISPFrame.hpp"
#include "ISPFrame.hpp"
// #include "ISPFrameInfo.hpp"
#include "ISPFrameInfo.hpp"
#include "ISPFrameInfoSharedMemory.hpp"
#include "SharedMemory.hpp"
#include "VideoInterfaceAlgorithmProxy.hpp"

class VideoBufferManager : public BufferManager, public BufferWriter<ISPFrame_t>
{
    // ISPFrameInfo holds physical address, width, & height for each video frame
    // This shared memory will be created here and used by Image module to access frame pixels by mapping physical
    // address into virtual address
    ISPFrameInfoSharedMemory mSharedMemory{"IMAGE_FRAME"};
    // ISPFrameInfoSharedMemory mSharedMemory{"VIDEO_DATA_MEMORY_TAG"};

    // storage to hold the Pop'd SDI Frames.
    std::array<ISPFrame_t, MAX_VIDEO_FRAMES> mISPFrames{};

    // CameraIds_e mCameraId{};

  protected:
    // implements pure virtuals from BufferManager superclass
    virtual bool isFullBuffer(size_t bufferIndex) override; // NOLINT -- redundant 'virtual' warns

    // 'virtual' is needed for MISRA Rule 10–3–2
    virtual void releaseBuffer(size_t bufferIndex) override; // NOLINT -- redundant 'virtual' warns

  public:
    RADS::VideoInterfaceAlgorithmProxy mVideoInterfaceAlgorithmProxy;
    VideoBufferManager() = default;
    virtual ~VideoBufferManager() override; // NOLINT -- virtual warns

    // Copy
    VideoBufferManager(const VideoBufferManager&) = delete;
    VideoBufferManager& operator=(const VideoBufferManager&) = delete;

    // Move
    VideoBufferManager(VideoBufferManager&&) = delete;
    VideoBufferManager& operator=(VideoBufferManager&&) = delete;

    // static void setVisionListener(VisionListener& listener);

    // void        setCameraId(CameraIds_e cameraId);
    // CameraIds_e getCameraId() const;

    // 'virtual' is needed for MISRA Rule 10–3–2
    // returns when the frame has been stored at the buffer index, or 0 if the buffer is empty
    virtual uint64_t getBufferReceivedTime_ms(size_t bufferIndex) override; // NOLINT -- redundant 'virtual' warns
    virtual uint32_t getBufferSequenceNumber(size_t bufferIndex) override;  // NOLINT -- redundant 'virtual' warns

    // Override BufferManager life-cycle API
    virtual void init() override;  // NOLINT -- redundant virtual warns
    virtual void start() override; // NOLINT -- redundant virtual warns
    virtual void stop() override;  // NOLINT -- redundant virtual warns

    // implements BufferWriter<ISPFrame_t> API
    // 'virtual' is needed for MISRA Rule 10–3–2
    virtual ISPFrame_t& getWriteBuffer() override;                 // NOLINT -- redundant 'virtual' warns
    virtual bool        receiveBufferContents() override;          // NOLINT -- redundant 'virtual' warns
    virtual void        invalidateBuffers() override;              // NOLINT -- redundant 'virtual' warns
    virtual void        updateFramesRate(float32_t framesRate_ms); // NOLINT -- redundant 'virtual' warns

    /// Read mActiveConsumersList and check whether HitchBallDetection or AutoHitchVision
    /// have registered and are active
    /// <br>Errors: None
    /// @param[in] None
    /// @return bool Whether any of the AutoHitch apps are running
    /// @exception None
    virtual bool checkAutoHitchVisionOn() override; // NOLINT -- redundant 'virtual' warns

    /// Set Dynamic ISP Configuration from VideoInterface.json an set
    /// it in the mIsDynamicISPIntrinsicsStatus variable
    /// <br>Errors: None
    /// @param[in] currentStatus Value to be set in mIsDynamicISPIntrinsicsStatus
    /// @return None
    /// @exception None
    virtual void setDynamicISPConfiguration(bool currentStatus) override; // NOLINT -- redundant 'virtual' warns

    /// Get dynamic configuration status value set in the mIsDynamicISPIntrinsicsStatus variable
    /// <br>Errors: None
    /// @param[in] None
    /// @return bool dynamic configuration status value mIsDynamicISPIntrinsicsStatus
    /// @exception None
    virtual bool getDynamicISPConfiguration() override; // NOLINT -- redundant 'virtual' warns

    /// Return whether the bufferManager is already running
    /// <br>Errors: None
    /// @param[in] None
    /// @return bool returns true if mStopped=false
    /// @exception None
    virtual bool isBufferManagerStopped() const;

    /// Returns number of consumers registered
    /// <br>Errors: None
    /// @param[in] None
    /// @return uint32_t returns number of registered consumers
    /// @exception None
    virtual uint32_t getNumberOfRegisteredConsumers() const;
};

#endif // VIDEO_BUFFER_MANAGER_H
