#include "travelagency.h"

/* dépendances -----------------------------------------------------*/
#include "booking.h"
#include "flightbooking.h"
#include "hotelbooking.h"
#include "rentalcarreservation.h"
#include "trainticket.h"
#include "travel.h"
#include "customer.h"
#include "airport.h"

#include "json.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

using json = nlohmann::json;

/*────────────────── recherche simples ───────────────────────────*/

BookingPtr TravelAgency::findBooking(const std::string& id) const
{
    for (const auto& b : allBookings)
        if (b->getId() == id) return b;
    return nullptr;
}

TravelPtr TravelAgency::findTravel(long id) const
{
    for (const auto& t : allTravels)
        if (t->getId() == id) return t;
    return nullptr;
}

CustomerPtr TravelAgency::findCustomer(long id) const
{
    for (const auto& c : allCustomers)
        if (c->getId() == id) return c;
    return nullptr;
}

/*────────────────── lecture bookings JSON ───────────────────────*/

std::string TravelAgency::readFile(const std::string& filePath)
{
    std::ifstream in(filePath);
    if (!in) throw std::runtime_error("Erreur ouverture : " + filePath);

    json root;  in >> root;
    if (!root.is_array())
        throw std::runtime_error("Attendu : tableau JSON racine");

    /* statistiques pour message de fin ----------------------------*/
    int cntFl=0,cntHo=0,cntRe=0,cntTr=0;
    double sumFl=0,sumHo=0,sumRe=0,sumTr=0;

    int idx = 0;
    for (const auto& item : root) {
        ++idx;
        try {
            /* champs communs ------------------------------------*/
            const std::string type     = item.at("type");
            const std::string bId      = item.at("id");
            const double      price    = item.at("price");
            const std::string fromDate = item.at("fromDate");
            const std::string toDate   = item.at("toDate");

            long  cId   = item.at("customerId");
            std::string fname = item.at("customerFirstName");
            std::string lname = item.at("customerLastName");
            long  tId   = item.at("travelId");

            /* Customer -----------------------------------------*/
            CustomerPtr cust = findCustomer(cId);
            if (!cust) {
                cust = std::make_shared<Customer>(cId, lname, fname);
                allCustomers.push_back(cust);
            }

            /* Travel ------------------------------------------*/
            TravelPtr trav = findTravel(tId);
            if (!trav) {
                trav = std::make_shared<Travel>(tId, cId);
                allTravels.push_back(trav);
                cust->addTravel(trav);
            }

            /* Booking concret ---------------------------------*/
            BookingPtr bk;

            if (type == "Flight") {
                bk = std::make_shared<FlightBooking>(
                        bId, price, fromDate, toDate, tId,
                        item.at("fromDest"), item.at("toDest"),
                        item.at("airline"),  item.at("bookingClass"),
                        item.at("fromDestLatitude"), item.at("fromDestLongitude"),
                        item.at("toDestLatitude"),   item.at("toDestLongitude"));
                ++cntFl;  sumFl += price;
            }
            else if (type == "Hotel") {
                bk = std::make_shared<HotelBooking>(
                        bId, price, fromDate, toDate, tId,
                        item.at("hotel"), item.at("town"), item.at("roomType"),
                        item.at("hotelLatitude"), item.at("hotelLongitude"));
                ++cntHo;  sumHo += price;
            }
            else if (type == "RentalCar") {
                bk = std::make_shared<RentalCarReservation>(
                        bId, price, fromDate, toDate, tId,
                        item.at("pickupLocation"), item.at("returnLocation"),
                        item.at("company"), item.at("vehicleClass"),
                        item.at("pickupLatitude"), item.at("pickupLongitude"),
                        item.at("returnLatitude"), item.at("returnLongitude"));
                ++cntRe;  sumRe += price;
            }
            else if (type == "Train") {
                /* ---- correspondances (3 formats) ---- */
                std::vector<std::string> conns;
                if (item.contains("connectingStations")) {
                    const auto& cs = item["connectingStations"];
                    if (cs.is_array() && !cs.empty() && cs[0].is_object()) {
                        for (const auto& st : cs)
                            conns.push_back(st.value("stationName",""));
                    } else if (cs.is_array()) {
                        conns = cs.get<std::vector<std::string>>();
                    } else if (cs.is_string()) {
                        std::string raw = cs.get<std::string>();
                        std::stringstream ss(raw);
                        std::string tok;
                        auto trimPush=[&](const std::string& s){
                            size_t b=s.find_first_not_of(" \t");
                            size_t e=s.find_last_not_of(" \t");
                            if (b!=std::string::npos)
                                conns.push_back(s.substr(b,e-b+1));
                        };
                        while(std::getline(ss,tok,',')) trimPush(tok);
                        if(conns.size()<=1){
                            conns.clear(); ss.clear(); ss.str(raw);
                            while(std::getline(ss,tok,';')) trimPush(tok);
                        }
                    }
                }

                bk = std::make_shared<TrainTicket>(
                        bId, price, fromDate, toDate, tId,
                        item.at("fromStation"), item.at("toStation"), conns,
                        item.value("departureTime",""),
                        item.value("arrivalTime"  ,""),
                        item.at("ticketType"),
                        item.at("fromStationLatitude"), item.at("fromStationLongitude"),
                        item.at("toStationLatitude"),   item.at("toStationLongitude"));
                ++cntTr;  sumTr += price;
            }
            else
                throw std::runtime_error("Type inconnu : " + type);

            /* insertion globale --------------------------------*/
            allBookings.push_back(bk);
            trav->addBooking(bk);
        }
        catch (std::exception& e) {
            throw std::runtime_error("Erreur à l’objet " + std::to_string(idx) +
                                     " : " + e.what());
        }
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2)
        << cntFl << " vols ("      << sumFl << " €), "
        << cntHo << " hôtels ("    << sumHo << " €), "
        << cntRe << " voitures ("  << sumRe << " €), "
        << cntTr << " trains ("    << sumTr << " €) importés.\n"
        << allCustomers.size() << " clients, "
        << allTravels.size()   << " voyages.";
    return oss.str();
}

