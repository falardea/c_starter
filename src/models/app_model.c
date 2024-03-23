/*
 * Created by french on 2/3/24.
*/
#include <stdio.h>
#include <stdbool.h>
#include "app_model.h"

// Thinking about making this static... would that make it a Singleton?
// A model interface might be nothing more than setter/getter functions
static ApplicationModel sAppModel = {INIT_UNINITIALIZED,
                                     RUNTIME_UNINITIALIZED,
                                     false,
                                     false,
                                     LOGLEVEL_ERROR};

void setAppModelInitState(APP_INIT_STATE appInitState) {
   sAppModel.initState = appInitState;
}
APP_INIT_STATE getAppModelInitState(void) {
   return sAppModel.initState;
}
void setAppModelRuntimeState(APP_RUNTIME_STATE appRuntimeState) {
   sAppModel.runtimeState = appRuntimeState;
}
APP_RUNTIME_STATE getAppModelRuntimeState(void) {
   return sAppModel.runtimeState;
}
void setAppModelDebuggingFlag(bool enable) {
   sAppModel.debuggingEnabled = enable;
   if (enable) {
      // TODO: set and immediately use log level?
      printf("Debugging Enabled\n");
   }
}
bool getAppModelDebuggingFlag(void) {
   return sAppModel.debuggingEnabled;
}

void setAppModelUseTimestampsFlag(bool enable) {
   sAppModel.useTimestamps = enable;
   if (enable) {
      // TODO: set and immediately use log level?
      printf("We're going to be a bit more chatty\n");
   }
}
bool getAppModelUseTimestampsFlag(void) {
   return sAppModel.useTimestamps;
}

/**
 * setAppModelLoglevel
 * @param logLevel
 * If the model runtimeLoglevel is set to a lower level already, it does not get raised.  Once set, it's set for the
 * runtime life of the application.
 */
void setAppModelLoglevel(LOGLEVEL_T logLevel) {
   if (sAppModel.runtimeLoglevel > logLevel)
      sAppModel.runtimeLoglevel = logLevel;
}

LOGLEVEL_T getAppModelLoglevel(void) {
   return sAppModel.runtimeLoglevel;
}
