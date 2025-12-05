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

    return false;
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

    cout << "-----------------------------------------------------------------------------" << endl;
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
    while (getline(infile, line)) // Read whole line
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
        getline(ss, item);
        r.riderCount = 0;

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

void showStatistics()
{
    printHeader("SYSTEM STATISTICS");

    ifstream userFile("users.csv");
    ifstream rideFile("rides.csv");

    if (!userFile)
    {
        cout << "No users registered yet." << endl;
        cout << "----------------------------------------" << endl;
        return;
    }

    // USER COUNTS
    int totalUsers = 0, totalDrivers = 0, totalRiders = 0, both = 0;

    string line;
    while (getline(userFile, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string cnic, name, dept, contact;
        string roleStr, ageStr, capacityStr;

        getline(ss, cnic, ',');
        getline(ss, name, ',');
        getline(ss, roleStr, ',');
        getline(ss, ageStr, ',');
        getline(ss, dept, ',');
        getline(ss, contact, ',');
        getline(ss, capacityStr, ',');

        int role = stoi(roleStr);
        totalUsers++;

        if (role == 1)
            totalDrivers++;
        else if (role == 2)
            totalRiders++;
        else if (role == 3)
        {
            totalDrivers++;
            totalRiders++;
            both++;
        }
    }

    // RIDE COUNTS
    int totalRides = 0;
    if (rideFile)
    {
        while (getline(rideFile, line))
        {
            if (!line.empty())
                totalRides++;
        }
    }

    // PRINT RESULTS
    printResults(totalUsers, totalDrivers, totalRiders, both, totalRides);
    
}


void printResults(const int totalUsers, const int totalDrivers, const int totalRiders, const int both, const int totalRides)
{
    cout << "Total Registered Users : " << totalUsers << endl;
    cout << "Total Drivers          : " << totalDrivers << endl;
    cout << "Total Riders           : " << totalRiders << endl;
    cout << "Users with Both Roles  : " << both << endl;

    cout << "----------------------------------------" << endl;

    cout << "Total Active Rides     : " << totalRides << endl;

    cout << "----------------------------------------" << endl;

    // Ask if user wants driver-specific stats
    char choice;
    cout << "Do you want statistics of a specific driver? (Y/N): ";
    cin >> choice;

    if (choice == 'Y' || choice == 'y')
        driverStats();
}


void driverStats()
{
    clearScreen();
    printHeader("DRIVER SPECIFIC STATISTICS");

    string cnic;

    while (true)
    {
        cout << "Enter Driver CNIC: ";
        cin >> cnic;
        cin.ignore();
        if (validateCNIC(cnic))
            break;
    }

    if (!isDriver(cnic))
    {
        cout << "This user is not registered as a driver!" << endl;
        cout << "----------------------------------------" << endl;
        return;
    }

    ifstream rideFile("rides.csv");

    if (!rideFile)
    {
        cout << "No rides available!" << endl;
        cout << "----------------------------------------" << endl;
        return;
    }

    int rideCount = 0;
    int totalSeatsOffered = 0;
    int totalSeatsFilled = 0;

    string line;
    while (getline(rideFile, line))
    {
        stringstream ss(line);
        string id, driver, route, date, time, seatTotal, seatsLeft, riders;

        getline(ss, id, ',');
        getline(ss, driver, ',');
        getline(ss, route, ',');
        getline(ss, date, ',');
        getline(ss, time, ',');
        getline(ss, seatTotal, ',');
        getline(ss, seatsLeft, ',');
        getline(ss, riders); // riders list

        if (driver == cnic)
        {
            rideCount++;
            int total = stoi(seatTotal);
            int left = stoi(seatsLeft);

            totalSeatsOffered += total;
            totalSeatsFilled += (total - left);
        }
    }

    if (rideCount == 0)
    {
        cout << "No rides found for this driver." << endl;
        cout << "----------------------------------------" << endl;
        return;
    }

    // PRINT RESULTS
    cout << "----------------------------------------" << endl;
    cout << "Total Rides Created  : " << rideCount << endl;
    cout << "Total Seats Offered  : " << totalSeatsOffered << endl;
    cout << "Total Seats Filled   : " << totalSeatsFilled << endl;

    double avgFill = (totalSeatsFilled * 100.0) / totalSeatsOffered;
    cout << "Average Seats Filled : " << fixed << setprecision(2) << avgFill << "%\n";

    cout << "----------------------------------------" << endl;


    if (avgFill < 50.0)
    {
        cout << "Recommendation: Consider posting earlier or adjusting your departure time." << endl;
        cout << "----------------------------------------" << endl;
    }
}


// ================= View All Users ================= //
void viewAllUsers()
{
    printHeader("\tALL REGISTERED USERS");

    ifstream infile("users.csv");
    if (!infile)
    {
        cout << "No users registered yet." << endl;
        return;
    }

    string line;
    cout << left << setw(8) << "S.No"
         << setw(25) << "CNIC"
         << setw(25) << "Name"
         << setw(10) << "Role"
         << setw(5) << "Age"
         << setw(25) << "Department"
         << setw(15) << "Contact"
         << setw(10) << "Capacity" << endl;
    printLine();

    int i = 0;
    while (getline(infile, line))
    {
        stringstream ss(line);
        string cnic, name, roleStr, ageStr, dept, contact, capacityStr;
        getline(ss, cnic, ',');
        getline(ss, name, ',');
        getline(ss, roleStr, ',');
        getline(ss, ageStr, ',');
        getline(ss, dept, ',');
        getline(ss, contact, ',');
        getline(ss, capacityStr, ',');

        int role = stoi(roleStr);
        string roleName;
        if (role == 1) roleName = "Driver";
        else if (role == 2) roleName = "Rider";
        else roleName = "Both";

        cout << left << setw(8) << i + 1
             << setw(25) << cnic
             << setw(25) << name
             << setw(10) << roleName
             << setw(5) << ageStr
             << setw(25) << dept
             << setw(15) << contact
             << setw(10) << capacityStr << endl;
        i++;
    }

   printLine();
}

// ================= View All Rides ================= //
void viewAllRides()
{
    printHeader("\t  ALL RIDES");

    ifstream infile("rides.csv");
    if (!infile)
    {
        cout << "No rides available yet!" << endl;
        return;
    }

    string line;
    cout << left << setw(10) << "RideID"
         << setw(25) << "Driver CNIC"
         << setw(15) << "Route"
         << setw(15) << "Date"
         << setw(10) << "Time"
         << setw(10) << "Seats"
         << setw(15) << "Available"
         << setw(20) << "Riders" << endl;
    printLine();

    while (getline(infile, line))
    {
        stringstream ss(line);
        string rideID, driver, route, date, time, totalSeats, availableSeats, riders;
        getline(ss, rideID, ',');
        getline(ss, driver, ',');
        getline(ss, route, ',');
        getline(ss, date, ',');
        getline(ss, time, ',');
        getline(ss, totalSeats, ',');
        getline(ss, availableSeats, ',');
        getline(ss, riders);

        cout << left << setw(10) << rideID
             << setw(25) << driver
             << setw(15) << route
             << setw(15) << date
             << setw(10) << time
             << setw(10) << totalSeats
             << setw(15) << availableSeats
             << setw(20) << riders << endl;
    }

    printLine();
}

// ================= Filter Rides by Date/Route ================= //
void filterRidesByDateRoute()
{
    printHeader("\t  FILTER RIDES");

    ifstream infile("rides.csv");
    if (!infile)
    {
        cout << "No rides available yet!" << endl;
        return;
    }

    string routeFilter, dateFilter;
    // cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clean buffer

    cout << "Enter Route to filter (leave blank for all): ";
    getline(cin, routeFilter);
    cout << "Enter Date to filter (DD-MM-YYYY, leave blank for all): ";
    getline(cin, dateFilter);

    string line;
    bool found = false;

    cout << endl;
    printLine();
    cout << left << setw(10) << "RideID"
         << setw(25) << "Driver CNIC"
         << setw(15) << "Route"
         << setw(15) << "Date"
         << setw(10) << "Time"
         << setw(10) << "Seats"
         << setw(15) << "Available"
         << setw(20) << "Riders" << endl;
    printLine();

    while (getline(infile, line))
    {
        stringstream ss(line);
        string rideID, driver, route, date, time, totalSeats, availableSeats, riders;
        getline(ss, rideID, ',');
        getline(ss, driver, ',');
        getline(ss, route, ',');
        getline(ss, date, ',');
        getline(ss, time, ',');
        getline(ss, totalSeats, ',');
        getline(ss, availableSeats, ',');
        getline(ss, riders);

        if ((routeFilter.empty() || route == routeFilter) &&
            (dateFilter.empty() || date == dateFilter))
        {
            found = true;
            cout << left << setw(10) << rideID
                 << setw(25) << driver
                 << setw(15) << route
                 << setw(15) << date
                 << setw(10) << time
                 << setw(10) << totalSeats
                 << setw(15) << availableSeats
                 << setw(20) << riders << endl;
        }
    }

    if (!found)
        cout << "No rides matched your filter criteria." << endl;

    printLine();
}

void printLine()
{
    cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
}