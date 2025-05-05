#include"menu_functions.h"
#include"definitions.h"
#include"user_management.h"
#include"inventory.h"

PriorityQueue appointment_queue = {NULL, NULL, NULL, NULL, 0, 0};
IncomeStats income = {0};

void mainmenu(){
    printf("\n=== Car Service Shop ===\n");
    printf("1. Register\n2. Login\n3. Exit\n");
    printf("Choice: ");
}

void usermenu(User* user){
    int choice;
    do{
        printf("\n=== %s's Dashboard (%s) ===\n", user->name, 
               user->type == ADMIN ? "Admin" : 
               user->type == SUB_MEMBER ? "Sub-Member" : "Normal Member");
        printf("1. Book Service\n2. Buy Parts\n3. View Appointments\n");

        if(user->type == SUB_MEMBER){
            printf("4. Check Member Points\n");
            printf("5. Logout\nChoice: ");
        }
        else if(user->type == ADMIN){
            printf("4. Admin Menu\n");
            printf("5. Logout\nChoice: ");
        }
        else{
            printf("4. Logout\nChoice: ");
        }
        if(scanf("%d", &choice) != 1){
            printf("Invalid input.\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');
        switch(choice){
            case 1: 
                book_service(user); 
                break;
            case 2: 
                buy_parts(user); 
                break;
            case 3: 
                view_queue(user); 
                break;
            case 4: 
                if(user->type == SUB_MEMBER){
                    printf("Your member points: %d\n", user->member_points);
                    printf("Your discount rate: %.1f%%\n", user->discount_rate * 100);
                }
                else if (user->type == ADMIN){
                    admin_menu();
                }
                else{
                    printf("Logging out...\n");
                    return;
                }
                break;
            case 5: 
                if(user->type == SUB_MEMBER || user->type == ADMIN){
                    printf("Logging out...\n");
                    return;
                }
            default: 
                printf("Invalid choice. Try again.\n");
        }
    } while (1);
}

void collect_parts_to_array(Part* root, Part* parts_array[], int* count){
    if(!root) return;
    collect_parts_to_array(root->left, parts_array, count);
    parts_array[(*count)++] = root;
    collect_parts_to_array(root->right, parts_array, count);
}

void buy_parts(User* user){
    printf("\n=== Buy Parts ===\n");
    printf("Available parts:\n");
    Part* parts_array[100];
    int part_count = 0;
    collect_parts_to_array(inventory_root, parts_array, &part_count);
    for(int i = 0; i < part_count; i++){
        printf("%d. %s ($%.2f, Qty: %d)\n", i+1, parts_array[i]->name, 
               parts_array[i]->price, parts_array[i]->quantity);
    }
    if(part_count == 0){
        printf("No parts available in inventory.\n");
        return;
    }
    printf("Enter part number to buy (1-%d): ", part_count);
    int choice;
    if(scanf("%d", &choice) != 1){
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    if(choice < 1 || choice > part_count){
        printf("Invalid part number.\n");
        return;
    }
    Part* selected_part = parts_array[choice-1];
    if(selected_part->quantity <= 0){
        printf("Sorry, %s is out of stock.\n", selected_part->name);
        return;
    }
    float cost = selected_part->price * (1 - user->discount_rate);
    printf("Purchased %s for $%.2f", selected_part->name, cost);
    if (user->discount_rate > 0) {
        printf(" (%.1f%% discount applied)", user->discount_rate * 100);
    }
    printf("\n");
    selected_part->quantity--;
    user->member_points += 5;
    income.total_income += cost;
}

Appointment* create_appointment(User* user, const char* service, float cost, bool is_priority){
    Appointment* new_appointment = (Appointment*)malloc(sizeof(Appointment));
    if(!new_appointment){
        printf("Memory allocation failed for appointment.\n");
        return NULL;
    }
    new_appointment->user = user;
    strncpy(new_appointment->service, service, 49);
    new_appointment->service[49] = '\0';
    new_appointment->cost = cost;
    new_appointment->is_priority = is_priority;
    new_appointment->next = NULL;
    return new_appointment;
}

void enqueue(PriorityQueue* queue, User* user, const char* service, float cost, bool is_priority){
    Appointment* new_appointment = create_appointment(user, service, cost, is_priority);
    if(!new_appointment) return;
    if(is_priority){
        if(!queue->priority_front){
            queue->priority_front = queue->priority_rear = new_appointment;
        }
        else{
            queue->priority_rear->next = new_appointment;
            queue->priority_rear = new_appointment;
        }
        queue->priority_count++;
        printf("Priority appointment added to queue.\n");
    }
    else{
        if(!queue->normal_front){
            queue->normal_front = queue->normal_rear = new_appointment;
        }
        else{
            queue->normal_rear->next = new_appointment;
            queue->normal_rear = new_appointment;
        }
        queue->normal_count++;
        printf("Appointment added to queue.\n");
    }
}

Appointment* dequeue(PriorityQueue* queue, bool priority_only){
    Appointment* appointment = NULL;
    if(queue->priority_front){
        appointment = queue->priority_front;
        queue->priority_front = appointment->next;
        if(!queue->priority_front){
            queue->priority_rear = NULL;
        }
        appointment->next = NULL;
        queue->priority_count--;
        return appointment;
    }
    if(priority_only) return NULL;
    if(queue->normal_front){
        appointment = queue->normal_front;
        queue->normal_front = appointment->next;
        if(!queue->normal_front){
            queue->normal_rear = NULL;
        }
        appointment->next = NULL;
        queue->normal_count--;
        return appointment;
    }
    return NULL;
}

void display_queue(PriorityQueue* queue){
    printf("\n=== Appointment Queue ===\n");
    printf("Priority Appointments (%d):\n", queue->priority_count);
    Appointment* current = queue->priority_front;
    int count = 1;
    while(current){
        printf("%d. %s - Service: %s, Cost: $%.2f (VIP)\n", 
               count++, current->user->name, current->service, current->cost);
        current = current->next;
    }
    printf("\nRegular Appointments (%d):\n", queue->normal_count);
    current = queue->normal_front;
    count = 1;
    while(current){
        printf("%d. %s - Service: %s, Cost: $%.2f\n", 
               count++, current->user->name, current->service, current->cost);
        current = current->next;
    }
}

void book_service(User* user){
    printf("\n=== Book Service ===\n");
    printf("Available services:\n");
    printf("1. Repair\n2. Wash Car\n3. Paint & Design\n4. Fill Fuel\n5. Fill Gas\n");
    printf("Choose service (1-5): ");
    int service_choice;
    if(scanf("%d", &service_choice) != 1){
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    while(getchar() != '\n');
    if (service_choice < 1 || service_choice > 5){
        printf("Invalid service choice.\n");
        return;
    }
    const char* services[] = {"Repair", "Wash Car", "Paint & Design", "Fill Fuel", "Fill Gas"};
    float prices[] = {100.0, 30.0, 200.0, 50.0, 40.0};
    float cost = prices[service_choice-1] * (1 - user->discount_rate);

    bool is_priority = false;
    if(user->type == SUB_MEMBER || user->type == ADMIN){
        printf("Would you like priority service? (50%% extra cost) (1-Yes, 0-No): ");
        int priority_choice;
        if(scanf("%d", &priority_choice) != 1){
            printf("Invalid input. Booking as regular service.\n");
            while (getchar() != '\n');
        }
        else{
            while (getchar() != '\n');
            is_priority = (priority_choice == 1);
            if (is_priority) {
                cost *= 1.5;
            }
        }
    }
    printf("Service '%s' booked successfully! Cost: $%.2f\n", 
           services[service_choice-1], cost);
    enqueue(&appointment_queue, user, services[service_choice-1], cost, is_priority);
    user->member_points += 10;
    income.total_income += cost;
}

void view_queue(User* user){
    printf("\n=== Your Appointments ===\n");
    bool found = false;
    int count = 1;
    Appointment* current = appointment_queue.priority_front;
    while(current){
        if (current->user == user){
            if (!found) found = true;
            printf("%d. Service: %s, Cost: $%.2f (VIP)\n", 
                   count++, current->service, current->cost);
        }
        current = current->next;
    }
    current = appointment_queue.normal_front;
    while(current){
        if(current->user == user){
            if(!found) found = true;
            printf("%d. Service: %s, Cost: $%.2f\n", 
                   count++, current->service, current->cost);
        }
        current = current->next;
    }
    if(!found){
        printf("No appointments found.\n");
    }
    else{
        printf("\n1. Cancel an appointment\n2. Back\nChoice: ");
        int choice;
        if(scanf("%d", &choice) != 1){
            printf("Invalid input.\n");
            while (getchar() != '\n');
            return;
        }
        while(getchar() != '\n');
        if(choice == 1){
            cancel_queue(user);
        }
    }
}

void cancel_queue(User* user){
    printf("Enter the number of the appointment to cancel: ");
    int target;
    if(scanf("%d", &target) != 1){
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    while(getchar() != '\n');
    int count = 1;
    bool found = false;
    Appointment* prev = NULL;
    Appointment* current = appointment_queue.priority_front;
    while(current && !found){
        if(current->user == user){
            if(count == target){
                if(prev){
                    prev->next = current->next;
                    if(current == appointment_queue.priority_rear){
                        appointment_queue.priority_rear = prev;
                    }
                }
                else{
                    appointment_queue.priority_front = current->next;
                    if(!appointment_queue.priority_front){
                        appointment_queue.priority_rear = NULL;
                    }
                }
                printf("Appointment for %s canceled.\n", current->service);
                free(current);
                appointment_queue.priority_count--;
                found = true;
                break;
            }
            count++;
        }
        prev = current;
        current = current->next;
    }
    if(!found){
        prev = NULL;
        current = appointment_queue.normal_front;
        while(current && !found){
            if(current->user == user){
                if(count == target){
                    if(prev){
                        prev->next = current->next;
                        if(current == appointment_queue.normal_rear){
                            appointment_queue.normal_rear = prev;
                        }
                    }
                    else{
                        appointment_queue.normal_front = current->next;
                        if(!appointment_queue.normal_front){
                            appointment_queue.normal_rear = NULL;
                        }
                    }
                    printf("Appointment for %s canceled.\n", current->service);
                    free(current);
                    appointment_queue.normal_count--;
                    found = true;
                    break;
                }
                count++;
            }
            prev = current;
            current = current->next;
        }
    }
    if(!found){
        printf("Appointment not found or already completed.\n");
    }
}

void DoTasks(){
    Appointment* next = dequeue(&appointment_queue, false);
    if(next){
        printf("Processing appointment for %s - Service: %s\n", next->user->name, next->service);
        printf("Cost: $%.2f\n", next->cost);
        free(next);
    }
    else{
        printf("No appointments in queue.\n");
    }
}

void save_appointments_to_csv(){
    FILE* file = fopen("appointments.csv", "w");
    if(!file){
        printf("Error saving appointment data.\n");
        return;
    }
    fprintf(file, "email,service,cost,is_priority\n");
    Appointment* current = appointment_queue.priority_front;
    while(current){
        fprintf(file, "%s,%s,%.2f,%d\n", 
               current->user->email, 
               current->service, 
               current->cost, 
               1);
        current = current->next;
    }
    current = appointment_queue.normal_front;
    while(current){
        fprintf(file, "%s,%s,%.2f,%d\n", 
               current->user->email, 
               current->service, 
               current->cost, 
               0);
        current = current->next;
    }
    fclose(file);
}

void load_appointments_from_csv(){
    FILE* file = fopen("appointments.csv", "r");
    if(!file){
        printf("No existing appointment data found.\n");
        return;
    }
    char line[256];
    fgets(line, sizeof(line), file);
    while(fgets(line, sizeof(line), file)){
        char email[MAX_EMAIL_LEN];
        char service[50];
        float cost;
        int is_priority;
        if(sscanf(line, "%[^,],%[^,],%f,%d", email, service, &cost, &is_priority) == 4){
            User* user = find_user(email);
            if(user){
                enqueue(&appointment_queue, user, service, cost, is_priority);
            }
        }
    }
    fclose(file);
}

void admin_menu(){
    int choice;
    do{
        printf("\n=== Admin Menu ===\n");
        printf("1. View All Users\n");
        printf("2. View Inventory\n");
        printf("3. View Income\n");
        printf("4. View Appointment Queue\n");
        printf("5. Process Next Appointment\n");
        printf("6. Back\n");
        printf("Choice: ");
        if(scanf("%d", &choice) != 1){
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');
        switch(choice) {
            case 1:
                printf("\n=== All Users ===\n");
                for(int i = 0; i < MAX_USERS; i++){
                    if(users_db[i]){
                        printf("%s (%s) - %s\n", 
                              users_db[i]->name, 
                              users_db[i]->email,
                              users_db[i]->type == ADMIN ? "Admin" :
                              users_db[i]->type == SUB_MEMBER ? "Sub-Member" : "Normal");
                    }
                }
                break;
            case 2:
                printf("\n=== Inventory ===\n");
                print_parts(inventory_root);
                break;
            case 3:
                printf("\nTotal Income: $%.2f\n", income.total_income);
                break;
            case 4:
                display_queue(&appointment_queue);
                break;
            case 5:
                DoTasks();
                break;
            case 6:
                return;
            default:
                printf("Invalid choice.\n");
        }
    } while(choice != 6);
}