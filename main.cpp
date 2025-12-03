#include <iostream>
#include "helpers.h"

using namespace std;


int main()
{
    int choice;

    while (true)
    {
        clearScreen();
        printHeader("CAMPUS RIDE CARPOOL MANAGEMENT SYSTEM");
        viewMenu();
        cout << "------------------------------------------------------------" << endl;

        do
        {
            cout << "Enter your choice: ";
            cin >> choice;

           if (choice < 1 || choice > 7)
            {
                cout << "Invalid Choice! Choice must be between 1-7" << endl;
                cout << "----------------------------------------" << endl;
                cout << endl;
            }

        } while (choice < 1 || choice > 7);

        switch (choice)
        {
        case 1:
            clearScreen();
            cout << "Register User selected." << endl;
            registerUser();
            waitForEnter();
            break;
        case 2:
            clearScreen();
            cout << "Create Ride (Driver) selected." << endl;
            waitForEnter();
            // Ride creation logic here
            break;
        case 3:
            clearScreen();
            cout << "Search Rides (Rider) selected." << endl;
            waitForEnter();
            // Ride search logic here
            break;
        case 4:
            clearScreen();
            cout << "Book Ride selected." << endl;
            waitForEnter();
            // Ride booking logic here
            break;
        case 5:
            clearScreen();
            cout << "Statistics selected." << endl;
            waitForEnter();
            // Statistics logic here
            break;
        case 6:
            clearScreen();
            cout << "Admin selected." << endl;
            waitForEnter();
            // Admin logic here
            break;
        case 7:
            cout << "------------------------------------------------------------" << endl;
            cout << "Exiting the system. Goodbye!" << endl;
            cout << "------------------------------------------------------------" << endl;
            return 0;
        }
    }

    return 0;
}


void registerUser()
{
    // Created a new user and collect details
    User newUser;
    printHeader("\t  REGISTER USER");

    // Get user CNIC and validate
    while (true)
    {
        cout << "Enter CNIC: ";
        cin >> newUser.cnic;
        // clear rest of line after formatted input
        cin.ignore();
        if (validateCNIC(newUser.cnic))
            break;
    }

    cout << "Enter Name: ";
    getline(cin, newUser.name);
    if (!newUser.name.empty())
        newUser.name[0] = toupper(newUser.name[0]);

    // Get user role and validate
    do
    {
        cout << "Enter Role | 1. Driver  2. Rider  3. Both: ";
        cin >> newUser.role;

        
        if (newUser.role < 1 || newUser.role > 3)
        {
            cout << "Invalid role selected. Please choose 1, 2, or 3." << endl;
            cout << "----------------------------------------" << endl;
            cout << endl;
        }
    } while (newUser.role < 1 || newUser.role > 3);

    // Get user age and validate
    cout << "Enter Age: ";
    cin >> newUser.age;

    // Age restriction applies only if the user is registering as a Driver or Both
    if ((newUser.role == 1 || newUser.role == 3) && newUser.age < 18)
    {
        cout << "User must be at least 18 years old to register as a Driver." << endl;
        cout << "----------------------------------------" << endl;
        cout << endl;
        return;
    }

    // Get department
    cout << "Enter Department: ";
    cin.ignore();
    getline(cin, newUser.department);
    newUser.department[0] = toupper(newUser.department[0]);

    // Get and validate contact number
    cout << "Enter Contact Number: ";
    cin >> newUser.contact;
    string cleaned = normalizePhone(newUser.contact);

    while (!isValidNumber(cleaned))
    {
        cout << endl
             << "Invalid Contact! Enter a valid Pakistani number like 03001234567." << endl;
        cout << "----------------------------------------" << endl;
        cout << "Enter Contact again: ";
        cin >> newUser.contact;
        cleaned = normalizePhone(newUser.contact);
    }

    newUser.contact = cleaned;

    // If user is a driver or both, get vehicle capacity
    if (newUser.role == 1 || newUser.role == 3)
    {
        cout << "Enter Vehicle Capacity: ";
        cin >> newUser.vehicle_capacity;
    }
    else
    {
        newUser.vehicle_capacity = 0; // No vehicle capacity for riders
    }

    // Save user data in file
    saveUser(newUser);
    printSuccessMessage(newUser);
}

