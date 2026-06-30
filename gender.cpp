#include "gender.h"

Gender stringToGender(const std::string& value) {
    if (value == "M") { return Gender::male; }
    if (value == "W") { return Gender::female; }
    return Gender::nonBinary;
}

std::string genderToString(Gender gender) {
    if (gender == Gender::male) { return "männlich"; }
    if (gender == Gender::female) { return "weiblich"; }
    return "divers";
}
