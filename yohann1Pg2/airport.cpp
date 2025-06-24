#include "airport.h"
#include <sstream>

Airport::Airport(std::string code,
                 std::string name,
                 std::string country,
                 std::string city,
                 double      lat,
                 double      lon)
    : code_(std::move(code)),
    name_(std::move(name)),
    country_(std::move(country)),
    city_(std::move(city)),
    lat_(lat),
    lon_(lon)
{}

std::string Airport::toString() const
{
    std::ostringstream oss;
    oss << code_ << " – " << name_ << " (" << country_ << ')';
    return oss.str();
}
