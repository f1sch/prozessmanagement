/*
 * Dieses Programm liest vom Benutzer auf dem Terminal eingegebene
 * Zahlen ein und speichert diese in einer doppelt verketteten Liste
 * in geordneter Reihenfolge.
 * Eingaben, die keine Zahlen sind werden igoriert.
 * Die Liste wird mit Bubblesort geordnet, wobei die Zeiger auf die Listen-
 * elemente getauscht werden.
 * */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>

#define MAX_NUMBERS 1024

typedef struct node {
    int val;
    struct node *next;
    struct node *prev;
} node_t;

node_t* create_node(int);
void insert_at_head(int);
void print_nodes(void);
void swap(node_t**, node_t*);
void bubble_sort(void);
void cleanup_memory(void);
void handle_sigint(int);
int list_contains(int);
int is_number(const char*);


node_t *g_head = NULL;
node_t *g_tail = NULL;
int g_memory_cleaned = 0;
int g_size_of_list = 0;

int main() {
    char input[MAX_NUMBERS];

    atexit(cleanup_memory);
    signal(SIGINT, handle_sigint);

    // zeile lesen
    while (1) {
        printf("Eingabe: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        // Zeilenumbruch entfernen
        input[strcspn(input, "\n")] = '\0';

        // Tokenisieren und Zahlen speichern
        char *token = strtok(input, " ");
        while (token != NULL) {
            if (is_number(token)) {
                int num = atoi(token);
                if (num > 0) {
                    // in liste einfügen
                    insert_at_head(num);
                }
            }
            token = strtok(NULL, " ");
        }
        
        // liste sortieren
        bubble_sort();
        
        // liste ausgeben
        print_nodes();
        
    }
    
    return 0;
}

node_t* create_node(int value) {
    node_t* new_node 
        = (node_t*) malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("create_node malloc\n");
        exit(EXIT_FAILURE);
    }
    new_node->val = value;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}
void insert_at_head(int value) {
    if (list_contains(value))
        return;
    node_t* new_node = create_node(value);
    if (g_head == NULL) {
        g_head = new_node;
        g_tail = new_node;
    } else {
        new_node->next = g_head;
        g_head->prev = new_node;
        g_head = new_node;
    }
    g_size_of_list++;
}
void print_nodes(void) {
    node_t *current = g_head;
    printf("Ausgabe:");
    while (current != NULL) {
        printf(" %d", current->val);
        current = current->next;
    }
    printf("\n");
}

void swap(node_t** a, node_t* b) {
    if ((*a) == NULL || b == NULL || (*a) == b) return;

    if (g_size_of_list == 2) {
        (*a)->prev = b;
        (*a)->next = NULL;
        (*b).next = (*a);
        (*b).prev = NULL;
        node_t* temp = *a;
        g_head = b;
        g_tail = temp;
        return;
    }
    (*b).prev = (*a)->prev;
    (*a)->prev = b;
    (*a)->next = (*b).next;
    (*b).next = *a;
    if (b == g_tail) {
        g_tail = (*a);
        b->prev->next = b;
    } else {
        (*a)->next->prev = *a;
        if (*a == g_head) {
            g_head = b;
        } else {
            b->prev->next = b;
        }
    }
}

void bubble_sort(void) {
    if (g_size_of_list < 2) return;
    int swapped;
    node_t** left_node;
    node_t* right_node = NULL;
    do {
        swapped = 0;
        left_node = &g_head;
        while ((*left_node) != right_node && (*left_node != g_tail)) {
            if ((*left_node)->val > (*left_node)->next->val) {
                swap(left_node, (*left_node)->next);
                swapped = 1;
            }
            // increment
            if (g_size_of_list == 2) return;
            left_node = &(*left_node)->next;
        }
        // increment
        right_node = *left_node;
    } while (swapped);
}

void cleanup_memory(void) {
    if (g_memory_cleaned) {
        return;
    }

    g_memory_cleaned = 1;
    
    // Speicher für Liste freigeben
    printf("\n==================\n");
    printf("Cleaning up memory");
    printf("\n==================\n");
    node_t* current = g_head;
    while (current != NULL) {
        node_t* next = current->next;
        free(current);
        current = next;
    }
    g_head = NULL;
    exit(EXIT_SUCCESS);
}

void handle_sigint(int sig) {
    cleanup_memory();
}

int list_contains(int num) {
    node_t* current = g_head;
    while (current != NULL) {
        if (current->val == num) {
            return 1; // zahl gefunden
        }
        current = current->next;
    }
    return 0;
}

int is_number(const char *str) {
    if (*str == '\0') {
        return 0;
    }
    
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}
