#ifndef CAPTEURGP2_HPP
#define CAPTEURGP2_HPP

class CapteurGP2{
public:
    /*!
     * \brief CapteurGP2 constructor
     * \param pin : pin par lequelle les valeurs sont retourné
     */
    CapteurGP2(Uint32 pin);
    /*!
     * \brief getValue
     * \return tention du GP2 compri entre 0 et 225
     */
    Uint8 getValue();
    ~CapteurGP2();
};

#endif // CAPTEURGP2_HPP
