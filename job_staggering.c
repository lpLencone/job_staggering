#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct job {
    int deadline;
    int fine;
};

void sample_jobs(struct job jobs[], int n);
void print_jobs(struct job jobs[], int n);
void stagger_jobs(struct job jobs[], int n);

int main(void)
{
    int n;
    printf("# of jobs: ");
    fflush(stdout);
    scanf("%d", &n);
    printf("\n");

    struct job *jobs = (struct job *)malloc(n * sizeof(struct job));
    sample_jobs(jobs, n);

    printf("Before staggering jobs:\n");
    print_jobs(jobs, n);

    stagger_jobs(jobs, n);

    printf("\nAfter:\n");
    print_jobs(jobs, n);

    free(jobs);
    return 0;
}

void stagger_jobs(struct job jobs[], int n)
{
    /** Initialize 'job_state' vector, where it will be stored
        the status of the jobs (whether their position has already been set,
        mapping to 1 and 0), so that a job that was set will not
        be selected to occupy another position. */
    int *job_state = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        job_state[i] = 0; // job position not set
    }

    /** Auxiliary variables that will store information about the last selected 
        job to be at a certain position in the array of jobs */
    int biggest_fine;
    int longest_deadline;
    int swap_i;

    // Loop that will stagger the jobs
    for (int i = n - 1; i >= 0; i--) {
        int compatible_job = 0;
        biggest_fine = jobs[i].fine;
        longest_deadline = jobs[i].deadline;
        swap_i = i;

        // Loop that looks for candidate jobs to occupy position 'i' in the jobs array
        for (int j = 0; j < n; j++) {
            /** jobs[j].fine >= biggest_fine: if job fine in 'j' is greater than or equal (to include itself) 
                than the largest fine found so far */
            /** (jobs[i].deadline < i + 1 && swap_i == i): if the job deadline at 'i' is past due
                 and a job has not yet been found to replace him at that position */
            // jobs[j].deadline >= i + 1: if the job deadline at 'j' is not expired at position 'i'
            // !job_state[j] checks that the job at j has not been assigned a position
            if ((jobs[j].fine >= biggest_fine || (jobs[i].deadline < i + 1 && swap_i == i)) && 
                 jobs[j].deadline >= i + 1 && !job_state[j])
            {
                biggest_fine = jobs[j].fine;            /* save jobs[j] */
                longest_deadline = jobs[j].deadline;    /* features     */

                swap_i = j;         // job to be swapped is at j
                compatible_job = 1; // found compatible job
            }
        }
        // If a compatible job was found to occupy position 'i'
        if (compatible_job) {
            int deadline_tmp = jobs[i].deadline;
            int fine_tmp = jobs[i].fine;
            jobs[i].deadline = jobs[swap_i].deadline;
            jobs[i].fine = jobs[swap_i].fine;
            jobs[swap_i].deadline = deadline_tmp;
            jobs[swap_i].fine = fine_tmp;
            job_state[i] = 1; // job set
        }
    }
    free(job_state);
}

void print_jobs(struct job jobs[], int n)
{
    int fine = 0;
    for (int i = 0; i < n; i++) {
        printf("%-2d {deadline %2d : %-2d fine}", i + 1, jobs[i].deadline, jobs[i].fine);
        if (jobs[i].deadline < i + 1) {
            fine += jobs[i].fine;
            printf(" << fine!");
        }
        printf("\n");
    }
    printf("Total fine: %d\n", fine);
}

void sample_jobs(struct job jobs[], int n)
{
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        jobs[i].deadline = 1 + rand() % n;
        jobs[i].fine = 1 + rand() % n;
    }
}