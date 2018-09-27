#ifndef au_loggin_h
#define au_loggin_h

// standard includes
#include <stdio.h>
#include <iostream>
#include <sstream>

// project includes
#include <spellbook/term/term.h>

#define AU_LOG_LEVEL_DEBUG 0   // extended-information of normal usage
#define AU_LOG_LEVEL_INFO  1   // normal usage
#define AU_LOG_LEVEL_CRIT  2   // important normal usage
#define AU_LOG_LEVEL_WARN  3   // existential cases that are gracefully handled
#define AU_LOG_LEVEL_ERROR 4   // existential cases that may have unintended consequences
#define AU_LOG_LEVEL_FATAL 5   // unrecoverable errors that result in program termination

#define AU_DEBUG_LOG_PREF   "[DEBUG] "
#define AU_INFO_LOG_PREF    "[INFO]  "
#define AU_CRIT_LOG_PREF    "[CRIT]  "
#define AU_WARN_LOG_PREF    "[WARN]  "
#define AU_ERROR_LOG_PREF   "[ERROR] "
#define AU_FATAL_LOG_PREF   "[FATAL] "

#ifndef AU_LOG_LEVEL
#define AU_LOG_LEVEL AU_LOG_LEVEL_INFO
#endif

// compile-time formatting options
#define AU_LOG_FILENAME_ONLY 1
#define AU_LOG_MSG_PADDING 120

namespace au {

char* padded(const char* str, ...);
const char* filename(const char* path);

} // namespace au

// level-prefixed, colored, format-style output to a given a stream
#define AU_LOG_STREAM_FIXED(stream, color, pre, msg, ...) \
{\
    stream << color << pre << au::padded(msg, ##__VA_ARGS__) << " | " << au::filename(__FILE__) << ':' << __LINE__ << au::term::nocolor << std::endl;\
}

// level-prefixed, colored, stream-style output to a given stream
#define AU_LOG_STREAM_FIXED_STREAM(stream, color, pre, seq) \
{\
    stream << color << pre << au::padded("%s", ((std::stringstream&)(std::stringstream() << seq)).str().c_str()) << " | " << au::filename(__FILE__) << ':' << __LINE__ << au::term::nocolor << std::endl;\
}

// level-prefixed format-style output to stdout
#define AU_LOG_STDOUT_FIXED(pre, msg, ...) \
{\
    AU_LOG_STREAM_FIXED(std::cout, au::term::white, pre, msg, ##__VA_ARGS__)\
}

// level-prefixed stream-style output to stdout
#define AU_LOG_STDOUT_FIXED_STREAM(pre, seq) \
{\
    AU_LOG_STREAM_FIXED_STREAM(std::cout, au::term::white, pre, seq)\
}

// level-prefixed format-style output to stderr
#define AU_LOG_STDERR_FIXED(pre, msg, ...) \
{\
    AU_LOG_STREAM_FIXED(std::cerr, au::term::red, pre, msg, ##__VA_ARGS__)\
}

#if AU_LOG_LEVEL <= AU_LOG_LEVEL_DEBUG
#define AU_DEBUG(msg, ...) AU_LOG_STDOUT_FIXED(AU_DEBUG_LOG_PREF, msg, ##__VA_ARGS__)
#define AU_DEBUG_STREAM(seq) AU_LOG_STDOUT_FIXED_STREAM(AU_DEBUG_LOG_PREF, seq)
#else
#define AU_DEBUG(msg, ...)
#define AU_DEBUG_STREAM(seq)
#endif

#if AU_LOG_LEVEL <= AU_LOG_LEVEL_INFO
#define AU_INFO(msg, ...)  AU_LOG_STDOUT_FIXED(AU_INFO_LOG_PREF, msg, ##__VA_ARGS__)
#define AU_INFO_STREAM(seq) AU_LOG_STDOUT_FIXED_STREAM(AU_INFO_LOG_PREF, seq)
#else
#define AU_INFO(msg, ...)
#define AU_INFO_STREAM(seq)
#endif

#if AU_LOG_LEVEL <= AU_LOG_LEVEL_CRIT
#define AU_CRIT(msg, ...)  AU_LOG_STDOUT_FIXED(AU_CRIT_LOG_PREF, msg, ##__VA_ARGS__)
#else
#define AU_CRIT(msg, ...)
#define AU_CRIT_STREAM(seq)
#endif

#if AU_LOG_LEVEL <= AU_LOG_LEVEL_CRIT
#define AU_WARN(msg, ...)  AU_LOG_STDERR_FIXED(AU_WARN_LOG_PREF, msg, ##__VA_ARGS__)
#else
#define AU_WARN_STREAM(seq)
#endif

#if AU_LOG_LEVEL <= AU_LOG_LEVEL_ERROR
#define AU_ERROR(msg, ...) AU_LOG_STDERR_FIXED(AU_ERROR_LOG_PREF, msg, ##__VA_ARGS__)
#else
#define AU_ERROR(msg, ...)
#define AU_ERROR_STREAM(seq)
#endif

#if AU_LOG_LEVEL <= AU_LOG_LEVEL_FATAL
#define AU_FATAL(msg, ...) AU_LOG_STDERR_FIXED(AU_FATAL_LOG_PREF, msg, ##__VA_ARGS__)
#else
#define AU_FATAL(msg, ...)
#define AU_FATAL_STREAM(seq)
#endif

// TODO: relative or base path filename
// TODO: configure filename and line number (env variable similar to ROS?)
// TODO: compile-time level configuration
// TODO: run-time level configuration
// TODO: named prints
// TODO: thread-specific prints
// TODO: multiple destinations to write to
// TODO: colored terminal output and compile-time configuration
// TODO: mt logger? maybe differently named loggers can be accessed in a mt manner
// TODO: configuration of buffering
// TODO: column-padding

#endif
