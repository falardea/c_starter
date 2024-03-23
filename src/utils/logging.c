/*
 * Created by falar on 2/24/2024.
*/
#include <stdio.h>
#include "logging.h"
#include "../models/app_model.h"
#include "../utils/sys_interface.h"

const char* DEBUG_STR = "DEBUG";
const char* INFO_STR = "INFO";
const char* ERROR_STR = "ERROR";
const int   LOGGING_MAX_MSG_LENGTH = 1024;
static char timestamp[20];  // not sure why it felt better to allocate the memory once

const char *strLoglevel(LOGLEVEL_T loglevelT)
{
   if (loglevelT == LOGLEVEL_ERROR)
      return ERROR_STR;
   else if (loglevelT == LOGLEVEL_INFO)
      return INFO_STR;
   else
      return DEBUG_STR;
}

int logging_llprintf(LOGLEVEL_T logLevel, const char *_format, ...) {
   int done = 0;
   if (logLevel >= getAppModelLoglevel()) {
      bool use_ts = getAppModelUseTimestampsFlag();
      // This started as effectively a copy of stdio.printf
      char line_out[LOGGING_MAX_MSG_LENGTH];

      if (use_ts)
         getTimestamp(timestamp, sizeof(timestamp));

      va_list arg;
      va_start(arg, _format);
      done = vsnprintf(line_out, sizeof (line_out), _format, arg);
      va_end(arg);

      /* TODO: This seems a bit sloppy, we slipped the use of use_ts in as an afterthought  */
      printf("%s%s%s:%s", (use_ts) ? timestamp : "", (use_ts) ? ":":"", strLoglevel(logLevel), line_out);

      return done;
   }
   return done;
}
