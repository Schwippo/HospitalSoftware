#include "resource.h"

Resource::Resource(const std::string& name, ResourceType type) : name(name), type(type) {}

std::string Resource::getName() const { return name; }

bool Resource::isAvailable(const std::string& date, int startHour, int duration) const {
    int endHour = startHour + duration;

    // öffnungszeiten
    if(startHour < 8 || endHour > 16) {
        return false;
    }

    for (const Appointment& appointment : appointments) {
        if(appointment.getDate() != date) {
            continue;
        }

        if(startHour < appointment.getEndHour() && endHour > appointment.getStartHour()) {
            return false;
        }
    }

    return true;
}

void Resource::addAppointment(const Appointment& appointment) {
    appointments.push_back(appointment);
}

const std::vector<Appointment>& Resource::getAppointments() const {
    return appointments;
}

ResourceType Resource::getType() const { return type; }
