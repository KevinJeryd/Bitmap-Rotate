#ifndef TERMINAL_BEEP_H
#define TERMINAL_BEEP_H

#ifdef _WIN32
#include <io.h>
#define access _access

#else
#include <unistd.h?
#endif

class TerminalBeep {
    public:
        static void greet();
};

#endif