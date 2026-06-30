#ifndef STAYONCOLOGY_H
#define STAYONCOLOGY_H

#include "stay.h"

class StayOncology : public Stay {
public:
    StayOncology(long patientID, long stayID, const std::string& admissionDate, const std::string& dischargeDate, const std::string& mainDiagnosisCode, const std::string& mainDiagnosisText, std::shared_ptr<Department> department);

    std::string showDetails() const override;
};

#endif // STAYONCOLOGY_H
