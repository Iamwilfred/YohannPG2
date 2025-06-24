#ifndef TRAINTICKET_H
#define TRAINTICKET_H

#include "booking.h"
#include <string>
#include <vector>

class TrainTicket : public Booking
{
    std::string fromStation_;
    std::string toStation_;
    std::vector<std::string> connectingStations_;
    std::string departureTime_;
    std::string arrivalTime_;
    std::string bookingClassCode_;   // e.g. “SSP1”
    double fromStationLatitude=0.0;
    double fromStationLongitude=0.0;
    double toStationLatitude=0.0;
    double toStationLongitude=0.0;

    /// convertit le code (‘SSP1’, ‘FP2’, …) en intitulé lisible


public:
    TrainTicket(const std::string& id,
                double             price,
                const std::string& fromDate,
                const std::string& toDate,
                long               travelId,
                const std::string& fromStation,
                const std::string& toStation,
                const std::vector<std::string>& connections,
                const std::string& departureTime,
                const std::string& arrivalTime,
                const std::string& bookingClassCode,
                const double& fromStationLatitude,
                const double& fromStationLongitude,
                const double& toStationLatitude,
                const double& toStationLongitude);

    void showDetails() const override;

    // getters utilisés par l’IHM
    const std::string&              getFrom()        const { return fromStation_; }
    const std::string&              getTo()          const { return toStation_;  }
    const std::vector<std::string>& getStopovers()   const { return connectingStations_; }
    const std::string&              getDepartureTime() const { return departureTime_; }
    const std::string&              getArrivalTime()   const { return arrivalTime_; }
    const std::string&              getBookingClass()  const { return bookingClassCode_; }
    static std::string classDescription(const std::string& code);

    double getFromStationLatitude() const {return fromStationLatitude;}
    double getFromStationLongitude() const {return fromStationLongitude;}
    double getToStationLatitude() const { return toStationLatitude;}
    double getToStationLongitude() const{ return toStationLongitude;}
};

#endif // TRAINTICKET_H
