/*
 * Created by french on 2/3/24.
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sys_interface.h"
#include "../common_defs.h"
#include "logging.h"

void getTimestamp(char *buf, size_t bufsz) {
   time_t current_time;
   struct tm *timeinfo;

   time(&current_time);
   timeinfo = localtime(&current_time);
   snprintf(buf, bufsz, "%d-%02d-%02dT%02d:%02d:%02d",
            timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}
