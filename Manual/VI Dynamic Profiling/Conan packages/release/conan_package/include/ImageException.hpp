// Copyright (c) 2022 Jangoo US - https://www.jangoo.us

#ifndef IMAGE_EXCEPTION_H
#define IMAGE_EXCEPTION_H

#include "RADSException.hpp"

class ImageException : public RADSException
{

  public:
    using RADSException::RADSException;
};

#endif // IMAGE_EXCEPTION_H


// match_str(string xxx){
//   return list of  index where you find sub-string in given string.

//   suppose the given string is {RADSException RADSException data}
//   then it return [0, 13, 26]
// }
