#include <algorithm>
#include <cctype>
#include <limits>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "helpers.h"

using namespace std;

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
    ofstream outfile("users.csv", ios::app);

    if (!outfile)
    {
        cout << "Error opening file!" << endl;
        cout << "----------------------------------------" << endl;
        cout << endl;
        return;
    }

    outfile << user.cnic << ","
            << user.name << ","
            << user.role << ","
            << user.age << ","
            << user.department << ","
            << user.contact << ","
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

    switch (user.role)
    {
    case 1:
        cout << left << setw(17) << "Role:" << "Driver" << endl;
        break;
    case 2:
        cout << left << setw(17) << "Role:" << "Rider" << endl;
        break;
    case 3:
        cout << left << setw(17) << "Role:" << "Both (Driver & Rider)" << endl;
        break;
    default:
        cout << left << setw(17) << "Role:" << "Unknown" << endl;
    }

    if (user.vehicle_capacity > 0)
        cout << left << setw(17) << "Capacity:" << user.vehicle_capacity << endl;
    cout << "----------------------------------------" << endl;
}

bool isDriver(const string &cnic)
{
    ifstream infile("users.csv");

    if (!infile)
        return false;

    string line;
    while (getline(infile, line))
    {                          // Read whole line
        stringstream ss(line); // Create string stream from that line

        string dummy, fileCNIC;
        int role;

        getline(ss, fileCNIC, ','); // Reads CNIC from line & stops at ','
        getline(ss, dummy, ',');    // Reads name
        getline(ss, dummy, ',');    // Name is overwritten by role here

        role = stoi(dummy); // Converts string into int

        if (fileCNIC == cnic && (role == 1 || role == 3))
        {
            return true;
        }
    }
    return false;
}

int get_next_ride_ID()
{
    ifstream infile("rides.csv");

    if (!infile)
        return 1; // File does not exist, so it's a first ride

    int maxID = 0;
    string line;

    while (getline(infile, line))
    {
        stringstream ss(line);
        string dummy;
        getline(ss, dummy, ','); // Get string rideID

        int id = stoi(dummy);

        if (id > maxID)
            maxID = id;
    }

    return maxID + 1;
}

bool rideConflict(const Ride &ride)
{
    /*
        Check Rides:
            Same driver -> same date -> same time
    */

    ifstream infile("rides.csv");

    if (!infile)
        return false; // No file means no conflict

    string line;

    while (getline(infile, line))
    {
        stringstream ss(line);
        string dummy, date, time, cnic;

        getline(ss, dummy, ','); // Ignore rideID
        getline(ss, cnic, ',');
        getline(ss, dummy, ','); // Ignore route
        getline(ss, date, ',');
        getline(ss, time, ',');

        if (cnic == ride.driver_cnic && date == ride.date && time == ride.time)
            return true;
    }
}

void append_rideto_csv(const Ride &ride)
{
    ofstream outfile("rides.csv", ios::app);
    if (!outfile)
    {
        cout << "Error opening rides.csv!" << endl;
        return;
    }

    outfile << ride.rideID << ","
            << ride.driver_cnic << ","
            << ride.route << ","
            << ride.date << ","
            << ride.time << ","
            << ride.totalSeats << ","
            << ride.availableSeats << ","
            << "" << endl; // For now rider array is empty
}

void searchRides()
{
    printHeader("SEARCH RIDES - RIDER MODULE");

    // --- Read all rides into an array ---
    Ride rides[20];
    int rideCount = 0;

    ifstream infile("rides.csv");
    if (!infile)
    {
        cout << "No rides available yet!" << endl;
        cout << "----------------------------------------" << endl;
        return;
    }

    string line;
    while (getline(infile, line) && rideCount < 20)
    {
        Ride r;
        string item;
        stringstream ss(line);

        getline(ss, item, ',');
        r.rideID = stoi(item);

        getline(ss, r.driver_cnic, ',');
        getline(ss, r.route, ',');
        getline(ss, r.date, ',');
        getline(ss, r.time, ',');

        getline(ss, item, ',');
        r.totalSeats = stoi(item);

        getline(ss, item, ',');
        r.availableSeats = stoi(item);

        getline(ss, item); // ignore rider list for now
        r.riderCount = 0;

        rides[rideCount++] = r;
    }

    // --- Ask user for route and date ---
    string route, date;

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clean buffer

    cout << "Enter Route to search (e.g. Johar -> University): ";
    getline(cin, route);

    cout << "Enter Date (DD-MM-YY): ";
    getline(cin, date);

    clearScreen();

    printHeader("\t Available Rides");

    cout << endl;
    printTableHeader();

    // --- Step 3: Search and display results ---
    bool found = false;

    for (int i = 0; i < rideCount; i++)
    {
        if (rides[i].route == route && rides[i].date == date)
        {
            found = true;

            cout << left << setw(15) << rides[i].rideID
                 << setw(25) << rides[i].driver_cnic
                 << setw(20) << rides[i].time
                 << setw(10) << rides[i].availableSeats
                 << endl;
        }
    }

    if (!found)
    {
        cout << left << setw(15) << "N/A"
             << setw(25) << "N/A"
             << setw(20) << "N/A"
             << setw(10) << "N/A"
             << endl;
        cout << endl
             << "No rides found for this route and date." << endl;
        cout << "--------------------------------------------------------" << endl;
    }

    cout << "--------------------------------------------------------" << endl;
}

