#include "hospitalsoftware.h"
#include "ui_hospitalsoftware.h"

#include <QAbstractItemView>
#include <QBrush>
#include <QChart>
#include <QDate>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineSeries>
#include <QPainter>
#include <QMessageBox>
#include <QSplitter>
#include <QStatusBar>
#include <QTableWidgetItem>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QWidget>
#include <QStringList>
#include <QInputDialog>

#include <algorithm>

namespace {
const int TreeItemTypeRole = Qt::UserRole;
const int PatientIDRole = Qt::UserRole + 1;
const int StayIDRole = Qt::UserRole + 2;

const int PatientItemType = 1;
const int HospitalStayItemType = 2; // P4 neu
const int StayItemType = 3;
}

HospitalSoftware::HospitalSoftware(Hospital* hospital, QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::HospitalSoftware),
      hospital(hospital),
      patientTreeWidget(nullptr),
      patientIDValueLabel(nullptr),
      patientNameValueLabel(nullptr),
      patientBirthDateValueLabel(nullptr),
      patientGenderValueLabel(nullptr),
      patientInsuranceValueLabel(nullptr),
      departmentValueLabel(nullptr),
      admissionDiagnosisCodeLineEdit(nullptr),
      admissionDiagnosisTextLineEdit(nullptr),
      currentDiagnosisCodeLineEdit(nullptr),
      currentDiagnosisTextValueLabel(nullptr),
      anamnesisTable(nullptr),
      remarksTextEdit(nullptr),
      bloodPressureDateEdit(nullptr),
      systolicSpinBox(nullptr),
      diastolicSpinBox(nullptr),
      addBloodPressureButton(nullptr),
      bloodPressureChartView(nullptr),
      updatingAnamnesisTable(false),
      updatingRemarks(false) {
    ui->setupUi(this);

    createUserInterface();

    connect(ui->actionLese_Aufenthalte, &QAction::triggered, this, &HospitalSoftware::readStays);
    connect(ui->actionLese_ICD_Codes, &QAction::triggered, this, &HospitalSoftware::readICDCodes);
    connect(ui->actionAufenthaltsueberblick, &QAction::triggered, this, &HospitalSoftware::showPatientOverview);

    connect(patientTreeWidget, &QTreeWidget::itemClicked, this, &HospitalSoftware::handleTreeItemClicked);
    connect(patientTreeWidget, &QTreeWidget::itemDoubleClicked, this, &HospitalSoftware::showInsuranceError);
    connect(currentDiagnosisCodeLineEdit, &QLineEdit::textChanged, this, &HospitalSoftware::updateCurrentDiagnosisText);
    connect(anamnesisTable, &QTableWidget::itemChanged, this, &HospitalSoftware::handleAnamnesisChanged);
    connect(remarksTextEdit, &QTextEdit::textChanged, this, &HospitalSoftware::saveRemarks);
    connect(addBloodPressureButton, &QPushButton::clicked, this, &HospitalSoftware::addBloodPressureValue);
    connect(ui->actionPatient_verlegen, &QAction::triggered, this, &HospitalSoftware::transferPatient);
    connect(ui->actionPatient_entlassen, &QAction::triggered, this, &HospitalSoftware::dischargePatient);
    connect(planProcedureButton, &QPushButton::clicked, this, &HospitalSoftware::onPlanProcedure);
    connect(transferPatientButton, &QPushButton::clicked, this, &HospitalSoftware::transferPatient);
    connect(dischargePatientButton, &QPushButton::clicked, this, &HospitalSoftware::dischargePatient);

    readDefaultDataAtStartup();
}

HospitalSoftware::~HospitalSoftware() {
    delete ui;
}

