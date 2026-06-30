#ifndef PATIENT_H
#define PATIENT_H

#include "gender.h"
#include "stay.h"
#include "hospitalstay.h"

#include <memory>
#include <string>
#include <vector>

class Patient {
private:
    long patientID;
    std::string birthDate;
    std::string firstName;
    std::string lastName;
    std::string insuranceNumber;
    Gender gender;
    std::vector<std::shared_ptr<HospitalStay>> hospitalStays;

    int calculateCheckDigit(const std::string& text) const;
    std::string prepareInsurancePart(const std::string& text) const;

public:
    Patient(long patientID,
            const std::string& birthDate,
            const std::string& firstName,
            const std::string& lastName,
            const std::string& insuranceNumber,
            Gender gender);

    std::string showOverview() const;

    long getPatientID() const;
    std::string getBirthDate() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getInsuranceNumber() const;
    Gender getGender() const;

    void addHospitalStay(std::shared_ptr<HospitalStay> hospitalStay);
    const std::vector<std::shared_ptr<HospitalStay>>& getHospitalStays() const;
    bool hasRunningStay() const;

    bool isFirstCheckDigitValid() const;
    bool isSecondCheckDigitValid() const;
    bool hasValidInsuranceNumber() const;
    std::string getInsuranceErrorText() const;
};

#endif // PATIENT_H
