#ifndef BOOKING_H
#define BOOKING_H

#include <string>

/**
 *  Classe de base abstraite pour toutes les réservations.
 *  - id       : identifiant unique (NON modifiable)
 *  - travelId : id de la Travel dont elle dépend (NON modifiable)
 *  - price, fromDate, toDate : modifiables selon l’UI (Aufgabe 4)
 */
class Booking
{
protected:
    std::string id;
    double      price;
    std::string fromDate;
    std::string toDate;
    long        travelId;

public:
    Booking(std::string id,
            double      price,
            std::string fromDate,
            std::string toDate,
            long        travelId);
    virtual ~Booking();

    // --- interface « lecture » ---
    std::string  getId()       const { return id; }
    double       getPrice()    const { return price; }
    std::string  getFromDate() const { return fromDate; }
    std::string  getToDate()   const { return toDate; }
    long         getTravelId() const { return travelId; }

    // --- interface « écriture » (tout sauf id & travelId) ---
    void setPrice(double p)                   { price = p; }
    void setFromDate(const std::string& d)    { fromDate = d; }
    void setToDate(const std::string& d)      { toDate   = d; }

    // Nom de la classe concrète (FlightBooking, …) – utile pour les icônes.
//    virtual std::string getClassName() const = 0;

    // Affichage console / debug
    virtual void showDetails() const = 0;
};

#endif // BOOKING_H
