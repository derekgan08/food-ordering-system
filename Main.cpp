#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <chrono>

using namespace std;

// Structure to hold details about users (manager and customer)
struct UserDetails {
    // Manager credentials for logging into the system
    string username; // Store the username of the restaurant manager
    string password; // Store the password of the restaurant manager

    // Customer details for making orders
    string customerName; // Store the name of the customer
};

// Function prototypes for various operations in the program
char getUserType(); // Function to determine whether the user is a manager or a customer
void signup(UserDetails& ud); // Function to allow manager to sign up
void login(UserDetails& ud); // Function to allow manager to log in
void getManagerAction(); // Function for manager to choose actions to perform
void getCustomerAction(UserDetails& ud); // Function for customer to choose their actions
//void trackManagerActions(int arrManagerChoices[]); // Tracks manager actions (currently not used)
void continueProgram(); // Asks user if they want to continue using the program
int logout(); // Allows the user to log out from the system

// Manager-specific operations
void createOrUpdateMenu(); // Manager can create or modify the restaurant menu
void updatePrices(); // Manager can adjust the price of menu items
void viewStats(); // Manager can view restaurant stats like popular dishes and sales
void managerHelpInfo(); // Provides help or instructions for the manager on using the system

// Internal functions for manager actions, not directly invoked by manager
float calcTotalPaymentsPerOrder(); // Calculates the total payment for a given order
int calcEstDeliveryTime(string& deliveryArea, int& totalPrepTime); // Estimates delivery time based on location and prep time
int displayMenu(); // Displays the menu to the user (manager or customer)
bool itemAlreadyExists(string itemName); // Checks if the item already exists in the menu when updating
int acceptOrder(int**, int x, int totalMenuItems); // Accepts or rejects an order based on item availability
void updateStocks(int menuIndex, int quantity, int totalMenuItems, int currentStock); // Updates stock after accepting an order
string** readMenu(int&); // Reads the current menu and returns it in a dynamic 2D array

// Customer-specific operations
void orderOnline(UserDetails& ud); // Allows customer to place an online food order
void makePayments(UserDetails& ud); // Allows customer to make a payment for the order

// Internal functions for customer actions, not directly invoked by customer
bool isNewcomer(UserDetails& ud); // Checks if the customer is new or existing based on their details

int main() {
    UserDetails ud; // Declare a variable to store user details (manager or customer)

    // Display a welcome message to the user
    cout << "===================================================================\n";
    cout << "====================== Welcome to NinjaFood! ======================\n";
    cout << "===================================================================\n\n";
		 
    char choice = 'Y'; // Variable to store user's choice to continue or exit the program

    cout << "\n=> Would you like to start using this program? [Y/N] "; // Prompt user to start
    cin >> choice; 

    // INPUT VALIDATION: Ensure user input is either 'Y', 'y', 'N', or 'n'
    while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n')
    {
        cout << "\n/// Invalid selection. Please try again!";
        cout << "\n=> Do you wish to continue using this program? [Y/N] "; // Re-prompt if invalid input
        cin >> choice;
    }

    if (choice == 'Y' || choice == 'y') // If user chooses to continue
    {

        // Ask user whether they are a manager or customer
        char userTypeChoice = getUserType();

        // If the user is a restaurant manager
        if (userTypeChoice == 'M' || userTypeChoice == 'm')
        {
            // Call the login function to verify manager's credentials
            login(ud);
            // Provide manager with options for tasks to perform
            getManagerAction();
        }
        else
        {
            // If the user is a customer, allow them to place an order
            getCustomerAction(ud);
        }
    }

    else // If user opts to exit
    {

        // Display a thank you message and exit the program
        cout << "\n====== Thank you for using NinjaFood's Food Ordering System program! ======\n"
            << "===================== Have a nice day and take care :) ====================\n";
        exit(0); // Terminate the program

    }
    return 0;
}

// Function to determine the user's identity: whether they are a restaurant manager or a customer
char getUserType() {
    string choice; // Variable to store the user input (string to prevent issues with garbage input)

    // Prompt the user to choose their role
    cout << "\n=> Please select your identity: \n"
        << "[M] Restaurant Manager\n"
        << "[C] Customer\n";
    cin >> choice; // Read the user's input, but only take the first character

    // INPUT VALIDATION - Ensure the user inputs only one character and check if it's valid
    while (choice.length() > 1) {
        cout << "\n/// Invalid selection. Please try again!\n";
        cout << "=> Please select your identity: \n"
            << "[M] Restaurant Manager\n"
            << "[C] Customer\n";
        cin >> choice; // Prompt again if input is not valid

        // Ensure the input is either 'M', 'm', 'C', or 'c'
        while (choice[0] != 'M' && choice[0] != 'm' && choice[0] != 'C' && choice[0] != 'c') {
            cout << "\n/// Invalid selection. Please try again!\n";
            cout << "=> Please select your identity: \n"
                << "[M] Restaurant Manager\n"
                << "[C] Customer\n";
            cin >> choice; // Re-prompt if the user input is invalid
        }
    }
    return choice[0]; // Return the first character of the choice to identify the user
}

// Login function to authenticate restaurant managers and secure system access
void login(UserDetails& ud) {
    bool matchFound = false; // Flag to track whether a valid login is found
    string choice; // Variable to store user input (login or sign up choice)
    string data; // Temporary variable to store data read from the login credentials file

    // Open the file containing manager login credentials
    ifstream file;
    file.open("login_credentials.txt");

    cout << "\n**************************** LOGIN PAGE ****************************\n";

    // Prompt the user to choose between logging in or signing up
    cout << "\n/// Welcome. You are accessing as RESTAURANT MANAGER.\n"
        << "=> Please select an option below to proceed:\n"
        << "[1] Login\n"
        << "[2] Sign up\n";
    cin >> choice; // Read user input for login or sign up
    cin.ignore(); // Clear the input buffer to avoid issues with subsequent inputs

    // INPUT VALIDATION - Ensure the user input is a valid selection (either '1' or '2')
    while (choice.length() > 1) {
        cout << "\n/// Invalid selection. Please try again!\n";
        cout << "=> Please select your identity: \n"
            << "[1] Login\n"
            << "[2] Sign up\n";
        cin >> choice; // Prompt again for valid input

        // Ensure the input is either '1' or '2'
        while (choice[0] != '1' && choice[0] != '2') {
            cout << "\n/// Invalid selection. Please try again!\n"
                << "=> Please select an option below:\n"
                << "[1] Login\n"
                << "[2] Sign up\n";
            cin >> choice;
            cin.ignore(); // Clear the buffer after reading input
        }
    }

    // If the manager chooses login
    if (choice[0] == '1') {
        // Check if the login credentials file exists
        if (file.fail()) {
            cout << "\n/// No login credentials found. Please sign up first!\n"
                << "/// Redirecting to sign up page...\n\n";

            // If file doesn't exist, redirect to sign-up process
            signup(ud);
        }

        // Check if the file is empty (no stored credentials)
        else if (file.seekg(0, ios::end), file.tellg() == 0) {
            cout << "\n/// No login credentials found. Please sign up first!\n"
                << "/// Redirecting to sign up page...\n\n";

            // If file is empty, prompt for sign-up
            signup(ud);
        }
        else {
            // Loop until valid login credentials are entered
            while (!matchFound) {
                cout << "\n=> Enter username: ";
                cin >> ud.username; // Get the manager's username
                cout << "=> Enter password: ";
                cin >> ud.password; // Get the manager's password

                file.clear(); // Clear any error flags from previous file operations
                file.seekg(0, ios::beg); // Rewind to the beginning of the file

                // Read each line in the file and check for matching username and password
                while (getline(file, data, '\t')) {
                    // If the username matches
                    if (ud.username == data) {
                        // Retrieve the password associated with the username
                        getline(file, data, '\n');

                        // If the password matches
                        if (ud.password == data) {
                            matchFound = true; // Login is successful
                            break; // Exit loop since credentials are verified
                        }
                    }
                    else {
                        // Skip password reading if the username doesn't match
                        getline(file, data);
                    }
                }

                // If no match is found, prompt user to try again
                if (!matchFound) {
                    cout << "\n/// Incorrect username or password."
                        << "\n/// Please ensure the username and password are entered correctly (case-sensitive)."
                        << "\n/// Try again!\n";
                }
                else {
                    cout << "\n/// Login successful! Welcome as RESTAURANT MANAGER.\n\n"; // Successful login message
                    break; // Exit the loop if login is successful
                }
            }
        }
        file.close(); // Close the credentials file
    }
    else {
        signup(ud); // If user selects sign-up, proceed to sign-up process
    }
}

