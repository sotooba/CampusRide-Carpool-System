#include <algorithm>
#include <cctype>
#include <limits>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

struct User
{
    string cnic, name, department, contact;
    int age, role;
    int vehicle_capacity;
};

struct Ride
{
    int rideID;
    string driver_cnic;
    string route, date, time;
    int totalSeats, availableSeats;
    string riders[50];
    int riderCount;
};

void printHeader(string prompt);
void viewMenu();
void clearScreen();
void waitForEnter();
bool validateCNIC(string &cnic);
void registerUser();
string normalizePhone(const string &number);
bool isValidNumber(const string &number);
void saveUser(const User &user);
void printSuccessMessage(const User &user);

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

void printHeader(string prompt)
{
    cout << "=======================================================" << endl;
    cout << "\t" << prompt << endl;
    cout << "=======================================================" << endl;
}

void viewMenu()
{
    cout << "1. Register User" << endl;
    cout << "2. Create Ride (Driver)" << endl;
    cout << "3. Search Rides (Rider)" << endl;
    cout << "4. Book Ride" << endl;
    cout << "5. Statistics" << endl;
    cout << "6. Admin" << endl;
    cout << "7. Exit" << endl;
}

void clearScreen()
{
// Clear the console screen
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForEnter()
{
    cout << "Press Enter key to continue: ";
    // Clear any leftover input on the line, then wait for the user to press Enter.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string _dummy;
    getline(cin, _dummy);
    cout << "----------------------------------------" << endl;
    cout << endl;
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
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

bool validateCNIC(string &cnic)
{
    // Remove any dashes from the CNIC
    cnic.erase(remove(cnic.begin(), cnic.end(), '-'), cnic.end());

    // Check if length is 13
    if (cnic.length() != 13)
    {
        cout << "Invalid CNIC! Valid CNIC must be 13 digits long." << endl;
        cout << "----------------------------------------" << endl;
        cout << endl;
        return false;
    }

    // Check if all characters are digits
    for (char c : cnic)
    {
        if (!isdigit(c))
        {
            cout << "Invalid CNIC! CNIC must contain only digits." << endl;
            cout << "----------------------------------------" << endl;
            cout << endl;
            return false;
        }
    }

    // Format CNIC as XXXXX-XXXXXXX-X
    string firstPart = cnic.substr(0, 5);
    string secondPart = cnic.substr(5, 7);
    string lastPart = cnic.substr(12, 1);

    cnic = firstPart + "-" + secondPart + "-" + lastPart;
    return true;
}

string normalizePhone(const string &number)
{
    string onlydigits;
    for (char c : number)
    {
        if (isdigit(c))
            onlydigits += c;
    }

    return onlydigits;
}

bool isValidNumber(const string &number)
{
    string n = normalizePhone(number);
    if (n.length() != 11)
        return false;
    if (n.substr(0, 2) != "03")
        return false;
    return true;
}

void saveUser(const User &user)
{
    ofstream outfile("users.txt", ios::app);

    if (!outfile)
    {
        cout << "Error opening file!" << endl;
        cout << "----------------------------------------" << endl;
        cout << endl;
        return;
    }

    outfile << user.cnic << " "
            << user.name << " "
            << user.role << " "
            << user.age << " "
            << user.department << " "
            << user.contact << " "
            << user.vehicle_capacity << endl;
}


void printSuccessMessage(const User &user)
{
    // Print the summary
    clearScreen();
    cout << "User Data Saved successfully!" << endl;
    printHeader("\t  User CONFIRMATION");

    cout << left << setw(17) << "CNIC:" << user.cnic << endl;
    cout << left << setw(17) << "Name:" << user.name << endl;
    cout << left << setw(17) << "Age:" << user.age << " y/o" << endl;
    cout << left << setw(17) << "Contact:" << user.contact << endl;
    cout << left << setw(17) << "Department:" << user.department << endl;
    cout << left << setw(17) << "Role:" << user.role << endl;
    if (user.vehicle_capacity > 0)
        cout << left << setw(17) << "Capacity:" << user.vehicle_capacity << endl;
    cout << "----------------------------------------" << endl;
}