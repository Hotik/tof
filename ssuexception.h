#ifndef SSUEXCEPTION_H
#define SSUEXCEPTION_H


#include <stdexcept>
#include <string>

class SsuException :
        public std::exception {
public:
    /** \brief Construct an exception
      */
    SsuException();
    SsuException(const std::string& format, ...);
    SsuException(const SsuException& src);

    /** \brief Destroy an exception
      */
    virtual ~SsuException() throw();

    /** \brief Exception assignments
      */
    SsuException& operator=(const SsuException& src);

    const char* what() const throw();
protected:
    std::string description;
};

#endif // SSUEXCEPTION_H
