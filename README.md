# Introduction
  A Car Service Management System improves user registration, inventory tracking, 
  and appointment scheduling using structured data handling. By automating processes 
  with optimized data structures, it enhances efficiency, reduces errors, 
  and ensures smooth service operations.

# Overview
  The Car Service Management System is designed to optimize the handling of user registration, 
  inventory management, and appointment scheduling. The system allows users to register, 
  purchase car parts, book services, and manage appointments effectively.
  - User Authentication & Management
  - Inventory Tracking
  - Appointment Scheduling
  - Data Persistence

# How to run
  # Clone this repository
  git clone https://github.com/Tell52/Full-stuck.git
  cd Full-stuck
  
  # Compile the program
  gcc main.c inventory.c menu_functions.c user_management.c -o run
  
  # Run the program
  ./run

# Test case
  # User Management Test Cases (user_management.c)
  1. Register New User
      Test Case: Register a new user
      Input: Name = "New1", Email = "new1", Vehicle Plate = "AB12", Type = "Normal" or Type = "Sub-member"
      Expected Output: User successfully registered and added to " users.csv "
  3. Duplicate Email Registration
      Test Case: Register a user with an existing email
      Input: Name = "New1", Email = "new1" (already in database)
      Expected Output: Error message: "Email already registered."
  5. User Login
      Test Case: User login with valid credentials
      Input: Email = "new1"
      Expected Output: " Welcome Back, new1 (Type) " and redirected to user menu
  7. Invalid User Login
      Test Case: User login with non-existent email
      Input: Email = "nonuser@example.com"
      Expected Output: Error message: "User not found. Please register first."
  # Inventory Management Test Cases (inventory.c)
  1. Reduce Inventory After Purchase
      Test Case: Buy a part
      Input: 1. Part = "Air Filter", Quantity = 1
      Expected Output: Message : " Purchased Air Filter for $17.99 " , Quantity reduced in inventory.csv when exit.

  # Appointment Scheduling Test Cases (menu_functions.c)
  1. Create Appointment
      Test Case: Schedule a new appointment (1.Book Service) (Normal)
      Input: User = "new1", Service = "Repair", Cost = $100, Priority = "No"
      Expected Output: Appointment saved to appointments.csv, added to queue
  3. Create Appointment
      Test Case: Schedule appointment with priority (1.Book Service) (Sub-member)
      Input: User = "new2", Service = "Repair", Cost = $135, Priority = "Yes"
      Expected Output: Appointment added to priority queue, processed earlier than normal queue
  4. View Appointment Queue
      Test Case: User views appointment queue
      Input: 3. View Appointments
      Expected Output: Show all pending appointments
  5. Cancel Appointment
      Test Case: User cancels their appointment (In View Appointments)
      Input: 1. Cancel an appointment 
      Expected Output: Appointment removed from queue, updated in appointments.csv
  6. Process Next Appointment (FIFO Handling) (UserType: admin in Admin Menu)
      Test Case: Process next available appointment
      Input: 5. Process Next Appointment
      Expected Output: Next appointment removed from queue, message : " Processing appointment for (name) - Appointment Cost: (price)"
  
  # Data Storage & CSV Test Cases
  1. Save Users to CSV
      Test Case: Ensure user data is correctly written to users.csv
      Input: User Type: "Admin"  (In Admin Menu) , 1. View All Users
      Expected Output: Read & show All users in users.csv correctly
  2. Load Inventory from CSV
      Test Case: Ensure inventory loads correctly 
      Input: User Type: "Admin"  (In Admin Menu) , 2. View Inventory
      Expected Output: Read & show parts in inventory.csv correctly
  3. Save Appointments to CSV
      Test Case: Ensure appointments are saved after scheduling
      Input: User Type: "Admin"  (In Admin Menu) , 4. View Appointment Queue
      Expected Output: Read & show All Appointment in appointments.csv correctly






  
