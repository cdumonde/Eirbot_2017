#ifndef SYSTEM_HPP
#define SYSTEM_HPP

class System{
public:
    System(/*de nombreux paramétre*/);
    /*!
     * \brief setFrequency
     * \param frequency : fréquence de fonctionement de la carte
     * \return true OK, false error
     */
    bool setFrequency(Uint32 frequency);
    /*!
     * \brief clock
     * \return le temp écoulé depuis le début du programe, Time à une méthode pour accédé au temps
     */
    static Time clock();
    ~System();
};

#endif // SYSTEM_HPP
