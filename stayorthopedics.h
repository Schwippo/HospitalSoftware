#ifndef STAYORTHOPEDICS_H
#define STAYORTHOPEDICS_H

#include "stay.h"

class StayOrthopedics : public Stay {
public:
    StayOrthopedics(long patientID, long stayID, const std::string& admissionDate, const std::string& dischargeDate, const std::string& mainDiagnosisCode, const std::string& mainDiagnosisText, std::shared_ptr<Department> department);

    std::string showDetails() const override;
};

#endif // STAYORTHOPEDICS_H