void HospitalSoftware::createUserInterface() {
    QWidget* mainWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(mainWidget);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, mainWidget);
    mainLayout->addWidget(splitter);

    patientTreeWidget = new QTreeWidget(splitter);
    patientTreeWidget->setHeaderLabels(QStringList() << "Name / Abteilung" << "Aufnahme" << "Entlassung");
    patientTreeWidget->setAlternatingRowColors(true);
    patientTreeWidget->setMinimumWidth(420);

    QWidget* rightSideWidget = new QWidget(splitter);
    QVBoxLayout* rightSideLayout = new QVBoxLayout(rightSideWidget);

    QGroupBox* patientGroupBox = new QGroupBox("Patientin / Patient", rightSideWidget);
    QFormLayout* patientFormLayout = new QFormLayout(patientGroupBox);

    patientIDValueLabel = new QLabel("-", patientGroupBox);
    patientNameValueLabel = new QLabel("-", patientGroupBox);
    patientBirthDateValueLabel = new QLabel("-", patientGroupBox);
    patientGenderValueLabel = new QLabel("-", patientGroupBox);
    patientInsuranceValueLabel = new QLabel("-", patientGroupBox);

    patientFormLayout->addRow("Patient-ID:", patientIDValueLabel);
    patientFormLayout->addRow("Name:", patientNameValueLabel);
    patientFormLayout->addRow("Geburtsdatum:", patientBirthDateValueLabel);
    patientFormLayout->addRow("Geschlecht:", patientGenderValueLabel);
    patientFormLayout->addRow("Versicherungsnummer:", patientInsuranceValueLabel);

    rightSideLayout->addWidget(patientGroupBox);

    QTabWidget* stayTabWidget = new QTabWidget(rightSideWidget);

    QWidget* diagnosisTab = new QWidget(stayTabWidget);
    QVBoxLayout* diagnosisTabLayout = new QVBoxLayout(diagnosisTab);

    QGroupBox* diagnosisGroupBox = new QGroupBox("Diagnose", diagnosisTab);
    QFormLayout* diagnosisFormLayout = new QFormLayout(diagnosisGroupBox);

    departmentValueLabel = new QLabel("-", diagnosisGroupBox);

    admissionDiagnosisCodeLineEdit = new QLineEdit(diagnosisGroupBox);
    admissionDiagnosisCodeLineEdit->setReadOnly(true);
    admissionDiagnosisCodeLineEdit->setMinimumWidth(250);

    admissionDiagnosisTextLineEdit = new QLineEdit(diagnosisGroupBox);
    admissionDiagnosisTextLineEdit->setMinimumWidth(500);
    admissionDiagnosisTextLineEdit->setReadOnly(true);

    currentDiagnosisCodeLineEdit = new QLineEdit(diagnosisGroupBox);
    currentDiagnosisCodeLineEdit->setPlaceholderText("z. B. I21.9");
    currentDiagnosisCodeLineEdit->setMinimumWidth(250);

    // ICD-Text
    currentDiagnosisTextValueLabel = new QTextEdit(diagnosisGroupBox);
    currentDiagnosisTextValueLabel->setReadOnly(true);
    currentDiagnosisTextValueLabel->setFixedWidth(800);
    currentDiagnosisTextValueLabel->setStyleSheet("");
    currentDiagnosisTextValueLabel->setPlainText("-");

    diagnosisFormLayout->addRow("Abteilung:", departmentValueLabel);
    diagnosisFormLayout->addRow("Aufnahmediagnose ICD:", admissionDiagnosisCodeLineEdit);
    diagnosisFormLayout->addRow("Aufnahmediagnose Text:", admissionDiagnosisTextLineEdit);
    diagnosisFormLayout->addRow("Aktuelle Diagnose:", currentDiagnosisCodeLineEdit);
    diagnosisFormLayout->addRow("ICD-Text:", currentDiagnosisTextValueLabel);

    diagnosisTabLayout->addWidget(diagnosisGroupBox);

    // p4 Patient verlegen oder entlassen
    QHBoxLayout* stayActionLayout = new QHBoxLayout();
    transferPatientButton = new QPushButton("Patient verlegen");
    dischargePatientButton = new QPushButton("Patient entlassen");
    stayActionLayout->addWidget(transferPatientButton);
    stayActionLayout->addWidget(dischargePatientButton);
    stayActionLayout->addStretch();
    //*

    diagnosisTabLayout->addLayout(stayActionLayout);

    QGroupBox* anamnesisGroupBox = new QGroupBox("Anamnese", diagnosisTab);
    QVBoxLayout* anamnesisLayout = new QVBoxLayout(anamnesisGroupBox);

    anamnesisTable = new QTableWidget(anamnesisGroupBox);
    anamnesisTable->setColumnCount(2);
    anamnesisTable->setHorizontalHeaderLabels(QStringList() << "ICD-Code" << "Text");
    anamnesisTable->horizontalHeader()->setStretchLastSection(true);
    anamnesisTable->verticalHeader()->setVisible(false);
    anamnesisTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    anamnesisTable->setEditTriggers(QAbstractItemView::DoubleClicked
                                    | QAbstractItemView::SelectedClicked
                                    | QAbstractItemView::EditKeyPressed);

    anamnesisLayout->addWidget(anamnesisTable);
    diagnosisTabLayout->addWidget(anamnesisGroupBox, 1);

    QGroupBox* remarksGroupBox = new QGroupBox("Bemerkungen", diagnosisTab);
    QVBoxLayout* remarksLayout = new QVBoxLayout(remarksGroupBox);
    remarksTextEdit = new QTextEdit(remarksGroupBox);
    remarksTextEdit->setPlaceholderText("Freitext für Bemerkungen eingeben ...");
    remarksLayout->addWidget(remarksTextEdit);
    diagnosisTabLayout->addWidget(remarksGroupBox, 1);

    stayTabWidget->addTab(diagnosisTab, "Diagnose und Anamnese");

    QWidget* curveTab = new QWidget(stayTabWidget);
    QVBoxLayout* curveLayout = new QVBoxLayout(curveTab);

    QGroupBox* inputGroupBox = new QGroupBox("Blutdruckwert eingeben", curveTab);
    QHBoxLayout* inputLayout = new QHBoxLayout(inputGroupBox);

    bloodPressureDateEdit = new QDateEdit(QDate::currentDate(), inputGroupBox);
    bloodPressureDateEdit->setDisplayFormat("dd.MM.yyyy");
    bloodPressureDateEdit->setCalendarPopup(true);

    systolicSpinBox = new QSpinBox(inputGroupBox);
    systolicSpinBox->setRange(50, 250);
    systolicSpinBox->setValue(120);
    systolicSpinBox->setSuffix(" mmHg");

    diastolicSpinBox = new QSpinBox(inputGroupBox);
    diastolicSpinBox->setRange(30, 160);
    diastolicSpinBox->setValue(80);
    diastolicSpinBox->setSuffix(" mmHg");

    addBloodPressureButton = new QPushButton("Speichern", inputGroupBox);

    inputLayout->addWidget(new QLabel("Datum:", inputGroupBox));
    inputLayout->addWidget(bloodPressureDateEdit);
    inputLayout->addWidget(new QLabel("Systolisch:", inputGroupBox));
    inputLayout->addWidget(systolicSpinBox);
    inputLayout->addWidget(new QLabel("Diastolisch:", inputGroupBox));
    inputLayout->addWidget(diastolicSpinBox);
    inputLayout->addWidget(addBloodPressureButton);

    curveLayout->addWidget(inputGroupBox);

    bloodPressureChartView = new QChartView(curveTab);
    bloodPressureChartView->setRenderHint(QPainter::Antialiasing);
    curveLayout->addWidget(bloodPressureChartView, 1);

    stayTabWidget->addTab(curveTab, "Kurve");

    // TabWidget für P4
    QWidget* procedureTab = new QWidget(stayTabWidget);
    QVBoxLayout* procedureLayout = new QVBoxLayout(procedureTab);
    QGroupBox* procedureGroupBox = new QGroupBox("Operation oder Diagnostik planen", procedureTab);
    QFormLayout* procedureFormLayout = new QFormLayout(procedureGroupBox);
    opsCodeLineEdit = new QLineEdit(procedureGroupBox);
    opsCodeLineEdit->setPlaceholderText("OPS-Code");
    procedureDateEdit = new QDateEdit(QDate::currentDate(), procedureGroupBox);
    procedureDateEdit->setDisplayFormat("dd.MM.yyyy");
    procedureDateEdit->setCalendarPopup(true);
    procedureStartHourSpinBox = new QSpinBox(procedureGroupBox);
    procedureStartHourSpinBox->setRange(8, 15);
    procedureStartHourSpinBox->setValue(8);
    procedureDurationSpinBox = new QSpinBox(procedureGroupBox);
    procedureDurationSpinBox->setRange(1, 8);
    procedureDurationSpinBox->setValue(1);
    procedureFormLayout->addRow("OPS-Code:", opsCodeLineEdit);
    procedureFormLayout->addRow("Datum:", procedureDateEdit);
    procedureFormLayout->addRow("Startzeit:", procedureStartHourSpinBox);
    procedureFormLayout->addRow("Dauer (h):", procedureDurationSpinBox);
    procedureLayout->addWidget(procedureGroupBox);

    planProcedureButton = new QPushButton("Operationen / Diagnostik planen", procedureTab);
    procedureLayout->addWidget(planProcedureButton);

    // Tabelle
    procedureTableWidget = new QTableWidget(procedureTab);
    procedureTableWidget->setColumnCount(4);
    procedureTableWidget->setHorizontalHeaderLabels(
        QStringList() << "OPS-Code"  << "Datum" << "Zeit" << "Ressource");

    procedureTableWidget->horizontalHeader()->setStretchLastSection(true);
    procedureTableWidget->verticalHeader()->setVisible(false);
    procedureLayout->addWidget(procedureTableWidget, 1);

    stayTabWidget->addTab(procedureTab, "Operationen und Prozeduren");
    //*ende P4

    rightSideLayout->addWidget(stayTabWidget, 1);

    splitter->addWidget(patientTreeWidget);
    splitter->addWidget(rightSideWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    setCentralWidget(mainWidget);
    resize(1200, 750);

    splitter->setSizes({300, 1200});
}

