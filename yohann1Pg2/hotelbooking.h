#ifndef HOTELBOOKING_H
#define HOTELBOOKING_H

#include "booking.h"
#include <string>

class HotelBooking : public Booking
{
    std::string hotelName;
    std::string city;
    std::string roomTypeCode;          // EZ / DZ / SU / AP
    double hotelLatitude=0.0;
    double hotelLongitude=0.0;

    /** Renvoie « Einzelzimmer », « Doppelzimmer », … */


public:
    std::string roomTypeText() const;
    HotelBooking(const std::string& id,
                 double             price,
                 const std::string& fromDate,
                 const std::string& toDate,
                 long               travelId,
                 const std::string& hotelName,
                 const std::string& city,
                 const std::string& roomTypeCode,
                 const double& hotelLatitude,
                 const double& hotelLongitude
                 );

    // -- interface requise --
    void showDetails() const override;

    // -- getters utiles au GUI --
    std::string getHotelName() const  { return hotelName; }
    std::string getCity()       const { return city; }
    std::string getRoomType()   const { return roomTypeCode; }
    double getHotelLatitude() const  {return hotelLatitude;}
    double getHotelLongitude() const {return hotelLongitude;}


    // blocage de la copie
    HotelBooking(const HotelBooking&)            = delete;
    HotelBooking& operator=(const HotelBooking&) = delete;
    HotelBooking(HotelBooking&&)                 = default;
    HotelBooking& operator=(HotelBooking&&)      = default;

};

#endif // HOTELBOOKING_H
