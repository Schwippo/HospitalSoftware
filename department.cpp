#include "department.h"

#include <stdexcept>

Department::Department(DepartmentType type, const std::string& name)
    : type(type), name(name) {}

DepartmentType Department::getDepartmentType() const { return type; }
std::string Department::getName() const { return name; }

DepartmentType departmentTypeFromInt(int value) {
    switch (value) {
    case 0: return DepartmentType::GeneralSurgery;
    case 1: return DepartmentType::Cardiology;
    case 2: return DepartmentType::InternalMedicine;
    case 3: return DepartmentType::Orthopedics;
    case 4: return DepartmentType::Oncology;
    case 5: return DepartmentType::Gastroenterology;
    default:
        throw std::runtime_error("Unbekannter departmentType in stays.json");
    }
}

GeneralSurgery::GeneralSurgery()
    : Department(DepartmentType::GeneralSurgery, "Allgemeine Chirurgie") {}

std::string GeneralSurgery::getClassName() const { return "GeneralSurgery"; }

Cardiology::Cardiology()
    : Department(DepartmentType::Cardiology, "Kardiologie") {}

std::string Cardiology::getClassName() const { return "Cardiology"; }

InternalMedicine::InternalMedicine()
    : Department(DepartmentType::InternalMedicine, "Innere Medizin") {}

std::string InternalMedicine::getClassName() const { return "InternalMedicine"; }

Orthopedics::Orthopedics()
    : Department(DepartmentType::Orthopedics, "Orthopädie") {}

std::string Orthopedics::getClassName() const { return "Orthopedics"; }

Oncology::Oncology()
    : Department(DepartmentType::Oncology, "Onkologie") {}

std::string Oncology::getClassName() const { return "Oncology"; }

Gastroenterology::Gastroenterology()
    : Department(DepartmentType::Gastroenterology, "Gastroenterologie") {}

std::string Gastroenterology::getClassName() const { return "Gastroenterology"; }
