#ifndef PCOLOGGER_H
#define PCOLOGGER_H

#include <iostream>
#include <string.h>
#include <mutex>
#include <sstream>

///
/// \brief The PcoLogger class
///
/// This class can be used as std::cout, but will avoid any issue of interleaving
/// in multi-threaded applications.
///
/// It shall be used like this:
///
/// PcoLogger() << "Hi guys. Here is a number : " << i << std::endl;
///
class PcoLogger : public std::ostringstream
{

public:

    /// Default constructor
    PcoLogger() = default;

    /// The descructor
    ///
    /// This method is where the writing to std::cout happens. That's the trick.
    ///
    ~PcoLogger()
    {
        std::lock_guard<std::mutex> guard(sm_mutex);
        std::cout << this->str();
    }

private:

    /// A mutex to protect the writing to std::cout
    static std::mutex sm_mutex;

};

#endif // PCOLOGGER_H
