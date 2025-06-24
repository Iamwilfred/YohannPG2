#ifndef TRAVELAGENCY_H
#define TRAVELAGENCY_H

#include <vector>
#include <string>
#include <map>
#include <memory>

#include "airport.h"

class Booking;
class Travel;
class Customer;

/* alias pratiques -------------------------------------------------*/
using BookingPtr  = std::shared_ptr<Booking>;
using TravelPtr   = std::shared_ptr<Travel>;
using CustomerPtr = std::shared_ptr<Customer>;

/**
 *  Conteneur racine :
 *   • allBookings  – chaque réservation (Flight/Hotel/…)
 *   • allTravels   – une Travel agrège des bookings
 *   • allCustomers – un Customer agrège des travels
 *   • airports_    – codes IATA → Airport
 */
class TravelAgency
{
    /* données ------------------------------------------------------*/
    std::vector<CustomerPtr> allCustomers;
    std::vector<TravelPtr>   allTravels;
    std::vector<BookingPtr>  allBookings;

    std::map<std::string, std::shared_ptr<Airport>> airports_;   // clé = IATA

public:
    /* construction / destruction ----------------------------------*/
    TravelAgency()  = default;
    ~TravelAgency() = default;                 // smart-ptr ⇒ désallocation auto

    /* E/S fichiers -------------------------------------------------*/
    std::string readFile (const std::string& filePath);   // charge bookings
    void        writeFile(const std::string& filePath) const;
    void        loadAirports(const std::string& path);    // charge iatacodes.json

    /* recherche ----------------------------------------------------*/
    BookingPtr  findBooking (const std::string& id) const;
    TravelPtr   findTravel  (long id)               const;
    CustomerPtr findCustomer(long id)               const;

    /* getters bruts (copie du vecteur de smart-ptr) ----------------*/
    std::vector<CustomerPtr> getAllCustomers() const { return allCustomers; }
    std::vector<TravelPtr>   getAllTravels()   const { return allTravels;   }
    std::vector<BookingPtr>  getAllBookings()  const { return allBookings;  }

    /* aéroports ----------------------------------------------------*/
    std::shared_ptr<Airport> getAirport(const std::string& code) const;
};

#endif // TRAVELAGENCY_H
