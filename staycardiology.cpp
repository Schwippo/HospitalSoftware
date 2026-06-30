#include "staycardiology.h"
#include <sstream>

StayCardiology::StayCardiology(long patientID, long stayID, const std::string& admissionDate, const std::string& dischargeDate, const std::string& mainDiagnosisCode, const std::string& mainDiagnosisText, std::shared_ptr<Department> department)
    : Stay(patientID, stayID, admissionDate, dischargeDate, mainDiagnosisCode, mainDiagnosisText, department) {}

std::string StayCardiology::showDetails() const {
    std::stringstream ss;

    ss << Stay::showDetails();
    ss << "\n=== Kardiologie ===\n";
    ss << "EKG durchgeführt\n";
    ss << "Herz-Kreislauf-Überwachung\n";

    return ss.str();
}
