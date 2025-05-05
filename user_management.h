#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H
#include"definitions.h"

extern User* users_db[MAX_USERS];
extern int user_count;

User* find_user(const char* email);
User* Admin(const char* email);
User* login();
void registers();
void load_users_from_csv();
void save_users_to_csv();

#endif