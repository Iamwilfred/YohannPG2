#include "flightbooking.h"
#include <iostream>
#include <iomanip>

FlightBooking::FlightBooking(const std::string& id,
                             double             price,
                             const std::string& fromDate,
                             const std::string& toDate,
                             long               travelId,
                             const std::string& from,
                             const std::string& to,
                             const std::string& airline,
                             const std::string& bookingClassCode,
                             const double& fromDestLatitude,
                             const double& fromDestLongitude,
                             const double& toDestLatitude,
                             const double& toDestLongitude)
    : Booking(id, price, fromDate, toDate, travelId),
    fromDestination(from),
    toDestination(to),
    airline(airline),
    bookingClassCode(bookingClassCode),
    fromDestLatitude(fromDestLatitude),
    fromDestLongitude(fromDestLongitude),
    toDestLatitude(toDestLatitude),
    toDestLongitude(toDestLongitude)
{
}


std::string FlightBooking::bookingClassText() const
{
    if (bookingClassCode == "Y") return "Economy";
    if (bookingClassCode == "W") return "Premium Economy";
    if (bookingClassCode == "J") return "Business";
    if (bookingClassCode == "F") return "First";
    return bookingClassCode;           // code inconnu → on l’affiche tel quel
}

void FlightBooking::showDetails() const
{
    std::cout << std::fixed << std::setprecision(2)
              << "Flugbuchung [" << id << "]  "
              << fromDestination << " → " << toDestination
              << "  (" << airline << ", " << bookingClassText() << ")  "
              << "vom " << fromDate << " bis " << toDate
              << "  Preis: " << price << " €\n";
}
