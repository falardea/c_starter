/*
 * Created by french on 2/3/24.
*/
#ifndef C_STARTER_APP_MODEL_H
#define C_STARTER_APP_MODEL_H

#include <stdbool.h>
#include "../common_defs.h"
#include "../utils/logging.h"

typedef enum {
   INIT_UNINITIALIZED,
   INIT_STARTED,
   INIT_RUNNING,
   INIT_SUCCESS,
   INIT_ERROR
} APP_INIT_STATE;

typedef enum {
   RUNTIME_UNINITIALIZED,
   RUNTIME_INITIALIZING,
   RUNTIME_RUNNING_NORMALLY,
   RUNTIME_HANDLING_ERROR,
   RUNTIME_RUNNING_DEPRECATED,
   RUNTIME_RUNNING_SAFE_MODE,
   RUNTIME_SHUTTING_DOWN,
} APP_RUNTIME_STATE;

typedef struct {
   APP_INIT_STATE initState;
   APP_RUNTIME_STATE runtimeState;
   bool debuggingEnabled;
   bool useTimestamps;
   LOGLEVEL_T runtimeLoglevel;
} ApplicationModel;

void setAppModelInitState(APP_INIT_STATE appInitState);
APP_INIT_STATE getAppModelInitState(void);

void setAppModelRuntimeState(APP_RUNTIME_STATE appRuntimeState);
APP_RUNTIME_STATE getAppModelRuntimeState(void);

void setAppModelDebuggingFlag(bool enable);
bool getAppModelDebuggingFlag(void);

void setAppModelUseTimestampsFlag(bool enable);
bool getAppModelUseTimestampsFlag(void);

void setAppModelLoglevel(LOGLEVEL_T logLevel);
LOGLEVEL_T getAppModelLoglevel(void);

#endif  /* C_STARTER_APP_MODEL_H */
