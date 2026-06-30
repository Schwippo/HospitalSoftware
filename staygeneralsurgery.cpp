#include "staygeneralsurgery.h"
#include <sstream>

StayGeneralSurgery::StayGeneralSurgery(long patientID, long stayID, const std::string& admissionDate, const std::string& dischargeDate, const std::string& mainDiagnosisCode, const std::string& mainDiagnosisText, std::shared_ptr<Department> department)
    : Stay(patientID, stayID, admissionDate, dischargeDate, mainDiagnosisCode, mainDiagnosisText, department) {}

std::string StayGeneralSurgery::showDetails() const {
    std::stringstream ss;

    ss << Stay::showDetails();
    ss << "\n=== Allgemeinchirurgie ===\n";
    ss << "Operationsvorbereitung abgeschlossen\n";

    return ss.str();
}
