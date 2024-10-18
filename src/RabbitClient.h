#include "SimpleAmqpClient/SimpleAmqpClient.h"

class RabbitClient
{
public:
         
    std::string send(std::string q, std::string d);

    std::string receive(std::string q);

};

