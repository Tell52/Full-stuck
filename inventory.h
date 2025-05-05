#ifndef INVENTORY_H
#define INVENTORY_H
#include"definitions.h"

extern Part* inventory_root;

Part* create_part(const char* name, float price);
Part* insert_part(Part* root, const char* name, float price);
void print_parts(Part* root);
Part* search_part(Part* root, const char* name);
void load_inventory_from_csv();
void save_inventory_to_csv();

#endif