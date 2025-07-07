
// Sports Club Booking System (C++) with Time Slots
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
using namespace std;

// --------------------- Base Class ---------------------
class Facility {
protected:
    int id;
    string name;
    double pricePerHour;
    map<string, bool> timeSlots; // slot => available or not

public:
    Facility(int id, string name, double price)
        : id(id), name(name), pricePerHour(price) {
        // Initialize default time slots
        timeSlots = {
            {"08:00-09:00", true},
            {"09:00-10:00", true},
            {"10:00-11:00", true},
            {"11:00-12:00", true},
            {"16:00-17:00", true},
            {"17:00-18:00", true},
            {"18:00-19:00", true}
        };
    }

    virtual ~Facility() {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return pricePerHour; }

    virtual void display() const {
        cout << "ID: " << id << ", Facility: " << name
             << ", Rate/hour: Rs." << pricePerHour << endl;
        cout << "Available Slots: ";
        for (const auto& slot : timeSlots) {
            if (slot.second)
                cout << slot.first << " ";
        }
        cout << endl;
    }

    bool isSlotAvailable(const string& slot) const {
        return timeSlots.count(slot) && timeSlots.at(slot);
    }

    bool bookSlot(const string& slot) {
        if (isSlotAvailable(slot)) {
            timeSlots[slot] = false;
            return true;
        }
        return false;
    }

    void releaseSlot(const string& slot) {
        if (timeSlots.count(slot)) {
            timeSlots[slot] = true;
        }
    }
};

// --------------------- Derived Class ---------------------
class PremiumFacility : public Facility {
private:
    string feature;
public:
    PremiumFacility(int id, string name, double price, string feat)
        : Facility(id, name, price), feature(feat) {}

    void display() const override {
        Facility::display();
        cout << "  -> Type: Premium | Feature: " << feature << endl;
    }
};

// --------------------- User Class ---------------------
class User {
private:
    string username;
    string password;
public:
    User(string u, string p) : username(u), password(p) {}
    string getUsername() const { return username; }
    bool login(string p) const { return p == password; }
};

// --------------------- Club Booking System ---------------------
class ClubSystem {
private:
    vector<Facility*> facilities;
    unordered_map<string, User> users;
    string loggedInUser;
    const string upi = "abc@hdfcbank";

public:
    ~ClubSystem() {
        for (auto f : facilities) delete f;
    }

    void registerUser(string username, string password) {
        users[username] = User(username, password);
    }

    bool loginUser(string username, string password) {
        if (users.count(username) && users[username].login(password)) {
            loggedInUser = username;
            cout << "Login successful! Welcome, " << username << "\n";
            return true;
        }
        cout << "Invalid credentials.\n";
        return false;
    }

    void addFacility(Facility* f) {
        facilities.push_back(f);
    }

    void showFacilities() const {
        cout << "\nAvailable Facilities and Time Slots:\n";
        for (const auto& f : facilities)
            f->display();
    }

    void bookFacility(int id, const string& slot) {
        if (loggedInUser.empty()) {
            cout << "Please login to book a facility.\n";
            return;
        }
        for (auto& f : facilities) {
            if (f->getId() == id) {
                if (f->isSlotAvailable(slot)) {
                    if (f->bookSlot(slot)) {
                        double total = f->getPrice();
                        cout << "Facility \"" << f->getName() << "\" booked for slot " << slot << ".\n";
                        cout << "Total Amount: Rs." << total << endl;
                        cout << "Please pay via UPI to: " << upi << "\n";
                    } else {
                        cout << "Failed to book slot.\n";
                    }
                } else {
                    cout << "Selected slot is not available.\n";
                }
                return;
            }
        }
        cout << "Invalid Facility ID.\n";
    }

    void releaseFacility(int id, const string& slot) {
        for (auto& f : facilities) {
            if (f->getId() == id) {
                f->releaseSlot(slot);
                cout << "Slot " << slot << " of Facility ID " << id << " has been released.\n";
                return;
            }
        }
        cout << "Invalid Facility ID.\n";
    }
};

// --------------------- Main ---------------------
int main() {
    ClubSystem club;

    // Register users
    club.registerUser("john", "1234");
    club.registerUser("alice", "pass");

    // Add facilities
    club.addFacility(new Facility(1, "Cricket Turf", 1200));
    club.addFacility(new PremiumFacility(2, "Indoor Basketball Court", 1800, "Air-conditioned"));
    club.addFacility(new Facility(3, "Pickleball Court", 900));

    int choice, id;
    string username, password, slot;

    do {
        cout << "\n--- Club Booking Menu ---\n";
        cout << "1. Login\n2. Show Facilities\n3. Book a Facility\n4. Release a Facility\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Username: "; cin >> username;
                cout << "Password: "; cin >> password;
                club.loginUser(username, password);
                break;
            case 2:
                club.showFacilities();
                break;
            case 3:
                cout << "Enter Facility ID to book: "; cin >> id;
                cout << "Enter Time Slot (e.g., 08:00-09:00): "; cin >> slot;
                club.bookFacility(id, slot);
                break;
            case 4:
                cout << "Enter Facility ID to release: "; cin >> id;
                cout << "Enter Time Slot to release: "; cin >> slot;
                club.releaseFacility(id, slot);
                break;
            case 5:
                cout << "Thank you for using the Club Booking System.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
