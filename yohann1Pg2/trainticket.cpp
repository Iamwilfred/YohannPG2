#include "trainticket.h"
#include <iostream>
#include <iomanip>
#include <unordered_map>

using std::string;
using std::vector;

static const std::unordered_map<string,string> s_classMap {
    {"SSP1","Supersparpreis 1. Klasse"},
    {"SSP2","Supersparpreis 2. Klasse"},
    {"SP1", "Sparpreis 1. Klasse"},
    {"SP2", "Sparpreis 2. Klasse"},
    {"FP1", "Flexpreis 1. Klasse"},
    {"FP2", "Flexpreis 2. Klasse"}
};

string TrainTicket::classDescription(const string& code)
{
    auto it = s_classMap.find(code);
    return (it != s_classMap.end()) ? it->second : code;   // inconnu → on affiche le code brut
}




TrainTicket::TrainTicket(const string& id,
                         double        price,
                         const string& fromDate,
                         const string& toDate,
                         long          travelId,
                         const string& fromStation,
                         const string& toStation,
                         const vector<string>& connections,
                         const string& departureTime,
                         const string& arrivalTime,
                         const string& bookingClassCode,
                         const double& fromStationLatitude,
                         const double& fromStationLongitude,
                         const double& toStationLatitude,
                         const double& toStationLongitude)
    : Booking(id, price, fromDate, toDate, travelId),
    fromStation_(fromStation),
    toStation_(toStation),
    connectingStations_(connections),
    departureTime_(departureTime),
    arrivalTime_(arrivalTime),
    bookingClassCode_(bookingClassCode),
    fromStationLatitude(fromStationLatitude),
    fromStationLongitude(fromStationLongitude),
    toStationLatitude(toStationLatitude),
    toStationLongitude(toStationLongitude)
{
}

void TrainTicket::showDetails() const
{
    std::cout << std::fixed << std::setprecision(2)
              << "Zugticket [" << id << "]  "
              << fromStation_ << " → " << toStation_
              << "  (" << classDescription(bookingClassCode_) << ")\n"
              << "Abfahrt : " << departureTime_
              << "   Ankunft : " << arrivalTime_ << '\n';

    if (!connectingStations_.empty()) {
        std::cout << "Zwischenhalte: ";
        for (size_t i = 0; i < connectingStations_.size(); ++i) {
            std::cout << connectingStations_[i];
            if (i + 1 < connectingStations_.size()) std::cout << ", ";
        }
        std::cout << '\n';
    }
    std::cout << "Preis : " << price << " €\n";
}
