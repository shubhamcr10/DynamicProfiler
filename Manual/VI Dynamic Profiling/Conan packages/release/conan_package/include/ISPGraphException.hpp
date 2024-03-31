/* Copyright (c) 2022 Jangoo US - https://www.jangoo.us */

#ifndef ISP_GRAPH_EXCEPTION_H
#define ISP_GRAPH_EXCEPTION_H

#include <cstring>
#include <stdexcept>
#include <exception>

#include "RADSException.hpp"

class ISPGraphException : public RADSException
{
  public:
    using RADSException::RADSException;
};

#endif // ISP_GRAPH_EXCEPTION_H
