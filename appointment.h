#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <memory>
#include <string>

class Resource;
class Stay;

class Appointment {
private:
    std::string opsCode;
    std::string date;
    int startHour;
    int duration;

    std::shared_ptr<Stay> stay;
    std::shared_ptr<Resource> resource;

public:
    Appointment(const std::string& opsCode, const std::string& date, int startHour, int duration, std::shared_ptr<Stay> stay, std::shared_ptr<Resource> resource);

    std::string getOPSCode() const;
    std::string getDate() const;
    int getStartHour() const;
    int getDuration() const;
    int getEndHour() const;

    std::shared_ptr<Stay> getStay() const;
    std::shared_ptr<Resource> getResource() const;
};

#endif // APPOINTMENT_H
