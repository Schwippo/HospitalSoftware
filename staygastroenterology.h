#ifndef STAYGASTROENTEROLOGY_H
#define STAYGASTROENTEROLOGY_H

#include "stay.h"

class StayGastroenterology : public Stay {
public:
    StayGastroenterology(long patientID, long stayID, const std::string& admissionDate, const std::string& dischargeDate, const std::string& mainDiagnosisCode, const std::string& mainDiagnosisText, std::shared_ptr<Department> department);

    std::string showDetails() const override;
};

#endif // STAYGASTROENTEROLOGY_H
