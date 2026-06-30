#include "stay.h"

#include <sstream>

Stay::Stay(long patientID,
           long stayID,
           const std::string& admissionDate,
           const std::string& dischargeDate,
           const std::string& mainDiagnosisCode,
           const std::string& mainDiagnosisText,
           std::shared_ptr<Department> department)
    : patientID(patientID),
      stayID(stayID),
      admissionDate(admissionDate),
      dischargeDate(dischargeDate),
      mainDiagnosisCode(mainDiagnosisCode),
      mainDiagnosisText(mainDiagnosisText),
      department(department) {}

std::string Stay::showDetails() const {
    std::stringstream text;

    text << stayID << ": von " << admissionDate << " bis ";

    if (dischargeDate.empty()) {
        text << "laufend";
    } else {
        text << dischargeDate;
    }

    text << "\n";
    text << "Abteilung: ";

    if (department != nullptr) {
        text << department->getName();
    } else {
        text << "unbekannt";
    }

    text << "\n";
    text << "Aufnahmediagnose: " << mainDiagnosisCode << " " << mainDiagnosisText << "\n";

    return text.str();
}

long Stay::getPatientID() const { return patientID; }
long Stay::getStayID() const { return stayID; }
std::string Stay::getAdmissionDate() const { return admissionDate; }
std::string Stay::getDischargeDate() const { return dischargeDate; }
std::string Stay::getMainDiagnosisCode() const { return mainDiagnosisCode; }
std::string Stay::getMainDiagnosisText() const { return mainDiagnosisText; }
std::shared_ptr<Department> Stay::getDepartment() const { return department; }
bool Stay::isRunning() const { return dischargeDate.empty(); }

void Stay::setPreviousStay(std::shared_ptr<Stay> stay) { previousStay = stay; }
void Stay::setNextStay(std::shared_ptr<Stay> stay) { nextStay = stay; }
std::shared_ptr<Stay> Stay::getPreviousStay() const { return previousStay.lock(); }
std::shared_ptr<Stay> Stay::getNextStay() const { return nextStay.lock(); }

void Stay::setCurrentDiagnosisCode(const std::string& code) { currentDiagnosisCode = code; }
std::string Stay::getCurrentDiagnosisCode() const { return currentDiagnosisCode; }

void Stay::setAnamnesisCode(int row, const std::string& code) {
    if (row < 0) {
        return;
    }

    if (row >= static_cast<int>(anamnesisCodes.size())) {
        anamnesisCodes.resize(row + 1);
    }

    anamnesisCodes[row] = code;

    // Leere Zeilen am Ende wieder entfernen, damit die Daten sauber bleiben.
    while (!anamnesisCodes.empty() && anamnesisCodes.back().empty()) {
        anamnesisCodes.pop_back();
    }
}

const std::vector<std::string>& Stay::getAnamnesisCodes() const { return anamnesisCodes; }

void Stay::setRemarks(const std::string& text) { remarks = text; }
std::string Stay::getRemarks() const { return remarks; }

void Stay::addBloodPressureValue(const std::string& date, int systolic, int diastolic) {
    BloodPressureValue value;
    value.date = date;
    value.systolic = systolic;
    value.diastolic = diastolic;
    bloodPressureValues.push_back(value);
}

const std::vector<BloodPressureValue>& Stay::getBloodPressureValues() const {
    return bloodPressureValues;
}

void Stay::setAdmissionDate(const std::string& admissionDate) { this->admissionDate = admissionDate; }
void Stay::setDischargeDate(const std::string& dischargeDate) { this->dischargeDate = dischargeDate; }

void Stay::addAppointment(const Appointment& appointment) {
    appointments.push_back(appointment);
}

const std::vector<Appointment>& Stay::getAppointments() const { return appointments; }