// Function to log out the restaurant manager and redirect back to the main page
int logout() {
    cout << "\n/// Redirecting to main page...\n\n"; // Notify the user that they are being redirected
    return main(); // Call the main function again to return to the main menu
}

// Function for a new restaurant manager to sign up, creating login credentials
void signup(UserDetails& ud) {
    string data; // Temporary variable to store any unnecessary data while reading the file
    string newUsername; // Variable to store the new username for sign up
    bool valid = false; // Flag to indicate whether the entered credentials are valid

    // Open the file to store login credentials (in both read and write mode, appending new data)
    fstream file;
    file.open("login_credentials.txt", ios::out | ios::in | ios::app);

    cout << "\n*************************** SIGN UP PAGE ***************************\n";
    cout << "\n/// You are signing up as RESTAURANT MANAGER.\n";

    // Loop until a valid username is entered
    while (!valid) {
        file.seekp(0, ios::beg); // Move the file pointer to the beginning of the file
        
        // Prompt the user to enter a new username
        cout << "\n=> Enter new username: ";
        getline(cin, newUsername);

        // Read through the file to check if the username already exists
        while (file) {
            getline(file, data, '\t'); // Read each username separated by a tab

            // If the new username matches an existing one, prompt the user to choose another one
            if (newUsername == data) {
                valid = false;
                cout << "\n/// This username already exists. Please try again!\n";
                break; // Exit the file-reading loop to re-enter a new username
            }
            else {
                valid = true;
                getline(file, data, '\n'); // Skip the password and check the next username
            }
        }
    }

    file.clear(); // Clear any error flags from the file operation

    // If the entered username is valid (doesn't exist already), write it to the file
    if (valid) {
        file << newUsername << '\t'; // Write the new username to the file
    }
    cin.clear(); // Clear any previous input errors from cin

    // Prompt the user to enter a new password with a minimum length of 6 characters
    cout << "=> Enter new password (min 6 characters): ";
    getline(cin, ud.password);

    // Validate the entered password length
    while (ud.password.length() < 6) {
        cout << "\n/// Password must be at least 6 characters!\n";
        cout << "=> Enter new password (min 6 characters): ";
        getline(cin, ud.password); // Re-enter the password if it doesn't meet the requirements
    }

    // If the password is valid, write it to the file along with the username
    file << ud.password << "\n";

    cout << "\n/// Sign up successful!\n"
        << "/// Redirecting to login page...\n\n";

    file.close(); // Close the file after writing the credentials

    // After sign-up, automatically redirect to the login page
    login(ud);
}

// Function to determine the action the restaurant manager would like to perform
void getManagerAction() {
    string actionChoice; // Variable to store the user's action choice as a string

    // Prompt the user to select an action from the available options
    cout << "\n=> Please select an action as RESTAURANT MANAGER: \n"
        << "[1] Create/update menu\n"
        << "[2] Update prices\n"
        << "[3] View stats (most popular dish, total number of customers, total sales, total number of orders)\n"
        << "\n/// Do you need help navigating the system? Select the option below! \n"
        << "[4] View Manager Help info\n";
    cin >> actionChoice; // Read the user's action choice

    // INPUT VALIDATION - Ensure that the input is a single character and valid
    while (actionChoice.length() > 1) {
        cout << "\n/// Invalid selection. Please try again!\n";
        cout << "=> Please select an action as RESTAURANT MANAGER: \n"
            << "[1] Create/update menu\n"
            << "[2] Update prices\n"
            << "[3] View stats (most popular dish, total number of customers, total sales, total number of orders)\n"
            << "\n/// Do you need help navigating the system? Select the option below! \n"
            << "[4] View Manager Help info\n";
        cin >> actionChoice; // Prompt again if the input is not valid

        // Check if the input is valid (choices are '1', '2', '3', or '4')
        while (actionChoice[0] != '1' && actionChoice[0] != '2' && actionChoice[0] != '3' && actionChoice[0] != '4') {
            cout << "\n/// Invalid selection. Please try again!\n";
            cout << "=> Please select an action as RESTAURANT MANAGER: \n"
                << "[1] Create/update menu\n"
                << "[2] Update prices\n"
                << "[3] View stats (most popular dish, total number of customers, total sales, total number of orders)\n"
                << "\n/// Do you need help navigating the system? Select the option below! \n"
                << "[4] View Manager Help info\n";
            cin >> actionChoice[0]; // Re-prompt for valid input
        }
    }

    // Based on the action choice, call the appropriate function to perform the task
    switch (actionChoice[0]) {
    case '1':
        createOrUpdateMenu(); // Call function to create or update the menu
        break;
    case '2':
        updatePrices(); // Call function to update prices in the menu
        break;
    case '3':
        viewStats(); // Call function to view the statistics (most popular dish, sales, etc.)
        break;
    case '4':
        managerHelpInfo(); // Call function to view help information for the manager
        break;
    default:
        // This block should not be reached due to the input validation above
        cout << "Error!\n";
    }
}

// Function to get the customer action, either to make an order or proceed with payment
void getCustomerAction(UserDetails& ud) {
    string actionChoice; // Variable to store the customer's action choice

    // Prompt the customer to choose between ordering online or making a payment
    cout << "\n=> Please select an action as CUSTOMER: \n"
        << "[1] Order online\n"
        << "[2] Make payment\n";
    cin >> actionChoice;

    // INPUT VALIDATION - Ensure that the input is a single character and valid
    while (actionChoice.length() > 1) {
        cout << "\n/// Sorry, that is not a valid selection. Please try again!\n";
        cout << "\n=> Please select an action as CUSTOMER: \n"
            << "[1] Order online\n"
            << "[2] Make payment\n";
        cin >> actionChoice;

        // Check if the entered choice is valid (either '1' or '2')
        while (actionChoice[0] != '1' && actionChoice[0] != '2') {
            cout << "\n/// Sorry, that is not a valid selection. Please try again!\n";
            cout << "\n=> Please select an action as CUSTOMER: \n"
                << "[1] Order online\n"
                << "[2] Make payment\n";
            cin >> actionChoice[0]; // Prompt again if input is invalid
        }
    }

    // Based on the action choice, proceed with the appropriate function
    switch (actionChoice[0]) {
    case '1':
        orderOnline(ud); // Call the function to allow the customer to order online
        break;
    case '2':
        makePayments(ud); // Call the function to allow the customer to make a payment
        break;
    default:
        cout << "Error!\n"; // This should not happen due to input validation
    }
}

// Function to display guidelines and help for the restaurant manager to use the system effectively
void managerHelpInfo() {
    cout << "\n********************** MANAGER HELP INFO PAGE ***********************\n";
    cout << "\n/// You have selected the option to: View Manager Help Info\n";
    cout << "\nWelcome to the NinjaFood Ordering System!\n"
        << "This system enables Restaurant Managers to manage the restaurant's menu, view statistics, and perform other administrative tasks.\n";

    // Description of the available options for the manager
    cout << "\n===> Options Specifications"
        << "\n\n[1] Create/Update Menu"
        << "\n\t This option allows the Restaurant Manager to create a new menu or update the existing one."
        << "\n\t The menu includes the following details for each item: "
        << "\n\t\t 1. Item name"
        << "\n\t\t 2. Item price"
        << "\n\t\t 3. Preparation time (the time required to prepare the item)"
        << "\n\t\t 4. Stock quantity (number of items available for order)"

        << "\n\n[2] Update Prices"
        << "\n\t This option allows the Restaurant Manager to modify the price of any menu item."

        << "\n\n[3] View Stats"
        << "\n\t This option displays the restaurant's key statistics, including the following details: "
        << "\n\t\t 1. Most popular dish"
        << "\n\t\t 2. Total number of orders for today"
        << "\n\t\t 3. Total sales for today"
        << "\n\t\t 4. Total number of customers for today";

    cout << "\n=============================================================\n";

    // Ask the user whether they want to continue using the program
    continueProgram();
}

