#include <stdio.h>
#include "version.h"
#include "common_defs.h"
#include "controllers/app_ctrl.h"
#include "models/app_model.h"

int main(int argc, char **argv) {
   printf("ver.%d.%d.%d\n", project_VERSION_MAJOR, project_VERSION_MINOR, project_VERSION_PATCH);
   app_init(argc, argv);

   if (getAppModelInitState() != INIT_ERROR)
   {
      logging_llprintf(LOGLEVEL_DEBUG, "application initialized, starting UI\n");

      logging_llprintf(LOGLEVEL_DEBUG, "%s: CHECKPOINT \n", __func__);

      logging_llprintf(LOGLEVEL_DEBUG, "UI closed, shutting down\n");
   }
   else
   {
      setAppModelRuntimeState(RUNTIME_HANDLING_ERROR);
      logging_llprintf(LOGLEVEL_ERROR, "application initialization failed, attempting a graceful shutdown\n");
      // Graceful shutdown stuff here
      setAppModelRuntimeState(RUNTIME_SHUTTING_DOWN);
   }

   app_finalize();

   return getAppModelRuntimeState();
}