void HospitalSoftware::readDefaultDataAtStartup() {
    try {
        hospital->readICDData(":/data/icdCodes.txt");
        hospital->readCaseFile(":/data/stays.json");
        fillPatientTree();
        statusBar()->showMessage("Standarddaten wurden automatisch eingelesen.");
    } catch (const std::exception& error) {
        QMessageBox::critical(this, "Fehler beim automatischen Einlesen", error.what());
    }
}

void HospitalSoftware::readICDCodes() {
    try {
        hospital->readICDData(":/data/icdCodes.txt");
        QMessageBox::information(this, "ICD-Codes", "Die ICD-Codes wurden erfolgreich eingelesen.");
    } catch (const std::exception& error) {
        QMessageBox::critical(this, "Fehler", error.what());
    }
}

void HospitalSoftware::readStays() {
    try {
        hospital->readCaseFile(":/data/stays.json");
        QMessageBox::information(this, "Aufenthalte", "Die Aufenthaltsdaten wurden erfolgreich eingelesen.");
        fillPatientTree();
        clearPatientDetails();
        clearStayDetails();
    } catch (const std::exception& error) {
        QMessageBox::critical(this, "Fehler", error.what());
    }
}

void HospitalSoftware::showPatientOverview() {
    fillPatientTree();
}

