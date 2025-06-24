#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include <memory>           // <-- indispensable

class Travel;

/* alias pratique (le même que dans travelagency.h) */
using TravelPtr = std::shared_ptr<Travel>;

class Customer
{
    long               id_;
    std::string        lastName_;
    std::string        firstName_;
    std::vector<TravelPtr> travels_;      // ← smart-ptr

public:
    Customer(long id, const std::string& lastName, const std::string& firstName);

    /* accesseurs */
long                getId()        const { return id_;        }
const std::string&  getFirstName() const { return firstName_; }
const std::string&  getLastName()  const { return lastName_;  }
std::string         getName()      const { return firstName_ + " " + lastName_; }

const std::vector<TravelPtr>& getTravels() const { return travels_; }

/* mutateur */
void addTravel(const TravelPtr& travel);   // <- paramètre smart-ptr
};

#endif // CUSTOMER_H
