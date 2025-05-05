#include"inventory.h"
#include"definitions.h"

Part* inventory_root = NULL;

Part* create_part(const char* name, float price){
    Part* new_part = (Part*)malloc(sizeof(Part));
    if(!new_part) return NULL;
    strncpy(new_part->name, name, 49);
    new_part->name[49] = '\0';
    new_part->quantity = 50;
    new_part->price = price;
    new_part->left = new_part->right = NULL;
    return new_part;
}

Part* insert_part(Part* root, const char* name, float price){
    if(!root) return create_part(name, price);
    int cmp = strcmp(name, root->name);
    if(cmp < 0){
        root->left = insert_part(root->left, name, price);
    }
    else if(cmp > 0){
        root->right = insert_part(root->right, name, price);
    }
    return root;
}

void print_parts(Part* root){
    if(!root) return;
    print_parts(root->left);
    printf("- %s ($%.2f, Qty: %d)\n", root->name, root->price, root->quantity);
    print_parts(root->right);
}

Part* search_part(Part* root, const char* name){
    if(!root) return NULL;
    int cmp = strcmp(name, root->name);
    if(cmp == 0) return root;
    return search_part(cmp < 0 ? root->left : root->right, name);
}

void load_inventory_from_csv(){
    FILE* file = fopen(INVENTORY_FILE, "r");
    if(!file){
        inventory_root = insert_part(inventory_root, "Engine Oil", 29.99);
        inventory_root = insert_part(inventory_root, "Brake Pads", 59.99);
        inventory_root = insert_part(inventory_root, "Air Filter", 19.99);
        return;
    }
    char line[256];
    fgets(line, sizeof(line), file);
    while(fgets(line, sizeof(line), file)){
        char name[50];
        int quantity;
        float price;
        if(sscanf(line, "%[^,],%d,%f", name, &quantity, &price) == 3){
            inventory_root = insert_part(inventory_root, name, price);
            Part* part = search_part(inventory_root, name);
            if (part) part->quantity = quantity;
        }
    }
    fclose(file);
}

void save_inventory_to_csv(){
    FILE* file = fopen(INVENTORY_FILE, "w");
    if(!file){
        printf("Error saving inventory data.\n");
        return;
    }
    fprintf(file, "name,quantity,price\n");
    Part* stack[100];
    int top = -1;
    Part* current = inventory_root;
    while(current || top != -1){
        while(current){
            stack[++top] = current;
            current = current->left;
        }
        current = stack[top--];
        fprintf(file, "%s,%d,%.2f\n", current->name, current->quantity, current->price);
        current = current->right;
    }
    fclose(file);
}