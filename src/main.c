#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <regex.h>
#include <limits.h>
#include "version.h"
#include "common_defs.h"
#include "controllers/app_ctrl.h"
#include "models/app_model.h"

char *search_string[] = {"Date,Ticks,Notes,field1,field2,field3,field4,field5,field6,field7,field8",
                         "2024-01-23 04:31:12,12341,\"aaaddd\",1,1,1,1,1,1,1,8.123\n",
                         "2024-02-23 05:56:32,12342,\"This line in the csv can have commas, and even comma\n",
                         "\n",
                         "seperated numbers, like: 1,2,3,4,5,a string,and,carriage returns, and more numbers: \n",
                         "not a date here though\",1,2,2,2,2,2,2,8.234\n",
                         "2024-03-23 07:17:41,12343,\"1,2,3,4\",1,3,3,3,3,3,3,8.345\n",
                         "2024-04-23 08:11:34,12344,\"some text, no newline\",1,4,4,4,4,4,4,8.456\n",
                         "2024-05-23 12:34:57,12345,\"\n",
                         "some text, but not a newline\n",
                         "\",1,5,5,5,5,5,5,8.567\n",
                         "2024-06-23 14:16:40,12346,\"some text, but not a newline\",1,6,6,6,6,6,6,8.678\n",
                         "2024-07-23 21:37:04,12347,\"some text, but not a newline\",1,7,7,7,7,7,7,8.789\n"};

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
   bool found = false;
   char str_out[SHRT_MAX];  // assume we're limited to an Excel spreadsheet cell
   char re_patt[128];
   int so_ret = REG_NOMATCH;
   regex_t regx;
   regmatch_t pmatch[4]; // we re-use this, so we size to the largest

   /* Build the regex with the timestamp, up to the first quote, then look for an end quote, and group that if found */
   snprintf(re_patt, sizeof(re_patt), "(^%s:[0-9]{2},[0-9]+,\")(.*(\".*$))?", timestamp);

   if (!compile_regex(&regx, re_patt)){
      regfree(&regx);
      return false;
   }

   for(size_t i = 0; i < (sizeof (search_string)/sizeof(search_string[0])); i++)
   {
      so_ret = regexec(&regx, search_string[i], 4, pmatch, 0);
      if (so_ret == 0)
      {
         strncpy(str_out, search_string[i], pmatch[1].rm_eo);
         str_out[pmatch[1].rm_eo] = 0;

         if (pmatch[2].rm_so != -1) {
            /* We have a complete csv line, we're going to drop the notes for cleanliness */
            int so_remain = (pmatch[3].rm_so != -1) ? pmatch[3].rm_so : pmatch[2].rm_so;
            int eo_remain = (pmatch[3].rm_eo != -1) ? pmatch[3].rm_eo : pmatch[2].rm_eo;

            strncpy(str_out + pmatch[1].rm_eo,
                    search_string[i] + so_remain,
                    eo_remain - so_remain);
            /* We're NOT null-terminated here, so we can't use strlen here */
            str_out[eo_remain - so_remain + pmatch[1].rm_eo - 1] = 0;
            found = true;
            break;
         }
         else
         {
            /* Build the regex to find the end of our open quote, grouping the quote and remainder of the line */
            snprintf(re_patt, sizeof(re_patt), "^[^\"]*(\".*$)");

            if (!compile_regex(&regx, re_patt)){
               regfree(&regx);
               return false;
            }

            for (size_t cont = i+1; cont < (sizeof (search_string)/sizeof(search_string[0])); cont++)
            {
               so_ret = regexec(&regx, search_string[cont], 2, pmatch, 0);
               if (so_ret == 0)
               {
                  size_t current_str_end = strlen(str_out);  // did we null terminate properly?
                  strncpy(str_out + current_str_end, /* overwrite the null-term */
                          search_string[cont] + pmatch[1].rm_so,
                          pmatch[1].rm_eo - pmatch[1].rm_so);
                  /* We're NOT null-terminated here, so we can't use strlen here */
                  str_out[current_str_end + pmatch[1].rm_eo - pmatch[1].rm_so - 1] = 0;
                  found = true;
                  break;
               }
            }
            break;
         }
      }
   }
   if (found)
   {
      char  re_read_timestamp[32];
      int t,d1,d2,d3,d4,d5,d6,d7;
      char  notes[10];
      float f8;
      sscanf(str_out, "%[^,]19s,%d,%s,%d,%d,%d,%d,%d,%d,%d,%f",
             re_read_timestamp,&t,notes,&d1,&d2,&d3,&d4,&d5,&d6,&d7,&f8);
      logging_llprintf(LOGLEVEL_DEBUG, "str_out: %s\n", str_out);
      logging_llprintf(LOGLEVEL_DEBUG, "%s,%d,%s,%d,%d,%d,%d,%d,%d,%d,%0.1f\n",
                       re_read_timestamp,t,notes,d1,d2,d3,d4,d5,d6,d7,f8);
   }
   regfree(&regx);
   return found;
}

void temp_test_fscan(void)
{
   char *str_out = "2023-01-13 11:16:40,4";
   char re_read_timestamp[32];
   char alt_tick[32];
   unsigned int t;
   // int read_count = sscanf(str_out, "%s,%s", re_read_timestamp,alt_tick);
   int read_count = sscanf(str_out, "%[^,]*19s,%d", re_read_timestamp,&t);
   logging_llprintf(LOGLEVEL_DEBUG, "read count = %d\n", read_count);

   logging_llprintf(LOGLEVEL_DEBUG, "str_out: %s\n", str_out);
   logging_llprintf(LOGLEVEL_DEBUG, "-------> %s,%d\n",
                    re_read_timestamp,t);
   /*logging_llprintf(LOGLEVEL_DEBUG, "-------> %s,%s\n",
                    re_read_timestamp,alt_tick);*/
}

int main(int argc, char **argv) {
   printf("ver.%d.%d.%d\n", project_VERSION_MAJOR, project_VERSION_MINOR, project_VERSION_PATCH);
   app_init(argc, argv);

   if (getAppModelInitState() != INIT_ERROR)
   {
      logging_llprintf(LOGLEVEL_DEBUG, "application initialized, starting UI\n");

      char *date_lookup[] = {"2024-01-23 04:31",
                             "2024-02-23 05:56",
                             "2024-03-23 07:17",
                             "2024-04-23 08:11",
                             "2024-05-23 12:34",
                             "2024-06-23 14:16",
                             "2024-07-23 21:37"
      };
      for (size_t i = 0; i < ( sizeof(date_lookup) / sizeof(date_lookup[0]) ); i++)
      {
         logging_llprintf(LOGLEVEL_DEBUG, "Looking for %s\n", date_lookup[i]);
         parse_psuedo_csv(date_lookup[i]);
      }

      temp_test_fscan();

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
