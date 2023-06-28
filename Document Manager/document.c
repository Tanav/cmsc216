/*
Tanav Marupudi
118051659
tanav
*/

#include <stdio.h>
#include <string.h>
#include "document.h"

void replace_all(Document *doc, const char *target, const char *replacement);
void string_replace(char *line, const char *replacement, const char *start,
                                                                int length);

int init_document(Document *doc, const char *name) {

    /* Checking cases that result in FAILURE */
    if (doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE) {
        return FAILURE;
    }

    /* Sets document to be empty with proper name */ 
    strcpy(doc->name, name);
    doc->number_of_paragraphs = 0;

    return SUCCESS;
}

int reset_document(Document *doc) {

    /* Checking cases that result in FAILURE */
    if (doc == NULL) {
        return FAILURE;
    }

    /* Resetting data in document */
    doc->number_of_paragraphs = 0;

    return SUCCESS;
}

int print_document(Document *doc) {
    int i = 0, j = 0;

    /* Checking cases that result in FAILURE */
    if (doc == NULL) {
        return FAILURE;
    }

    /* Prints information */
    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            printf("%s\n", doc->paragraphs[i].lines[j]);
        }

        if (i == doc->number_of_paragraphs - 1 || 
                                doc->paragraphs[i].number_of_lines != 0) {
            printf("\n");
        }
    }

    return SUCCESS;
}

int add_paragraph_after(Document *doc, int paragraph_number) {
    int i = 0;

    /* Checking cases that result in FAILURE */
    if (doc == NULL || doc->number_of_paragraphs >= MAX_PARAGRAPHS
                    || paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }

    /* Add paragraph to front if doc is empty OR adds to end of array 
    Moves elements to the right and inserts in array if otherwise */
    if ((doc->number_of_paragraphs == 0 && paragraph_number == 0)
                            || doc->number_of_paragraphs == paragraph_number) {
        doc->paragraphs[paragraph_number].number_of_lines = 0;
    } else {
        for (i = doc->number_of_paragraphs - 1; i >= paragraph_number; i--) {
            doc->paragraphs[i + 1] = doc->paragraphs[i];
        }
        doc->paragraphs[paragraph_number].number_of_lines = 0;
    }

    /* Increment number_of_paragraphs */
    doc->number_of_paragraphs++;

    return SUCCESS;
}

int add_line_after(Document *doc, int paragraph_number, int line_number,
		    const char *new_line) {
    int i = 0;
    Paragraph *para = NULL;

    /* Checking cases that result in FAILURE */
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs
                    || doc->paragraphs[paragraph_number - 1].number_of_lines
                    >= MAX_PARAGRAPH_LINES || new_line == NULL
                    || line_number < 0 || line_number > 
                    doc->paragraphs[paragraph_number - 1].number_of_lines) {
        return FAILURE;
    }

    /* Stores wanted paragraph index in pointer */
    para = &doc->paragraphs[paragraph_number - 1];

    /* Add line to front if doc is empty OR adds to end of array 
    Moves elements to the right and inserts in array if otherwise */
    if ((para->number_of_lines == 0 && line_number == 0) ||
                                    para->number_of_lines == line_number) {
        strcpy(para->lines[line_number], new_line);
    } else {
        for (i = para->number_of_lines - 1; i >= line_number; i--) {
            strcpy(para->lines[i + 1], para->lines[i]);
        }
        strcpy(para->lines[line_number], new_line);
    }

    /* Increment number_of_lines */
    doc->paragraphs[paragraph_number - 1].number_of_lines++;

    return SUCCESS;
}

int get_number_lines_paragraph(Document *doc, int paragraph_number,
            int *number_of_lines) {

    /* Checking cases that result in FAILURE */
    if (doc == NULL || number_of_lines == NULL
                    || paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;        
    }

    /* Sets number_of_lines pointer */
    *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;

    return SUCCESS;
}

int append_line(Document *doc, int paragraph_number, const char *new_line) {
    int line_index = 0;

    /* Checking cases that result in FAILURE */
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs
                    || doc->paragraphs[paragraph_number - 1].number_of_lines
                    >= MAX_PARAGRAPH_LINES || new_line == NULL) {
        return FAILURE;
    }

    /* Find index to append line at */
    line_index = doc->paragraphs[paragraph_number - 1].number_of_lines;

    /* Copies line into proper index */
    strcpy(doc->paragraphs[paragraph_number - 1].lines[line_index], new_line);

    /* Increment number_of_lines */
    doc->paragraphs[paragraph_number - 1].number_of_lines++;

    return SUCCESS;
}

