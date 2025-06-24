#include "hotelbooking.h"
#include <iostream>
#include <iomanip>

HotelBooking::HotelBooking(const std::string& id,
                           double             price,
                           const std::string& fromDate,
                           const std::string& toDate,
                           long               travelId,
                           const std::string& hotelName,
                           const std::string& city,
                           const std::string& roomTypeCode,
                           const double& hotelLatitude,
                           const double& hotelLongitude)
    : Booking(id, price, fromDate, toDate, travelId),
    hotelName(hotelName),
    city(city),
    roomTypeCode(roomTypeCode),
    hotelLatitude(hotelLatitude),
    hotelLongitude(hotelLongitude)

{
}



std::string HotelBooking::roomTypeText() const
{
    if (roomTypeCode == "EZ") return "Einzelzimmer";
    if (roomTypeCode == "DZ") return "Doppelzimmer";
    if (roomTypeCode == "SU") return "Suite";
    if (roomTypeCode == "AP") return "Appartment";
    return roomTypeCode;              // code inconnu → brut
}

void HotelBooking::showDetails() const
{
    std::cout << std::fixed << std::setprecision(2)
              << "Hotelbuchung [" << id << "]  "
              << hotelName << " (" << city << ")  "
              << "vom " << fromDate << " bis " << toDate
              << "  Zimmer: " << roomTypeText()
              << "  Preis: " << price << " €\n";
}
