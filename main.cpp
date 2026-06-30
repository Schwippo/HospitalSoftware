#include "hospitalsoftware.h"
#include "hospital.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    Hospital hospital("Prog2");

    HospitalSoftware window(&hospital);
    window.show();

    return app.exec();
}
