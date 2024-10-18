#include <iostream>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "RabbitClient.h"


std::string RabbitClient::send(std::string q, std::string d)
{
    //¬ерднуть статус записи данных d в очередь q от сервера RabbitMQ

    std::string m1 = "[x] send ";

    std::string queue_name = q;

    std::string message = d;

    std::string uri = "amqp://rabbit:mq@173.34.4.16:5672";

   /* AmqpClient::Channel::OpenOpts opts = AmqpClient::Channel::OpenOpts::FromUri(uri);

    AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Open(opts);

    channel->DeclareQueue(queue_name, false, true, false, false);

    channel->BasicPublish("", queue_name, AmqpClient::BasicMessage::Create(message));*/


    return { m1 + "###" + message};

}



std::string RabbitClient::receive(std::string q)
{
    //¬ерднуть данные d из очереди q от сервера RabbitMQ либо статус???


    std::string m1 = "[y]  receive";

    std::string queue_name = q;

    std::string message = "{JSON1}";
    if (queue_name != "1") {
        message = queue_name;

    std::string uri = "amqp://rabbit:mq@173.34.4.16:5672";

    //AmqpClient::Channel::OpenOpts opts = AmqpClient::Channel::OpenOpts::FromUri(uri);

    //AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Open(opts);

    //channel->DeclareQueue(queue_name, false, true, false, false);

    //std::string consumer_tag = channel->BasicConsume(queue_name, "");

    //while (1) {
    //    AmqpClient::Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer_tag);
    //    std::string buffer = envelope->Message()->Body();
    //    //std::cout << "[y] recive " << buffer << std::endl;
    //    message += {"$$$" + buffer};
    //}
    //channel->BasicCancel(consumer_tag);


    return { m1 + "###" + message };
    
    }
}

