#include "stayoncology.h"
#include <sstream>

StayOncology::StayOncology(long patientID, long stayID, const std::string& admissionDate, const std::string& dischargeDate, const std::string& mainDiagnosisCode, const std::string& mainDiagnosisText, std::shared_ptr<Department> department)
    : Stay(patientID, stayID, admissionDate, dischargeDate, mainDiagnosisCode, mainDiagnosisText, department) {}

std::string StayOncology::showDetails() const {
    std::stringstream ss;

    ss << Stay::showDetails();
    ss << "\n=== Onkologie ===\n";
    ss << "Chemotherapie geplant\n";

    return ss.str();
}
