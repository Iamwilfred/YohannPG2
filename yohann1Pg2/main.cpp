#include "travelagency.h"

//int main() {
//    TravelAgency agency;
//    agency.readFile();
//    return 0;
//}
#include "travelagencyui.h"
#include <QApplication>

int main( int argc, char **argv ) {
    QApplication app( argc, argv );
    TravelAgency agency;
    TravelAgencyUI TravAgUI(&agency);
    TravAgUI.setWindowTitle("up and away ");
    TravAgUI.show();
    return app.exec();
}
