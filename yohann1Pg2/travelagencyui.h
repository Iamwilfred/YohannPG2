#ifndef TRAVELAGENCYUI_H
#define TRAVELAGENCYUI_H

#include <memory>
#include <vector>
#include <QMainWindow>

class QLineEdit;
class QTableWidget;
class QGroupBox;
class QAction;
class QWebEngineView;
class QSplitter;

class TravelAgency;
class Customer;
class Travel;
class Booking;

using CustomerPtr = std::shared_ptr<Customer>;
using TravelPtr   = std::shared_ptr<Travel>;
using BookingPtr  = std::shared_ptr<Booking>;

class TravelAgencyUI : public QMainWindow
{
    Q_OBJECT
public:
    explicit TravelAgencyUI(TravelAgency *agency, QWidget *parent=nullptr);

private slots:
    void onReadFile();
    void onSaveToFile();
    void onSearchCustomer();
    void openTravel (int row,int col);
    void openBooking(int row,int col);

private:
    /* helpers */
    void fillTravelTable (const CustomerPtr &c);
    void fillBookingTable(const TravelPtr   &t);
    void fillCoordTable  (const TravelPtr   &t);
    QString buildGeoJson (const TravelPtr   &t) const;
    void updateMap       (const TravelPtr   &t);
    void showBookingDetails(const BookingPtr &b);
    void markChanged();

    /* widgets */
    QLineEdit    *idEdit     = nullptr;
    QLineEdit    *firstEdit  = nullptr;
    QLineEdit    *lastEdit   = nullptr;
    QTableWidget *travelTable   = nullptr;
    QTableWidget *bookingTable  = nullptr;
    QTableWidget *coordTable_   = nullptr;
    QWebEngineView *mapView_    = nullptr;
    QGroupBox    *grpTravels = nullptr;
    QGroupBox    *grpBookings= nullptr;
    QAction      *actSave    = nullptr;

    /* données courantes */
    std::vector<TravelPtr>  currentTravels;
    std::vector<BookingPtr> currentBookings;

    TravelAgency *ag;
    bool dataChanged=false;
};

#endif // TRAVELAGENCYUI_H
