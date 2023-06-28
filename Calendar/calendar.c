/*
Tanav Marupudi
tanav
118051659
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calendar.h"
#include "event.h"
#include "my_memory_checker_216.h"

int init_calendar(const char *name, int days,
                  int (*comp_func) (const void *ptr1, const void *ptr2),
                  void (*free_info_func) (void *ptr), Calendar ** calendar) {
    
    /* Checks cases that cause FAILURE */
    if (!calendar || !name || days < 1) {
        return FAILURE;
    }

    /* Allocates necessary memory */
    *calendar = malloc(sizeof(Calendar));
    (*calendar)->name = malloc(strlen(name) + 1);
    (*calendar)->events = calloc(days, sizeof(Event));

    if (!*calendar || !(*calendar)->name || !(*calendar)->events) {
        return FAILURE;
    }

    /* Assigns values needed to intialize calendar */
    strcpy((*calendar)->name, name);
    (*calendar)->days = days;
    (*calendar)->total_events = 0;
    (*calendar)->comp_func = comp_func;
    (*calendar)->free_info_func = free_info_func;

    return SUCCESS;
}

int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
    int i = 0;
    Event *curr = NULL;

    /* Checks cases that cause FAILURE */
    if (!calendar || !output_stream) {
        return FAILURE;
    }

    /* Handles if print_all is true */
    if (print_all) {
        fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
        fprintf(output_stream, "Days: %d\n", calendar->days);
        fprintf(output_stream, "Total Events: %d\n\n", calendar->total_events);
    }

    /* Prints events */
    fprintf(output_stream, "**** Events ****\n");
    if (calendar->total_events > 0) {
        for (i = 0; i < calendar->days; i++) {
            fprintf(output_stream, "Day %d\n", i + 1);

            /* Iterates through all events on calendar */
            curr = calendar->events[i];
            while (curr) {
                fprintf(output_stream, "Event's Name: \"%s\"", curr->name);
                fprintf(output_stream, ", Start_time: %d", curr->start_time);
                fprintf(output_stream, ", Duration: %d\n",
                                                    curr->duration_minutes);

                curr = curr->next;
            }
        }
    }

    return SUCCESS;
}

int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day) {
    Event *new_event = NULL, *curr = NULL, *prev = NULL;

    /* Checks cases that cause FAILURE */
    if (!calendar || !name || start_time < 0 || start_time > 2400 ||
        duration_minutes <= 0 || day < 1 || day > calendar->days ||
                        find_event(calendar, name, &new_event) == SUCCESS) {
        return FAILURE;
    }

    /* Allocates necessary memory */
    new_event = malloc(sizeof(Event));
    new_event->name = malloc(strlen(name) + 1);

    if (!new_event || !new_event->name) {
        return FAILURE;
    }

    /* Assigns values to new_event */
    strcpy(new_event->name, name);
    new_event->start_time = start_time;
    new_event->duration_minutes = duration_minutes;
    new_event->info = info;

    /* Inserts event into linked list */
    curr = calendar->events[day - 1];
    if (!curr) {
        calendar->events[day - 1] = new_event;
        new_event->next = NULL;
    } else {
        if (calendar->comp_func(curr, new_event) > 0) {
            calendar->events[day - 1] = new_event;
            new_event->next = curr;
        } else {
            while (curr && calendar->comp_func(curr, new_event) <= 0) {
                prev = curr;
                curr = curr->next;
            }

            prev->next = new_event;
            new_event->next = curr;
        }
    }
    (calendar->total_events)++;

    return SUCCESS;
}

int find_event(Calendar *calendar, const char *name, Event **event) {
    int i = 0;
    Event *curr = NULL;

    /* Checks cases that cause FAILURE */
    if (!calendar || !name) {
        return FAILURE;
    }

    /* Iterates through all events looking for name */
    for (i = 0; i < calendar->days; i++) {
        curr = calendar->events[i];
        
        while (curr) {
            if (!strcmp(curr->name, name)) {
                
                /* Only sets event pointer if it is not NULL */
                if (event) {
                    *event = curr;
                }

                return SUCCESS;
            }

            curr = curr->next;
        }
    }

    return FAILURE;
}

int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event) {
    Event *curr = NULL;
    
    /* Checks cases that cause FAILURE */
    if (!calendar || !name || day < 1 || day > calendar->days) {
        return FAILURE;
    }

    /* Iterates through all events in day looking for name */
    curr = calendar->events[day - 1];
    while (curr) {
        if (!strcmp(curr->name, name)) {
                
            /* Only sets event pointer if it is not NULL */
            if (event) {
                *event = curr;
            }

            return SUCCESS;
        }

        curr = curr->next;
    }

    return FAILURE;
}

int remove_event(Calendar *calendar, const char *name) {
    int i = 0;
    Event *curr = NULL, *prev = NULL, *found = NULL;

    /* Checks cases that cause FAILURE */
    if (!calendar || !name || find_event(calendar, name, &found) == FAILURE) {
        return FAILURE;
    }

    /* Iterates through all events and removes event with passed in name */
    for (i = 0; i < calendar->days; i++) {
        curr = calendar->events[i];

        if (!curr) {
            continue;
        }

        /* Removes event from linked list */
        if (!strcmp(curr->name, name)) {
            calendar->events[i] = curr->next;
            curr->next = NULL;
            found = curr;
        } else {
            while (curr && strcmp(curr->name, name)) {
                prev = curr;
                curr = curr->next;
            }

            prev->next = curr->next;
            curr->next = NULL;
            found = curr;
        }
    }

    /* Frees memory from removed event */
    free(found->name);
    
    if (found->info && calendar->free_info_func) {
        calendar->free_info_func(found->info);
    }
    (calendar->total_events)--;

    free(found);

    return SUCCESS;
}

void *get_event_info(Calendar *calendar, const char *name) {
    Event *found;

    /* Returns info field from event with matching name parameter */
    if (find_event(calendar, name, &found) == SUCCESS) {
        return found->info;
    }

    return NULL;
}

int clear_calendar(Calendar *calendar) {
    int i = 0;

    /* Checks case that causes FAILURE */
    if (!calendar) {
        return FAILURE;
    }

    /* Iterates through each day and passes to clear_day */
    for (i = 0; i < calendar->days; i++) {
        clear_day(calendar, i + 1);
    }

    return SUCCESS;
}

int clear_day(Calendar *calendar, int day) {
    Event *curr = NULL, *prev = NULL;

    /* Checks cases that cause FAILURE */
    if (!calendar || day < 1 || day > calendar->days) {
        return FAILURE;
    }

    /* Clears all events on a given day */
    curr = calendar->events[day - 1];
    while (curr) {
        free(curr->name);

        prev = curr;
        curr = curr->next;

        if (prev->info && calendar->free_info_func) {
            calendar->free_info_func(prev->info);
        }
        (calendar->total_events)--;

        free(prev);
    }
    calendar->events[day - 1] = NULL;

    return SUCCESS;
}

int destroy_calendar(Calendar *calendar) {

    /* Checks case that causes FAILURE */
    if (!calendar) {
        return FAILURE;
    }

    /* Frees in specific order to ensure all fields can be accessed */
    free(calendar->name);
    clear_calendar(calendar);
    free(calendar->events);
    free(calendar);

    return SUCCESS;
}