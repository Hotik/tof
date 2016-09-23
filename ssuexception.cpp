#include <cstdarg>
#include <cstdio>

#include "SsuException.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SsuException::SsuException() {
}

SsuException::SsuException(const std::string& format, ...) {
  char description[1024];
  va_list arguments;

  va_start(arguments, format);
  vsprintf(description, format.c_str(), arguments);
  va_end(arguments);

  this->description = description;
}

SsuException::SsuException(const SsuException& src) :
  description(src.description) {
}

SsuException::~SsuException() throw() {
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SsuException& SsuException::operator=(const SsuException&
    src) {
  description = src.description;
  return *this;
}

const char* SsuException::what() const throw() {
  if (!description.empty())
    return description.c_str();
  else
    return "Unknown error";
}
