/*
 * Created by falar on 2/24/2024.
*/
#ifndef C_STARTER_LOGGING_H
#define C_STARTER_LOGGING_H

#include <stdarg.h>
#include "../common_defs.h"

const char *strLoglevel(LOGLEVEL_T loglevelT);
int logging_llprintf(LOGLEVEL_T logLevel, const char *_format, ...);

#endif  /* C_STARTER_LOGGING_H */
