/*
 * Dieses Programm veranschaulicht die Reduzierung der Rechenzeit eines
 * Programmes durch Nutzen von Threads.
 * Das Programm berechnet das Matrix Vektor Produkt y = A · ~x mit ~x, ~y ∈ Rn, 
 * A ∈ Rn×n und n ∈ N +
 * n ist eine vom Benutzer beim Programmaufruf als Parameter angegebene gerade Zahl.
 *
 * Hier werden zwei Threads erzeugt, die jeweils eine Hälfte von ~y berechnen und
 * somit die bnötigte Rechenzeit verkürzen.
*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define MAX_THREADS 2

/* Struct with Data for the Threads */
typedef struct {
    int thread_id;
    int n;
    double **a;
    double *x;
    double *y;
} ThreadData;

/* Calculate the Matrix Vector Product */
void *matVecMul(void *arg) {
    ThreadData *data = arg;
    int thread_id = data->thread_id;
    int n = data->n;
    double **a = data->a;
    double *x = data->x;
    double *y = data->y;
    
    int start = (thread_id * n) / MAX_THREADS;
    int end = ((thread_id + 1) * n) / MAX_THREADS;

    /* Calculate Vector y */    
    for(int i = start; i < end; i++) {
        y[i] = 0.0;
        for(int j = 0; j < n; j++) {
            y[i] += a[i][j] * x[j];
        }
    }
    pthread_exit(NULL);
}
int main(int argc, char const *argv[])
{
    int n = atoi(argv[1]);
    if (n % 2 != 0) {
        printf("n muss gerade sein.\n");
        return 1;
    }

    /* Initialize Variables */
    double **a = (double **) malloc(n * sizeof(double *));
    double *x = (double *) malloc(n * sizeof(double));
    double *y = (double *) malloc(n * sizeof(double));
    
    a[0] = (double *) malloc(n * n * sizeof(double *));

    for(int i = 0; i < n; i++)
        a[i] = (double *) malloc(n * sizeof(double));

    /* Fill the Matrix */
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            a[i][j] = i*n + j + 1.0;
        }
    }

    /* Fill Vector x */
    for(int i = 0; i < n; i++)
        x[i] = i + 1.0;
 
    pthread_t threads[MAX_THREADS];
    ThreadData thread_data[MAX_THREADS];

    for(int i = 0; i < MAX_THREADS; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].n = n;
        thread_data[i].a = a;
        thread_data[i].x = x;
        thread_data[i].y = y;
    }
    /* Create Threads and calculate Product */
    for(int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&threads[i], NULL, matVecMul,
         (void *) &thread_data[i]);
    }

    /* Wait for every Thread to finish */
    for(int i = 0; i < MAX_THREADS; i++)
        pthread_join(threads[i], NULL);

    /* Print vector y to Terminal */
    for(int i = 0; i < n; i++) {
        printf("%f ", y[i]);
    }
    printf("\n");

    /* Free Memory */
    for(int i = 0; i < n; i++)
        free(a[i]);

    free(a);
    free(x);
    free(y);

    return 0;
}
