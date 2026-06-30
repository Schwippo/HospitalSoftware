#ifndef STAYCARDIOLOGY_H
#define STAYCARDIOLOGY_H

#include "stay.h"

class StayCardiology : public Stay {
public:
    StayCardiology(long patientID, long stayID, const std::string& admissionDate, const std::string& dischargeDate, const std::string& mainDiagnosisCode, const std::string& mainDiagnosisText, std::shared_ptr<Department> department);

    std::string showDetails() const override;
};

#endif // STAYCARDIOLOGY_H