// Function to read the menu file and store all its contents into a dynamically allocated 2D array
// The function takes a reference to totalNumItems, which represents the total number of items in the menu
string** readMenu(int& totalNumItems) {
    string itemName; // Variable to store the name of a food item
    string line; // Variable to store each line in the menu file temporarily
    float itemPrice = 0; // Variable to store the price of a food item
    string strItemPrice; // String representation of item price
    int preparationTime = 0; // Variable to store the preparation time of the food item
    string strPreparationTime; // String representation of preparation time
    int stock = 0; // Variable to store the stock quantity of the item
    string strStock; // String representation of stock quantity
    char toSkip; // Variable to store a character to skip (e.g., commas in the file)
    string strIndex; // String representation of the index/ID of the food item

    fstream file;
    file.open("menu.txt", ios::in); // Open the menu file in read mode

    int index = 0; // Variable to count the number of menu items

    // Count the number of items in the menu by counting the number of lines in the file
    while (getline(file, line)) 
        ++index;
    totalNumItems = index; // Update totalNumItems with the total number of menu items

    // Dynamically allocate a 2D array to store the menu details
    string** arrMenuContent = new string*[index];
    for (int j = 0; j < index; j++) {
        arrMenuContent[j] = new string[5]; // Each item has 5 details (ID, name, price, preparation time, stock)
    }

    // If the menu is empty, return the empty array
    if (index == 0)
        return arrMenuContent;
    else {
        // Clear the file and set the file pointer to the beginning for reading
        file.clear();
        file.seekp(0, ios::beg);

        int i = 0; // Index for the array to store menu items
        while (file >> index) { // Read the index of the item
            file >> toSkip; // Skip the comma separating the index and item name
            getline(file, itemName, ','); // Read the item name (up to the comma delimiter)
            file >> itemPrice; // Read the item price
            file >> toSkip; // Skip the comma
            file >> preparationTime; // Read the preparation time
            file >> toSkip; // Skip the comma
            file >> stock; // Read the stock quantity

            // Convert the values to strings to store in the array
            strIndex = to_string(index);
            strItemPrice = to_string(itemPrice);
            strPreparationTime = to_string(preparationTime);
            strStock = to_string(stock);

            // Store the item details into the dynamically allocated array
            arrMenuContent[i][0] = strIndex; 
            arrMenuContent[i][1] = itemName;
            arrMenuContent[i][2] = strItemPrice;
            arrMenuContent[i][3] = strPreparationTime;
            arrMenuContent[i][4] = strStock;

            ++i; // Increment to store the next item in the array
        }
    }

    file.close(); // Close the file after reading the menu

    return arrMenuContent; // Return the 2D array containing the menu details
}

// Manager function to allow the restaurant manager to create or update the menu
void createOrUpdateMenu() {
    int numbering = 1; // Integer variable to track the sequence number for menu items
    char choice = 'Y'; // Variable to store the user's choice for whether they wish to continue adding/updating menu items
    string itemName; // String variable to hold the name of the food item
    string line; // Temporary string to store each line from the file
    float itemPrice = 0; // Float variable to store the price of the food item
    int preparationTime = 0; // Integer variable to store the preparation time of the food item (in minutes)
    int stock = 0; // Integer variable to store the available stock quantity of the item

    fstream file;
    file.open("menu.txt", ios::out | ios::app); // Open the menu file in append mode to update the existing content

    cout << "\n*********************** CREATE/UPDATE MENU PAGE ***********************\n";
    cout << "\n/// You have selected the option to: Update/Create Menu\n";

    // Retrieve the current menu index by displaying the existing menu
    int index = displayMenu();
    numbering = ++index; // Increment the index to start numbering new items

    if (index == 0) // Check if the menu file is empty
        cout << "/// Menu does not exist. Creating menu...\n"; // Inform user that a new menu will be created

    // Loop to allow the manager to add/update items until they choose to stop
    while (choice == 'Y' || choice == 'y') {
        cout << "\n=> Enter name of item #" << numbering << ": "; // Prompt for the name of the new food item
        cin.ignore(); // Clear any extra newline characters left in the input buffer
        getline(cin, itemName); // Get the item name from the user input

        // Validate the item name - it should not be empty, too long, or already exist in the menu
        while (itemName.empty() || itemName == " " || itemName.length() >= 25 || itemAlreadyExists(itemName)) {
            if (itemName.empty() || itemName == " ") {
                cout << "/// Item name cannot be empty!\n"; // Error if the name is empty
            }
            else if (itemName.length() >= 25) {
                cout << "/// Item name cannot be more than 25 characters! Please try again.\n"; // Error if the name is too long
            }
            else {
                cout << "\n/// This item already exists in the menu!";
                cout << "\n/// Please consider the \"Update Prices\" option for this item instead.\n"; // Error if the item already exists
            }

            cout << "\n=> Enter name of item #" << numbering << ": "; // Prompt to re-enter the item name
            getline(cin, itemName); // Get the item name again
        }
        file << numbering << "," << itemName << ","; // Write the item number and name to the file

        cout << "=> Enter price of item #" << numbering << ": $"; // Prompt for the item price
        cin >> itemPrice;
        // Validate the price - it must be greater than zero
        while (itemPrice <= 0) {
            cout << "\n/// Item price cannot be equal to or less than zero! Please try again.\n";
            cout << "=> Enter price of item #" << numbering << ": $"; // Prompt to re-enter the price
            cin >> itemPrice;
        }
        file << fixed << setprecision(2) << itemPrice << ","; // Write the price to the file with two decimal places

        cout << "=> Enter preparation time of item #" << numbering << " (in minutes): "; // Prompt for preparation time
        cin >> preparationTime;
        // Validate the preparation time - it must be greater than zero
        while (preparationTime <= 0) {
            cout << "\n/// Preparation time cannot be less than or equal to zero! Please try again.\n";
            cout << "=> Enter preparation time of item #" << numbering << " (in minutes): "; // Prompt to re-enter the preparation time
            cin >> preparationTime;
        }
        file << preparationTime << ","; // Write the preparation time to the file

        cout << "=> Enter stock quantity of item #" << numbering << " : "; // Prompt for the stock quantity
        cin >> stock;
        // Validate the stock quantity - it must be greater than zero
        while (stock <= 0) {
            cout << "\n/// Stock quantity cannot be less than or equal to zero! Please try again.\n";
            cout << "=> Enter stock quantity of item #" << numbering << " : "; // Prompt to re-enter the stock quantity
            cin >> stock;
        }
        file << stock << "\n"; // Write the stock quantity to the file

        // Ask if the manager wants to continue adding more items to the menu
        cout << "=> Do you wish to continue? [Y/N] "; // Prompt for continuation choice
        cin >> choice;

        // Validate the user's choice for continuation
        while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n') {
            cout << "\n/// Sorry, that is not a valid selection. Please try again!";
            cout << "\n=> Do you wish to continue? [Y/N] "; // Prompt again if the input is invalid
            cin >> choice;
        }

        ++numbering; // Increment the item numbering for the next item
    }

    // After updating the menu, move the file pointer to the beginning for displaying the updated menu
    file.seekp(0, ios::beg); // Set file pointer to the start of the file

    cout << "\n/// Displaying updated menu...\n";
    displayMenu(); // Display the updated menu to the manager

    // Ask the manager if they want to continue using the system
    continueProgram(); // Proceed with the next action in the program
}