void printTableHeader()
{
    cout << left << setw(15) << "Ride ID"
         << setw(25) << "Driver CNIC"
         << setw(20) << "Departure Time"
         << setw(10) << "Seats left";

    cout << endl
         << "-----------------------------------------------------------------------------" << endl;
}

bool isUser(const string &cnic)
{
    ifstream infile("users.csv");

    if (!infile)
        return false;

    string line;
    while (getline(infile, line))   // Read whole line
    {                          
        stringstream ss(line); // Create string stream from that line

        string dummy, fileCNIC;
        int role;

        getline(ss, fileCNIC, ','); // Reads CNIC from line & stops at ','
        getline(ss, dummy, ',');    // Reads name
        getline(ss, dummy, ',');    // Name is overwritten by role here

        role = stoi(dummy); // Converts string into int

        if (fileCNIC == cnic && (role == 2 || role == 3))
        {
            return true;
        }
    }
    return false;
}

void bookRide()
{
    printHeader("\tBOOK RIDE - RIDER MODULE");

    // ----------- Load rides into array -----------
    Ride rides[20];
    int rideCount = 0;

    ifstream infile("rides.csv");
    if (!infile)
    {
        cout << "No rides available to book!" << endl;
        cout << "----------------------------------------" << endl;
        return;
    }

    string line;
    while (getline(infile, line) && rideCount < 20)
    {
        Ride r;
        string item;
        stringstream ss(line);

        getline(ss, item, ','); r.rideID = stoi(item);
        getline(ss, r.driver_cnic, ',');
        getline(ss, r.route, ',');
        getline(ss, r.date, ',');
        getline(ss, r.time, ',');
        getline(ss, item, ','); r.totalSeats = stoi(item);
        getline(ss, item, ','); r.availableSeats = stoi(item);
        getline(ss, item); r.riderCount = 0;

        stringstream ridersStream(item);
        string riderCNIC;
        while (getline(ridersStream, riderCNIC, '|') && r.riderCount < 20)
            r.riders[r.riderCount++] = riderCNIC;

        rides[rideCount++] = r;
    }

    // ----------- Ask for Ride ID -----------
    int selectedID;
    cout << "Enter Ride ID to book: ";
    cin >> selectedID;

    // Find the ride
    int idx = -1;
    for (int i = 0; i < rideCount; i++)
    {
        if (rides[i].rideID == selectedID)
        {
            idx = i;
            break;
        }
    }

    if (idx == -1)
    {
        cout << "Ride not found!" << endl;
        cout << "----------------------------------------" << endl;
        return;
    }

    Ride &ride = rides[idx];

    if (ride.availableSeats <= 0)
    {
        cout << "No seats available on this ride!" << endl;
        cout << "----------------------------------------" << endl;
        return;
    }

    // ----------- Ask Rider CNIC -----------
    string cnic;
    while (true)
    {
        cout << "Enter your CNIC: ";
        cin >> cnic;
        cin.ignore();

        if (!validateCNIC(cnic))
            continue;

        if (!isUser(cnic))
        {
            cout << "You are not registered as a Rider! Please register before booking a ride." << endl;
            cout << "----------------------------------------" << endl;
            return;
        }
        break;
    }

    // Prevent double booking
    for (int i = 0; i < ride.riderCount; i++)
    {
        if (ride.riders[i] == cnic)
        {
            cout << "You have already booked this ride!" << endl;
            cout << "----------------------------------------" << endl;
            return;
        }
    }

    // ----------- Book Seat -----------
    ride.riders[ride.riderCount++] = cnic;
    ride.availableSeats--;

    // ----------- Save updated rides back to CSV -----------
    ofstream outfile("rides.csv", ios::trunc);

    for (int i = 0; i < rideCount; i++)
    {
        outfile << rides[i].rideID << ","
                << rides[i].driver_cnic << ","
                << rides[i].route << ","
                << rides[i].date << ","
                << rides[i].time << ","
                << rides[i].totalSeats << ","
                << rides[i].availableSeats << ",";

        // join rider list
        for (int j = 0; j < rides[i].riderCount; j++)
        {
            outfile << rides[i].riders[j];
            if (j != rides[i].riderCount - 1)
                outfile << "|";
        }
        outfile << endl;
    }

    cout << endl
         << "Seat booked successfully!" << endl;
    cout << "----------------------------------------" << endl;
}
