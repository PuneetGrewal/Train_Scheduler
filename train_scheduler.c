#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define BILLION 1000000000.0;
struct timespec start;
struct timespec stop;
double accum;


// Linked List Structure used from following sources:
// https://www.geeksforgeeks.org/data-structures/linked-list/
// https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm
// https://www.learn-c.org/en/Linked_lists


typedef struct Train {

    int TIME_TO_LOAD;
    int TIME_TO_CROSS;
    int TRAIN_NUMBER;
    char TRAIN_DIRECTION;
    int priority;
    pthread_cond_t *train_convar;
} train;


typedef struct node {
    train* data;
    int priority;
    struct node* next;
} node;


node* newNode(train* data) {
    node* temp  = (node*)malloc(sizeof(node));
    temp->data = data;
    temp->priority = data->priority;
    temp->next = NULL;
}

int isEmpty(node** head) {
    return (*head) == NULL;
}

pthread_mutex_t track;
pthread_cond_t loaded;

node* WEST_STATION;
node* EAST_STATION;


int n;
int NUMBER_OF_TRAINS;


int priority(char direction) {
    if (direction == 'W' || direction == 'E')
        return 1;
    else
        return 0;
}


int line_number(char *f) {
    FILE *fp = fopen(f, "r");
    char c;
    int count = 0;
    for (c = fgetc(fp); c != EOF; c = fgetc(fp)) {
        if (c == '\n') count++;
    }
    fclose(fp);
    return count;
}



void SETUP_TRAINS(train *trains, char *f, pthread_cond_t *CONDITIONS) {

    for (int i = 0; i < n; i++) {
        pthread_cond_init(&CONDITIONS[i], NULL);
    }

    char TRAIN_DIRECTION[3], TIME_TO_LOAD[3], TIME_TO_CROSS[3];
    int train_number = 0;
    FILE *fp = fopen(f, "r");

    while (fscanf(fp, "%s %s %s", TRAIN_DIRECTION, TIME_TO_LOAD, TIME_TO_CROSS) != EOF) {

        trains[train_number].TIME_TO_LOAD = atoi(TIME_TO_LOAD);
        trains[train_number].TIME_TO_CROSS = atoi(TIME_TO_CROSS);
        trains[train_number].TRAIN_NUMBER = train_number;
        trains[train_number].TRAIN_DIRECTION = TRAIN_DIRECTION[0];
        trains[train_number].priority = priority(TRAIN_DIRECTION[0]);
        trains[train_number].train_convar = &CONDITIONS[train_number];
        train_number++;
        NUMBER_OF_TRAINS++;
    }
    fclose(fp);
}


char* direction_of_train(char dir){
    char* temp;
    if (dir == 'w' || dir == 'W')
        temp = "West";
    else
        temp = "East";
    return temp;
}



void Timer_Start() {
    if(clock_gettime(CLOCK_REALTIME, &start) == -1) {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }
}

void Timer_Stop() {
    if(clock_gettime(CLOCK_REALTIME, &stop) == -1) {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }
}

void print_time() {

    Timer_Stop();

    accum = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / BILLION;
    int minutes = (int)accum/60;
    int hours = (int)accum/(60*60);
    printf("%02d:%02d:%04.1f ", hours, minutes, accum);
}

void *START_PROCESS(void *args) {
    train* ptrain = (train*)args;
    unsigned int loadTime = (ptrain->TIME_TO_LOAD) * 100000;
    usleep(loadTime);
    print_time();
    printf("Train%2d is ready to go %4s\n", ptrain->TRAIN_NUMBER, direction_of_train(ptrain->TRAIN_DIRECTION));
}


void dispatch() {
    int temp = -1;
    while (n) {
        pthread_mutex_lock(&track);

        if (isEmpty(&EAST_STATION) && isEmpty(&WEST_STATION)) {
            pthread_cond_wait(&loaded, &track);
        }
    }
}

int line_number_option2(char *f) {
    FILE *fp = fopen(f, "r");
    char c;
    int count = 0;
    for (c = fgetc(fp); c != EOF; c = fgetc(fp)) {
        if (c == '\n') count++;
    }
    fclose(fp);
    return count;
}


int main(int argc, char *argv[]) {


    n = line_number(argv[1]);
    NUMBER_OF_TRAINS = 0;

    pthread_t main_threads[n];
    pthread_cond_t CONDITIONS[n];

    train *trains = malloc(n * sizeof(*trains));
    SETUP_TRAINS(trains, argv[1], CONDITIONS);

    Timer_Start();

    for (int i = 0; i < n; i++) {
        int temp = pthread_create(&main_threads[i], NULL, START_PROCESS, (void *) &trains[i]);
    }

    sleep(0.01);

    dispatch();

    free(trains);
    pthread_mutex_destroy(&track);
    pthread_cond_destroy(&loaded);
    _Exit(0);
}