// Function to allow the restaurant manager to update the prices of menu items
void updatePrices() {
    int index = 0; // Index variable to store the menu item's index
    int userIndex = 0; // Variable to store the user's input for selecting an item by its index
    int totalNumItems = 0; // Variable to store the total number of items in the menu
    string itemName; // Variable to store the name of the menu item
    string strItemPrice; // Variable to store the item price as a string
    float itemPrice = 0; // Variable to store the item price as a float
    string strOldPrice; // Variable to store the old price of the item as a string
    float oldPrice = 0; // Variable to store the old price of the item as a float
    string strNewPrice; // Variable to store the new price of the item as a string
    float newPrice = 0; // Variable to store the new price of the item as a float
    char choice = 'Y'; // Variable to determine whether the manager wants to continue updating prices
    int preparationTime = 0; // Variable to store the preparation time of the item (in minutes)
    string strPreparationTime; // Variable to store the preparation time as a string
    int stock = 0; // Variable to store the stock quantity of the item
    string strStock; // Variable to store the stock quantity as a string
    string line; // Temporary variable to hold unused data from the file

    // Read the current menu into a 2D array and retrieve the total number of items
    string** arrMenuContent = readMenu(totalNumItems);

    cout << "\n**************************** UPDATE PRICES ****************************\n";
    cout << "\n/// You have selected the option to: Update Prices\n";

    // Display the current menu to the manager
    displayMenu();

    fstream file;
    file.open("menu.txt", ios::out); // Open the menu file for writing (to update prices)

    // If the menu file doesn't exist or is empty, prompt the manager to create a menu first
    if (file.fail() || totalNumItems == 0) {
        cout << "/// Menu not found! Please create the menu first and try again.\n";
        cout << "/// Redirecting to Create/Update Menu...\n";
        createOrUpdateMenu(); // Redirect to menu creation if menu doesn't exist
    } else {
        // Proceed if the menu exists and contains items
        while (choice == 'Y' || choice == 'y') {
            cout << "\n=> Please enter the index of the item to update the price: ";
            cin >> userIndex;

            // Validate that the user input index is within valid range
            while (userIndex < 1 || userIndex > totalNumItems) {
                cout << "/// Invalid index. Please try again!\n";
                cout << "\n=> Please enter index of item to update price: ";
                cin >> userIndex;
            }

            // Iterate through the menu and find the selected item to update its price
            for (int i = 0; i < totalNumItems; i++) {
                index = stoi(arrMenuContent[i][0]); // Get the index of the current item
                if (index == userIndex) { // If item matches the user's selection
                    itemName = arrMenuContent[i][1]; // Get the item name
                    strOldPrice = arrMenuContent[i][2]; // Get the old price as a string
                    oldPrice = stof(strOldPrice); // Convert the old price to float
                    cout << "\n/// Current price for " << itemName << ": $" << oldPrice;
                    cout << "\n=> Please enter the new price for " << itemName << ": $";
                    cin >> newPrice; // Get the new price from the user

                    // Validate that the new price is valid (not equal to the old price or non-positive)
                    while (newPrice == oldPrice || newPrice <= 0) {
                        if (newPrice == oldPrice) {
                            cout << "\n/// New price cannot be equal to the old price!\n";
                            cout << "=> Please enter the new price for " << itemName << ": $";
                            cin >> newPrice;
                        } else if (newPrice <= 0) {
                            cout << "\n/// New price cannot be less than or equal to zero!\n";
                            cout << "=> Please enter the new price for " << itemName << ": $";
                            cin >> newPrice;
                        }
                    }

                    // Store the new price as a string in the menu content
                    strNewPrice = to_string(newPrice);
                    arrMenuContent[i][2] = strNewPrice; // Update the price in the array
                    break;
                }
            }

            // Update the file with the modified menu data
            for (int j = 0; j < totalNumItems; j++) {
                string strIndex = arrMenuContent[j][0]; // Get the index as a string
                file << strIndex << ","; // Write the item index to the file

                itemName = arrMenuContent[j][1]; // Get the item name
                file << itemName << ","; // Write the item name to the file

                strItemPrice = arrMenuContent[j][2]; // Get the price as a string
                itemPrice = stof(strItemPrice); // Convert the price to a float
                file << fixed << setprecision(2) << itemPrice << ","; // Write the price to the file with 2 decimals

                strPreparationTime = arrMenuContent[j][3]; // Get the preparation time as a string
                preparationTime = stoi(strPreparationTime); // Convert preparation time to integer
                file << preparationTime << ","; // Write the preparation time to the file

                strStock = arrMenuContent[j][4]; // Get the stock quantity as a string
                stock = stoi(strStock); // Convert stock quantity to integer
                file << stock << "\n"; // Write the stock quantity to the file
            }

            file.clear(); // Clear any error flags
            file.seekp(0, ios::beg); // Move the file pointer back to the beginning of the file

            cout << "\n/// Displaying updated menu...\n";
            displayMenu(); // Display the updated menu to the manager

            // Ask the manager if they want to continue updating prices for other items
            cout << "\n=> Do you wish to continue updating prices? [Y/N] ";
            cin >> choice;

            // Validate the user's choice for continuation
            while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n') {
                cout << "\n/// Sorry, that is not a valid selection. Please try again!";
                cout << "\n=> Do you wish to continue updating prices? [Y/N] ";
                cin >> choice;
            }
        }
    }

    // Release the dynamically allocated memory for the menu array
    delete[] arrMenuContent;
    file.close(); // Close the file after writing

    // Ask the manager whether they want to continue using the program
    continueProgram(); // Proceed with the next action in the program
}

// Function to display restaurant statistics: top dish, total sales, and customer count
void viewStats() {
    int index = 0; // Variable to track the number of sales lines in the total sales file
    float line = 0; // Represents each line of sales data in the file
    int numOfCustomers = 0; // Tracks the number of customers
    string data; // Temporary variable to store data from files that are not needed for processing
    int numbering = 0; // To count the number of entries in the top dish file
    int menuIndex = 0; // The index of the menu item
    string strMenuIndex; // To store the menu index as a string
    string itemName; // To store the item name
    string strItemPrice; // To store the item price as a string
    float itemPrice = 0; // To store the item price as a float
    int quantity = 0; // Quantity of items ordered
    int preparationTime = 0; // Preparation time of the item
    char toSkip; // To skip characters while reading the file
    bool alreadyExists = false; // Flag to check if a dish has already been recorded
    int topDishIndex = 0; // The index of the most popular dish
    int maxQuantity = 0; // Maximum quantity ordered for a dish
    int totalNumItems; // Total number of items in the menu

    float totalSales = 0; // Total sales value
    fstream totalSalesFile;
    totalSalesFile.open("total_sales.txt", ios::in | ios::out | ios::app); // Open sales file to read total sales data

    fstream topdishFile;
    topdishFile.open("topdish.txt", ios::in | ios::out | ios::app); // Open top dish file to read most popular dishes

    // Read the menu into an array and store the total number of items
    string** arrMenuContent = readMenu(totalNumItems);

    cout << "\n**************************** VIEW STATS ****************************\n";

    cout << "\n/// You have selected the option to: View Stats\n";

    // Check if there is any data in the top dish file
    while (getline(topdishFile, data))  
        ++numbering; // Count the number of lines in the file
    
    // If no data exists in the top dish file, show a message and exit
    if (numbering == 0) {
        cout << "\n/// Stats not available. \n/// Reason: No orders have been made yet.\n"
             << "/// Please wait for a customer to order first, then try again!\n";
    } else {
        // Dynamically allocate arrays to store dish indices and quantities
        int* arrDishes = nullptr;
        arrDishes = new int[numbering]; // Array to store dish indices
        int* arrQuantity = nullptr;
        arrQuantity = new int[numbering]; // Array to store corresponding quantities ordered

        topdishFile.clear();
        topdishFile.seekg(0, ios::beg); // Reset file pointer to the beginning

        int x = 0;
        // Read the data from the topdish file and populate the arrays
        while (topdishFile >> menuIndex) {
            topdishFile >> toSkip; // Skip the separator
            topdishFile >> quantity; // Read the quantity of the item ordered

            // Check if the dish has already been recorded, update quantity if so
            for (int z = 0; z < x + 1; z++) {
                if (arrDishes[z] == menuIndex) {
                    arrQuantity[z] += quantity; // Increment the quantity for the existing dish
                    alreadyExists = true; // Flag that the dish already exists
                    break;
                }
            }
            // If the dish is not found, add it to the arrays
            if (!alreadyExists) {
                arrDishes[x] = menuIndex; // Add the new dish to the array
                arrQuantity[x] = quantity; // Add the corresponding quantity
                ++x; // Increment the index for the next entry
            }
        }

        // Find the most popular dish by looking for the dish with the highest order quantity
        for (int y = 0; y < x; y++) {
            if (arrQuantity[y] > maxQuantity) {
                maxQuantity = arrQuantity[y]; // Update maximum quantity
                topDishIndex = arrDishes[y]; // Set the index of the most popular dish
            }
        }

        // If the menu is empty, prompt the user to create a menu first
        if (totalNumItems == 0) {
            cout << "/// Menu not found! Please create the menu first and try again.\n";
            cout << "/// Redirecting to Create/Update Menu...";
            createOrUpdateMenu();
        } else {
            // Display the menu and find the most popular dish
            for (int i = 0; i < totalNumItems; i++) {
                strMenuIndex = arrMenuContent[i][0];
                menuIndex = stoi(strMenuIndex);
                if (menuIndex == topDishIndex) {
                    itemName = arrMenuContent[i][1];
                    strItemPrice = arrMenuContent[i][2];
                    itemPrice = stof(strItemPrice);

                    // Display information about the most popular dish
                    cout << "\n1. MOST POPULAR DISH OF NINJAFOOD: \n";
                    cout << "===> " << left << setw(20) << itemName
                         << "$" << setw(5) << fixed << setprecision(2) << itemPrice
                         << "\t\tTotal orders: " << maxQuantity
                         << "\tTotal profit: " << "$" << setw(5) << fixed << setprecision(2) << maxQuantity * itemPrice << "\n";
                    break;
                }
            }
        }

        // Read the total sales data from the file and calculate total sales and customer count
        while (getline(totalSalesFile, data))  
            ++index; // Count the number of sales records
        totalSalesFile.clear();
        totalSalesFile.seekg(0, ios::beg); // Reset file pointer to the beginning

        // Read each line and add to the total sales
        while (totalSalesFile >> line) {
            totalSales += line; // Add each sale amount to the total
            ++numOfCustomers; // Increment the customer count for each sale
        }

        // Display the statistics: total orders, total sales, and number of customers
        cout << "\n2. TOTAL ORDERS TODAY: " << index;
        cout << fixed << setprecision(2) << "\n3. TOTAL SALES TODAY: $" << totalSales;
        cout << "\n4. TOTAL NUMBER OF CUSTOMERS TODAY: " << numOfCustomers << "\n";
    }

    totalSalesFile.close(); // Close the total sales file
    topdishFile.close(); // Close the top dish file

    // Ask the user whether they want to continue using the program
    continueProgram(); // Proceed with the next step in the program
}

