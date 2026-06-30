#ifndef STAY_H
#define STAY_H

#include "department.h"
#include "appointment.h"

#include <memory>
#include <string>
#include <vector>

struct BloodPressureValue {
    std::string date;
    int systolic;
    int diastolic;
};

class HospitalStay;

class Stay {
protected:
    long patientID;
    long stayID;
    std::string admissionDate;
    std::string dischargeDate;
    std::string mainDiagnosisCode;
    std::string mainDiagnosisText;
    std::shared_ptr<Department> department;

    // P4
    std::weak_ptr<Stay> previousStay;
    std::weak_ptr<Stay> nextStay;

    std::string currentDiagnosisCode;
    std::vector<std::string> anamnesisCodes;
    std::string remarks;
    std::vector<BloodPressureValue> bloodPressureValues;

    std::vector<Appointment> appointments;

public:
    Stay(long patientID,
         long stayID,
         const std::string& admissionDate,
         const std::string& dischargeDate,
         const std::string& mainDiagnosisCode,
         const std::string& mainDiagnosisText,
         std::shared_ptr<Department> department);

    virtual ~Stay() = default;

    virtual std::string showDetails() const;

    long getPatientID() const;
    long getStayID() const;
    std::string getAdmissionDate() const;
    std::string getDischargeDate() const;
    std::string getMainDiagnosisCode() const;
    std::string getMainDiagnosisText() const;
    std::shared_ptr<Department> getDepartment() const;
    bool isRunning() const;

    void setPreviousStay(std::shared_ptr<Stay> stay);
    void setNextStay(std::shared_ptr<Stay> stay);

    std::shared_ptr<Stay> getPreviousStay() const;
    std::shared_ptr<Stay> getNextStay() const;

    void setCurrentDiagnosisCode(const std::string& code);
    std::string getCurrentDiagnosisCode() const;

    void setAnamnesisCode(int row, const std::string& code);
    const std::vector<std::string>& getAnamnesisCodes() const;

    void setRemarks(const std::string& text);
    std::string getRemarks() const;

    void addBloodPressureValue(const std::string& date, int systolic, int diastolic);
    const std::vector<BloodPressureValue>& getBloodPressureValues() const;

    void setAdmissionDate(const std::string& admissionDate);
    void setDischargeDate(const std::string& dischargeDate);

    void addAppointment(const Appointment& appointment);
    const std::vector<Appointment>& getAppointments() const;
};

#endif // STAY_H
