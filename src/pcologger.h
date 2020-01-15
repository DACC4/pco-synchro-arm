#ifndef PCOLOGGER_H
#define PCOLOGGER_H

#include <iostream>
#include <string.h>

class PcoLogger
{
private:
    PcoLogger() {}

public:
    static PcoLogger *getInstance(){
        static PcoLogger logger;
        return &logger;
    }

    void setVerbosity(int level) {
        this->level = level;
    }

    template <typename T>
    PcoLogger& operator <<( T const& value) {
        if (level > 0) {
            std::cout << value;
        }
        return *this;
    }

    PcoLogger &operator<<(std::ostream & (*manip)(std::ostream &)) {
        if (level > 0){
            manip(std::cout);
        }
        return *this;
    }

    void initialize(int argc, char **argv) {
        if (argc == 2) {
            if (strcmp(argv[1], "-verbose") == 0) {
                setVerbosity(1);
            }
        }
    }

private:
    int level{0};
};

PcoLogger &logger()
{
    return *PcoLogger::getInstance();
}

#endif // PCOLOGGER_H
