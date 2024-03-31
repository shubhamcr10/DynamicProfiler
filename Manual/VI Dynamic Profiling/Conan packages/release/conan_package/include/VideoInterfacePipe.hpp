/* Copyright (c) 2018 Jangoo US - https://www.jangoo.us */

#ifndef VIDEO_INTERFACE_PIPE_H
#define VIDEO_INTERFACE_PIPE_H

#include "ChannelBaseReaderWriter.hpp"  //channel
#include "VideoInterfaceAlgorithmProxy.hpp"

namespace RADS
{

class VideoInterfacePipe : public ChannelBaseReaderWriter<VideoInterfaceInputAggregate_t>
{
    static constexpr uint16_t DEFAULT_PIPE_DEPTH           = 28U;
    static constexpr MessageDefines_t RECEIVED_IMAGE_DATA  = 0b000000011010; // RESULT OF IMAGE CAPTURING 26
    static constexpr MessageDefines_t FULL_MESSAGE         = 0b000000110011; 

    bool mLookingForAllMessages = true; // first trigger ever needs all messages in input aggregate
  protected:
    virtual bool isTrigger() override; // NOLINT -- virtual warns
    virtual size_t writeToBuffer(const RADS::PDUMessage_t& receivedMessage) override; // NOLINT -- virtual warns
    virtual size_t writeToBuffer(uint32_t    receivedMessageId, // NOLINT -- virtual warns
                                 const void* inputBuffer,
                                 size_t      inputBufferSize);

  public:
    /// VideoInterfacePipe constructor
    /// create a function buffer with the given depth
    /// @param[in] pipeDepth is the depth of the triggers function buffer
    /// @throw none
    explicit VideoInterfacePipe(uint16_t pipeDepth = DEFAULT_PIPE_DEPTH)
        : ChannelBaseReaderWriter{pipeDepth}
    {
    }
};

} // namespace RADS

#endif // VIDEO_INTERFACE_PIPE_H