// Function to display the menu of the restaurant, showing items, prices, preparation time, and stock
int displayMenu() {
    string itemName; // To store the name of the menu item
    string line; // Temporary variable to store each line from the menu file
    float itemPrice = 0; // To store the price of the item as a float
    string strItemPrice; // To store the price as a string
    int preparationTime = 0; // To store the preparation time of the item
    string strPreparationTime; // To store the preparation time as a string
    int stock = 0; // To store the stock quantity of the item
    string strStock; // To store the stock quantity as a string
    string strIndex; // To store the index of the item as a string
    int index = 0; // To store the index of the item
    int totalNumItems = 0; // To store the total number of items in the menu

    // Read the menu into an array and store the total number of items
    string** arrMenuContent = readMenu(totalNumItems);

    // If the menu is empty, return 0
    if (totalNumItems == 0) {
        return 0;
    } else {
        // Display the menu with item details
        cout << "\n/// There are currently " << totalNumItems << " item(s) on the menu.\n"; // Show the number of items on the menu
        cout << "\n============================================================================\n";
        cout << "=================================== MENU ===================================\n";
        cout << "============================================================================\n\n";
        cout << "NO.  ITEM NAME\t\t      ITEM PRICE\tPREPARATION TIME\t STOCK";
        cout << "\n-----------------------------------------------------------------------------\n";

        // Loop through the menu and display each item
        for (int i = 0; i < totalNumItems; i++) {
            strIndex = arrMenuContent[i][0];
            index = stoi(strIndex); // Convert the string index to integer

            itemName = arrMenuContent[i][1]; // Get the item name
            strItemPrice = arrMenuContent[i][2];
            itemPrice = stof(strItemPrice); // Convert item price to float

            strPreparationTime = arrMenuContent[i][3];
            preparationTime = stoi(strPreparationTime); // Convert preparation time to integer

            strStock = arrMenuContent[i][4];
            stock = stoi(strStock); // Convert stock quantity to integer

            // Display item details in a tabular format
            cout << index << ")   " << left << setw(25) << itemName
                 << "\t$" << fixed << setprecision(2) << itemPrice;
            cout << "\t\t" << preparationTime << " minutes";
            cout << "\t\t" << stock << "\n";
        }
    }
    return totalNumItems; // Return the total number of items
}

// Function to accept an order and verify stock availability, updates the order if valid
int acceptOrder(int** arrOrder, int x, int totalMenuItems) {
    int numbering = 0; // Temporary variable to store the item numbering
    int menuIndex = 0; // To store the menu item index
    string line; // Temporary variable for reading file lines
    string itemName; // To store the name of the item
    int preparationTime = 0; // Preparation time of the item
    int stock = 0; // Stock quantity of the item
    int invalidItemIndex = 0; // To track the index of invalid items
    int totalNumItems = 0; // Total number of items in the menu
    string strStock; // Stock quantity in string form for conversion

    fstream topdishFile;
    topdishFile.open("topdish.txt", ios::out | ios::app); // Open topdish file for appending new orders

    // Menu structure: index, itemName, itemPrice, preparationTime, stock
    // Dynamically allocated array to hold menu content
    string** arrMenuContent = readMenu(totalNumItems);

    // Loop through the menu items and check if the order exists
    for (int i = 0; i < totalMenuItems; i++) {
        menuIndex = stoi(arrMenuContent[i][0]); // Get the menu item index
        if (menuIndex == arrOrder[x][0]) { // Match order with menu item
            strStock = arrMenuContent[i][4]; // Get the current stock quantity
            stock = stoi(strStock); // Convert stock to integer
            break;
        }
    }

    // Check if the ordered quantity exceeds the available stock
    if (arrOrder[x][1] > stock) {
        invalidItemIndex = arrOrder[x][0]; // Store the invalid item's menu index
        arrOrder[x][0] = 0; // Set menu index to 0 to indicate invalid item
        arrOrder[x][1] = 0; // Set quantity to 0
    } else {
        // If the order is valid, append the menu index and quantity to the top dish file
        topdishFile << arrOrder[x][0] << "," << arrOrder[x][1] << "\n";
        updateStocks(menuIndex, arrOrder[x][1], totalMenuItems, stock); // Update stock based on order
    }

    topdishFile.close(); // Close the top dish file
    return invalidItemIndex; // Return the index of invalid items (if any)
}

