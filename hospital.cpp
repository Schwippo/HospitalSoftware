#include "hospital.h"
#include "gender.h"
#include "json.hpp"
#include "hospitalstay.h"

#include "staycardiology.h"
#include "staygeneralsurgery.h"
#include "stayinternalmedicine.h"
#include "stayorthopedics.h"
#include "stayoncology.h"
#include "staygastroenterology.h"

#include "operationroom.h"
#include "mrt.h"
#include "ct.h"
#include "resource.h"

#include <QFile>
#include <QIODevice>
#include <QString>
#include <QDate>

#include <sstream>
#include <stdexcept>

using json = nlohmann::json;

Hospital::Hospital(const std::string& name)
    : name(name), icdDataWasRead(false), nextPatientID(1), nextStayID(1) {
    createDepartments();

    // Operationsräume direkt im Konstruktor erstellen
    resources.push_back(std::make_shared<Operationroom>("OP 1"));
    resources.push_back(std::make_shared<Operationroom>("OP 2"));

    resources.push_back(std::make_shared<MRT>("MRT 1"));
    resources.push_back(std::make_shared<MRT>("MRT 2"));

    resources.push_back(std::make_shared<CT>("CT 1"));
    resources.push_back(std::make_shared<CT>("CT 2"));
}

void Hospital::createDepartments() {
    departments[DepartmentType::GeneralSurgery] = std::make_shared<GeneralSurgery>();
    departments[DepartmentType::Cardiology] = std::make_shared<Cardiology>();
    departments[DepartmentType::InternalMedicine] = std::make_shared<InternalMedicine>();
    departments[DepartmentType::Orthopedics] = std::make_shared<Orthopedics>();
    departments[DepartmentType::Oncology] = std::make_shared<Oncology>();
    departments[DepartmentType::Gastroenterology] = std::make_shared<Gastroenterology>();
}

std::string Hospital::readCompleteFile(const std::string& fileName) const {
    QFile file(QString::fromStdString(fileName));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Die Datei konnte nicht geöffnet werden: " + fileName);
    }

    QByteArray content = file.readAll();
    return content.toStdString();
}

std::vector<std::string> Hospital::splitLine(const std::string& line, char separator) const {
    std::vector<std::string> parts;
    std::string currentPart;

    for (char currentChar : line) {
        if (currentChar == separator) {
            parts.push_back(currentPart);
            currentPart = "";
        } else {
            currentPart += currentChar;
        }
    }

    parts.push_back(currentPart);
    return parts;
}

std::shared_ptr<Patient> Hospital::findPatient(const std::string& insuranceNumber) const {
    for (const std::shared_ptr<Patient>& patient : patients) {
        if (patient->getInsuranceNumber() == insuranceNumber) {
            return patient;
        }
    }

    return nullptr;
}

std::shared_ptr<Department> Hospital::getDepartment(DepartmentType departmentType) const {
    auto iterator = departments.find(departmentType);

    if (iterator == departments.end()) {
        throw std::runtime_error("Für diesen departmentType wurde keine Abteilung angelegt");
    }

    return iterator->second;
}

// p4 createStay
std::shared_ptr<Stay> Hospital::createStay(long patientID, long stayID, const std::string& admissionDate, const std::string& dischargeDate, const std::string& mainDiagnosisCode, const std::string& mainDiagnosisText, std::shared_ptr<Department> department) {
    switch(department->getDepartmentType()) {
    case DepartmentType::Cardiology:
        return std::make_shared<StayCardiology>(patientID, stayID, admissionDate, dischargeDate, mainDiagnosisCode, mainDiagnosisText, department);

    case DepartmentType::GeneralSurgery:
        return std::make_shared<StayGeneralSurgery>(patientID, stayID, admissionDate, dischargeDate, mainDiagnosisCode, mainDiagnosisText, department);

    case DepartmentType::InternalMedicine:
        return std::make_shared<StayInternalMedicine>(patientID, stayID, admissionDate, dischargeDate, mainDiagnosisCode, mainDiagnosisText, department);

    case DepartmentType::Orthopedics:
        return std::make_shared<StayOrthopedics>(patientID, stayID, admissionDate, dischargeDate, mainDiagnosisCode, mainDiagnosisText, department);

    case DepartmentType::Oncology:
        return std::make_shared<StayOncology>(patientID, stayID, admissionDate, dischargeDate, mainDiagnosisCode, mainDiagnosisText, department);

    case DepartmentType::Gastroenterology:
        return std::make_shared<StayGastroenterology>(patientID, stayID, admissionDate, dischargeDate, mainDiagnosisCode, mainDiagnosisText, department);
    }

    throw std::runtime_error("Unbekannter DepartmentType");
}

