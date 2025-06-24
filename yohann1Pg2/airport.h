#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>

/**
 * @brief Représente un aéroport identifié par un code IATA (3 lettres).
 *
 * La classe est volontairement “légère” : seulement des getters read-only.
 * Les instances seront partagées via std::shared_ptr pour qu’un même
 * objet Airport soit utilisé par plusieurs vols.
 */
class Airport
{
public:
    /* --- constructeur ---------------------------------------------------- */
    Airport(std::string code,
            std::string name,
            std::string country,
            std::string city,
            double      lat      = 0.0,
            double      lon      = 0.0);

    /* --- getters  (read-only) ------------------------------------------- */
    const std::string& code()    const { return code_;    }
    const std::string& name()    const { return name_;    }
    const std::string& country() const { return country_; }
    const std::string& city()    const { return city_;    }
    double             lat()     const { return lat_;     }
    double             lon()     const { return lon_;     }

    /* --- aide : affichage court ----------------------------------------- */
    std::string toString() const;   // retourne "FRA – Frankfurt Airport (DE)"

private:
    std::string code_;     // "FRA"
    std::string name_;     // "Frankfurt Airport"
    std::string country_;  // "DE"
    std::string city_;     // "Frankfurt"
    double      lat_;      // latitude
    double      lon_;      // longitude
};

#endif // AIRPORT_H
