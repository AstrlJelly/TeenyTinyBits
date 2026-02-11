
#include <array>
#include <cstddef>
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

#define _ANSI_JOIN_SEQ_HELPER(major, minor) major##minor
#define ANSI_JOIN_SEQ(major, minor) _ANSI_JOIN_SEQ_HELPER(major, minor)

#define ANSI_STRINGIZE(ansiSeq) "\e[" STRINGIZE(ansiSeq) "m"

#define ANSI_FG(colorMinor) ANSI_JOIN_SEQ(ANSI_FG_MAJOR, colorMinor)

#define ANSI_RESET_STR ANSI_STRINGIZE(ANSI_RESET_MINOR)


namespace teeny
{
    enum class Severity
    {
        INFO, IMPORTANT, WARNING, ERROR, FATAL
    };

    constexpr std::array severityTags{
        "[INFO]",
        
        ANSI_STRINGIZE(ANSI_FG(ANSI_GREEN_MINOR))
        "[[IMPORTANT]]",

        ANSI_STRINGIZE(ANSI_FG(ANSI_YELLOW_MINOR))
        "[WARNING]",

        ANSI_STRINGIZE(ANSI_FG(ANSI_RED_MINOR))
        "[ERROR]",

        ANSI_STRINGIZE(ANSI_FG(ANSI_RED_MINOR))
        "[[FATAL]]",
    };

    template<typename T>
    concept Printable = requires (T t) { std::cout << t; };

    template<char sep = ' ', Printable... TArgs>
    inline void println(const TArgs&... args)
    {
        // calls lambda for every argument
        ([args]{
            std::cout << args << sep;
        }(), ...);

        // todo: check the edge cases with this to make sure
        // todo  the carriage return doesn't break things
        // remove the last space, and reset the ansi color code
        std::cout << "\r" ANSI_STRINGIZE(ANSI_RESET_MINOR) << std::endl;
    }

    template<char sep = ' ', Printable... TArgs>
    inline void println(const Severity severity = Severity::INFO, const TArgs&... args)
    {
        std::string severityLog = severityTags.at(static_cast<size_t>(severity));
        teeny::println<sep>(severityLog, args...);
    }
}