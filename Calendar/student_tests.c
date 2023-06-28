#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

typedef struct task_info {
  double cost;
  char *prog_language;
} Task_info;

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

static void free_task_info(void *ptr) {
   Task_info *task_info = (Task_info *)ptr;

   free(task_info->prog_language);
   free(task_info);
}

/* Tests init_calendar edge cases */
static int test1() {
   int days = 7;
   Calendar *cal_1, *cal_2, *cal_3;

   if (init_calendar("Cal 1", days, comp_minutes, free_task_info, &cal_1)
                                                                  == SUCCESS) {
      if (init_calendar("Cal 2", 0, comp_minutes, NULL, &cal_2) == FAILURE) {
         if (init_calendar(NULL, days, comp_minutes, NULL, &cal_3)
                                                                  == FAILURE) {
            return destroy_calendar(cal_1);
         }
      }
   }
    
   return FAILURE;
}

/* Tests print_calendar edge cases */
static int test2() {
   Calendar *calendar;

   init_calendar("Test", 3, comp_minutes, NULL, &calendar);
   if (print_calendar(calendar, NULL, 0) == FAILURE) {
      return destroy_calendar(calendar);
   }

   return FAILURE;
}

/* Tests adding two events of same name */
static int test3() {
   Calendar *calendar;

   init_calendar("Test", 3, comp_minutes, NULL, &calendar);
   add_event(calendar, "zoo", 900, 80, NULL, 1);

   if (add_event(calendar, "zoo", 350, 30, NULL, 2) == FAILURE) {
      return destroy_calendar(calendar);
   }

   return FAILURE;
}

/* Tests add_event edge cases */
static int test4() {
   Calendar *calendar;

   init_calendar("Test", 3, comp_minutes, NULL, &calendar);
   if (add_event(calendar, "meeting", -10, 10, NULL, 1) == FAILURE) {
      if (add_event(calendar, "picnic", 2500, 10, NULL, 1) == FAILURE) {
         if (add_event(calendar, "garden", 1300, -5, NULL, 1) == FAILURE) {
            return destroy_calendar(calendar);
         }
      }
   }

   return FAILURE;
}

/* Tests basic functionality of find_event */
static int test5() {
   Calendar *calendar;
   Event *event;

   init_calendar("Test", 3, comp_minutes, NULL, &calendar);
   add_event(calendar, "meeting", 50, 10, NULL, 1);
   if (find_event(calendar, "meeting", &event) == SUCCESS) {
      if (!strcmp(event->name, "meeting")) {
         return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/* Tests basic functionality of find_event_in_day */
static int test6() {
   Calendar *calendar;
   Event *event;

   init_calendar("Test", 3, comp_minutes, NULL, &calendar);
   add_event(calendar, "meeting", 50, 10, NULL, 1);
   if (find_event_in_day(calendar, "meeting", 1, &event) == SUCCESS) {
      if (!strcmp(event->name, "meeting")) {
         return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/* Tests find_event_in_day when right day is not given */
static int test7() {
   Calendar *calendar;
   Event *event;

   init_calendar("Test", 3, comp_minutes, NULL, &calendar);
   add_event(calendar, "meeting", 50, 10, NULL, 1);
   if (find_event_in_day(calendar, "meeting", 2, &event) == FAILURE) {
         return destroy_calendar(calendar);
   }

   return FAILURE;
}

/* Tests removing element that does not exist */
static int test8() {
   Calendar *calendar;

   init_calendar("Test", 3, comp_minutes, NULL, &calendar);
   add_event(calendar, "zoo", 50, 10, NULL, 1);
   if (remove_event(calendar, "meeting") == FAILURE) {
         return destroy_calendar(calendar);
   }

   return FAILURE;
}

/* Tests removing many elements until empty */
static int test9() {
   Calendar *calendar;

   init_calendar("Test", 3, comp_minutes, NULL, &calendar);
   add_event(calendar, "review meeting", 800, 80, NULL, 1); 
   add_event(calendar, "group meeting", 500, 60, NULL, 1);
   add_event(calendar, "lunch", 1200, 45, NULL, 1);

   if (remove_event(calendar, "review meeting") == SUCCESS) {
      if (remove_event(calendar, "group meeting") == SUCCESS) {
         if (remove_event(calendar, "lunch") == SUCCESS) {
            return destroy_calendar(calendar);
         }
      }
   } 

   return FAILURE;
}

/* Tests basic functionality of get_event_info */
static int test10() {
   Calendar *calendar;

   init_calendar("Test", 3, comp_minutes, NULL, &calendar);
   if (get_event_info(calendar, "meeting") == NULL) {
      add_event(calendar, "meeting", 50, 10, NULL, 1);
      if (get_event_info(calendar, "meeting") == NULL) {
         return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/* Tests edge cases of clear_calendar */
static int test11() {
   Calendar *calendar = NULL;

   if (clear_calendar(calendar) == FAILURE) {
      return SUCCESS;
   }

   return FAILURE;
}

/* Tests edge cases of clear_day */
static int test12() {
   Calendar *calendar = NULL, *real;

   if (clear_day(calendar, 2) == FAILURE) {
      if (clear_day(real, -1) == FAILURE) {
         return SUCCESS;
      }
   }

   return FAILURE;
}

/* Tests edge cases of destroy_calendar */
static int test13() {
   Calendar *calendar = NULL;

   if (destroy_calendar(calendar) == FAILURE) {
      return SUCCESS;
   }

   return FAILURE;
}

int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   if (test1() == FAILURE) result = FAILURE;
   if (test2() == FAILURE) result = FAILURE;
   if (test3() == FAILURE) result = FAILURE;
   if (test4() == FAILURE) result = FAILURE;
   if (test5() == FAILURE) result = FAILURE;
   if (test6() == FAILURE) result = FAILURE;
   if (test7() == FAILURE) result = FAILURE;
   if (test8() == FAILURE) result = FAILURE;
   if (test9() == FAILURE) result = FAILURE;
   if (test10() == FAILURE) result = FAILURE;
   if (test11() == FAILURE) result = FAILURE;
   if (test12() == FAILURE) result = FAILURE;
   if (test13() == FAILURE) result = FAILURE;

   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/
   
   if (result == FAILURE) {
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
