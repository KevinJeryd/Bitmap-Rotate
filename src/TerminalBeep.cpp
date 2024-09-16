#include "TerminalBeep.h"

void TerminalBeep::greet() {
    char buf = '\x07';
    write(1, &buf, sizeof(buf));
}