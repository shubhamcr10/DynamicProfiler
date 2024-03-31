/* Copyright (c) 2022 Jangoo US - https://www.jangoo.us */

#ifndef ISP_FRAME_INFO_SHARED_MEMORY_H
#define ISP_FRAME_INFO_SHARED_MEMORY_H

#include <array>
#include <cassert>
#include <cstdint>

#include "RADSInterApplicationTypes.hpp"
#include "RADSCommon.hpp"
#include "RADSStdFloat.hpp"
#include "ISPFrameInfo.hpp"
#include "SharedMemory.hpp"
#include "RADSLoggingLevel.hpp"
#include "RADSError.hpp"
#include "BufferManagerException.hpp"

// Simplify access to SharedMemory interface
// Also allows access to protected methods
// It also keeps track of empty/full slots
// Image module uses this shared memory to get the frames info (physical address, width, & height)

class ISPFrameInfoSharedMemory : public SharedMemory
{
    std::array<bool, MAX_VIDEO_FRAMES> mFull{{false}};

  public:
    explicit ISPFrameInfoSharedMemory(const char* name)
        : SharedMemory{name}
    {
    }

    void create()
    {
        SharedMemory::create(sizeof(ISPFrameInfo_t), MAX_VIDEO_FRAMES, true);
    }

    void create(size_t size)
    {
        SharedMemory::create(size, MAX_VIDEO_FRAMES, true);
    }

    ISPFrameInfo_t& getISPFrameInfo(size_t index)
    {
        assert(index >= 0 && index < MAX_VIDEO_FRAMES &&
               "ISPFrameInfoSharedMemory.getISPFrameInfo(): out of bounds index");
        
        void* ptr = SharedMemory::getAddress(index);

        assert(ptr != nullptr);
        radsLogInfo("getISPFrameInfo()");

        return *static_cast<ISPFrameInfo_t*>(ptr);
    }
    void store(size_t index, ISPFrameInfo_t frameInfo)
    {
        try
        {
            // copy into shared memory
            getISPFrameInfo(index) = frameInfo;
            radsLogInfo("store() getISPFrameInfo");

            setFull(index, true);
            radsLogInfo("store() setFull");

        }
        catch (const std::exception& exceptionSharedMemory)
        {
            std::cout<<"store() exception"<<std::endl;
 
            radsLogError(VIDEO_INTERFACE_SHARED_MEMORY_DESTROY_ERROR, exceptionSharedMemory.what());
            throw BufferManagerException(VIDEO_INTERFACE_SHARED_MEMORY_DESTROY_ERROR, exceptionSharedMemory.what());
        }
        radsLogInfo("store() success");

    }

    uint64_t getBufferReceivedTime_ms(size_t index)
    {
        assert(index >= 0 && index < MAX_VIDEO_FRAMES &&
               "ISPFrameInfoSharedMemory.getBufferReceivedTime_ms(): out of bounds index");
        return getISPFrameInfo(index).receivedTime_ms;
    }
    uint32_t getBufferSequenceNumber(size_t index)
    {
        assert(index >= 0 && index < MAX_VIDEO_FRAMES &&
               "ISPFrameInfoSharedMemory.getBufferSequenceNumber(): out of bounds index");

        return getISPFrameInfo(index).sequenceNumber;
    }
    void setFull(size_t index, bool full)
    {
        assert(index >= 0 && index < MAX_VIDEO_FRAMES && "ISPFrameInfoSharedMemory.setFull(): out of bounds index");
        mFull.at(index) = full;
    }
    bool isFull(size_t index)
    {
        assert(index >= 0 && index < MAX_VIDEO_FRAMES && "ISPFrameInfoSharedMemory.isFull(): out of bounds index");
        return mFull.at(index);
    }
    bool isEmpty(size_t index)
    {
        return !isFull(index);
    }
    void clear()
    {
        mFull.fill(false);
    }
};

#endif // ISP_FRAME_INFO_SHARED_MEMORY_H
