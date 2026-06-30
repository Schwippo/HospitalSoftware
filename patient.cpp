#include "patient.h"

#include <cctype>
#include <sstream>

Patient::Patient(long patientID,
                 const std::string& birthDate,
                 const std::string& firstName,
                 const std::string& lastName,
                 const std::string& insuranceNumber,
                 Gender gender)
    : patientID(patientID),
      birthDate(birthDate),
      firstName(firstName),
      lastName(lastName),
      insuranceNumber(insuranceNumber),
      gender(gender) {}

long Patient::getPatientID() const { return patientID; }
std::string Patient::getBirthDate() const { return birthDate; }
std::string Patient::getFirstName() const { return firstName; }
std::string Patient::getLastName() const { return lastName; }
std::string Patient::getInsuranceNumber() const { return insuranceNumber; }
Gender Patient::getGender() const { return gender; }

void Patient::addHospitalStay(std::shared_ptr<HospitalStay> hospitalStay) { hospitalStays.push_back(hospitalStay); }
const std::vector<std::shared_ptr<HospitalStay>>& Patient::getHospitalStays() const { return hospitalStays; }

bool Patient::hasRunningStay() const {
    for(const auto& hospitalStay : hospitalStays) {
        for(const auto& stay : hospitalStay->getStays()) {
            if(stay->isRunning()) {
                return true;
            }
        }
    }

    return false;
}

std::string Patient::showOverview() const {
    std::stringstream text;

    text << "-----------------------------------------------\n";
    text << "Patient*in " << patientID << ": "
         << firstName << " " << lastName
         << ", Versicherungsnummer: " << insuranceNumber << "\n";
    text << "Geschlecht: " << genderToString(gender)
         << ", Geburtsdatum: " << birthDate << "\n";
    text << "Aufenthalte:\n";

    for (const auto& hospitalStay : hospitalStays) {
        text << "Krankenhausaufenthalt "
             << hospitalStay->getHospitalStayID().toString().toStdString()
             << "\n";

        for(const auto& stay : hospitalStay->getStays()) {
            text << stay->showDetails();
        }
    }

    return text.str();
}

std::string Patient::prepareInsurancePart(const std::string& text) const {
    std::string preparedText;

    for (char currentChar : text) {
        if (std::isalpha(static_cast<unsigned char>(currentChar))) {
            char upperChar = std::toupper(static_cast<unsigned char>(currentChar));
            int letterNumber = upperChar - 'A' + 1;

            if (letterNumber < 10) {
                preparedText += "0";
            }

            preparedText += std::to_string(letterNumber);
        } else {
            preparedText += currentChar;
        }
    }

    return preparedText;
}

int Patient::calculateCheckDigit(const std::string& text) const {
    std::string preparedText = prepareInsurancePart(text);
    int sum = 0;

    for (int i = 0; i < static_cast<int>(preparedText.size()); i++) {
        int digit = preparedText[i] - '0';
        int factor;

        if (i % 2 == 0) {
            factor = 1;
        } else {
            factor = 2;
        }

        int product = digit * factor;

        if (product >= 10) {
            sum += product / 10;
            sum += product % 10;
        } else {
            sum += product;
        }
    }

    return sum % 10;
}

bool Patient::isFirstCheckDigitValid() const {
    if (insuranceNumber.size() != 20) {
        return false;
    }

    std::string firstPartWithoutCheckDigit = insuranceNumber.substr(0, 9);
    int expectedCheckDigit = calculateCheckDigit(firstPartWithoutCheckDigit);
    int actualCheckDigit = insuranceNumber[9] - '0';

    return expectedCheckDigit == actualCheckDigit;
}

bool Patient::isSecondCheckDigitValid() const {
    if (insuranceNumber.size() != 20) {
        return false;
    }

    std::string completeNumberWithoutSecondCheckDigit = insuranceNumber.substr(0, 19);
    int expectedCheckDigit = calculateCheckDigit(completeNumberWithoutSecondCheckDigit);
    int actualCheckDigit = insuranceNumber[19] - '0';

    return expectedCheckDigit == actualCheckDigit;
}

bool Patient::hasValidInsuranceNumber() const {
    return isFirstCheckDigitValid() && isSecondCheckDigitValid();
}

std::string Patient::getInsuranceErrorText() const {
    std::stringstream text;

    text << "Patient*in " << patientID << ": " << firstName << " " << lastName << "\n";
    text << "Versicherungsnummer: " << insuranceNumber << "\n\n";

    if (insuranceNumber.size() != 20) {
        text << "Die Versicherungsnummer hat nicht genau 20 Zeichen\n";
        return text.str();
    }

    std::string firstPartWithoutCheckDigit = insuranceNumber.substr(0, 9);
    int expectedFirstCheckDigit = calculateCheckDigit(firstPartWithoutCheckDigit);
    int actualFirstCheckDigit = insuranceNumber[9] - '0';

    std::string completeNumberWithoutSecondCheckDigit = insuranceNumber.substr(0, 19);
    int expectedSecondCheckDigit = calculateCheckDigit(completeNumberWithoutSecondCheckDigit);
    int actualSecondCheckDigit = insuranceNumber[19] - '0';

    if (expectedFirstCheckDigit != actualFirstCheckDigit) {
        text << "Die erste Prüfziffer ist falsch\n";
        text << "Aktuelle Prüfziffer an Stelle 10: " << actualFirstCheckDigit << "\n";
        text << "Korrekte Prüfziffer: " << expectedFirstCheckDigit << "\n\n";
    }

    if (expectedSecondCheckDigit != actualSecondCheckDigit) {
        text << "Die zweite Prüfziffer ist falsch\n";
        text << "Aktuelle Prüfziffer an Stelle 20: " << actualSecondCheckDigit << "\n";
        text << "Korrekte Prüfziffer: " << expectedSecondCheckDigit << "\n\n";
    }

    return text.str();
}
