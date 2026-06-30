#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "department.h"
#include "patient.h"
#include "hospitalstay.h"
#include "resource.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

class Hospital {
private:
    std::string name;
    std::vector<std::shared_ptr<Patient>> patients;
    std::map<DepartmentType, std::shared_ptr<Department>> departments;

    std::map<std::string, std::string> icdCodeMap;
    std::map<std::string, std::string> opsCodeMap;
    bool icdDataWasRead;
    long nextPatientID;
    long nextStayID;

    std::string readCompleteFile(const std::string& fileName) const;
    std::vector<std::string> splitLine(const std::string& line, char separator) const;
    std::shared_ptr<Patient> findPatient(const std::string& insuranceNumber) const;
    std::shared_ptr<Department> getDepartment(DepartmentType departmentType) const;
    void createDepartments();

    // p4
    std::shared_ptr<Stay> createStay(long patientID, long stayID, const std::string& admissionDate, const std::string& dischargeDate, const std::string& mainDiagnosisCode, const std::string& mainDiagnosisText, std::shared_ptr<Department> department);
    std::shared_ptr<HospitalStay> findHospitalStay(std::shared_ptr<Stay> stay) const;

    std::vector<std::shared_ptr<Resource>> resources; // Im Konstruktor wird autom. OP 1, OP 2, MRT 1, MRT 2, CT 1, CT2 erzeugt

public:
    Hospital(const std::string& name);

    void readCaseFile(const std::string& fileName);
    void readICDData(const std::string& fileName);

    const std::vector<std::shared_ptr<Patient>>& getPatients() const;
    std::shared_ptr<Patient> findPatientByID(long patientID) const;
    std::shared_ptr<Stay> findStayByID(long stayID) const;

    std::string getICDText(const std::string& icdCode) const;
    bool isICDCodeValid(const std::string& icdCode) const;

    std::string printPatientList() const;

    void readOPSData(const std::string& filename);

    // p4 Aufenthalt/Patient verlegen
    void transferStay(std::shared_ptr<Stay> currentStay, DepartmentType newDepartmentType);

    void dischargeStay(std::shared_ptr<Stay> stay);

    std::shared_ptr<Resource> findFreeResource(ResourceType resourceType, const std::string& date, int startHour, int duration);
    const std::vector<std::shared_ptr<Resource>>& getResources() const;

    bool scheduleProcedure(std::shared_ptr<Stay> stay, const std::string& opsCode, const std::string& date, int startHour, int duration);
};

#endif // HOSPITAL_H
