#ifndef __SCONSOLE_H__
#define __SCONSOLE_H__

#define eRESET "[0m"
#define eBOLD "[1m"
#define eDIM "[2m"
#define eUNDERLINED "[4m"
#define eBLINK "[5m"
#define eREVERSE "[7m"
#define eHIDDEN "[8m"

#define fRESET "[39m"
#define fBLACK "[30m"
#define fRED "[31m"
#define fGREEN "[32m"
#define fYELLOW "[33m"
#define fBLUE "[34m"
#define fMAGENTA "[35m"
#define fCYAN "[36m"
#define fLIGHT_GRAY "[37m"
#define fDARK_GRAY "[90m"
#define fLIGHT_RED "[91m"
#define fLIGHT_GREEN "[92m"
#define fLIGHT_YELLOW "[93m"
#define fLIGHT_BLUE "[94m"
#define fLIGHT_MAGENTA "[95m"
#define fLIGHT_CYAN "[96m"
#define fWHITE "[97m"

#define bRESET "[49m"
#define bBLACK "[40m"
#define bRED "[41m"
#define bGREEN "[42m"
#define bYELLOW "[43m"
#define bBLUE "[44m"
#define bMAGENTA "[45m"
#define bCYAN "[46m"
#define bLIGHT_GRAY "[47m"
#define bDARK_GRAY "[100m"
#define bLIGHT_RED "[101m"
#define bLIGHT_GREEN "[102m"
#define bLIGHT_YELLOW "[103m"
#define bLIGHT_BLUE "[104m"
#define bLIGHT_MAGENTA "[105m"
#define bLIGHT_CYAN "[106m"
#define bWHITE "[107m"

#define aRESET eRESET << bRESET << fRESET

#define COLORSPACE(command, color) \
    std::cout << color;            \
    command;                       \
    std::cout << aRESET;

#define COLORSTREAM(stream, color) \
    color << stream << aRESET

#define LINEINFO __FILE__ << ":" << __LINE__
#ifdef DEBUG
#define Debug(x) std::cout << COLORSTREAM("[DEBUG] " << LINEINFO << ": " << x, fDARK_GRAY) << std::endl
#define DebugFrame(destR, color) drawRectangle(destR, color)
#else
#define Debug(x)
#define DebugFrame(destR, color)
#endif
#define Log(x) std::cout << "[LOG] " << LINEINFO << ": " << x << std::endl
#define Warn(x) std::cout << COLORSTREAM("[WARN] " << LINEINFO << ": " << x, fLIGHT_YELLOW) << std::endl
#define Error(x) std::cout << COLORSTREAM("[ERROR] " << LINEINFO << ": " << x, fRED) << std::endl
#define Critical(x) std::cout << COLORSTREAM("[CRITICAL] " << LINEINFO << ": " << x, bWHITE << fRED) << std::endl

#endif // __SCONSOLE_H__