// P4 Änderung
std::string Hospital::getICDText(const std::string& icdCode) const {
    auto it = icdCodeMap.find(icdCode);

    if(it != icdCodeMap.end()) {
        return it->second;
    }

    return "";
}

bool Hospital::isICDCodeValid(const std::string& icdCode) const {
    return !getICDText(icdCode).empty();
}

void Hospital::readCaseFile(const std::string& fileName) {
    if (!icdDataWasRead) {
        throw std::runtime_error("Die ICD-Daten müssen zuerst eingelesen werden");
    }

    patients.clear();
    nextPatientID = 1;
    nextStayID = 1;

    std::string fileContent = readCompleteFile(fileName);
    json staysFromFile = json::parse(fileContent);

    for (const auto& stayFromFile : staysFromFile) {
        std::string admissionDate = stayFromFile["admissionDate"];
        std::string dischargeDate = stayFromFile["dischargeDate"];
        std::string mainDiagnosisCode = stayFromFile["mainDiagnosis"];
        std::string firstName = stayFromFile["firstName"];
        std::string lastName = stayFromFile["lastName"];
        std::string birthDate = stayFromFile["birthDate"];
        std::string insuranceNumber = stayFromFile["insuranceNumber"];
        std::string genderAsText = stayFromFile["gender"];
        int departmentTypeNumber = stayFromFile["departmentType"];

        std::string mainDiagnosisText = getICDText(mainDiagnosisCode);
        if (mainDiagnosisText.empty()) {
            mainDiagnosisText = "Diagnosetext nicht gefunden";
        }

        std::shared_ptr<Patient> patient = findPatient(insuranceNumber);

        if (patient == nullptr) {
            patient = std::make_shared<Patient>(nextPatientID,
                                                birthDate,
                                                firstName,
                                                lastName,
                                                insuranceNumber,
                                                stringToGender(genderAsText));
            patients.push_back(patient);
            nextPatientID++;
        }

        DepartmentType departmentType = departmentTypeFromInt(departmentTypeNumber);
        std::shared_ptr<Department> department = getDepartment(departmentType);

        // P4 NEU
        // neuen krankenhausaufenthalt anlegen
        std::shared_ptr<HospitalStay> hospitalStay = std::make_shared<HospitalStay>();

        // klinikaufenthalt anlegen
        std::shared_ptr<Stay> stay = createStay(patient->getPatientID(), nextStayID, admissionDate, dischargeDate, mainDiagnosisCode, mainDiagnosisText, department);

        // aufenthalt dem krankenhausaufenthalt hinzufügen
        hospitalStay->addStay(stay);

        // krankenhausaufenthalt dem patienten hinzufügen
        patient->addHospitalStay(hospitalStay);
        nextStayID++;
    }
}

void Hospital::readICDData(const std::string& fileName) {
    icdCodeMap.clear();

    std::string fileContent = readCompleteFile(fileName);
    std::stringstream stream(fileContent);
    std::string line;

    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        std::vector<std::string> columns = splitLine(line, ';');

        if (columns.size() >= 9) {
            std::string code = columns[5];
            std::string text = columns[8];

            icdCodeMap[code] = text;
        }
    }

    icdDataWasRead = true;
}

// P4 NEU
void Hospital::readOPSData(const std::string& fileName) {
    opsCodeMap.clear();

    std::string fileContent = readCompleteFile(fileName);
    std::stringstream stream(fileContent);
    std::string line;

    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        std::vector<std::string> columns = splitLine(line, ';');

        if (columns.size() >= 9) {
            std::string code = columns[6]; // OPS-Code
            std::string text = columns[8]; // Beschreibung

            opsCodeMap[code] = text;
        }
    }

    icdDataWasRead = true;
}

const std::vector<std::shared_ptr<Patient>>& Hospital::getPatients() const {
    return patients;
}

std::shared_ptr<Patient> Hospital::findPatientByID(long patientID) const {
    for (const std::shared_ptr<Patient>& patient : patients) {
        if (patient->getPatientID() == patientID) {
            return patient;
        }
    }

    return nullptr;
}

