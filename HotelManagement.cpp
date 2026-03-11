#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum RoomType{
    SINGLE,
    DOUBLE,
    SUITE
};

enum BookingStatus{
    CONFIRMED,
    CANCELLED,
    PENDING
};

enum PaymentStatus{
    SUCCESS,
    FAILED
};

class Room{
private:
    int roomNum;
    RoomType type;
    bool roomStatus;

public:
    int price;

    Room(int num, RoomType type, int price)
        : roomNum(num), type(type), price(price), roomStatus(false) {}

    bool checkStatus(){
        return roomStatus;
    }

    void reserveRoom(){
        if(roomStatus == false){
            roomStatus = true;
        }
        else{
            cout << "Room already reserved\n";
        }
    }

    int getRoomNum(){
        return roomNum;
    }
};

class User{
public:
    string name;
    int userId;
    string email;
    string phoneNum;

    User(string name, int userId, string email, string phoneNum)
        : name(name), userId(userId), email(email), phoneNum(phoneNum) {}

    string getName(){
        return name;
    }

    int getUserId(){
        return userId;
    }
};

class Payment{
private:
    int paymentId;
    double amount;
    PaymentStatus status;

public:
    Payment(int id, double amt)
        : paymentId(id), amount(amt), status(FAILED) {}

    void processPayment(){
        status = SUCCESS;
        cout << "Payment Successful\n";
    }
};

class Booking{
private:
    int bookingId;
    Room* room;
    User* user;
    BookingStatus status;

public:
    Booking(int id, Room* r, User* u)
        : bookingId(id), room(r), user(u), status(PENDING) {}

    void confirmBooking(){
        if(!room->checkStatus()){
            room->reserveRoom();
            status = CONFIRMED;
            cout << "Booking confirmed for " << user->getName() << endl;
        }
        else{
            cout << "Room already booked\n";
        }
    }

    void cancelBooking(){
        status = CANCELLED;
        cout << "Booking cancelled\n";
    }
};

class Hotel{
private:
    string name;
    vector<Room*> rooms;

public:
    Hotel(string name) : name(name) {}

    void addRoom(Room* room){
        rooms.push_back(room);
    }

    void showAvailableRooms(){
        for(auto room : rooms){
            if(!room->checkStatus()){
                cout << "Room " << room->getRoomNum() << " available\n";
            }
        }
    }
};

int main(){

    Hotel hotel("Grand Hotel");

    Room r1(101, SINGLE, 1000);
    Room r2(102, DOUBLE, 2000);

    hotel.addRoom(&r1);
    hotel.addRoom(&r2);

    User u1("Owais", 1, "owais@email.com", "999999");

    Booking b1(1, &r1, &u1);

    hotel.showAvailableRooms();

    b1.confirmBooking();

    hotel.showAvailableRooms();

}
