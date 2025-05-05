#ifndef MENU_FUNCTIONS_H
#define MENU_FUNCTIONS_H

#include"definitions.h"

extern PriorityQueue appointment_queue;
extern IncomeStats income;

void collect_parts_to_array(Part* root, Part* parts_array[], int* count);

void mainmenu();
void usermenu(User* user);
void book_service(User* user);
void buy_parts(User* user);
void view_queue(User* user);
void admin_menu();

void enqueue(PriorityQueue* queue, User* user, const char* service, float cost, bool is_priority);
Appointment* dequeue(PriorityQueue* queue, bool priority_only);
void display_queue(PriorityQueue* queue);
void save_appointments_to_csv();
void load_appointments_from_csv();
void cancel_queue(User* user);
void DoTasks();

#endif