void HospitalSoftware::fillPatientTree() {
    patientTreeWidget->clear();

    const std::vector<std::shared_ptr<Patient>>& patients = hospital->getPatients();

    for (const std::shared_ptr<Patient>& patient : patients) {
        QTreeWidgetItem* patientItem = new QTreeWidgetItem(patientTreeWidget);

        patientItem->setText(0, QString::fromStdString(patient->getLastName()));
        patientItem->setText(1, QString::fromStdString(patient->getFirstName()));
        patientItem->setData(0, TreeItemTypeRole, PatientItemType);
        patientItem->setData(0, PatientIDRole, QVariant::fromValue<qint64>(patient->getPatientID()));

        if (!patient->hasValidInsuranceNumber()) {
            patientItem->setForeground(0, QBrush(Qt::red));
            patientItem->setForeground(1, QBrush(Qt::red));
            patientItem->setToolTip(0, "Ungültige Versicherungsnummer - Doppelklick für Details");
        }

        // P4 änderung QTreeWidget aufgabe 2b
        const std::vector<std::shared_ptr<HospitalStay>>& hospitalStays = patient->getHospitalStays();
        for (const std::shared_ptr<HospitalStay>& hospitalStay : hospitalStays) {
            QTreeWidgetItem* hospitalStayItem = new QTreeWidgetItem(patientItem);

            // p4 -> Gui weiß dann, dass das HospitalStay ist
            hospitalStayItem->setData(0, TreeItemTypeRole, HospitalStayItemType);
            hospitalStayItem->setData(0, PatientIDRole, QVariant::fromValue<qint64>(patient->getPatientID()));
            //

            hospitalStayItem->setText(0, "Krankenhausaufenthalt");
            hospitalStayItem->setText(1, hospitalStay->getHospitalStayID().toString(QUuid::WithoutBraces));

            std::vector<std::shared_ptr<Stay>> stays = hospitalStay->getStays();
            std::sort(stays.begin(), stays.end(), [](const std::shared_ptr<Stay>& left, const std::shared_ptr<Stay>& right) {
                          return left->getAdmissionDate() < right->getAdmissionDate();
                      });

            for (const std::shared_ptr<Stay>& stay : stays) {
                QTreeWidgetItem* stayItem = new QTreeWidgetItem(hospitalStayItem);

                // Name der Klinik
                stayItem->setText(0, QString::fromStdString(stay->getDepartment()->getName()));

                // Aufnahme
                stayItem->setText(1, formatDate(stay->getAdmissionDate()));

                // Entlassung
                if(stay->isRunning()) {
                    stayItem->setText(2, "laufend");
                } else {
                    stayItem->setText(2, formatDate(stay->getDischargeDate()));
                }

                stayItem->setData(0, TreeItemTypeRole, StayItemType);
                stayItem->setData(0, PatientIDRole, QVariant::fromValue<qint64>(patient->getPatientID()));
                stayItem->setData(0, StayIDRole, QVariant::fromValue<qint64>(stay->getStayID()));

            }
        }
        patientItem->setExpanded(patient->hasRunningStay());
    }

    patientTreeWidget->setColumnWidth(0, 180);
    patientTreeWidget->setColumnWidth(1, 90);
    patientTreeWidget->setColumnWidth(2, 90);
}

