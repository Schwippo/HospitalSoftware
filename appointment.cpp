#include "appointment.h"

Appointment::Appointment(const std::string& opsCode, const std::string& date, int startHour, int duration, std::shared_ptr<Stay> stay, std::shared_ptr<Resource> resource)
    : opsCode(opsCode), date(date), startHour(startHour), duration(duration), stay(stay), resource(resource) {}

std::string Appointment::getOPSCode() const { return opsCode; }
std::string Appointment::getDate() const { return date; }
int Appointment::getStartHour() const { return startHour; }
int Appointment::getDuration() const { return duration; }
int Appointment::getEndHour() const { return startHour + duration; }
std::shared_ptr<Stay> Appointment::getStay() const { return stay; }
std::shared_ptr<Resource> Appointment::getResource() const { return resource; }
