#include "hospitalstay.h"

HospitalStay::HospitalStay() : hospitalStayID(QUuid::createUuid()) {}

QUuid HospitalStay::getHospitalStayID() const { return hospitalStayID; }

void HospitalStay::addStay(std::shared_ptr<Stay> stay) { stays.push_back(stay); }

const std::vector<std::shared_ptr<Stay>>& HospitalStay::getStays() const { return stays; }