int remove_line(Document *doc, int paragraph_number, int line_number) {
    int i = 0, line_count = 0;

    /* Checking cases that result in FAILURE */
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
                    line_number < 0 || line_number >
                    doc->paragraphs[paragraph_number - 1].number_of_lines) {
        return FAILURE;
    }

    /* Removes last line in array or shifts all elements right */
    if (doc->paragraphs[paragraph_number - 1].number_of_lines == line_number) {
        strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number - 1], "");
    } else {
        line_count = doc->paragraphs[paragraph_number - 1].number_of_lines;

        for (i = line_number - 1; i < line_count - 1; i++) {
            strcpy(doc->paragraphs[paragraph_number - 1].lines[i],
                    doc->paragraphs[paragraph_number - 1].lines[i + 1]);
        }
    }

    /* Decrement number_of_lines */
    doc->paragraphs[paragraph_number - 1].number_of_lines--;

    return SUCCESS;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1],
            int data_lines) {
    int i = 0, paragraph_index = 0;

    /* Checking cases that result in FAILURE */
    if (doc == NULL || data == NULL || data_lines == 0) {
        return FAILURE;
    }

    /* Sets index */
    paragraph_index = doc->number_of_paragraphs;

    /* Adds paragraph if document is empty */
    if (doc->number_of_paragraphs == 0) {
        add_paragraph_after(doc, 0);
        paragraph_index++;
    }

    /* Loads document according to specifications */
    for (i = 0; i < data_lines; i++) {
        if (strcmp(data[i], "") == 0) {
            add_paragraph_after(doc, doc->number_of_paragraphs);
            paragraph_index++;
        } else {
            append_line(doc, paragraph_index, data[i]);
        }
    }

    return SUCCESS;
}

int replace_text(Document *doc, const char *target, const char *replacement) {

    /* Checking cases that result in FAILURE */
    if (doc == NULL || target == NULL || replacement == NULL) {
        return FAILURE;
    }

    /* Calling helper function to replace all throughout document */
    replace_all(doc, target, replacement);

    return SUCCESS;
}

int highlight_text(Document *doc, const char *target) {
    char replacement[MAX_STR_SIZE] = {""};

    /* Checking cases that result in FAILURE */
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }

    /* Setting replacement equal to target + highlights */
    strcpy(replacement, HIGHLIGHT_START_STR);
    strcat(replacement, target);
    strcat(replacement, HIGHLIGHT_END_STR);

    /* Calling helper function to replace all throughout document */
    replace_all(doc, target, replacement);

    return SUCCESS;
}

int remove_text(Document *doc, const char *target) {

    /* Checking cases that result in FAILURE */
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }

    /* Calling helper function to replace all throughout document */
    replace_all(doc, target, "");

    return SUCCESS;
}

/* Iterates through entire document and replaces all
instances of string target with string replacement */
void replace_all(Document *doc, const char *target, const char *replacement) {
    int i = 0, j = 0, length = strlen(target);
    char *start = '\0';
    Paragraph *para = NULL;

    /* Iterates through all paragraphs and lines in document */
    for (i = 0; i < doc->number_of_paragraphs; i++) {
        /* Updates pointer value to current paragraph  */
        para = &doc->paragraphs[i];

        for (j = 0; j < para->number_of_lines; j++) {
            /* Sets start equal to index where target is found */
            start = strstr(para->lines[j], target);

            /* Continues to replace until target is no longer in given line */
            while (start != NULL && start - para->lines[j] < MAX_STR_SIZE
                                            && start - para->lines[j] > -1) {
                string_replace(para->lines[j], replacement, start, length);
                start = strstr(start + strlen(replacement), target);
            }
        }
    }
}

/* Replaces substring with replacement given a line */
void string_replace(char *line, const char *replacement, const char *start,
                                                                int length) {
    int index = start - line;
    char result[MAX_STR_SIZE] = {""}, temp[MAX_STR_SIZE] = {""};
    
    /* Creates result string */
    strncpy(result, line, index);
    strcat(result, replacement);
    strncpy(temp, line + index + length, strlen(line));
    strcat(result, temp);

    /* Copies result to replace the old line */
    strcpy(line, result);
}