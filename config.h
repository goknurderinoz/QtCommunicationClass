#ifndef CONFIG_H
#define CONFIG_H
#include <QString>

class config
{
public:
    config();
    static QString IP_ADRESS;
    static int TCP_PORT_NUMBER;
    static int WS_PORT_NUMBER;
};

#endif // CONFIG_H