// Function to update the stock after an order is processed
void updateStocks(int menuIndex, int quantity, int totalMenuItems, int currentStock) {
    int index = 0; // To store the menu item index
    string itemName; // To store the name of the menu item
    string strItemPrice; // To store the price as a string
    float itemPrice = 0; // To store the item price
    char choice = 'Y'; // User choice for confirming the update (not used here)
    int preparationTime = 0; // Preparation time of the item
    string strPreparationTime; // Preparation time as a string
    int stock = 0; // To store the stock quantity
    string strStock; // Stock quantity in string form
    string line; // Temporary variable for reading lines from the file
    int totalNumItems = 0; // To store the total number of items in the menu

    // Menu structure: index, itemName, itemPrice, preparationTime, stock
    // Dynamically allocated memory to hold menu content
    string** arrMenuContent = readMenu(totalNumItems);

    // Loop through the menu and find the item to update its stock
    for (int i = 0; i < totalMenuItems; i++) {
        index = stoi(arrMenuContent[i][0]); // Get the index of the item
        if (index == menuIndex) { // If we find the correct menu item
            stock = currentStock - quantity; // Subtract the ordered quantity from the current stock
            strStock = to_string(stock); // Convert the updated stock to string
            arrMenuContent[i][4] = strStock; // Update the stock in the array
            break; // Break after updating the stock
        }
    }

    // Open the menu file to update it with the new stock values
    fstream file;
    file.open("menu.txt", ios::out); // Open the file in write mode to overwrite it

    // Loop through the menu and write each item's details back to the file
    for (int j = 0; j < totalNumItems; j++) {
        string strIndex = arrMenuContent[j][0]; // Get the index of the item as a string
        file << strIndex << ","; // Write the index to the file

        itemName = arrMenuContent[j][1]; // Get the name of the item
        file << itemName << ","; // Write the item name to the file

        strItemPrice = arrMenuContent[j][2]; // Get the item price as a string
        itemPrice = stof(strItemPrice); // Convert the price to float
        file << fixed << setprecision(2) << itemPrice << ","; // Write the price to the file

        strPreparationTime = arrMenuContent[j][3]; // Get the preparation time as a string
        preparationTime = stoi(strPreparationTime); // Convert preparation time to integer
        file << preparationTime << ","; // Write the preparation time to the file

        strStock = arrMenuContent[j][4]; // Get the stock quantity as a string
        stock = stoi(strStock); // Convert stock to integer
        file << stock << "\n"; // Write the stock to the file
    }

    // Clean up dynamically allocated memory for the menu content
    delete[] arrMenuContent;

    file.close(); // Close the menu file after updating
}

// Function to calculate the total payment for an order by reading from the receipt
float calcTotalPaymentsPerOrder() {
    ifstream receipt;
    receipt.open("receipt.txt"); // Open the receipt file to read order data

    fstream totalSalesFile;
    totalSalesFile.open("total_sales.txt", ios::in | ios::out | ios::app); // Open total sales file to append the total payment

    string datetime; // To store the date and time of the order
    int numbering = 0; // To store the numbering (line count or entry number)
    int menuIndex = 0; // To store the menu index of the ordered item
    string itemName; // To store the name of the item
    float itemPrice = 0; // To store the price of the item
    int quantity = 0; // To store the quantity of the item ordered
    int preparationTime = 0; // To store the preparation time
    char toSkip; // To skip delimiters or unwanted characters

    float totalPayment = 0; // To store the total payment for the order

    // Read the date and time from the receipt file
    getline(receipt, datetime);

    // Process each item in the receipt and calculate the total payment
    while (receipt >> numbering) {
        receipt >> toSkip; // Skip the comma
        receipt >> menuIndex; // Read the menu index
        receipt >> toSkip; // Skip the comma
        getline(receipt, itemName, ','); // Read the item name
        receipt >> itemPrice; // Read the item price
        receipt >> toSkip; // Skip the comma
        receipt >> preparationTime; // Read the preparation time
        receipt >> toSkip; // Skip the comma
        receipt >> quantity; // Read the quantity ordered

        totalPayment += itemPrice * quantity; // Calculate the total payment for this item
    }

    // Append the total payment of the order to the total sales file
    totalSalesFile << totalPayment << "\n";

    receipt.close(); // Close the receipt file
    totalSalesFile.close(); // Close the total sales file

    return totalPayment; // Return the total payment for this order
}

// Function to calculate the estimated delivery time, including preparation time and travel time
int calcEstDeliveryTime(string& deliveryArea, int& totalPrepTime) {
    ifstream file;
    file.open("receipt.txt"); // Open the receipt file to read the order details

    string datetime; // Store the date and time of the order
    int numbering = 0; // Temporary variable for the item numbering in the receipt
    int menuIndex = 0; // Store the menu index of the ordered item
    string itemName; // Store the name of the item
    float itemPrice = 0; // Store the price of the item
    int quantity = 0; // Store the quantity of the item ordered
    char toSkip; // Temporary variable to skip commas in the file

    int deliveryTravelTime = 0; // To store the delivery travel time based on the area
    int preparationTime = 0; // To store the preparation time for each item
    int deliveryTime = 0; // Total delivery time in minutes (prep time + travel time)

    // The file structure (receipt) is: 
    //      date & time
    //      numbering, menuIndex, itemName, itemPrice, preparationTime, quantity
    getline(file, datetime); // Read the date and time of the order (skipped for now)
    
    // Process each item in the receipt to calculate the total preparation time
    while (file >> numbering) {
        file >> toSkip; // Skip the comma
        file >> menuIndex; // Read the menu index
        file >> toSkip; // Skip the comma
        getline(file, itemName, ','); // Read the item name
        file >> itemPrice; // Read the item price (not used in time calculation)
        file >> toSkip; // Skip the comma
        file >> preparationTime; // Read the preparation time for the item
        file >> toSkip; // Skip the comma
        file >> quantity; // Read the quantity of the item ordered

        // Accumulate total preparation time (considering the quantity ordered)
        totalPrepTime += quantity * preparationTime;
    }

    // Ask the user to input their delivery area choice
    cout << "\n=> Please enter your delivery area (1 - 6): "
         << "\n[1] Cahaya Gemilang"
         << "\n[2] Aman Damai"
         << "\n[3] Indah Kembara"
         << "\n[4] Restu"
         << "\n[5] Saujana"
         << "\n[6] Tekun\n";
    cin >> deliveryArea;

    // INPUT VALIDATION - Ensure the input is a single character and valid
    while (deliveryArea.length() > 1) {
        cout << "\n/// Invalid delivery area! Please try again.";
        cout << "\n=> Please enter your delivery area (1 - 6): "
             << "\n[1] Cahaya Gemilang"
             << "\n[2] Aman Damai"
             << "\n[3] Indah Kembara"
             << "\n[4] Restu"
             << "\n[5] Saujana"
             << "\n[6] Tekun\n";
        cin >> deliveryArea;

        // Validate that the input is one of the allowed areas (1-6)
        while (deliveryArea[0] != '1' && deliveryArea[0] != '2' && deliveryArea[0] != '3' &&
               deliveryArea[0] != '4' && deliveryArea[0] != '5' && deliveryArea[0] != '6') {
            cout << "\n/// Invalid delivery area! Please try again.";
            cout << "\n=> Please enter your delivery area (1 - 6): "
                 << "\n[1] Cahaya Gemilang"
                 << "\n[2] Aman Damai"
                 << "\n[3] Indah Kembara"
                 << "\n[4] Restu"
                 << "\n[5] Saujana"
                 << "\n[6] Tekun\n";
            cin >> deliveryArea;
        }
    }

    // Determine the delivery travel time based on the selected area
    switch (deliveryArea[0]) {
        case '1':
        case '2': // Cahaya Gemilang and Aman Damai both have a travel time of 5 minutes
            deliveryTravelTime = 5;
            break;
        case '3': // Indah Kembara has a travel time of 6 minutes
            deliveryTravelTime = 6;
            break;
        case '4': // Restu has a travel time of 10 minutes
            deliveryTravelTime = 10;
            break;
        case '5': // Saujana has a travel time of 9 minutes
            deliveryTravelTime = 9;
            break;
        default: // Any other area (assumed to be Tekun) has a travel time of 8 minutes
            deliveryTravelTime = 8;
            break;
    }

    // Calculate the total delivery time (preparation time + travel time)
    deliveryTime = totalPrepTime + deliveryTravelTime;

    return deliveryTime; // Return the calculated total delivery time
}

// Function to check if a menu item already exists based on its name
bool itemAlreadyExists(string itemName) {
    char toSkip; // Temporary variable to skip unwanted characters in the file
    int numbering; // Menu item numbering
    string line; // To store the item name
    float itemPrice; // To store the item price (not used in this function)
    int preparationTime; // To store the preparation time (not used here)
    
    fstream file;
    file.open("menu.txt", ios::in); // Open the menu file in read mode

    // Process each item in the menu file
    while (file >> numbering) {
        file >> toSkip; // Skip the comma
        getline(file, line, ','); // Read the item name
        if (line == itemName) { // If the item name matches the input name
            return true; // The item already exists, return true
        }
        file >> itemPrice; // Skip the item price
        file >> toSkip; // Skip the comma
        file >> preparationTime; // Skip the preparation time
    }

    file.close(); // Close the menu file after checking
    return false; // Return false if the item name was not found
}

