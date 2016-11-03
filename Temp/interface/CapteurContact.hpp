#ifndef CAPTEURCONTACT_HPP
#define CAPTEURCONTACT_HPP

class CapteurContact{
public:
    /*!
     * \brief CapteurContact
     * \param pin : adresse du pin qui reçois la donné
     */
    CapteurContact(Uint32 pin);
    /*!
     * \brief isPush
     * \return trus si le capteur est enfoncé, false sinon
     */
    bool isPush();
    ~CapteurContact();
};

#endif // CAPTEURCONTACT_HPP
