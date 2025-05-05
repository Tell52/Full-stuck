#include"user_management.h"
#include"definitions.h"

User* users_db[MAX_USERS] = {NULL};
int user_count = 0;

User* find_user(const char* email){
    for(int i = 0; i < MAX_USERS; i++) {
        if(users_db[i] && strcmp(users_db[i]->email, email) == 0){
            return users_db[i];
        }
    }
    return NULL;
}

User* Admin(const char* email){
    FILE* file = fopen("admin.csv", "r");
    if(!file){
        printf("Admin database not found.\n");
        return NULL;
    }
    char line[256];
    fgets(line, sizeof(line), file);
    while(fgets(line, sizeof(line), file)){
        char admin_email[MAX_EMAIL_LEN];
        char admin_name[MAX_NAME_LEN];
        if(sscanf(line, "%[^,],%[^\n]", admin_email, admin_name) >= 1){
            if(strcmp(admin_email, email) == 0){
                fclose(file);
                User* admin_user = (User*)malloc(sizeof(User));
                if(!admin_user){
                    printf("Memory allocation failed.\n");
                    return NULL;
                }
                strncpy(admin_user->email, admin_email, MAX_EMAIL_LEN-1);
                admin_user->email[MAX_EMAIL_LEN-1] = '\0';
                if(admin_name[0] != '\0'){
                    strncpy(admin_user->name, admin_name, MAX_NAME_LEN-1);
                }
                else{
                    strncpy(admin_user->name, "Admin", MAX_NAME_LEN-1);
                }
                admin_user->name[MAX_NAME_LEN-1] = '\0';
                admin_user->type = ADMIN;
                admin_user->member_points = 0;
                admin_user->discount_rate = 0.0f;
                strncpy(admin_user->vehicle_plate, "ADMIN", MAX_PLATE_LEN-1);
                admin_user->vehicle_plate[MAX_PLATE_LEN-1] = '\0';
                return admin_user;
            }
        }
    }
    fclose(file);
    return NULL;
}

User* login(){
    printf("\n=== Login ===\n");
    printf("1. User Login\n2. Admin Login\nChoice: ");
    int login_type;
    if(scanf("%d", &login_type) != 1){
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return NULL;
    }
    while (getchar() != '\n');
    char email[MAX_EMAIL_LEN];
    printf("Enter email: ");
    if(fgets(email, MAX_EMAIL_LEN, stdin) == NULL){
        printf("Error reading input.\n");
        return NULL;
    }
    email[strcspn(email, "\n")] = '\0';
    if (login_type == 1) {
        User* user = find_user(email);
        if (!user) {
            printf("User not found. Please register first.\n");
            return NULL;
        }
        printf("Welcome back, %s! (%s)\n", user->name, 
               user->type == SUB_MEMBER ? "Sub-Member" : "Normal Member");
        return user;
    }
    else if(login_type == 2){
        User* admin = Admin(email);
        if(!admin){
            printf("Admin credentials not found.\n");
            return NULL;
        }
        printf("Welcome, %s (Admin)!\n", admin->name);
        return admin;
    }
    else{
        printf("Invalid login type.\n");
        return NULL;
    }
}

void registers(){
    if(user_count >= MAX_USERS){
        printf("Maximum user capacity reached.\n");
        return;
    }
    User* new_user = (User*)malloc(sizeof(User));
    if(!new_user){
        printf("Memory allocation failed.\n");
        return;
    }
    printf("\n=== Registration ===\n");
    printf("Enter name: ");
    if(fgets(new_user->name, MAX_NAME_LEN, stdin) == NULL){
        free(new_user);
        return;
    }
    new_user->name[strcspn(new_user->name, "\n")] = '\0';
    printf("Enter email: ");
    if(fgets(new_user->email, MAX_EMAIL_LEN, stdin) == NULL){
        free(new_user);
        return;
    }
    new_user->email[strcspn(new_user->email, "\n")] = '\0';
    if(find_user(new_user->email)){
        printf("Email already registered.\n");
        free(new_user);
        return;
    }
    printf("Enter vehicle plate: ");
    if(fgets(new_user->vehicle_plate, MAX_PLATE_LEN, stdin) == NULL){
        free(new_user);
        return;
    }
    new_user->vehicle_plate[strcspn(new_user->vehicle_plate, "\n")] = '\0';
    printf("Select user type:\n1. Normal Member\n2. Sub-Member\nChoice: ");
    int type_choice;
    if(scanf("%d", &type_choice) != 1){
        printf("Invalid input.\n");
        free(new_user);
        while (getchar() != '\n');
        return;
    }
    while(getchar() != '\n');
    switch(type_choice){
        case 1: 
            new_user->type = NORMAL;
            new_user->discount_rate = 0.0f;
            break;
        case 2: 
            new_user->type = SUB_MEMBER;
            new_user->discount_rate = 0.1f;
            break;
        default:
            printf("Invalid type. Setting to Normal Member.\n");
            new_user->type = NORMAL;
            new_user->discount_rate = 0.0f;
    }
    new_user->member_points = 0;
    for(int i = 0; i < MAX_USERS; i++){
        if(users_db[i] == NULL){
            users_db[i] = new_user;
            user_count++;
            printf("Registration successful as %s!\n", 
                  (new_user->type == SUB_MEMBER) ? "Sub-Member" : "Normal Member");
            return;
        }
    }
    free(new_user);
    printf("Registration failed - no space in database.\n");
}

void load_users_from_csv(){
    FILE* file = fopen(USERS_FILE, "r");
    if(!file){
        printf("No existing user data found. Starting fresh.\n");
        return;
    }
    char line[256];
    fgets(line, sizeof(line), file);
    while(fgets(line, sizeof(line), file)){
        User* user = (User*)malloc(sizeof(User));
        if(!user) continue;
        if(sscanf(line, "%[^,],%[^,],%[^,],%d,%d,%f",
                   user->name,
                   user->email,
                   user->vehicle_plate,
                   &user->type,
                   &user->member_points,
                   &user->discount_rate) == 6) {
            if(user->type == ADMIN){
                free(user);
                continue;
            }
            for(int i = 0; i < MAX_USERS; i++){
                if(users_db[i] == NULL) {
                    users_db[i] = user;
                    user_count++;
                    break;
                }
            }
        }
        else{
            free(user);
        }
    }
    fclose(file);
}

void save_users_to_csv(){
    FILE* file = fopen(USERS_FILE, "w");
    if(!file){
        printf("Error saving user data.\n");
        return;
    }
    fprintf(file, "name,email,vehicle_plate,type,member_points,discount_rate\n");
    for(int i = 0; i < MAX_USERS; i++){
        if(users_db[i]){
            fprintf(file, "%s,%s,%s,%d,%d,%.2f\n",
                   users_db[i]->name,
                   users_db[i]->email,
                   users_db[i]->vehicle_plate,
                   users_db[i]->type,
                   users_db[i]->member_points,
                   users_db[i]->discount_rate);
        }
    }
    fclose(file);
}