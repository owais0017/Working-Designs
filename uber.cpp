#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum RideStatus {
    REQUESTED,
    DRIVER_ASSIGNED,
    STARTED,
    COMPLETED,
    CANCELLED
};

enum VehicleType {
    BIKE,
    CAR,
    AUTO
};

class Location {
public:
    double latitude;
    double longitude;

    Location(double lat = 0, double lon = 0)
        : latitude(lat), longitude(lon) {}
};

class Vehicle {
private:
    int vehicleId;
    VehicleType type;
    string plateNumber;

public:
    Vehicle(int id, VehicleType type, string plate)
        : vehicleId(id), type(type), plateNumber(plate) {}

    VehicleType getType() {
        return type;
    }

    string getPlateNumber() {
        return plateNumber;
    }
};

class User {
protected:
    int userId;
    string name;
    string phone;

public:
    User(int id, string name, string phone)
        : userId(id), name(name), phone(phone) {}

    int getUserId() {
        return userId;
    }

    string getName() {
        return name;
    }
};

class Rider : public User {
public:
    Rider(int id, string name, string phone)
        : User(id, name, phone) {}

    void requestRide() {
        cout << name << " requested a ride\n";
    }
};

class Driver : public User {
private:
    Vehicle vehicle;
    Location location;
    bool available;

public:
    Driver(int id, string name, string phone,
           Vehicle vehicle, Location loc)
        : User(id, name, phone),
          vehicle(vehicle),
          location(loc),
          available(true) {}

    bool isAvailable() {
        return available;
    }

    void setAvailability(bool status) {
        available = status;
    }

    Location getLocation() {
        return location;
    }

    string getVehicleNumber() {
        return vehicle.getPlateNumber();
    }
};

class Ride {
private:
    int rideId;
    Rider* rider;
    Driver* driver;
    Location source;
    Location destination;
    RideStatus status;
    double fare;

public:
    Ride(int id, Rider* r, Driver* d,
         Location src, Location dest)
        : rideId(id), rider(r), driver(d),
          source(src), destination(dest),
          status(REQUESTED), fare(0) {}

    void startRide() {
        status = STARTED;
        cout << "Ride started\n";
    }

    void endRide() {
        status = COMPLETED;
        cout << "Ride completed\n";
    }

    void setFare(double f) {
        fare = f;
    }

    double getFare() {
        return fare;
    }

    Driver* getDriver() {
        return driver;
    }
};

class FareStrategy {
public:
    virtual double calculateFare(double distance) = 0;
};

class NormalFare : public FareStrategy {
public:
    double calculateFare(double distance) override {
        return distance * 10;
    }
};

class MatchingService {
public:
    Driver* findDriver(vector<Driver*>& drivers) {

        for (auto driver : drivers) {
            if (driver->isAvailable()) {
                return driver;
            }
        }

        return nullptr;
    }
};

class RideService {
private:
    MatchingService matcher;
    FareStrategy* fareStrategy;
    int rideCounter;

public:
    RideService(FareStrategy* strategy)
        : fareStrategy(strategy), rideCounter(1) {}

    Ride* requestRide(Rider* rider,
                      Location src,
                      Location dest,
                      vector<Driver*>& drivers) {

        Driver* driver = matcher.findDriver(drivers);

        if (!driver) {
            cout << "No drivers available\n";
            return nullptr;
        }

        driver->setAvailability(false);

        Ride* ride = new Ride(rideCounter++, rider, driver, src, dest);

        cout << "Driver assigned: " << driver->getName() << endl;

        return ride;
    }

    void completeRide(Ride* ride, double distance) {

        ride->endRide();

        double fare = fareStrategy->calculateFare(distance);

        ride->setFare(fare);

        cout << "Fare calculated: " << fare << endl;

        ride->getDriver()->setAvailability(true);
    }
};

int main() {

    Location riderLoc(28.61, 77.23);
    Location destination(28.65, 77.28);

    Vehicle car1(1, CAR, "DL01AB1234");

    Driver d1(1, "Rahul", "9999999999", car1, riderLoc);

    Rider r1(101, "Owais", "8888888888");

    vector<Driver*> drivers = {&d1};

    NormalFare fareStrategy;

    RideService rideService(&fareStrategy);

    r1.requestRide();

    Ride* ride = rideService.requestRide(&r1, riderLoc, destination, drivers);

    if (ride) {

        ride->startRide();

        rideService.completeRide(ride, 12.5);
    }

    return 0;
}
