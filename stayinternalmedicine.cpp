#include "stayinternalmedicine.h"
#include <sstream>

StayInternalMedicine::StayInternalMedicine(long patientID, long stayID, const std::string& admissionDate, const std::string& dischargeDate, const std::string& mainDiagnosisCode, const std::string& mainDiagnosisText, std::shared_ptr<Department> department)
    : Stay(patientID, stayID, admissionDate, dischargeDate, mainDiagnosisCode, mainDiagnosisText, department) {}

std::string StayInternalMedicine::showDetails() const {
    std::stringstream ss;

    ss << Stay::showDetails();
    ss << "\n=== Innere Medizin ===\n";
    ss << "Internistische Verlaufskontrolle\n";

    return ss.str(); }
