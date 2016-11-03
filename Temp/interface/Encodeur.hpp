#ifndef ENCODEUR_HPP
#define ENCODEUR_HPP

class Encodeur{
public:
    Encodeur(Uint32 pin);
    /*!
     * \brief getSpeed
     * \return vitesse : 0 = 0 et 255 vitesse max théorique du moteur (/!\ c'est bancal)
     */
    Uint8 getSpeed();
    /*!
     * \brief getDirection
     * \return sens dans le quel avence le robot
     */
    bool getDirection();
    ~Encodeur();
};

#endif // ENCODEUR_HPP