void HospitalSoftware::handleTreeItemClicked(QTreeWidgetItem* item, int column) {
    Q_UNUSED(column);

    if (item == nullptr) {
        return;
    }

    int type = item->data(0, TreeItemTypeRole).toInt();
    long patientID = item->data(0, PatientIDRole).toLongLong();
    selectedPatient = hospital->findPatientByID(patientID);
    fillPatientDetails(selectedPatient);

    if (type == StayItemType) {
        long stayID = item->data(0, StayIDRole).toLongLong();
        selectedStay = hospital->findStayByID(stayID);
        fillStayDetails(selectedStay);

        // p4
    } else if (type == HospitalStayItemType) {
        selectedStay = nullptr;
        clearStayDetails();

    } else {
        selectedStay = nullptr;
        clearStayDetails();
    }

    updateProcedureTable();
}

void HospitalSoftware::showInsuranceError(QTreeWidgetItem* item, int column) {
    Q_UNUSED(column);

    if (item == nullptr) {
        return;
    }

    long patientID = item->data(0, PatientIDRole).toLongLong();
    std::shared_ptr<Patient> patient = hospital->findPatientByID(patientID);

    if (patient == nullptr || patient->hasValidInsuranceNumber()) {
        return;
    }

    QMessageBox::warning(this,
                         "Ungültige Versicherungsnummer",
                         QString::fromStdString(patient->getInsuranceErrorText()));
}

void HospitalSoftware::fillPatientDetails(std::shared_ptr<Patient> patient) {
    if (patient == nullptr) {
        clearPatientDetails();
        return;
    }

    patientIDValueLabel->setText(QString::number(patient->getPatientID()));
    patientNameValueLabel->setText(QString::fromStdString(patient->getFirstName() + " " + patient->getLastName()));
    patientBirthDateValueLabel->setText(formatDate(patient->getBirthDate()));
    patientGenderValueLabel->setText(QString::fromStdString(genderToString(patient->getGender())));
    patientInsuranceValueLabel->setText(QString::fromStdString(patient->getInsuranceNumber()));
}

