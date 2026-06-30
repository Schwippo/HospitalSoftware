#ifndef STAYINTERNALMEDICINE_H
#define STAYINTERNALMEDICINE_H

#include "stay.h"

class StayInternalMedicine : public Stay {
public:
    StayInternalMedicine (long patientID, long stayID, const std::string& admissionDate, const std::string& dischargeDate, const std::string& mainDiagnosisCode, const std::string& mainDiagnosisText, std::shared_ptr<Department> department);

    std::string showDetails() const override;
};

#endif // STAYINTERNALMEDICINE_H
