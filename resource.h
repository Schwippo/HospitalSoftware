#ifndef RESOURCE_H
#define RESOURCE_H

#include "appointment.h"
#include "ResourceType.h"

#include <memory>
#include <string>
#include <vector>

class Resource {
protected:
    std::string name;
    std::vector<Appointment> appointments;
    ResourceType type;

public:
    Resource(const std::string& name, ResourceType type);
    virtual ~Resource() = default;

    std::string getName() const;

    bool isAvailable(const std::string& date, int startHour, int duration) const;
    void addAppointment(const Appointment& appointment);
    const std::vector<Appointment>& getAppointments() const;

    ResourceType getType() const;
};

#endif // RESOURCE_H
