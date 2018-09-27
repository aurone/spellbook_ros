#include <spellbook/log/logging.h>

// standard includes
#include <stdarg.h>
#include <string.h>
#include <algorithm>

namespace au {

static char FixedWidthLogBuffer[AU_LOG_MSG_PADDING];

char* padded(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    std::fill(FixedWidthLogBuffer, FixedWidthLogBuffer + sizeof(FixedWidthLogBuffer), ' ');
    FixedWidthLogBuffer[AU_LOG_MSG_PADDING - 1] = '\0';

    int ret = vsnprintf(FixedWidthLogBuffer, AU_LOG_MSG_PADDING, fmt, args);
    if (ret < AU_LOG_MSG_PADDING - 1) {
        FixedWidthLogBuffer[ret] = ' ';
    }

    va_end(args);
    return FixedWidthLogBuffer;
}

const char* filename(const char* path)
{
#if AU_LOG_FILENAME_ONLY
    const char* fbase = strrchr(path, '/');
    const char* bbase = strrchr(path, '\\');
    if (fbase && !bbase) {
        return fbase + 1;
    }
    else if (bbase && !fbase) {
        return bbase + 1;
    }
    else if (bbase && fbase) {
        return std::max(bbase, fbase) + 1;
    }
    else {
        return path;
    }
#else
    return path;
#endif
}

} // namespace au