// Function to ask the user if they want to continue using the program
void continueProgram() {
    char choice = 'Y'; // Variable to store user's choice

    // Prompt the user to decide if they wish to continue
    cout << "\n=> Do you wish to continue using this program? [Y/N] ";
    cin >> choice;

    // Validate the user's input to ensure it's either 'Y' or 'N'
    while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n') {
        cout << "\n/// Sorry, that is not a valid selection. Please try again!";
        cout << "\n=> Do you wish to continue using this program? [Y/N] ";
        cin >> choice;
    }

    // If the user chooses 'Y' or 'y', continue with the manager actions
    if (choice == 'Y' || choice == 'y')
        getManagerAction(); // Call the function to get the manager's next action
    else {
        cout << "\n/// Redirecting to logout...\n"; // Logout message
        logout(); // Call the logout function
    }
}

// Function to allow customers to order food online
void orderOnline(UserDetails& ud) {
    int index = 0; // Index to iterate through the menu items
    int menuIndex = 0; // Index to store the selected food item
    string itemName; // Name of the food item
    float itemPrice = 0; // Price of the food item
    string strItemPrice; // Temporary string for item price
    string strPreparationTime; // Temporary string for preparation time
    int preparationTime = 0; // Time needed to prepare the item
    string strStock; // Temporary string to store stock info
    int stock = 0; // Stock count for the food item
    int menuChoice = 0; // Customer's menu choice (index)
    int quantity = 0; // Quantity of the ordered item
    float totalPrice = 0; // Total price of the order
    char choice = 'Y'; // Choice to continue ordering
    int invalidItemIndex = 0; // Index of invalid item (insufficient stock)
    int proceedChoice; // Choice for proceeding after the order
    int totalNumItems = 0; // Total number of items in the menu
    string line; // Temporary variable to read unnecessary data from file
    bool itemAlreadyExists = false; // Flag to check if item has been ordered previously

    time_t now = time(0); // Get current system time (date and time)
    char* datetime = ctime(&now); // Convert current time to string

    fstream receipt;
    receipt.open("receipt.txt", ios::out); // Open file to write the receipt

    cout << "\n**************************** ORDER PAGE ****************************\n";

    // Read the menu content into a dynamically allocated array
    string** arrMenuContent = readMenu(totalNumItems);

    // Check if menu is empty
    if (totalNumItems == 0) {
        cout << "\n/// Sorry, menu does not exist. Please contact a RESTAURANT MANAGER for help to CREATE MENU.";
        logout(); // If no menu exists, log out the user
    } else {
        cout << "\n/// You are now ordering online as customer.\n";

        // Display the menu for the customer to choose from
        displayMenu();

        // Dynamically allocate 2D array to store the customer's order
        int** arrMenuChoices = new int*[totalNumItems];
        for (int i = 0; i < totalNumItems; i++) {
            arrMenuChoices[i] = new int[2]; // Each entry stores menu index and quantity
        }

        int x = 0; // Index to track the number of items ordered
        while (choice == 'Y' || choice == 'y') {
            // Prompt the customer to select a menu item
            cout << "\n=> Please enter the index number of your choice of food (1 - " << totalNumItems << "): ";
            cin >> menuChoice;

            // Validate the customer's menu choice
            while (menuChoice < 1 || menuChoice > totalNumItems) {
                cout << "/// Sorry, that is not a valid selection. Please try again!\n";
                cout << "\n=> Please enter the index number of your choice of food (1 - " << totalNumItems << "): ";
                cin >> menuChoice;
            }

            // Prompt the customer to enter the quantity
            cout << "=> Please enter the quantity: ";
            cin >> quantity;

            // Ensure the quantity is a positive number
            while (quantity < 1) {
                cout << "/// Sorry, the quantity cannot be less than 1. Please try again!\n";
                cout << "\n=> Please enter the quantity: ";
                cin >> quantity;
            }

            // Check if the customer has already ordered this item
            for (int k = 0; k < x + 1; k++) {
                if (menuChoice == arrMenuChoices[k][0] && k != x) {
                    // If the item was previously ordered, add the new quantity to the existing quantity
                    arrMenuChoices[k][1] += quantity;
                    itemAlreadyExists = true;
                    cout << "\n/// This item has been ordered before. Updating quantity...\n";
                    break;
                } else {
                    itemAlreadyExists = false; // Mark the item as new if not found
                }
            }

            // If the item was not previously ordered, add it to the order
            if (itemAlreadyExists == false) {
                arrMenuChoices[x][0] = menuChoice; // Store the selected menu item index
                arrMenuChoices[x][1] = quantity; // Store the quantity ordered
                ++x; // Increment the order count
            }

            cout << "\n/// Processing order...\n";

            // Check whether each item is valid (e.g., sufficient stock)
            invalidItemIndex = acceptOrder(arrMenuChoices, x - 1, totalNumItems);

            // If the item is invalid (insufficient stock), notify the customer
            if (invalidItemIndex != 0) {
                for (int i = 0; i < totalNumItems; i++) {
                    index = stoi(arrMenuContent[i][0]);
                    if (index == invalidItemIndex) {
                        itemName = arrMenuContent[i][1]; // Retrieve the item name
                    }
                }

                cout << "/// Apologies! Order of " << itemName << " is rejected due to insufficient stock.\n";
            }

            // Ask the customer if they want to continue ordering
            cout << "\n=> Do you wish to continue ordering? [Y/N] ";
            cin >> choice;

            // Validate the choice to continue or stop ordering
            while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n') {
                cout << "\n/// Sorry, that is not a valid selection. Please try again!";
                cout << "\n=> Do you wish to continue ordering? [Y/N] "; // Ask again for valid input
                cin >> choice;
            }
        }

        // Write the order to the receipt file
        int numLine = 1;
        receipt << datetime; // Write the date and time of the order to the receipt

        // Iterate through the order and write each item to the receipt
        for (int k = 0; k < x; k++) {
            if (arrMenuChoices[k][0] == 0) continue; // Skip empty entries

            // Write the order details to the receipt
            // Menu structure: index, itemName, itemPrice, preparationTime
            // Receipt structure: date & time, numbering, menuIndex, itemName, itemPrice, preparationTime, quantity
            for (int j = 0; j < totalNumItems; j++) {
                menuIndex = stoi(arrMenuContent[j][0]);
                if (arrMenuChoices[k][0] == menuIndex) {
                    receipt << numLine << ","; // Write the item number to the receipt
                    receipt << menuIndex << ","; // Write the menu index of the item
                    itemName = arrMenuContent[j][1];
                    receipt << itemName << ","; // Write the name of the ordered food
                    itemPrice = stof(arrMenuContent[j][2]);
                    receipt << itemPrice << ","; // Write the price of the item
                    preparationTime = stoi(arrMenuContent[j][3]);
                    receipt << preparationTime << ","; // Write the preparation time
                    receipt << arrMenuChoices[k][1] << "\n"; // Write the ordered quantity
                    numLine++; // Increment the line number for the next item
                    break;
                }
            }
        }

        // Clean up dynamically allocated memory
        delete[] arrMenuChoices;
        delete[] arrMenuContent;
        receipt.close(); // Close the receipt file

        // Ask the customer if they want to proceed to payment or reorder
        cout << "\n/// Would you like to proceed to make payment?\n";
        cout << "[1] Make payment\n"
             << "[2] Re-order\n";
        cin >> proceedChoice;

        // Validate the choice to proceed
        while (proceedChoice != 1 && proceedChoice != 2) {
            cout << "\n/// Sorry, that is not a valid selection. Please try again!";
            cout << "\n/// Would you like to proceed to make payment?\n";
            cout << "[1] Make payment\n"
                 << "[2] Re-order\n";
            cin >> proceedChoice;
        }

        // Handle the proceed choice: either redirect to payment or reorder
        if (proceedChoice == 1) {
            cout << "\n/// Redirecting to Make Payment...\n";
            makePayments(ud); // Call the makePayments function
        } else {
            cout << "\n/// Redirecting to Order Online again...\n";
            orderOnline(ud); // Recurse to allow another order
        }
    }
}

