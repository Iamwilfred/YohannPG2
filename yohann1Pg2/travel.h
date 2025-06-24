#ifndef TRAVEL_H
#define TRAVEL_H

#include <vector>
#include <memory>
#include <string>

class Booking;

/* alias pratique — même nom que dans travelagency.h */
using BookingPtr = std::shared_ptr<Booking>;

class Travel
{
    long id_;
    long customerId_;
    std::vector<BookingPtr> bookings_;          // ← smart-pointers

public:
    Travel(long id, long customerId);

    /* plus besoin de destructeur : shared_ptr nettoie : */
    ~Travel() = default;

    /* getters */
    long getId()         const { return id_; }
    long getCustomerId() const { return customerId_; }
    const std::vector<BookingPtr>& getBookings() const { return bookings_; }

    /* mutateur */
    void addBooking(const BookingPtr& b);

    /* helpers texte */
    std::string getOverview() const;
    std::string showDetails() const;
};

#endif // TRAVEL_H
