#ifndef HELPERS_H
#define HELPERS_H

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
void createRide();
bool isDriver(const string &cnic);
int get_next_ride_ID();
void append_rideto_csv(const Ride &ride);
bool rideConflict(const Ride &ride);
void searchRides();
void printTableHeader();
void bookRide();
bool isUser(const string &cnic);
void showStatistics();
void driverStats();


#endif