#include <iostream>
#include <string>

enum class Zone { Inner = 0, Middle = 1, Outer = 2 };
enum class PassengerType { Regular, Student, SeniorCitizen };

class FareTable {
public:
    static constexpr long long BASE_FARE_PAISE[3] = {1000, 2000, 3000};
    static constexpr int PEAK_SURCHARGE_PCT = 20;
    static constexpr int CONCESSION_PCT = 25;

    static long long computeFare(Zone z, bool isPeak, PassengerType type) {
        long long fare = BASE_FARE_PAISE[static_cast<int>(z)];
        if (isPeak) fare += (fare * PEAK_SURCHARGE_PCT) / 100;
        if (type == PassengerType::Student || type == PassengerType::SeniorCitizen) {
            fare -= (fare * CONCESSION_PCT) / 100;
        }
        return fare;
    }
};

class SmartCard {
    std::string cardId;
    long long balancePaise;
    static constexpr long long MIN_REMAINING_PAISE = 5000;
public:
    SmartCard(std::string id, long long initialPaise)
        : cardId(std::move(id)), balancePaise(initialPaise < 0 ? 0 : initialPaise) {}
    bool canTravel(long long farePaise) const { return (balancePaise - farePaise) >= MIN_REMAINING_PAISE; }
    bool deductFare(long long farePaise) {
        if (!canTravel(farePaise)) return false;
        balancePaise -= farePaise;
        return true;
    }
    long long balance() const { return balancePaise; }
};

class Passenger {
    std::string name;
    PassengerType type;
public:
    Passenger(std::string n, PassengerType t) : name(std::move(n)), type(t) {}
    PassengerType getType() const { return type; }
    const std::string& getName() const { return name; }
};

class Journey {
    Zone farthestZone;
    int hour;
public:
    Journey(Zone z, int h) : farthestZone(z), hour(h) {}
    bool isPeak() const { return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20); }
    Zone getZone() const { return farthestZone; }
};

int main() {
    Passenger p("Ananya", PassengerType::Student);
    SmartCard card("NM-1001", 10000);
    Journey j(Zone::Outer, 9);
    long long fare = FareTable::computeFare(j.getZone(), j.isPeak(), p.getType());
    std::cout << "Passenger: " << p.getName() << " | Fare: " << fare << " paise\n";
    if (card.deductFare(fare)) {
        std::cout << "Approved. Balance: " << card.balance() << " paise\n";
    }
    return 0;
}