void HospitalSoftware::fillStayDetails(std::shared_ptr<Stay> stay) {
    if (stay == nullptr) {
        clearStayDetails();
        return;
    }

    if (stay->getDepartment() != nullptr) {
        departmentValueLabel->setText(QString::fromStdString(stay->getDepartment()->getName()));
    } else {
        departmentValueLabel->setText("-");
    }

    admissionDiagnosisCodeLineEdit->setText(QString::fromStdString(stay->getMainDiagnosisCode()));
    admissionDiagnosisTextLineEdit->setText(QString::fromStdString(stay->getMainDiagnosisText()));

    currentDiagnosisCodeLineEdit->blockSignals(true);
    currentDiagnosisCodeLineEdit->setText(QString::fromStdString(stay->getCurrentDiagnosisCode()));
    currentDiagnosisCodeLineEdit->blockSignals(false);
    updateCurrentDiagnosisText(currentDiagnosisCodeLineEdit->text());

    fillAnamnesisTable();

    updatingRemarks = true;
    remarksTextEdit->setPlainText(QString::fromStdString(stay->getRemarks()));
    updatingRemarks = false;

    refreshBloodPressureChart();
}

void HospitalSoftware::clearPatientDetails() {
    selectedPatient = nullptr;
    patientIDValueLabel->setText("-");
    patientNameValueLabel->setText("-");
    patientBirthDateValueLabel->setText("-");
    patientGenderValueLabel->setText("-");
    patientInsuranceValueLabel->setText("-");
}

void HospitalSoftware::clearStayDetails() {
    selectedStay = nullptr;
    departmentValueLabel->setText("-");
    admissionDiagnosisCodeLineEdit->clear();
    admissionDiagnosisTextLineEdit->clear();
    currentDiagnosisCodeLineEdit->clear();
    currentDiagnosisTextValueLabel->setPlainText("-");

    updatingAnamnesisTable = true;
    anamnesisTable->clearContents();
    anamnesisTable->setRowCount(0);
    updatingAnamnesisTable = false;

    updatingRemarks = true;
    remarksTextEdit->clear();
    updatingRemarks = false;

    refreshBloodPressureChart();
}

void HospitalSoftware::updateCurrentDiagnosisText(const QString& code) {
    if (selectedStay != nullptr) {
        selectedStay->setCurrentDiagnosisCode(code.trimmed().toStdString());
    }

    currentDiagnosisTextValueLabel->setPlainText(getTextForICDCode(code));
}

QString HospitalSoftware::getTextForICDCode(const QString& code) const {
    QString trimmedCode = code.trimmed();

    if (trimmedCode.isEmpty()) {
        return "-";
    }

    std::string text = hospital->getICDText(trimmedCode.toStdString());

    if (text.empty()) {
        return "Warnung: ungültiger ICD-Code";
    }

    return QString::fromStdString(text);
}

void HospitalSoftware::fillAnamnesisTable() {
    updatingAnamnesisTable = true;
    anamnesisTable->clearContents();

    if (selectedStay == nullptr) {
        anamnesisTable->setRowCount(0);
        updatingAnamnesisTable = false;
        return;
    }

    const std::vector<std::string>& anamnesisCodes = selectedStay->getAnamnesisCodes();
    anamnesisTable->setRowCount(static_cast<int>(anamnesisCodes.size()) + 1);

    for (int row = 0; row < static_cast<int>(anamnesisCodes.size()); row++) {
        addAnamnesisRow(row, QString::fromStdString(anamnesisCodes[row]));
    }

    addAnamnesisRow(static_cast<int>(anamnesisCodes.size()), "");
    anamnesisTable->resizeColumnsToContents();
    updatingAnamnesisTable = false;
}

void HospitalSoftware::addAnamnesisRow(int row, const QString& code) {
    QTableWidgetItem* codeItem = new QTableWidgetItem(code);
    codeItem->setFlags(codeItem->flags() | Qt::ItemIsEditable);
    anamnesisTable->setItem(row, 0, codeItem);

    QTableWidgetItem* textItem = new QTableWidgetItem(getTextForICDCode(code));
    textItem->setFlags(textItem->flags() & ~Qt::ItemIsEditable);
    anamnesisTable->setItem(row, 1, textItem);
}

