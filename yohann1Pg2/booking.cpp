#include "booking.h"

Booking::Booking(std::string id,
                 double      price,
                 std::string fromDate,
                 std::string toDate,
                 long        travelId)
    : id(std::move(id)),
    price(price),
    fromDate(std::move(fromDate)),
    toDate(std::move(toDate)),
    travelId(travelId)
{}

Booking::~Booking() = default;
