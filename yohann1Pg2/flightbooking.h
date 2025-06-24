#ifndef FLIGHTBOOKING_H
#define FLIGHTBOOKING_H

#include "booking.h"
#include <string>

class FlightBooking : public Booking
{
private:
    std::string fromDestination;
    std::string toDestination;
    std::string airline;
    std::string bookingClassCode;   // Y / W / J / F
    double fromDestLatitude=0.0;
    double fromDestLongitude=0.0;
    double toDestLatitude=0.0;
    double toDestLongitude=0.0;

    // Renvoie la description lisible de la classe de réservation


public:
    std::string bookingClassText() const;
    FlightBooking(const std::string& id,
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
                  const double& toDestLongitude);


    // --- interface requise ---
    void showDetails() const override;

    // --- getters utiles au GUI ---
    std::string getFrom()         const { return fromDestination; }
    std::string getTo()           const { return toDestination; }
    std::string getAirline()      const { return airline; }
    std::string getBookingClass() const { return bookingClassCode; }
    double getFromDestLatitude() const{ return fromDestLatitude;}
    double getFromDestLongitude() const{ return fromDestLongitude;}
    double getToDestLatitude() const{ return toDestLatitude;}
    double getToDestLongitude()const{ return toDestLongitude;}


    // --- blocage de la copie, autorisation du move (optionnel mais conseillé) ---
    FlightBooking(const FlightBooking&)            = delete;
    FlightBooking& operator=(const FlightBooking&) = delete;
    FlightBooking(FlightBooking&&)                 = default;
    FlightBooking& operator=(FlightBooking&&)      = default;

};

#endif // FLIGHTBOOKING_H