// Function to process the payment for the customer's order
void makePayments(UserDetails& ud) {
    string line;
    fstream receipt;
    receipt.open("receipt.txt", ios::in); // Open the receipt file to read the order details

    int index = 0; // Index to count the total number of items in the order
    while (getline(receipt, line))  // Loop through the receipt lines to count the number of ordered items
        ++index;

    // If no items have been ordered, prompt the user to make an order
    if (index == 0) {
        cout << "\n/// You have not made any orders yet!.\n";
        cout << "/// Redirecting to Order Online...\n";
        orderOnline(ud); // If no orders are made, redirect to the ordering page
    } else {
        // Reset the receipt file pointer to the beginning
        receipt.clear();
        receipt.seekg(0, ios::beg);

        bool eligibleNewcomerDiscount = isNewcomer(ud); // Check if the user is eligible for the newcomer discount

        string datetime;
        int numbering;
        int menuIndex = 0;
        string itemName;
        float itemPrice = 0;
        int quantity = 0;
        int preparationTime = 0;
        char toSkip;

        float totalPayment = 0; // Total payment due
        int totalPrepTime = 0; // Total preparation time for all ordered items
        int deliveryTime = 0; // Estimated delivery time
        string deliveryAreaNum; // The number representing the delivery area
        string deliveryArea; // The name of the delivery area

        float userPayment = 0; // Amount the user pays
        float change = 0; // Change to be returned to the customer
        bool hasChange = false; // Flag to indicate if the customer is due for change

        // Calculate the estimated delivery time based on the delivery area number
        deliveryTime = calcEstDeliveryTime(deliveryAreaNum, totalPrepTime);
        switch (deliveryAreaNum[0]) {
            case '1': deliveryArea = "Cahaya Gemilang"; break;
            case '2': deliveryArea = "Aman Damai"; break;
            case '3': deliveryArea = "Indah Kembara"; break;
            case '4': deliveryArea = "Restu"; break;
            case '5': deliveryArea = "Saujana"; break;
            default: deliveryArea = "Tekun"; break;
        }

        // Display the payment page header
        cout << "\n*************************** PAYMENT PAGE ***************************\n";
        cout << "\n/// You are now Making Payment.\n";

        // Display receipt header
        cout << "\n=================================================================\n";
        cout << "============================ RECEIPT ============================\n";
        cout << "=================================================================\n\n";
        cout << "NO.  ITEM NAME\t\t      ITEM PRICE\tQUANTITY\tPREPARATION TIME";
        cout << "\n---------------------------------------------------------------------------------\n";

        // Reset file pointer and go through each line to display order details
        receipt.clear();
        receipt.seekp(0, ios::beg);

        getline(receipt, datetime); // Read and display the date/time of the order
        while (receipt >> numbering) {
            receipt >> toSkip; // Skip the delimiter
            receipt >> menuIndex;
            receipt >> toSkip; // Skip the delimiter
            getline(receipt, itemName, ','); // Read the item name
            receipt >> itemPrice; // Read the item price
            receipt >> toSkip; // Skip the delimiter
            receipt >> preparationTime; // Read the preparation time
            receipt >> toSkip; // Skip the delimiter
            receipt >> quantity; // Read the quantity ordered

            // Display the item details in a formatted manner
            cout << numbering << ") " << left << setw(25) << itemName;
            cout << "\t$" << fixed << setprecision(2) << itemPrice;
            cout << "\t\t" << quantity;
            cout << "\t\t" << preparationTime << " minutes\n";
        }

        // Calculate the total payment based on the ordered items
        totalPayment = calcTotalPaymentsPerOrder();

        // Apply a newcomer discount if eligible
        if (eligibleNewcomerDiscount) {
            cout << "\nCongratulations! As a first-time user, you are entitled to 10% Newcomer Discount :)\n";
            float discount = totalPayment * 10 / 100; // Calculate the discount (10% of total payment)
            cout << fixed << setprecision(2) << "You save: $" << discount << "\n";
            totalPayment -= discount; // Deduct the discount from the total payment
        } else {
            cout << "\nThanks for dining with us again, " << ud.customerName << "! :) \n";
        }

        // Display final order details, including total payment, preparation time, and delivery information
        cout << "\n==================== ORDER DETAILS ====================\n";
        cout << setw(30) << "\nTOTAL PAYMENT: " << fixed << setprecision(2) << "$" << totalPayment;
        cout << setw(30) << "\nFOOD PREPARATION TIME: " << totalPrepTime << " minutes";
        cout << setw(30) << "\nDELIVERY AREA: " << deliveryArea;
        cout << setw(30) << "\nTOTAL DELIVERY TIME: " << deliveryTime << " minutes. \nThank you for your patience!\t";
        cout << setw(30) << "\n\nDATE & TIME OF ORDER: " << datetime;
        cout << "\n=================================================================\n";

        // Prompt the user for the amount they wish to pay
        cout << "\n=> Please enter the amount to pay: $";
        cin >> userPayment;

        // Ensure the customer has entered sufficient payment
        while (userPayment < totalPayment) {
            cout << "/// Insufficient payment! Please try again.\n";
            cout << "\n=> Please enter the amount to pay: $";
            cin >> userPayment;
        }

        // If the user has overpaid, calculate the change to be returned
        if (userPayment > totalPayment) {
            hasChange = true;
            change = userPayment - totalPayment; // Calculate the change
        }

        // Display the amount paid and any change
        cout << "\n/// PAID AMOUNT: $" << fixed << setprecision(2) << userPayment;

        if (hasChange) {
            cout << "\n/// Dispensing change...";
            cout << "\n/// CHANGE: $" << fixed << setprecision(2) << change;
        }

        // Thank the customer for their order and finalize the transaction
        cout << "\n/// Thank you for choosing NinjaFood! Enjoy your meal and see you again!\n";

        // Close the receipt file and remove it after the transaction
        receipt.close();
        remove("receipt.txt"); // Delete the receipt file to finalize the process
        logout(); // Log the user out after payment
    }
}

// Function to check if the customer is a newcomer and eligible for the discount
bool isNewcomer(UserDetails& ud) {
    string line;
    string phoneNumber;

    string phoneNumberFromFile; // Variable to store the phone number from the file for comparison
    bool eligible = true; // Assume the customer is eligible for the newcomer discount

    // Open the customer records file for reading and writing
    fstream file;
    file.open("customer_record.txt", ios::in | ios::out | ios::app);

    // Display introductory message to collect customer details
    cout << "\n********************** CUSTOMER DETAILS PAGE **********************\n";
    cout << "\n/// We require your details to ensure we deliver the correct order to the right person and address!\n";

    // Clear the input buffer before collecting data
    cin.ignore();

    // Prompt the customer to enter their name
    cout << "\n=> Please enter your details to proceed.\n"
         << "\n=> Name: ";
    getline(cin, ud.customerName); // Read the customer's name

    // Prompt the customer to enter their phone number
    cout << "\n=> Phone number (eg 0123456789): ";
    getline(cin, phoneNumber); // Read the phone number

    // Validate phone number length (it should be between 10 to 11 digits)
    while (phoneNumber.length() < 10 || phoneNumber.length() > 11) {
        cout << "\nInvalid phone number! Please try again.\n";
        cout << "=> Phone number (eg 0123456789): ";
        getline(cin, phoneNumber); // Prompt the user again for a valid phone number
    }

    // Check if the phone number already exists in the customer record database
    while (getline(file, line, ',')) {
        getline(file, phoneNumberFromFile); // Read the phone number from the file
        
        // If the phone number matches an existing record, the customer is not a newcomer
        if (phoneNumber == phoneNumberFromFile) {
            eligible = false; // The customer is not eligible for the newcomer discount
            break; // Exit the loop once we find a match
        }
    }

    // Clear any flags that may be set on the file stream after reading
    file.clear();

    // Save the customer's details (name and phone number) to the file
    file << ud.customerName << ","; // Write the customer's name
    file << phoneNumber << "\n"; // Write the phone number to the file

    // Close the customer record file after processing
    file.close();

    // Return whether the customer is eligible for the newcomer discount
    return eligible;
}
