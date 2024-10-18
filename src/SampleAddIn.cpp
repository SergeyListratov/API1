#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

#include "SampleAddIn.h"

#include <iostream>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "RabbitClient.h"



std::string RabbitAddIn::extensionName() {
    return "RabbitMQ";
}

RabbitAddIn::RabbitAddIn() {
    // Universal property. Could store any supported by native api type.
    sample_property = std::make_shared<variant_t>();
    AddProperty(L"SampleProperty", L"ОбразецСвойства", sample_property);

    // Full featured property registration example
    AddProperty(L"Version", L"ВерсияКомпоненты", [&]() {
        auto s = std::string(Version);
        return std::make_shared<variant_t>(std::move(s));
    });


    AddMethod(L"send", L"Отправить", this, &RabbitAddIn::send);
    AddMethod(L"receive", L"Получить", this, &RabbitAddIn::receive);


    // Method registration.
    // Lambdas as method handlers are not supported.
    AddMethod(L"Add", L"Сложить", this, &RabbitAddIn::add);
    AddMethod(L"Message", L"Сообщить", this, &RabbitAddIn::message);
    AddMethod(L"CurrentDate", L"ТекущаяДата", this, &RabbitAddIn::currentDate);
    AddMethod(L"Assign", L"Присвоить", this, &RabbitAddIn::assign);
    AddMethod(L"SamplePropertyValue", L"ЗначениеСвойстваОбразца", this, &RabbitAddIn::samplePropertyValue);

    // Method registration with default arguments
    //
    // Notice that if variant_t would be non-copy you can't use initializer list.
    // Proper way to register def args would be then:
    //        std::map<long, variant_t> def_args;
    //        def_args.insert({0, 5});
    //        AddMethod(u"Sleep", u"Ожидать", this, &SampleAddIn::sleep, std::move(def_args));
    //
    AddMethod(L"Sleep", L"Ожидать", this, &RabbitAddIn::sleep, {{0, 5}});

}


variant_t RabbitAddIn::send(const variant_t& q, const variant_t& d) {
    //Верднуть статус записи данных d в очередь q от сервера RabbitMQ

    std::string queue_name = std::get<std::string>(q);

    std::string message = std::get<std::string>(d);

    RabbitClient rabbitclient;
    
    std::string res = rabbitclient.send(queue_name, message);

    return res;
}


variant_t RabbitAddIn::receive(const variant_t& q) {
    //Верднуть данные d из очереди q от сервера RabbitMQ либо статус???

    std::string queue_name = std::get<std::string>(q);

    RabbitClient rabbitclient;

    std::string res = rabbitclient.receive(queue_name);

    return res;
}



// Sample of addition method. Support both integer and string params.
// Every exceptions derived from std::exceptions are handled by components API
variant_t RabbitAddIn::add(const variant_t &a, const variant_t &b) {
    if (std::holds_alternative<int32_t>(a) && std::holds_alternative<int32_t>(b)) {
        return std::get<int32_t>(a) + std::get<int32_t>(b);
    } else if (std::holds_alternative<std::string>(a) && std::holds_alternative<std::string>(b)) {
        return std::string{std::get<std::string>(a) + std::get<std::string>(b)};
    } else {
        throw std::runtime_error(u8"Неподдерживаемые типы данных");
    }
}

void RabbitAddIn::message(const variant_t &msg) {
    std::visit(overloaded{
            [&](const std::string &v) { AddError(ADDIN_E_INFO, extensionName(), v, false); },
            [&](const int32_t &v) {
                AddError(ADDIN_E_INFO, extensionName(), std::to_string(static_cast<int>(v)), false);
            },
            [&](const double &v) { AddError(ADDIN_E_INFO, extensionName(), std::to_string(v), false); },
            [&](const bool &v) {
                AddError(ADDIN_E_INFO, extensionName(), std::string(v ? u8"Истина" : u8"Ложь"), false);
            },
            [&](const std::tm &v) {
                std::ostringstream oss;
                oss.imbue(std::locale("ru_RU.utf8"));
                oss << std::put_time(&v, "%c");
                AddError(ADDIN_E_INFO, extensionName(), oss.str(), false);
            },
            [&](const std::vector<char> &v) {},
            [&](const std::monostate &) {}
    }, msg);
}

void RabbitAddIn::sleep(const variant_t &delay) {
    using namespace std;
    // It safe to get any type from variant.
    // Exceptions are handled by component API.
    this_thread::sleep_for(chrono::seconds(get<int32_t>(delay)));
}

// Out params support option must be enabled for this to work
void RabbitAddIn::assign(variant_t &out) {
    out = true;
}

// Despite that you can return property value through method this is not recommended
// due to unwanted data copying
variant_t RabbitAddIn::samplePropertyValue() {
    return *sample_property;
}

variant_t RabbitAddIn::currentDate() {
    using namespace std;
    tm current{};
    time_t t = time(nullptr);
#ifdef _WINDOWS
    localtime_s(&current, &t);
#else
    localtime_r(&t, &current);
#endif
    return current;
}
