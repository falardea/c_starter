/*
 * Created by french on 2/4/24.
 */
#include "app_ctrl.h"
#include "../common_defs.h"
#include "../utils/parse_args.h"
#include "../utils/logging.h"
#include "../models/app_model.h"

void app_init(int argc, char **argv) {
   RVALUE_T retVal;

   retVal = parse_input_args(argc, argv);
   if (retVal == PARSE_ARG_ERR) {
      setAppModelInitState(INIT_ERROR);
      logging_llprintf(LOGLEVEL_ERROR, "error parsing input args, exiting...\n");
      return;
   }
   setAppModelInitState(INIT_SUCCESS);
}

void app_finalize(void) {}
