#ifndef SERVO_HPP
#define SERVO_HPP

class Servo{
public:
    /*!
     * \brief Servo constructor
     * \param pinControl : pin par le quelle on envois la PWM
     */
    Servo(Uint32 pinControl);
    /*!
     * \brief setAngle
     * \param angle : commande angle du servo, 0 = 0 degree, 255 = 360 degree
     * \return true OK, false erreur
     */
    bool setAngle(Uint8 angle);
    /*!
     * \brief getAngle
     * \return la commande pour l'angle du servo
     */
    Uint8 getAngle();
    ~Servo();
};

#endif // SERVO_HPP
