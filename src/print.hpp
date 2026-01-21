
#include <stdlib.h>
#include <iostream>
#include <string>

#define ANSI_ESCAPE_CHAR \x1B

#define ANSI_RESET_MINOR 0

#define ANSI_BLACK_MINOR   0
#define ANSI_RED_MINOR     1
#define ANSI_GREEN_MINOR   2
#define ANSI_YELLOW_MINOR  3
#define ANSI_BLUE_MINOR    4
#define ANSI_MAGENTA_MINOR 5
#define ANSI_CYAN_MINOR    6
#define ANSI_WHITE_MINOR   7

#define ANSI_FG_MAJOR 3
#define ANSI_BG_MAJOR 4


// expands macro before stringizing
#define _STRINGIZE_HELPER(m) #m
#define STRINGIZE(m) _STRINGIZE_HELPER(m)
#define EXPAND(m) m
#define _ANSI_JOIN_SEQ_HELPER(major, minor) major##minor
#define ANSI_JOIN_SEQ(major, minor) _ANSI_JOIN_SEQ_HELPER(major, minor)

#define ANSI_STRINGIZE(ansiSeq) "\e[" STRINGIZE(ansiSeq) "m"

#define ANSI_FG(colorMinor) ANSI_JOIN_SEQ(ANSI_FG_MAJOR, colorMinor)

#define ANSI_RESET ANSI_STRINGIZE(ANSI_RESET_MINOR)


enum class Severity
{
    INFO, WARNING, ERROR, FATAL
};

template<typename T>
concept Printable = requires (T t) { std::cout << t; };

template<Printable... TArgs>
inline void t_print(const TArgs&... args)
{
    std::cout << (std::string(args) + ...) << ANSI_STRINGIZE(ANSI_RESET_MINOR) << std::endl;
}

template<Printable... TArgs>
inline void t_print(Severity severity, const TArgs&... args)
{
    std::string severityLog = "";
    switch (severity) {
        case Severity::WARNING:
            severityLog = ANSI_STRINGIZE(ANSI_FG(ANSI_YELLOW_MINOR)) "[WARNING] ";
            break;
        case Severity::ERROR:
            severityLog = ANSI_STRINGIZE(ANSI_FG(ANSI_RED_MINOR)) "[ERROR] ";
            break;
        case Severity::FATAL:
            severityLog = ANSI_STRINGIZE(ANSI_FG(ANSI_RED_MINOR)) "[[FATAL]] ";
            break;
        case Severity::INFO:
        default:
            break;
    }
    t_print(severityLog, args...);
}