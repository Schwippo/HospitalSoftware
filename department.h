#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <string>

enum class DepartmentType {
    GeneralSurgery = 0,
    Cardiology = 1,
    InternalMedicine = 2,
    Orthopedics = 3,
    Oncology = 4,
    Gastroenterology = 5
};

DepartmentType departmentTypeFromInt(int value);

class Department {
private:
    DepartmentType type;
    std::string name;

public:
    Department(DepartmentType type, const std::string& name);
    virtual ~Department() = default;

    DepartmentType getDepartmentType() const;
    std::string getName() const;

    virtual std::string getClassName() const = 0;
};

class GeneralSurgery : public Department {
public:
    GeneralSurgery();
    std::string getClassName() const override;
};

class Cardiology : public Department {
public:
    Cardiology();
    std::string getClassName() const override;
};

class InternalMedicine : public Department {
public:
    InternalMedicine();
    std::string getClassName() const override;
};

class Orthopedics : public Department {
public:
    Orthopedics();
    std::string getClassName() const override;
};

class Oncology : public Department {
public:
    Oncology();
    std::string getClassName() const override;
};

class Gastroenterology : public Department {
public:
    Gastroenterology();
    std::string getClassName() const override;
};

#endif // DEPARTMENT_H
