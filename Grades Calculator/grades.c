/*
Tanav Marupudi
118051659
tanav
*/

#include <stdio.h>
#include <math.h>

/* max array length is 50 and maximum is greater than 100*100 which
is necessary as it needs to be greater than any possible assignment value */
#define ARR_LENGTH 50
#define MAXIMUM 10001

double calc_num_score(int scores[], int weights[], int days_late[],
                        int per_day_minus, int drop_size, int size);
void calc_stats(int scores[], int days_late[], int per_day_minus, int size);
void sort_array(int array[], int size);
void swap(int* one, int* two);

int main() {
    int penalty, dropped_count, assignment_count, i, index, total_weight = 0;
    char stats_flag;
    int number_arr[ARR_LENGTH], score_arr[ARR_LENGTH], weight_arr[ARR_LENGTH],
        days_late_arr[ARR_LENGTH];

    /* reading values from input into variables */
    scanf(" %d %d %c", &penalty, &dropped_count, &stats_flag);
    scanf(" %d", &assignment_count);

    /* iterates through each assignment line of input and
    sets values into array. sorts by assignment number */
    for (i = 0; i < assignment_count; i++) {
        int number, score, weight, days_late;
        
        scanf(" %d, %d, %d, %d", &number, &score, &weight, &days_late);
        total_weight += weight;
        index = number - 1;

        number_arr[index] = number;
        score_arr[index] = score;
        weight_arr[index] = weight;
        days_late_arr[index] = days_late;
    }

    /* exits code and returns error message if weight does not equal 100 */
    if (total_weight != 100) {
        printf("ERROR: Invalid values provided\n");
        return -1;
    }

    /* prints all information aside from assignment details */
    printf("Numeric Score: %5.4f\n", calc_num_score(score_arr, weight_arr,
                                        days_late_arr, penalty,
                                        dropped_count, assignment_count));
    printf("Points Penalty Per Day Late: %d\n", penalty);
    printf("Number of Assignments Dropped: %d\n", dropped_count);
    printf("Values Provided:\nAssignment, Score, Weight, Days Late\n");

    /* iterates and prints each assignment in individual lines */
    for (i = 0; i < assignment_count; i++) {
        printf("%d, %d, %d, %d\n", number_arr[i], score_arr[i],
                                    weight_arr[i], days_late_arr[i]);
    }

    /* only prints mean and sd if specified by input having a Y/y */
    if (stats_flag == 'Y' || stats_flag == 'y') {
        calc_stats(score_arr, days_late_arr, penalty, assignment_count);
    }

    return 0;
}

/* calculates numeric score given using parameters for all scores, all weights
all days late, the late penalty, amount of assignments dropped, and size */
double calc_num_score(int scores[], int weights[], int days_late[],
                        int per_day_minus, int drop_size, int size) {
    double numeric_score = 0, total_weight = 0;
    int i, count = 0, min = MAXIMUM, min_index, value;
    /* map is used to track which indices have had already had a min value */
    int drop_indices[ARR_LENGTH], map[ARR_LENGTH];

    /* need to initialize each index to a value that does not overlap with
    the range between i and size. ARR_LENGTH fulfils these requirments */
    for (i = 0; i < ARR_LENGTH; i++) {
        drop_indices[i] = ARR_LENGTH;
    }

    while (count < drop_size) {
        for (i = 0; i < size; i++) {
            /* skips iterations where map has been altered to
            prevent the same assignment from being dropped */
            if (map[i] == -1) {
                continue;
            }

            /* finds lowest value in array and saves the index of it */
            value = scores[i] * weights[i];
            if (value < min) {
                min = value;
                min_index = i;
            }
        }
        /* record where the min value has been pulled from */
        map[min_index] = -1;

        /* adds the min index to drop_indices and sets the loop variables */
        drop_indices[count] = min_index;
        min = MAXIMUM;
        count++;
    }

    /* need to sort drop_indices for loop below to drop assignments properly */
    sort_array(drop_indices, drop_size);
    count = 0;

    for (i = 0; i < size; i++) {
        /* drop_indices being sorted allows the proper iterations of the
        loop being skipped so they are not taken into calculations */
        if (i == drop_indices[count]) {
            count++;
            continue;
        }

        /* does not perform calculation if late days
        and penalty would drop score below or to 0 */
        if (scores[i] - (days_late[i] * per_day_minus) > 0) {
            numeric_score += (scores[i] - (days_late[i] * per_day_minus)) *
                                (double) (weights[i] / 100.0);
        }

        /* need to find total weight after assignments
        get dropped to then divide by it */
        total_weight += (double) weights[i];
    }
    numeric_score /= (total_weight / 100.0);

    return numeric_score;
}

/* calculates mean and standard deviation using four parameters
for all scores, all days late, the per day penalty, and array size */
void calc_stats(int scores[], int days_late[], int per_day_minus, int size) {
    double mean = 0, sd = 0, denominator = size, temp;
    int i;
    
    for (i = 0; i < size; i++) {
        if (scores[i] - (days_late[i] * per_day_minus) > 0) {
            mean += scores[i] - (days_late[i] * per_day_minus);
        }
    }
    mean /= denominator;

    for (i = 0; i < size; i++) {
        temp = ((double) (scores[i] - (days_late[i] * per_day_minus))) - mean;
        /* accounts for late penalty dropping assignment score below 0 */
        if (scores[i] - (days_late[i] * per_day_minus) < 0) {
            temp = mean;
        }
        sd += pow(temp, 2);
    }
    sd /= denominator;
    sd = sqrt(sd);

    /* does not return anything. prints the mean and sd in method */
    printf("Mean: %5.4f, Standard Deviation: %5.4f\n", mean, sd);
}

/* uses bubble sort algorithm to sort array.
takes array and size as parameters */
void sort_array(int array[], int size) {
    int i, j;

    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                swap(&array[j], &array[j + 1]);
            }
        }
    }
}

/* swaps two int values passed in using pointers */
void swap(int* one, int* two) {
    int temp = *one;
    *one = *two;
    *two = temp;
}