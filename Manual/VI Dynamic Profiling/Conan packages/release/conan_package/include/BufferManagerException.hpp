/* Copyright (c) 2022 Jangoo US - https://www.jangoo.us */

#ifndef BUFFER_MANAGER_EXCEPTION_H
#define BUFFER_MANAGER_EXCEPTION_H

#include "RADSException.hpp"

class BufferManagerException : public RADSException
{

  public:
    using RADSException::RADSException;
};

#endif // BUFFER_MANAGER_EXCEPTION_H
