// Copyright (c) 2022 Jangoo US - https://www.jangoo.us

#ifndef BUFFER_LISTENER_H
#define BUFFER_LISTENER_H

#include <cstddef>

// A listener should be registered in BufferManager to receive notifications when a buffer is available
class BufferListener
{

  public:
    // called back by BufferManager when a buffer is ready for processing
    // @Params
    // bufferIndex: the index of the ready buffer
    virtual void bufferReady(size_t bufferIndex) = 0;
};

#endif // BUFFER_LISTENER_H
