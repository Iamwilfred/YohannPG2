#include "rentalcarreservation.h"
#include <iostream>
#include <iomanip>

RentalCarReservation::RentalCarReservation(const std::string& id,
                                           double             price,
                                           const std::string& fromDate,
                                           const std::string& toDate,
                                           long               travelId,
                                           const std::string& pickupLocation,
                                           const std::string& returnLocation,
                                           const std::string& company,
                                           const std::string& vehicleClass,
                                           const double &pickupLatitude,
                                           const double &pickupLongitude,
                                           const double &returnLatitude,
                                           const double &returnLongitude)
    : Booking(id, price, fromDate, toDate, travelId),
    pickupLocation(pickupLocation),
    returnLocation(returnLocation),
    company(company),
    vehicleClass(vehicleClass),
    pickupLatitude(pickupLatitude),
    pickupLongitude(pickupLongitude),
    returnLatitude(returnLatitude),
    returnLongitude(returnLongitude)
{
}

void RentalCarReservation::showDetails() const
{
    std::cout << std::fixed << std::setprecision(2)
              << "Mietwagen [" << id << "]  "
              << company << "  Klasse: " << vehicleClass << '\n'
              << "Abholung  : " << fromDate << "  (" << pickupLocation  << ")\n"
              << "Rückgabe  : " << toDate   << "  (" << returnLocation << ")\n"
              << "Preis     : " << price    << " €\n";
}
