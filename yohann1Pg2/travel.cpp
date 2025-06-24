#include "travel.h"
#include "booking.h"
#include <sstream>

Travel::Travel(long id, long customerId)
    : id_(id), customerId_(customerId)
{}

void Travel::addBooking(const BookingPtr& b)
{
    bookings_.push_back(b);
}

std::string Travel::getOverview() const
{
    std::ostringstream oss;
    oss << "Travel ID: " << id_ << " (" << bookings_.size() << " bookings)";
    return oss.str();
}

std::string Travel::showDetails() const
{
    /* pour l’instant identique à overview */
    return getOverview();
}
