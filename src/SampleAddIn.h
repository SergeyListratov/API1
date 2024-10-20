#ifndef SAMPLEADDIN_H
#define SAMPLEADDIN_H

#include "Component.h"

class RabbitAddIn final : public Component {
public:
    const char *Version = u8"1.0.0";

    RabbitAddIn();

private:
    std::string extensionName() override;


    variant_t send(const variant_t& q, const variant_t& d);

    variant_t receive(const variant_t& q);


    variant_t add(const variant_t &a, const variant_t &b);

    void message(const variant_t &msg);

    void sleep(const variant_t &delay);

    void assign(variant_t &out);

    variant_t samplePropertyValue();

    variant_t currentDate();

    std::shared_ptr<variant_t> sample_property;
};

#endif //SAMPLEADDIN_H
