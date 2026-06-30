#ifndef HOSPITALSOFTWARE_H
#define HOSPITALSOFTWARE_H

#include "hospital.h"

#include <QChartView>
#include <QDateEdit>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QTableWidget>
#include <QTextEdit>
#include <QTreeWidget>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class HospitalSoftware;
}
QT_END_NAMESPACE

class HospitalSoftware : public QMainWindow {
    Q_OBJECT

public:
    HospitalSoftware(Hospital* hospital, QWidget* parent = nullptr);
    ~HospitalSoftware();

private slots:
    void readStays();
    void readICDCodes();
    void showPatientOverview();
    void handleTreeItemClicked(QTreeWidgetItem* item, int column);
    void showInsuranceError(QTreeWidgetItem* item, int column);
    void updateCurrentDiagnosisText(const QString& code);
    void handleAnamnesisChanged(QTableWidgetItem* item);
    void saveRemarks();
    void addBloodPressureValue();
    void transferPatient();
    void dischargePatient();
    void onPlanProcedure();

private:
    Ui::HospitalSoftware* ui;
    Hospital* hospital;

    QTreeWidget* patientTreeWidget;

    QLabel* patientIDValueLabel;
    QLabel* patientNameValueLabel;
    QLabel* patientBirthDateValueLabel;
    QLabel* patientGenderValueLabel;
    QLabel* patientInsuranceValueLabel;

    QLabel* departmentValueLabel;
    QLineEdit* admissionDiagnosisCodeLineEdit;
    QLineEdit* admissionDiagnosisTextLineEdit;
    QLineEdit* currentDiagnosisCodeLineEdit;
    QTextEdit* currentDiagnosisTextValueLabel;
    QTableWidget* anamnesisTable;
    QTextEdit* remarksTextEdit;

    QDateEdit* bloodPressureDateEdit;
    QSpinBox* systolicSpinBox;
    QSpinBox* diastolicSpinBox;
    QPushButton* addBloodPressureButton;
    QChartView* bloodPressureChartView;

    QPushButton* transferPatientButton;
    QPushButton* dischargePatientButton;

    QLineEdit* opsCodeLineEdit;
    QDateEdit* procedureDateEdit;
    QSpinBox* procedureStartHourSpinBox;
    QSpinBox* procedureDurationSpinBox;
    QPushButton* planProcedureButton;
    QTableWidget* procedureTableWidget;

    std::shared_ptr<Patient> selectedPatient;
    std::shared_ptr<Stay> selectedStay;
    bool updatingAnamnesisTable;
    bool updatingRemarks;

    void createUserInterface();
    void readDefaultDataAtStartup();
    void fillPatientTree();
    void fillPatientDetails(std::shared_ptr<Patient> patient);
    void fillStayDetails(std::shared_ptr<Stay> stay);
    void clearPatientDetails();
    void clearStayDetails();
    void fillAnamnesisTable();
    void addAnamnesisRow(int row, const QString& code);
    void refreshBloodPressureChart();

    QString formatDate(const std::string& date) const;
    QString getTextForICDCode(const QString& code) const;

    DepartmentType departmentTypeFromString(const QString& text) const;

    void updateProcedureTable();
};

#endif // HOSPITALSOFTWARE_H
