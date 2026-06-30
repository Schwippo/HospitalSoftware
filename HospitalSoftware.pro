QT += core gui widgets charts

CONFIG += c++17

SOURCES += \
    appointment.cpp \
    ct.cpp \
    department.cpp \
    gender.cpp \
    hospital.cpp \
    hospitalsoftware.cpp \
    hospitalstay.cpp \
    main.cpp \
    mrt.cpp \
    operationroom.cpp \
    patient.cpp \
    resource.cpp \
    stay.cpp \
    staycardiology.cpp \
    staygastroenterology.cpp \
    staygeneralsurgery.cpp \
    stayinternalmedicine.cpp \
    stayoncology.cpp \
    stayorthopedics.cpp

HEADERS += \
    ResourceType.h \
    appointment.h \
    ct.h \
    department.h \
    gender.h \
    hospital.h \
    hospitalsoftware.h \
    hospitalstay.h \
    mrt.h \
    operationroom.h \
    patient.h \
    resource.h \
    stay.h \
    staycardiology.h \
    staygastroenterology.h \
    staygeneralsurgery.h \
    stayinternalmedicine.h \
    stayoncology.h \
    stayorthopedics.h

FORMS += \
    hospitalsoftware.ui

RESOURCES += \
    resources.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
