#ifndef HOSPITALSTAY_H
#define HOSPITALSTAY_H

#include "stay.h"

#include <QUuid>
#include <vector>

class HospitalStay {
private:
    QUuid hospitalStayID;
    std::vector<std::shared_ptr<Stay>> stays;

public:
    HospitalStay();

    QUuid getHospitalStayID() const;
    void addStay(std::shared_ptr<Stay> stay);
    const std::vector<std::shared_ptr<Stay>>& getStays() const;
};

#endif // HOSPITALSTAY_H
