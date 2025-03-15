//
// Created by Kirill Smychok on 12.03.25.
//

#ifndef TICKET_H
#define TICKET_H


#include <string>


class Ticket {

public:
    enum Status: uint8_t {
        DEFAULT,
        YELLOW,
        GREEN
    };

    Ticket() {}
    Ticket(std::string name): number(-1), name(name), status(DEFAULT) {}
    Ticket(int number, std::string name): number(number), name(name), status(DEFAULT) {}
    Ticket(int number, std::string name, std::string status): number(number), name(name), status(DEFAULT) {}
    ~Ticket() {}

    int getNumber() const { return number; }
    std::string getName() { return name; }
    Status getStatus() { return status; }

    void setNumber(int number) { this->number = number; }
    void setName(std::string name) { this->name = name; }
    void setStatus(Status status) { this->status = status; }

    bool operator==(const Ticket& other) const {
        return number == other.number;
    }
    // bool operator!=(const Ticket& other) const {
    //     return number != other.number;
    // }
    bool operator<(const Ticket& other) const {
        return number < other.getNumber();
    }

private:
    int number;
    std::string name;
    Status status;
};
const std::pair<std::string, QColor> status_data[] = {
    {"Не выучен", QColor(128, 128, 128)},
    {"Повторить",QColor(224, 206, 4)},
    {"Выучен", QColor(39, 168, 23)}
};
namespace std {
    template <>
    struct hash<Ticket> {
        std::size_t operator()(const Ticket& ticket) const {
            return hash<int>()(ticket.getNumber());
        }
    };
}
#endif //TICKET_H
