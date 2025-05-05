#include"definitions.h"
#include"user_management.h"
#include"inventory.h"
#include"menu_functions.h"

int main(){
    load_users_from_csv();
    load_inventory_from_csv();
    load_appointments_from_csv();
    int choice;
    User* current_user = NULL;
    while(1){
        mainmenu();
        if(scanf("%d", &choice) != 1){
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');
        switch(choice){
            case 1: 
                registers(); 
                break;
            case 2: 
                current_user = login();
                if (current_user) {
                    usermenu(current_user);
                }
                break;
            case 3:
                save_users_to_csv();
                save_inventory_to_csv();
                save_appointments_to_csv();
                printf("Thank you for using Car Service Shop.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}