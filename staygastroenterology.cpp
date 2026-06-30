#include "staygastroenterology.h"

StayGastroenterology::StayGastroenterology(long patientID, long stayID, const std::string& admissionDate, const std::string& dischargeDate, const std::string& mainDiagnosisCode, const std::string& mainDiagnosisText, std::shared_ptr<Department> department)
    : Stay(patientID, stayID, admissionDate, dischargeDate, mainDiagnosisCode, mainDiagnosisText, department) {}

std::string StayGastroenterology::showDetails() const { return Stay::showDetails(); }
