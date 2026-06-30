# Hospital Software

A desktop-based hospital management application developed with **C++** and the **Qt Framework** as part of the *Programming 2* practical course.

The application demonstrates object-oriented programming, GUI development with Qt Widgets, and structured management of patients, hospital stays, diagnoses, medical history, and vital signs.

> **Current Version:** Practical Assignment 4

---

## Features

### Patient Management
- Display all patients in a tree view
- Sort patients by last and first name
- Highlight invalid insurance numbers
- Store and display patient information
- Automatic selection of patient details

### Hospital Stays
- Display admission and discharge dates
- Show responsible department
- Support multiple stays per patient
- Structured navigation between patients and stays

### Diagnosis Management
- Display admission diagnosis
- Edit current diagnosis
- ICD code lookup
- Automatic diagnosis description updates
- Read-only admission diagnosis

### Medical History (Anamnesis)
- Editable table of ICD diagnoses
- Automatic lookup of diagnosis descriptions
- Dynamic insertion of new rows
- Persistent storage of medical history

### Patient Notes
- Editable remarks field
- Automatic saving after modifications

### Blood Pressure Monitoring
- Record systolic and diastolic blood pressure
- Save measurements with date
- Visualize measurements using Qt Charts
- Display separate curves for systolic and diastolic values

### User Interface
- Qt Widgets based GUI
- Split-view layout
- Tree navigation
- Tabbed interface
- Read-only and editable fields where appropriate
- Automatic resizing of important UI elements

---

## Technologies

- **Language:** C++17
- **Framework:** Qt 6
- **Modules**
  - Qt Widgets
  - Qt Charts
  - Qt Core
  - Qt GUI
- JSON for patient and stay data
- Resource system (`.qrc`) for icons and data files

---

## Project Structure

```
HospitalSoftware/
│
├── data/
│   ├── stays.json
│   └── icdCodes.txt
│
├── icons/
│
├── hospital.cpp
├── hospital.h
├── patient.cpp
├── patient.h
├── stay.cpp
├── stay.h
├── hospitalsoftware.cpp
├── hospitalsoftware.h
├── main.cpp
│
├── resources.qrc
├── HospitalSoftware.pro
└── README.md
```

---

## Current Functionality

The application currently provides:

- Patient overview
- Hospital stay management
- ICD diagnosis handling
- Medical history management
- Blood pressure recording
- Blood pressure chart visualization
- Automatic validation of insurance numbers
- Automatic diagnosis lookup

---

## Getting Started

### Requirements

- Qt Creator
- Qt 6.x
- C++17 compatible compiler

### Build

Clone the repository:

```bash
git clone https://github.com/yourusername/HospitalSoftware.git
```

Open the project in **Qt Creator** and build it using the configured Qt kit.

---

## Learning Objectives

This project focuses on learning:

- Object-Oriented Programming (OOP)
- Class design
- Smart pointers
- Model management
- Qt Widgets
- Signals & Slots
- GUI programming
- JSON parsing
- Data validation
- Qt Charts
- Event-driven programming

---

## 📄 License

This project was developed for educational purposes as part of the **Programming 2** practical course.

---
