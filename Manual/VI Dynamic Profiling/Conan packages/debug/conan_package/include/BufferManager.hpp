// Copyright (c) 2022 Jangoo US - https://www.jangoo.us

#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include <cassert>
#include <atomic>
#include <cstdint>
#include <mutex>

#include "BufferTracker.hpp"
#include "BufferListener.hpp"
#include "CommunicationDefs.hpp"
#include "RADSInterApplicationTypes.hpp"
#include "RADSStdFloat.hpp"

#define MAX_VIDEO_FRAMES 10

// This class is designed to be extended. It provides functionality to send listener notifications
// It manages adding/removing active consumers and uses BufferTracker as a helper
// class. Right now, we have two subclasses: BufferManager and VideoBufferManager.
// Both subclasses have their own buffers to store received image frames (Hostsim) and  SDI_Frame frames (Video
// Pipeline). A listener should be registered in this class to be called back when a buffer becomes full and
// ready for processing.
class BufferManager
{

  private:
    RADS::SystemApplicationStatus_t mSystemApplicationStatus{};

    // holds actual frames rate based on timing measurements inside popping frames (for live video)
    // ..or based on receiving rates of complete frames over TCP connection (Hostsim)
    float32_t  mFramesRate_ms{};
    std::mutex mFramesRateMutex;

    bool hasRunningStatus(uint32_t consumerId)
    {
        assert((consumerId >= 0U) && (consumerId < static_cast<uint32_t>(RADS::SourceAppNames_e::MAX_SOURCE_APPS)));

        return (mSystemApplicationStatus.applicationStatus.at(consumerId) == RADS::ApplicationStatus_e::RUNNING);
    }

    std::array<size_t, MAX_VIDEO_FRAMES - 1> mBufferLocations = {{0}};

    size_t next(size_t index)
    {
        if (++index >= MAX_VIDEO_FRAMES - 1)
        {
            index = 0;
        }
        return index;
    }

  protected:
    std::mutex mMutex;

    // notify this listener when a buffer is ready
    BufferListener* mBufferListenerPtr = nullptr;

    // The current list of registered video consumers into VideoInterface application
    std::array<ConsumerRegisterInfo_t, RADS::SourceAppNames_e::MAX_SOURCE_APPS> mActiveConsumersList = {
        {ConsumerRegisterInfo_t{}}};
    uint32_t mNumberOfActiveConsumers     = 0U;
    uint32_t mNumberOfRegisteredConsumers = 0U;

    // Notify VideoInterfaceAlgorithmProxy about next available read buffer
    virtual void notifyBufferListener();

    void resetBufferLocations();

    size_t end()
    {
        return MAX_VIDEO_FRAMES;
    }

    // These two are indexing into mBufferLocations
    // The effective buffer index to be read is mBufferLocations.at(mReadIndex) (Or,  mBufferLocations.at(mWriteIndex)
    // in case of writing)
    size_t mReadIndex  = 0;
    size_t mWriteIndex = 0;
    // end() marks no buffer index is being processed by consumers
    size_t mTrackIndex = end();

    std::atomic_bool  mStopped{true};
    std::atomic_bool  mInitialized{false};
    std::mutex        mLifeCycleMutex;
    std::atomic<bool> mIsDynamicISPIntrinsicsStatus{false}; ///< Store dynamic isp config VideoInterface.json

    virtual bool isFullBuffer(size_t bufferIndex) = 0;

    // release any associated resources (push frame in case of Video Pipeline)
    virtual void releaseBuffer(size_t bufferIndex) = 0;

    size_t getReadBufferIndex()
    {
        return mBufferLocations.at(mReadIndex);
    }
    size_t getWriteBufferIndex()
    {
        return mBufferLocations.at(mWriteIndex);
    }

    void advanceRead()
    {
        mReadIndex = next(mReadIndex);
    }
    void advanceWrite()
    {
        mWriteIndex = next(mWriteIndex);
    }

    BufferManager();
    virtual ~BufferManager() = default;

    // counters
    size_t mNumberOfReceivedImages           = 0;
    size_t mNumberOfNotificationsReadyImages = 0;
    size_t mNumberOfDroppedImagesFullBuffers = 0;

  public:
    // Copy
    BufferManager(const BufferManager&) = delete;
    BufferManager& operator=(const BufferManager&) = delete;

    // Move
    BufferManager(BufferManager&&) = delete;
    BufferManager& operator=(BufferManager&&) = delete;

    void setBufferListener(BufferListener& listener)
    {
        mBufferListenerPtr = &listener;
    }

    // returns when the frame has been stored at the buffer index, or 0 if the buffer is empty
    virtual uint64_t getBufferReceivedTime_ms(size_t bufferIndex) = 0;

    virtual uint32_t getBufferSequenceNumber(size_t bufferIndex) = 0;

    // VideoInterface will call these
    // separate init() is needed (instead of doing everything inside constructor) because VideoInterfaceAlgorithmProxy
    // needs to decide wether or not to use LiveVideo later on during it initialize() call. So, it will later call
    // init() for the configured type
    virtual void init() = 0;

    virtual void start() = 0;
    virtual void stop()  = 0;

    // This method will be called once ModeManager switches modes, consumers start, or crashes
    // remove any crashed consumer from the active consumers list
    // add any consumer if its register message did not arrive before
    void applicationsStatusUpdated(const RADS::SystemApplicationStatus_t& systemApplicationStatus);

    // Consumers should send register/unregister message
    void registerConsumer(uint32_t consumerId);
    void unregisterConsumer(uint32_t consumerId);
    // polyspace +1 MISRA-CPP:9-3-3 [Justified:Low] "lock_guard used"
    uint32_t getNumberOfActiveConsumers();

    float32_t getFramesRate_fps();

    // polyspace +1 MISRA-CPP:9-3-3 [Justified:Low] "lock_guard used"
    float32_t getFramesRate_ms();

    void updateFramesRate(float32_t framesRate_ms);

    /// Read mActiveConsumersList and check whether HitchBallDetection or AutoHitchVision
    /// have registered and are active
    /// <br>Errors: None
    /// @param[in] None
    /// @return bool Whether any of the AutoHitch apps are running
    /// @exception None
    bool checkAutoHitchVisionOn();

    /// Set Dynamic ISP Configuration from VideoInterface.json an set
    /// it in the mIsDynamicISPIntrinsicsStatus variable
    /// <br>Errors: None
    /// @param[in] currentStatus bool: Value to be set in mIsDynamicISPIntrinsicsStatus
    /// @return None
    /// @exception None
    void setDynamicISPConfiguration(bool currentStatus);

    /// Get dynamic configuration status value set in the mIsDynamicISPIntrinsicsStatus variable
    /// <br>Errors: None
    /// @param[in] None
    /// @return bool dynamic configuration status value mIsDynamicISPIntrinsicsStatus
    /// @exception None
    bool getDynamicISPConfiguration() const;

    /// Return whether the bufferManager is already running
    /// <br>Errors: None
    /// @param[in] None
    /// @return bool returns true if mStopped=false
    /// @exception None
    bool isBufferManagerStopped() const;

    /// Returns number of consumers registered
    /// <br>Errors: None
    /// @param[in] None
    /// @return uint32_t returns number of registered consumers
    /// @exception None
    uint32_t getNumberOfRegisteredConsumers() const;
};

#endif // BUFFER_MANAGER_H