// P4 veränderung
std::shared_ptr<Stay> Hospital::findStayByID(long stayID) const {
    for(const std::shared_ptr<Patient>& patient : patients) {
        for(const std::shared_ptr<HospitalStay>& hospitalStay : patient->getHospitalStays()) {
            for(const std::shared_ptr<Stay>& stay : hospitalStay->getStays()) {
                if(stay->getStayID() == stayID) {
                    return stay;
                }
            }
        }
    }

    return nullptr;
}

std::string Hospital::printPatientList() const {
    std::stringstream text;

    if (patients.empty()) {
        text << "Es wurden keine Daten gelesen\n";
        return text.str();
    }

    text << "Krankenhaus: " << name << "\n";

    for (const std::shared_ptr<Patient>& patient : patients) {
        text << patient->showOverview();
    }

    text << "-----------------------------------------------\n";
    return text.str();
}

std::shared_ptr<HospitalStay> Hospital::findHospitalStay(std::shared_ptr<Stay> stay) const {
    for(const std::shared_ptr<Patient>& patient : patients) {
        for(const std::shared_ptr<HospitalStay>& hospitalStay : patient->getHospitalStays()) {
            for(const std::shared_ptr<Stay>& currentStay : hospitalStay->getStays()) {
                if(currentStay == stay) {
                    return hospitalStay;
                }
            }
        }
    }

    return nullptr;
}

void Hospital::transferStay(std::shared_ptr<Stay> currentStay, DepartmentType newDepartmentType) {
    if(currentStay == nullptr) {
        throw std::runtime_error("Kein Aufenthalt ausgewählt.");
    }

    std::shared_ptr<HospitalStay> hospitalStay = findHospitalStay(currentStay);

    if(hospitalStay == nullptr) {
        throw std::runtime_error("HospitalStay wurde nicht gefunden.");
    }

    // neue Abteilung holen
    std::shared_ptr<Department> newDepartment = getDepartment(newDepartmentType);

    // heutiges Datum bestimmen
    std::string today = QDate::currentDate().toString("dd.MM.yyyy").toStdString();

    // alten aufenthalt beenden
    currentStay->setDischargeDate(today);

    // neuen stay erzeugen
    std::shared_ptr<Stay> newStay = createStay(currentStay->getPatientID(), nextStayID, today, "", currentStay->getMainDiagnosisCode(), currentStay->getMainDiagnosisText(), newDepartment);

    // beziehung setzen
    currentStay->setNextStay(newStay);
    newStay->setPreviousStay(currentStay); // Stay 1 <-> Stay 2

    hospitalStay->addStay(newStay);
    nextStayID++;
}

// aufenthalt durch setzen des entlassungsdatums beenden
void Hospital::dischargeStay(std::shared_ptr<Stay> stay) {
    if(stay == nullptr) {
        throw std::runtime_error("Kein Aufenthalt ausgewählt.");
    }

    std::string today = QDate::currentDate().toString("dd.MM.yyyy").toStdString();
    stay->setDischargeDate(today);
}

const std::vector<std::shared_ptr<Resource>>& Hospital::getResources() const {
    return resources;
}

std::shared_ptr<Resource> Hospital::findFreeResource(ResourceType resourceType, const std::string& date, int startHour, int duration) {
    for(const auto& resource : resources) {
        if(resource->getType() != resourceType) {
            continue;
        }

        if(resource->isAvailable(date, startHour, duration)) {
            return resource;
        }
    }
    return nullptr;
}

bool Hospital::scheduleProcedure(std::shared_ptr<Stay> stay, const std::string& opsCode, const std::string& date, int startHour, int duration) {
    if(stay == nullptr) {
        return false;
    }

    // Ressourcentyp mit ops-codes bestimmmen -> Aktuell: alle Eingriffe in einem Operationssaal geplant -> erweitern (MRT, CT)
    ResourceType resourceType = ResourceType::Operationroom;

    // erste freie Ressource suchen
    std::shared_ptr<Resource> resource = findFreeResource(resourceType, date, startHour, duration);

    if(resource == nullptr) {
        return false;
    }

    // termin erstellen
    Appointment appointment(opsCode, date, startHour, duration, stay, resource);

    resource->addAppointment(appointment);
    stay->addAppointment(appointment);
    return true;
}
