#include "asserts.h"

void on_assertion_failure(const char* expr, const char* msg, const char* file, i32 line) {
    log_message(SM_LOG_ERROR, "Assertion Failure: %s, message: '%s', in file: %s, line: %d", expr, msg, file,
                line);
}