/*────────────────── sauvegarde bookings JSON ───────────────────*/

void TravelAgency::writeFile(const std::string& filePath) const
{
    json root = json::array();

    for (const auto& b : allBookings) {
        json j;
        j["id"]       = b->getId();
        j["price"]    = b->getPrice();
        j["fromDate"] = b->getFromDate();
        j["toDate"]   = b->getToDate();
        j["travelId"] = b->getTravelId();

        const auto t = findTravel(b->getTravelId());
        const auto c = t ? findCustomer(t->getCustomerId()) : nullptr;
        j["customerId"]        = c ? c->getId()        : 0;
        j["customerFirstName"] = c ? c->getFirstName() : "";
        j["customerLastName"]  = c ? c->getLastName()  : "";

        if (auto f = std::dynamic_pointer_cast<FlightBooking>(b)) {
            j["type"]         = "Flight";
            j["fromDest"]     = f->getFrom();
            j["toDest"]       = f->getTo();
            j["airline"]      = f->getAirline();
            j["bookingClass"] = f->getBookingClass();
            j["fromDestLatitude"]  = f->getFromDestLatitude();
            j["fromDestLongitude"] = f->getFromDestLongitude();
            j["toDestLatitude"]    = f->getToDestLatitude();
            j["toDestLongitude"]   = f->getToDestLongitude();
        }
        else if (auto h = std::dynamic_pointer_cast<HotelBooking>(b)) {
            j["type"]        = "Hotel";
            j["hotel"]       = h->getHotelName();
            j["town"]        = h->getCity();
            j["roomType"]    = h->getRoomType();
            j["hotelLatitude"]  = h->getHotelLatitude();
            j["hotelLongitude"] = h->getHotelLongitude();
        }
        else if (auto r = std::dynamic_pointer_cast<RentalCarReservation>(b)) {
            j["type"]           = "RentalCar";
            j["pickupLocation"] = r->getPickupLocation();
            j["returnLocation"] = r->getReturnLocation();
            j["company"]        = r->getCompany();
            j["vehicleClass"]   = r->getVehicleClass();
            j["pickupLatitude"]  = r->getPickupLatitude();
            j["pickupLongitude"] = r->getPickupLongitude();
            j["returnLatitude"]  = r->getReturnLatitude();
            j["returnLongitude"] = r->getReturnLongitude();
        }
        else if (auto tkt = std::dynamic_pointer_cast<TrainTicket>(b)) {
            j["type"]              = "Train";
            j["fromStation"]       = tkt->getFrom();
            j["toStation"]         = tkt->getTo();
            j["ticketType"]        = tkt->getBookingClass();
            j["departureTime"]     = tkt->getDepartureTime();
            j["arrivalTime"]       = tkt->getArrivalTime();
            j["fromStationLatitude"]  = tkt->getFromStationLatitude();
            j["fromStationLongitude"] = tkt->getFromStationLongitude();
            j["toStationLatitude"]    = tkt->getToStationLatitude();
            j["toStationLongitude"]   = tkt->getToStationLongitude();
            j["connectingStations"]   = tkt->getStopovers();
        }
        root.push_back(j);
    }

    std::ofstream out(filePath);
    if (!out) throw std::runtime_error("Impossible d’écrire " + filePath);
    out << root.dump(4);
}

/*────────────────── IATA ───────────────────────────────────────*/

void TravelAgency::loadAirports(const std::string& path)
{
    std::ifstream in(path);
    if (!in) throw std::runtime_error("IATA introuvable : " + path);

    json j;  in >> j;
    for (const auto& rec : j) {
        std::string code = rec.value("iata_code", "");
        if (code.empty()) continue;

        airports_[code] = std::make_shared<Airport>(
            code,
            rec.value("name",         ""),
            rec.value("iso_country",  ""),
            rec.value("municipality", ""),
            rec.value("latitude_deg",  0.0),
            rec.value("longitude_deg", 0.0));
    }
}

std::shared_ptr<Airport>
TravelAgency::getAirport(const std::string& code) const
{
    auto it = airports_.find(code);
    return it == airports_.end() ? nullptr : it->second;
}
