#ifndef RENTALCARRESERVATION_H
#define RENTALCARRESERVATION_H

#include "booking.h"
#include <string>

class RentalCarReservation : public Booking
{
    std::string pickupLocation;   // lieu de prise en charge
    std::string returnLocation;   // lieu de restitution
    std::string company;          // société de location
    std::string vehicleClass;     // code véhicule (CBAR, ICAR, …)
    double pickupLatitude=0.0;
    double pickupLongitude=0.0;
    double returnLatitude=0.0;
    double returnLongitude=0.0;

public:
    RentalCarReservation(const std::string& id,
                         double             price,
                         const std::string& fromDate,
                         const std::string& toDate,
                         long               travelId,
                         const std::string& pickupLocation,
                         const std::string& returnLocation,
                         const std::string& company,
                         const std::string& vehicleClass,
                         const double& pickupLatitude,
                         const double& pickupLongitude,
                         const double& returnLatitude,
                         const double& returnLongitude);

    // — interface virtuelle —
    void showDetails() const override;

    // — getters utilisés par le GUI —
    std::string getPickupLocation() const { return pickupLocation; }
    std::string getReturnLocation() const { return returnLocation; }
    std::string getCompany()        const { return company; }
    std::string getVehicleClass()   const { return vehicleClass; }
    double getPickupLatitude() const  {return pickupLatitude;}
    double getPickupLongitude() const {return pickupLongitude;}
    double getReturnLatitude() const  { return returnLatitude;}
    double getReturnLongitude() const { return returnLongitude;}


    // blocage de la copie
    RentalCarReservation(const RentalCarReservation&)            = delete;
    RentalCarReservation& operator=(const RentalCarReservation&) = delete;
    RentalCarReservation(RentalCarReservation&&)                 = default;
    RentalCarReservation& operator=(RentalCarReservation&&)      = default;
};

#endif // RENTALCARRESERVATION_H