void HospitalSoftware::handleAnamnesisChanged(QTableWidgetItem* item) {
    if (updatingAnamnesisTable || selectedStay == nullptr || item == nullptr) {
        return;
    }

    if (item->column() != 0) {
        return;
    }

    int row = item->row();
    QString code = item->text().trimmed();

    updatingAnamnesisTable = true;
    if (item->text() != code) {
        item->setText(code);
    }
    updatingAnamnesisTable = false;

    selectedStay->setAnamnesisCode(row, code.toStdString());

    updatingAnamnesisTable = true;

    QTableWidgetItem* textItem = anamnesisTable->item(row, 1);
    if (textItem == nullptr) {
        textItem = new QTableWidgetItem();
        textItem->setFlags(textItem->flags() & ~Qt::ItemIsEditable);
        anamnesisTable->setItem(row, 1, textItem);
    }
    textItem->setText(getTextForICDCode(code));

    if (row == anamnesisTable->rowCount() - 1 && !code.isEmpty()) {
        anamnesisTable->insertRow(anamnesisTable->rowCount());
        addAnamnesisRow(anamnesisTable->rowCount() - 1, "");
    }

    anamnesisTable->resizeColumnsToContents();
    updatingAnamnesisTable = false;
}

void HospitalSoftware::saveRemarks() {
    if (updatingRemarks || selectedStay == nullptr) {
        return;
    }

    selectedStay->setRemarks(remarksTextEdit->toPlainText().toStdString());
}

void HospitalSoftware::addBloodPressureValue() {
    if (selectedStay == nullptr) {
        QMessageBox::information(this, "Kein Aufenthalt ausgewählt", "Bitte wählen Sie zuerst einen Aufenthalt aus.");
        return;
    }

    std::string date = bloodPressureDateEdit->date().toString("yyyyMMdd").toStdString();
    selectedStay->addBloodPressureValue(date, systolicSpinBox->value(), diastolicSpinBox->value());
    refreshBloodPressureChart();
}

void HospitalSoftware::refreshBloodPressureChart() {
        QChart* chart = new QChart();
        chart->setTitle("Blutdruckkurve");

        QLineSeries* systolicSeries = new QLineSeries(chart);
        systolicSeries->setName("Systolisch");
        systolicSeries->setPointsVisible(true);

        QLineSeries* diastolicSeries = new QLineSeries(chart);
        diastolicSeries->setName("Diastolisch");
        diastolicSeries->setPointsVisible(true);

        std::vector<BloodPressureValue> values;

        if (selectedStay != nullptr) {
            values = selectedStay->getBloodPressureValues();
        }

        std::sort(values.begin(), values.end(), [](const BloodPressureValue& left, const BloodPressureValue& right) {
            return left.date < right.date;
        });

        QDate firstDate;
        QDate lastDate;

        for (const BloodPressureValue& value : values) {
            QDate date = QDate::fromString(QString::fromStdString(value.date), "yyyyMMdd");

            if (date.isValid()) {
                if (!firstDate.isValid()) {
                    firstDate = date;
                }

                lastDate = date;

                qint64 xValue = QDateTime(date, QTime(0, 0)).toMSecsSinceEpoch();

                systolicSeries->append(xValue, value.systolic);
                diastolicSeries->append(xValue, value.diastolic);
            }
        }

        chart->addSeries(systolicSeries);
        chart->addSeries(diastolicSeries);

        QDateTimeAxis* axisX = new QDateTimeAxis(chart);
        axisX->setTitleText("Datum");
        axisX->setFormat("dd.MM.yyyy");

        if (firstDate.isValid() && lastDate.isValid()) {
            if (firstDate == lastDate) {
                axisX->setRange(
                    QDateTime(firstDate.addDays(-1), QTime(0, 0)),
                    QDateTime(lastDate.addDays(1), QTime(0, 0))
                    );
            } else {
                axisX->setRange(
                    QDateTime(firstDate, QTime(0, 0)),
                    QDateTime(lastDate, QTime(0, 0))
                    );
            }
        }

        QValueAxis* axisY = new QValueAxis(chart);
        axisY->setTitleText("mmHg");
        axisY->setRange(40, 220);

        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);

        systolicSeries->attachAxis(axisX);
        systolicSeries->attachAxis(axisY);

        diastolicSeries->attachAxis(axisX);
        diastolicSeries->attachAxis(axisY);

        QChart* oldChart = bloodPressureChartView->chart();
        bloodPressureChartView->setChart(chart);
        delete oldChart;
}

QString HospitalSoftware::formatDate(const std::string& date) const {
    if (date.empty()) {
        return "laufend";
    }

    QDate qDate = QDate::fromString(QString::fromStdString(date), "yyyyMMdd");

    if (!qDate.isValid()) {
        return QString::fromStdString(date);
    }

    return qDate.toString("dd.MM.yyyy");
}

