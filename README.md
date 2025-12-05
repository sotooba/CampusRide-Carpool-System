# Campus Ride Carpool Management System

A console-based C++ application for managing ride-sharing and carpooling among university students. It allows users to **register as drivers, riders, or both**, create rides, search for rides, book rides, view statistics, and includes an **admin module** to manage users and rides.

---

## Features

### User Module
- Register as:
  - **Driver** – can create rides and offer seats.
  - **Rider** – can search and book rides.
  - **Both** – access to both roles.
- CNIC validation and formatted input.
- Contact number validation (Pakistani format `03XXXXXXXXX`).
- Age restriction for drivers (must be ≥ 18).
- Vehicle capacity input for drivers.

### Driver Module
- Create a ride with:
  - Route (e.g., Johar -> University)
  - Date (DD-MM-YYYY)
  - Time (HH:MM)
  - Total seats available
- Prevent ride conflicts for same driver at the same date and time.
- Ride ID auto-increment system.

### Rider Module
- Search rides by route and date.
- Book available seats.
- Prevent double booking for the same ride.

### Statistics
- Total users, drivers, riders, and users with both roles.
- Total active rides.
- Driver-specific statistics:
  - Total rides created
  - Seats offered
  - Seats filled
  - Average seat occupancy with recommendations

### Admin Module
- **View all users** – CNIC, name, role, age, department, contact, vehicle capacity.
- **View all rides** – Ride ID, driver CNIC, route, date, time, total and available seats, riders.
- **Filter rides** by route and/or date.

---

## Folder Structure

CampusRide-Carpool-System/
<br>
├── main.cpp // Main program and menu logic<br>
├── helpers.h // Header file with structures and function declarations<br>
├── helpers.cpp // Helper functions implementation<br>
├── users.csv // Stores registered users<br>
└── rides.csv // Stores created rides<br>

---

## Compilation & Running

1. **Compile using g++ (Linux/macOS/Windows with MinGW):**

```bash
g++ main.cpp helpers.cpp -o CampusRide
```

2. **Run the Program:**
./CampusRide

---

## How It Works
1. Users register first with valid CNIC and contact number.

2. Drivers can create rides specifying route, date, time, and available seats.

3. Riders can search rides and book seats.

4. Users can view system-wide statistics or driver-specific stats.

5. Admins can manage all users and rides.

---

## Dependencies
Standard C++ libraries: iostream, fstream, string, sstream, iomanip,  algorithm

---

Any type of valueable contributions are appreciated.
