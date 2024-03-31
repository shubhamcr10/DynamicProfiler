// Copyright (c) 2022 Jangoo US - https://www.jangoo.us

#ifndef BUFFER_WRITER_H
#define BUFFER_WRITER_H

#include <cstdint>

#include "RADSStdFloat.hpp"

template <typename E>
class BufferWriter
{

  public:
    // Get an empty buffer to write into it.
    // This call will reserve the buffer until either ungetWriteBuffer() or receiveBufferContents() is called
    virtual E& getWriteBuffer() = 0;

    // called when buffer content is complete and ready for processing
    // returns true of the contents is marked for processing, or false if dropped
    virtual bool receiveBufferContents() = 0;

    // Called when video pipeline switches from streaming into recovery and back again so we have data integrity
    // This method should clear "full flag" and reset read/write buffer locations
    // it assumes that the contents had already been copied by video consumers
    virtual void invalidateBuffers() = 0;

    // called to update the inter-arrival frames rate for this buffer writter (both live video, or Hostsim)
    virtual void updateFramesRate(float32_t framesRate_ms) = 0;

    /// Read mActiveConsumersList and check whether HitchBallDetection or AutoHitchVision
    /// have registered and are active
    /// <br>Errors: None
    /// @param[in] None
    /// @return bool dynamic configuration status value mIsDynamicISPIntrinsicsStatus
    /// @exception None
    virtual bool checkAutoHitchVisionOn() = 0;

    /// Set Dynamic ISP Configuration from VideoInterface.json an set
    /// it in the mIsDynamicISPIntrinsicsStatus variable
    /// <br>Errors: None
    /// @param[in] currentStatus bool: Value to be set in mIsDynamicISPIntrinsicsStatus
    /// @return None
    /// @exception None
    virtual void setDynamicISPConfiguration(bool currentStatus) = 0;

    /// Get dynamic configuration status value set in the mIsDynamicISPIntrinsicsStatus variable
    /// <br>Errors: None
    /// @param[in] None
    /// @return bool dynamic configuration status value mIsDynamicISPIntrinsicsStatus
    /// @exception None
    virtual bool getDynamicISPConfiguration() = 0;
};

#endif // BUFFER_WRITER_H
