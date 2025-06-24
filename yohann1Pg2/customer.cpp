#include "customer.h"
#include "travel.h"

Customer::Customer(long id,
                   const std::string& lastName,
                   const std::string& firstName)
    : id_(id),
    lastName_(lastName),
    firstName_(firstName)
{}

void Customer::addTravel(const TravelPtr& travel)
{
    travels_.push_back(travel);      // plus de pointer brut
}