// p4: Patient verlegen -> Neue Abteilung: Allgemeinchirugie, Kardiologie, ... OK oder abbrechen
void HospitalSoftware::transferPatient() {
    if(selectedStay == nullptr) {
        QMessageBox::information(this, "Patient verlegen", "Bitte wählen Sie zuerst einen Aufenthalt aus.");
        return;
    }

    if(!selectedStay->isRunning()) {
        QMessageBox::information(this, "Patient verlegen", "Der Aufenthalt ist bereits beendet.");
        return;
    }

    QStringList departments;
    departments << "Allgemeinchirurgie" << "Kardiologie" << "Innere Medizin" << "Orthopädie" << "Onkologie" << "Gastroenterologie";

    bool ok = false;

    QString selectedDepartment = QInputDialog::getItem(this, "Patient verlegen", "Neue Abteilung", departments, 0, false, &ok);

    // abbrechen
    if(!ok) {
        return;
    }

    // DepartmentType bestimmen
    DepartmentType newDepartment = departmentTypeFromString(selectedDepartment);
    hospital->transferStay(selectedStay, newDepartment);

    fillPatientTree();
    clearStayDetails();
    selectedStay = nullptr; // Verlegung an Hospital übergeben
}

DepartmentType HospitalSoftware::departmentTypeFromString(const QString& text) const {
    if(text == "Allgemeinchirurgie") { return DepartmentType::GeneralSurgery; }
    else if(text == "Kardiologie") { return DepartmentType::Cardiology; }
    else if(text == "Innere Medizin") { return DepartmentType::InternalMedicine; }
    else if(text == "Orthopädie") { return DepartmentType::Orthopedics; }
    else if(text == "Onkologie") { return DepartmentType::Oncology; }
    else if(text == "Gastroenterologie") { return DepartmentType::Gastroenterology; }

    throw std::runtime_error("Unbekannter DepartmentType");
}

void HospitalSoftware::dischargePatient() {
    if(selectedStay == nullptr) {
        QMessageBox::information(this, "Patient entlassen", "Bitte wählen Sie zuerst einen Aufenthalt aus.");
        return;
    }

    if(!selectedStay->isRunning()) {
        QMessageBox::information(this, "Patient entlassen", "Der Aufenthalt wurde bereits beendet.");
        return;
    }

    hospital->dischargeStay(selectedStay);
    fillPatientTree();
    clearStayDetails();
    selectedStay = nullptr;
}

void HospitalSoftware::onPlanProcedure() {
    if(selectedStay == nullptr) {
        QMessageBox::information(this, "Operation planen", "Bitte wählen Sie zuerst einen Aufenthalt aus.");
        return;
    }

    std::string opsCode = opsCodeLineEdit->text().toStdString();
    std::string date = procedureDateEdit->date().toString("dd.MM.yyyy").toStdString();

    int startHour = procedureStartHourSpinBox->value();
    int duration = procedureDurationSpinBox->value();

    bool success = hospital->scheduleProcedure(selectedStay, opsCode, date, startHour, duration);
    if(!success) {
        QMessageBox::warning(this, "Operation planen", "Keine freie Ressource gefunden.");
        return;
    }

    updateProcedureTable();
    QMessageBox::information(this, "Operation planen", "Operation erfolgreich geplant.");
}

void HospitalSoftware::updateProcedureTable() {
    procedureTableWidget->setRowCount(0);

    if (selectedStay == nullptr)
    return;

    const auto& appointments = selectedStay->getAppointments();

    for (const Appointment& appointment : appointments) {
        int row = procedureTableWidget->rowCount();

        procedureTableWidget->insertRow(row);
        procedureTableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(appointment.getOPSCode())));
        procedureTableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(appointment.getDate())));
        procedureTableWidget->setItem(row, 2, new QTableWidgetItem(QString("%1:00 - %2:00").arg(appointment.getStartHour()).arg(appointment.getEndHour())));

        QString resourceName = "-";

        if (appointment.getResource() != nullptr){
            resourceName = QString::fromStdString(appointment.getResource()->getName());

        }

        procedureTableWidget->setItem(row, 3, new QTableWidgetItem(resourceName));
    }
}
