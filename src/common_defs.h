/*
 * Created by french on 2/18/24.
*/
#ifndef C_STARTER_COMMON_DEFS_H
#define C_STARTER_COMMON_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

#define REALLY_LONG_ISO_TIME_STRING "2000-01-31T12:34:56.789-01:23"

typedef enum {
   SUCCESS,
   PARSE_ARG_ERR,
   DISPLAY_ENV_ERR // TODO: fill in more values here... or delete this
} RVALUE_T;

typedef enum {
   LOGLEVEL_DEBUG,
   LOGLEVEL_INFO,
   LOGLEVEL_ERROR
} LOGLEVEL_T;

/*
extern const char* DEBUG_STR;
extern const char* INFO_STR;
extern const char* ERROR_STR;
extern const int   LOGGING_MAX_MSG_LENGTH;
*/

#ifdef __cplusplus
}  /* closing brace for extern "C" */
#endif
#endif  /* C_STARTER_COMMON_DEFS_H */
