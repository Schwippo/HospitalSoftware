#ifndef STAYGENERALSURGERY_H
#define STAYGENERALSURGERY_H

#include "stay.h"

class StayGeneralSurgery : public Stay {
public:
    StayGeneralSurgery(long patientID, long stayID, const std::string& admissionDate, const std::string& dischargeDate, const std::string& mainDiagnosisCode, const std::string& mainDiagnosisText, std::shared_ptr<Department> department);

    std::string showDetails() const override;
};

#endif // STAYGENERALSURGERY_H
