#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <limits.h>
#include "version.h"
#include "common_defs.h"
#include "controllers/app_ctrl.h"
#include "models/app_model.h"

char *search_string[] = {"Date,Ticks,Notes,field1,field2,field3,field4,field5,field6,field7,field8",
                         "2024-01-23 04:31:12,10,\"\",1,2,3,4,5,6,7,8\n",
                         "2024-02-23 05:56:32,11,\"This line in the csv can have commas, and even comma\n",
                         "seperated numbers, like: 1,2,3,4,5,a string,and,carriage returns, and more numbers: \n",
                         "not a date here though\",1.0,1.2,1.3,1.4,1.5,1.6,1.7,1.8\n",
                         "2024-03-23 07:17:41,12,\"1,2,3,4\",8,7,6,5,4,3,2,1\n",
                         "2024-04-23 08:11:34,12,\"some text, no newline\",8,7,6,5,4,3,2,1\n",
                         "2024-05-23 12:34:57,12,\"\n",
                         "some text, but not a newline\",8,7,6,5,4,3,2,1\n",
                         "2024-06-23 14:16:40,12,\"some text, but not a newline\",8,7,6,5,4,3,2,1\n",
                         "2024-07-23 21:37:04,12,\"some text, but not a newline\",8,7,6,5,4,3,2,1\n"};

bool compile_regex(regex_t *re, const char *patt)
{
   /* Use REG_EXTENDED for POSIX regex notation */
   reg_errcode_t re_comp_ret = regcomp(re, patt, REG_EXTENDED);
   if (re_comp_ret != 0) {
      char err_buff[1024];
      regerror(re_comp_ret, re, err_buff, sizeof (err_buff));
      logging_llprintf(LOGLEVEL_ERROR, "%s: 'regcomp' failed to compile %s, reporting: %s\n",
                       __func__, patt, err_buff);
      return false;
   }
   return true;
}

bool parse_psuedo_csv(char *timestamp)
{
   char re_prefix_patt[128];
   char search_line_str[SHRT_MAX];
   regex_t re_prefix;

   bool found = false;

   /* Build the regex with the timestamp, up to the first quote, then look for an end quote, and group that if found */
   snprintf(re_prefix_patt, sizeof(re_prefix_patt), "(^%s:[0-9]{2},[0-9]+,\")(.*(\".*$))?", timestamp);

   if (!compile_regex(&re_prefix, re_prefix_patt)) return false;

   int so_ret = REG_NOMATCH;
   regmatch_t so_pmatch[4]; // three groups plus the whole
   for(size_t i = 0; i < (sizeof (search_string)/sizeof(search_string[0])); i++)  // While(fgets...
   {
      so_ret = regexec(&re_prefix, search_string[i], 4, so_pmatch, 0);
      if (so_ret == 0) /* IF WE MATCH, WE EXIT FROM HERE */
      {
         // it appears rm_eo is the first index of the non-matching string, in the match group (we use one here)
         memcpy(&search_line_str, search_string[i], so_pmatch[1].rm_eo);
         search_line_str[so_pmatch[1].rm_eo] = '\0';

         /*logging_llprintf(LOGLEVEL_DEBUG, "TIMESTAMP FOUND: CSV LINE START:");
         logging_llprintf(LOGLEVEL_DEBUG, "search_string[%d]: %s", i, search_string[i]);

         logging_llprintf(LOGLEVEL_DEBUG, "whole match span..(0)=(%d, %d)\n", so_pmatch[0].rm_so, so_pmatch[0].rm_eo);
         logging_llprintf(LOGLEVEL_DEBUG, "matched timestamp.(1)=(%d, %d)\n", so_pmatch[1].rm_so, so_pmatch[1].rm_eo);
         logging_llprintf(LOGLEVEL_DEBUG, "remainder.........(2)=(%d, %d)\n", so_pmatch[2].rm_so, so_pmatch[2].rm_eo);
         logging_llprintf(LOGLEVEL_DEBUG, "after quote.......(3)=(%d, %d)\n", so_pmatch[3].rm_so, so_pmatch[3].rm_eo);
         logging_llprintf(LOGLEVEL_DEBUG, "after quote%.*s(3)=(x,x)\n",
                          18-strlen("after quote"), "..................", so_pmatch[3].rm_so, so_pmatch[3].rm_eo);*/
         if (so_pmatch[2].rm_so != -1) {
            /* We have a complete csv line, we're going to drop the notes for cleanliness */

            int so_remain = (so_pmatch[3].rm_so != -1) ? so_pmatch[3].rm_so : so_pmatch[2].rm_so;
            int eo_remain = (so_pmatch[3].rm_eo != -1) ? so_pmatch[3].rm_eo : so_pmatch[2].rm_eo;
            size_t sz_remain = (so_pmatch[3].rm_eo != -1) ? eo_remain - so_remain - so_pmatch[1].rm_eo : eo_remain;
            strncpy(search_line_str + so_pmatch[1].rm_eo,
                    search_string[i] + so_remain,
                    eo_remain - so_remain + 1);
            search_line_str[sz_remain - 1] = '\0';  // clear newline
            logging_llprintf(LOGLEVEL_DEBUG, "search_line_str: %s\n", search_line_str);

            /* Find our data? */

            found = true;
            break;
         }
         else
         {
            snprintf(re_prefix_patt, sizeof(re_prefix_patt), "^[^\"]*(\".*$)");
            if (!compile_regex(&re_prefix, re_prefix_patt)) return false;

            for (size_t cont = i+1; cont < (sizeof (search_string)/sizeof(search_string[0])); cont++)
            {
               so_ret = regexec(&re_prefix, search_string[cont], 2, so_pmatch, 0);
               if (so_ret == 0) /* IF WE MATCH, WE EXIT FROM HERE */
               {
                  strncpy(search_line_str + strlen(search_line_str), /* overwrite the null-term */
                          search_string[cont] + so_pmatch[1].rm_so,
                          so_pmatch[1].rm_eo - so_pmatch[1].rm_so);
                  search_line_str[strlen(search_line_str) + so_pmatch[1].rm_eo - 1] = '\0';  // clears newline

                  logging_llprintf(LOGLEVEL_DEBUG, "search_line_str: %s\n", search_line_str);
                  found = true;
                  break;
               }
            }
            break;
         }
      }
   }
   regfree(&re_prefix);
   return found;
}

int main(int argc, char **argv) {
   printf("ver.%d.%d.%d\n", project_VERSION_MAJOR, project_VERSION_MINOR, project_VERSION_PATCH);
   app_init(argc, argv);

   if (getAppModelInitState() != INIT_ERROR)
   {
      logging_llprintf(LOGLEVEL_DEBUG, "application initialized, starting UI\n");

      char *date_lookup[] = {"2024-01-23 04:31",
                             "2024-02-23 05:56",
                             "2024-03-23 07:17"
      };
      for (size_t i = 0; i < ( sizeof(date_lookup) / sizeof(date_lookup[0]) ); i++)
      {
         logging_llprintf(LOGLEVEL_DEBUG, "//////////////////////////////////////////////////////////\n");
         logging_llprintf(LOGLEVEL_DEBUG,
                          "// Looking for <FIELD> from <CSV-SOURCE>, that occurred on %s >>>>>\n", date_lookup[i]);
         logging_llprintf(LOGLEVEL_DEBUG, "//////////////////////////////////////////////////////////\n");
         parse_psuedo_csv(date_lookup[i]);
         logging_llprintf(LOGLEVEL_DEBUG, "\n");
      }
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
