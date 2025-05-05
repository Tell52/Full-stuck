#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERS 100
#define MAX_NAME_LEN 50
#define MAX_EMAIL_LEN 50
#define MAX_PLATE_LEN 20
#define USERS_FILE "users.csv"
#define INVENTORY_FILE "inventory.csv"

typedef enum { NORMAL, SUB_MEMBER, ADMIN } UserType;

typedef struct {
    char name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    char vehicle_plate[MAX_PLATE_LEN];
    UserType type;
    int member_points;
    float discount_rate;
} User;

typedef struct Part {
    char name[50];
    int quantity;
    float price;
    struct Part* left;
    struct Part* right;
} Part;

typedef struct Appointment {
    User* user;
    char service[50];
    float cost;
    bool is_priority;
    struct Appointment* next;
} Appointment;

typedef struct {
    Appointment* normal_front;
    Appointment* normal_rear;
    Appointment* priority_front;
    Appointment* priority_rear;
    int normal_count;
    int priority_count;
} PriorityQueue;

typedef struct {
    float total_income;
} IncomeStats;

#endif