#ifndef MOTEUR_HPP
#define MOTEUR_HPP

#define DIRECTION_FOWARD 0
#define DIRECTION_BACK 1

class Moteur{
public:
    /*!
     * \brief Moteur constructor
     * \param pinHacheur adresse du pin auquelle sera brancher le hacheur
     */
    Moteur(Uint32 pinHacheur);
    /*!
     * \brief setSpeed
     * \param speed : commande de vitesse 0 imobile, 255 vitesse maximale
     * \return true OK, false il y à eu une érreur
     */
    bool setSpeed(Uint8 speed);
    /*!
     * \brief setDirection
     * \param direction : DIRECTION_FOWARD ou DIRECTION_BACK (relatif à la position du moteur)
     * \return true OK, false il y à eu une érreur
     */
    bool setDirection(bool direction);
    /*!
     * \brief getSpeed
     * \return valeur de la commande de vitesse (pas la vitesse réelle, voir Encodeur)
     */
    Uint8 getSpeed();
    /*!
     * \brief getDirection
     * \return valeur de la commande de direction
     */
    bool getDirection();
    ~Moteur();
};

#endif // MOTEUR_HPP
