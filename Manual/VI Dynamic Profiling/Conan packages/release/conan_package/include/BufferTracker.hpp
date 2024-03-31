// Copyright (c) 2022 Jangoo US - https://www.jangoo.us

#ifndef BUFFER_TRACKER_H
#define BUFFER_TRACKER_H

#include <array>
#include <cstdint>
#include <cstddef>

#include "CommunicationDefs.hpp"
#include "ConsumerRegisterInfo.hpp"

// This class will be used to track the active consumers of the allocated buffers
// Once BufferManager has full buffer with video frame, the video interface app will send 'ImageReady' for those
// consumers to start reading that buffer An object from this class will be associated with the buffer to keep track
// when it should be free

class BufferTracker
{

    size_t mNumberOfReadingConsumers = 0;

    // index of tracked buffer
    size_t mTrackedBufferIndex = 0;

    // a snapshot of BufferManager's current active consumer list to be used for tracking this buffer
    // Consumers list is indexed by the consumer Id. true means active reading consumer and the buffer's contents should
    // remain valid
    std::array<bool, RADS::SourceAppNames_e::MAX_SOURCE_APPS> mReadingConsumersList = {{false}};

#ifdef RADS_LOG_LEVEL_DEBUG
    void startTrackingDebug(size_t bufferIndex);
    void consumerDoneDebug();
#endif

  public:
    BufferTracker() = default;

    // return true if there is at least one consumer still consuming this buffer (That is , still did not send
    // ImageReadComplete message)
    bool isTracking()
    {
        return mNumberOfReadingConsumers != 0;
    }

    // Start tracking the consumers for the frame stored at the indexed buffer
    // a snapshot of the BufferManager's mActiveConsumersList will be taken and stored into mReadConsumersList
    // @params
    //   index: the index of buffer which contains the image to track
    // 	 activeConsumersList: the list of registered active video consumers into VideoInterface application

    void startTracking(
        size_t                                                                            bufferIndex,
        uint64_t                                                                          receivedTime_ms,
        const std::array<ConsumerRegisterInfo_t, RADS::SourceAppNames_e::MAX_SOURCE_APPS>& activeConsumersList);

    // should be called when a consumer is done processing the buffer's contents
    // or, if a consumer unregisters itself from the VideoInterface application
    // or, if a consumer crashes. Crashed consumers might not get a chance to un-register themselves. Supervisor will
    // send SystemApplicationStatus message to videointerface in those cases This call might mark the buffer as free
    // (not tracked anymore) if this was the last consumer reading this buffer's contents
    // @params:
    // 		consumerId: the Id of the video consumer application which finishes reading this buffer and sent
    // ImageReadComplete_t, or the consumer which unregisters itself from VideoInterface application
    // @return: true if consumer was being tracked & removed from mReadConsumerList
    bool consumerDone(size_t consumerId);
};

#endif // BUFFER_TRACKER_H
