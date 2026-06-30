#ifndef GENDER_H
#define GENDER_H

#include <string>

enum class Gender {
    male,
    female,
    nonBinary
};

Gender stringToGender(const std::string& value);
std::string genderToString(Gender gender);

#endif // GENDER_H
