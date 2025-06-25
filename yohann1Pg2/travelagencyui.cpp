#include "travelagencyui.h"

/* Qt ----------------------------------------------------------------*/
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QIcon>
#include <QListWidget>
#include <QLabel>
#include <QTabWidget>
#include <QSplitter>
#include <QWebEngineView>
#include <QTableWidget>
#include <QTableWidgetItem>
/* STL / helpers -----------------------------------------------------*/
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>

/* métier ------------------------------------------------------------*/
#include "travelagency.h"
#include "customer.h"
#include "travel.h"
#include "booking.h"
#include "flightbooking.h"
#include "hotelbooking.h"
#include "rentalcarreservation.h"
#include "trainticket.h"

using json = nlohmann::json;

/*───────────────────────────────────────────────────────────────────*/
TravelAgencyUI::TravelAgencyUI(TravelAgency *agency, QWidget *parent)
    : QMainWindow(parent), ag(agency)
{
    /* actions ------------------------------------------------------*/
    auto *actRead = new QAction("Lire JSON…", this);
    actRead->setShortcut(QKeySequence("Ctrl+I"));

    actSave = new QAction("Enregistrer…", this);
    actSave->setShortcut(QKeySequence("Ctrl+S"));
    actSave->setEnabled(false);

    auto *actSearch = new QAction("Chercher client…", this);
    actSearch->setShortcut(QKeySequence("Ctrl+F"));

    /* menus / toolbar ---------------------------------------------*/
    menuBar()->addMenu("Fichier")->addActions({actRead, actSave});
    menuBar()->addMenu("Client")->addAction(actSearch);

    auto *tb = addToolBar("Main");
    tb->addActions({actRead, actSave, actSearch});
    statusBar();

    connect(actRead ,&QAction::triggered,this,&TravelAgencyUI::onReadFile);
    connect(actSave ,&QAction::triggered,this,&TravelAgencyUI::onSaveToFile);
    connect(actSearch,&QAction::triggered,this,&TravelAgencyUI::onSearchCustomer);

    /* zone centrale ------------------------------------------------*/
    QWidget *cw = new QWidget(this);
    setCentralWidget(cw);
    auto *lay = new QVBoxLayout(cw);

    /* (a) infos client --------------------------------------------*/
    auto *info = new QFormLayout;
    idEdit    = new QLineEdit; idEdit   ->setReadOnly(true);
    firstEdit = new QLineEdit; firstEdit->setReadOnly(true);
    lastEdit  = new QLineEdit; lastEdit ->setReadOnly(true);
    info->addRow("ID :",      idEdit);
    info->addRow("Prénom :",  firstEdit);
    info->addRow("Nom :",     lastEdit);
    lay->addLayout(info);

    /* (b) tableau voyages ----------------------------------------*/
    travelTable = new QTableWidget(0,3,this);
    travelTable->setHorizontalHeaderLabels({"Reise-ID","Début","Fin"});
    travelTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    travelTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    travelTable->horizontalHeader()->setStretchLastSection(true);

    grpTravels = new QGroupBox("Reisen");
    auto *g1 = new QVBoxLayout(grpTravels);
    g1->addWidget(travelTable);
    lay->addWidget(grpTravels);

    /* (c) bookings + carte + tableau GPS --------------------------*/
    bookingTable = new QTableWidget(0,4,this);
    bookingTable->setHorizontalHeaderLabels({"","Début","Fin","Prix (€)"});
    bookingTable->verticalHeader()->setVisible(false);
    bookingTable->horizontalHeader()->setStretchLastSection(true);
    bookingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* nouveau : carte + tableau coords */
    mapView_ = new QWebEngineView;
    mapView_->load(QUrl("qrc:/map.html"));        // voir ressource Qt

    coordTable_ = new QTableWidget(0,4,this);
    coordTable_->setHorizontalHeaderLabels({"Lat","Lon","Label","Type"});
    coordTable_->verticalHeader()->setVisible(false);
    coordTable_->horizontalHeader()->setStretchLastSection(true);
    coordTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSplitter *split = new QSplitter(Qt::Horizontal);
    split->addWidget(bookingTable);
    split->addWidget(mapView_);
    split->setStretchFactor(1,1);

    grpBookings = new QGroupBox("Buchungen / Carte");
    auto *g2 = new QVBoxLayout(grpBookings);
    g2->addWidget(split);
    g2->addWidget(coordTable_);
    lay->addWidget(grpBookings);

    connect(travelTable ,&QTableWidget::cellDoubleClicked,this,&TravelAgencyUI::openTravel);
    connect(bookingTable,&QTableWidget::cellDoubleClicked,this,&TravelAgencyUI::openBooking);
}

/*───────────────────────────────────────────────────────────────────*/
void TravelAgencyUI::onReadFile()
{
    QString fn = QFileDialog::getOpenFileName(this,"Sélectionner bookings.json",{},
                                              "JSON (*.json)");
    if (fn.isEmpty()) return;

    try{
        QString sum = QString::fromStdString(ag->readFile(fn.toStdString()));

        /* IATA – chemin absolu ou voisin du JSON */
        QString iata = "/home/feyem-tadaha-wilfred/Bilder/iatacodes.json";
        ag->loadAirports(iata.toStdString());

        QMessageBox::information(this,"Succès", sum+"\nCodes IATA chargés.");
        statusBar()->showMessage("Fichiers chargés",4000);

        travelTable ->setRowCount(0);
        bookingTable->setRowCount(0);
        coordTable_ ->setRowCount(0);
        currentTravels.clear();
        currentBookings.clear();
        dataChanged=false;
        actSave->setEnabled(false);
    }
    catch(const std::exception& e){
        QMessageBox::critical(this,"Erreur",e.what());
    }
}

/*───────────────────────────────────────────────────────────────────*/
void TravelAgencyUI::onSaveToFile()
{
    QString fn = QFileDialog::getSaveFileName(this,"Enregistrer sous",{},
                                              "JSON (*.json)");
    if (fn.isEmpty()) return;
    try{
        ag->writeFile(fn.toStdString());
        QMessageBox::information(this,"Succès","Données sauvegardées.");
        statusBar()->showMessage("Sauvegarde OK",4000);
        dataChanged=false; actSave->setEnabled(false); setWindowModified(false);
    }
    catch(const std::exception& e){ QMessageBox::critical(this,"Erreur",e.what()); }
}

/*───────────────────────────────────────────────────────────────────*/
void TravelAgencyUI::onSearchCustomer()
{
    bool ok=false;
    long id = QInputDialog::getInt(this,"Recherche","ID client :",1,1,10'000'000,1,&ok);
    if (!ok) return;

    CustomerPtr c = ag->findCustomer(id);
    if (!c){
        QMessageBox::warning(this,"Introuvable",
                             QString("Client %1 inconnu").arg(id));
        return;
    }
    fillTravelTable(c);
}

/*───────────────────────────────────────────────────────────────────*/
void TravelAgencyUI::fillTravelTable(const CustomerPtr &c)
{
    idEdit->setText(QString::number(c->getId()));
    firstEdit->setText(QString::fromStdString(c->getFirstName()));
    lastEdit ->setText(QString::fromStdString(c->getLastName()));

    currentTravels = c->getTravels();
    travelTable->setRowCount(int(currentTravels.size()));

    for(int i=0;i<int(currentTravels.size());++i){
        TravelPtr t = currentTravels[i];

        std::string dmin="99999999", dmax="00000000";
        for(const BookingPtr &b : t->getBookings()){
            dmin = std::min(dmin,b->getFromDate());
            dmax = std::max(dmax,b->getToDate());
        }
        travelTable->setItem(i,0,new QTableWidgetItem(QString::number(t->getId())));
        travelTable->setItem(i,1,new QTableWidgetItem(QString::fromStdString(dmin)));
        travelTable->setItem(i,2,new QTableWidgetItem(QString::fromStdString(dmax)));
    }
    bookingTable->setRowCount(0);
    coordTable_ ->setRowCount(0);
}

/*───────────────────────────────────────────────────────────────────*/
void TravelAgencyUI::openTravel(int row,int)
{
    if(row<0 || row>=int(currentTravels.size())) return;
    fillBookingTable(currentTravels[row]);
}

/*───────────────────────────────────────────────────────────────────*/
void TravelAgencyUI::fillBookingTable(const TravelPtr &t)
{
    currentBookings = t->getBookings();
    bookingTable->setRowCount(int(currentBookings.size()));

    for(int r=0;r<int(currentBookings.size());++r){
        BookingPtr b = currentBookings[r];

        auto *icon = new QTableWidgetItem;
        if(std::dynamic_pointer_cast<FlightBooking>(b))
            icon->setIcon(QIcon(":/res/plane.png"));
        else if(std::dynamic_pointer_cast<HotelBooking>(b))
            icon->setIcon(QIcon(":/res/hotel.png"));
        else if(std::dynamic_pointer_cast<RentalCarReservation>(b))
            icon->setIcon(QIcon(":/res/car.png"));
        else if(std::dynamic_pointer_cast<TrainTicket>(b))
            icon->setIcon(QIcon(":/res/train.png"));

        bookingTable->setItem(r,0,icon);
        bookingTable->setItem(r,1,new QTableWidgetItem(
                                  QString::fromStdString(b->getFromDate())));
        bookingTable->setItem(r,2,new QTableWidgetItem(
                                  QString::fromStdString(b->getToDate())));

        std::ostringstream os; os<<std::fixed<<std::setprecision(2)<<b->getPrice();
        bookingTable->setItem(r,3,new QTableWidgetItem(QString::fromStdString(os.str())));
    }

    fillCoordTable(t);
    updateMap(t);
}

/*───────────────────────────────────────────────────────────────────*/
void TravelAgencyUI::fillCoordTable(const TravelPtr &t)
{
    coordTable_->setRowCount(0);

    auto addRow=[&](double lat,double lon,const QString &lab,const QString &typ){
        int r=coordTable_->rowCount(); coordTable_->insertRow(r);
        coordTable_->setItem(r,0,new QTableWidgetItem(QString::number(lat,'f',6)));
        coordTable_->setItem(r,1,new QTableWidgetItem(QString::number(lon,'f',6)));
        coordTable_->setItem(r,2,new QTableWidgetItem(lab));
        coordTable_->setItem(r,3,new QTableWidgetItem(typ));
    };

    for(const BookingPtr &b : t->getBookings()){
        if(auto f=std::dynamic_pointer_cast<FlightBooking>(b)){
            addRow(f->getFromDestLatitude(),f->getFromDestLongitude(),
                   QString::fromStdString(f->getFrom()),"Aéroport");
            addRow(f->getToDestLatitude()
                   ,f->getToDestLongitude(),
                   QString::fromStdString(f->getTo()),"Aéroport");
        }
        else if(auto h=std::dynamic_pointer_cast<HotelBooking>(b)){
            addRow(h->getHotelLatitude(),h->getHotelLongitude(),
                   QString::fromStdString(h->getHotelName()),"Hôtel");
        }
        else if(auto r=std::dynamic_pointer_cast<RentalCarReservation>(b)){
            addRow(r->getPickupLatitude(),r->getPickupLongitude(),
                   QString::fromStdString(r->getPickupLocation()),"Pick-up");
            if(std::abs(r->getPickupLatitude()-r->getReturnLatitude())>1e-6 ||
               std::abs(r->getPickupLongitude()-r->getReturnLongitude())>1e-6)
                addRow(r->getReturnLatitude(),r->getReturnLongitude(),
                       QString::fromStdString(r->getReturnLocation()),"Return");
        }
        else if(auto tr=std::dynamic_pointer_cast<TrainTicket>(b)){
            addRow(tr->getFromStationLatitude(),tr->getFromStationLongitude(),
                   QString::fromStdString(tr->getFrom()),"Gare");
            for(const auto &s : tr->getStopovers())
                // addRow(s.lat,s.lon,QString::fromStdString(s.name),"Stop");
            addRow(tr->getToStationLatitude(),tr->getToStationLongitude(),
                   QString::fromStdString(tr->getTo()),"Gare");
        }
    }
}

/*───────────────────────────────────────────────────────────────────*/
QString TravelAgencyUI::buildGeoJson(const TravelPtr &t) const
{
    json feats=json::array();

    for(const BookingPtr &b : t->getBookings()){
        if(auto f=std::dynamic_pointer_cast<FlightBooking>(b)){
            feats.push_back({
                {"type","Feature"},
                {"geometry",{
                    {"type","LineString"},
                    {"coordinates",{{f->getFromDestLongitude(),f->getFromDestLatitude()},
                                    {f->getToDestLongitude(),  f->getToDestLatitude()}}}}},
                {"properties",{{"label",f->getFrom()+" → "+f->getTo()}}}
            });
        }
        else if(auto h=std::dynamic_pointer_cast<HotelBooking>(b)){
            feats.push_back({
                {"type","Feature"},
                {"geometry",{{"type","Point"},
                             {"coordinates",{h->getHotelLongitude(),h->getHotelLatitude()}}}},
                {"properties",{{"label",h->getHotelName()+" ("+h->getCity()+")"}}}
            });
        }
        else if(auto r=std::dynamic_pointer_cast<RentalCarReservation>(b)){
            double la1=r->getPickupLatitude(), lo1=r->getPickupLongitude();
            double la2=r->getReturnLatitude(), lo2=r->getReturnLongitude();
            if(std::abs(la1-la2)<1e-6 && std::abs(lo1-lo2)<1e-6){
                feats.push_back({
                    {"type","Feature"},
                    {"geometry",{{"type","Point"},
                                 {"coordinates",{lo1,la1}}}},
                    {"properties",{{"label",r->getCompany()}}}
                });
            }else{
                feats.push_back({
                    {"type","Feature"},
                    {"geometry",{{"type","LineString"},
                                 {"coordinates",{{lo1,la1},{lo2,la2}}}}},
                    {"properties",{{"label",r->getCompany()}}}
                });
            }
        }
        else if(auto tr=std::dynamic_pointer_cast<TrainTicket>(b)){
            std::vector<std::array<double,2>> line{
                {tr->getFromStationLongitude(),tr->getFromStationLatitude()} };
            for(const auto &s : tr->getStopovers())
                // line.push_back({s.lon,s.lat});
            line.push_back({tr->getToStationLongitude(),tr->getToStationLatitude()});
            feats.push_back({
                {"type","Feature"},
                {"geometry",{{"type","LineString"},{"coordinates",line}}},
                {"properties",{{"label",tr->getFrom()+" → "+tr->getTo()}}}
            });
        }
    }
    json fc={{"type","FeatureCollection"},{"features",feats}};
    return QString::fromStdString(fc.dump());
}

/*───────────────────────────────────────────────────────────────────*/
void TravelAgencyUI::updateMap(const TravelPtr &t)
{
    QString geo = buildGeoJson(t);
    mapView_->page()->runJavaScript(
        QString("showData(%1);").arg(geo));
}

/*───────────────────────────────────────────────────────────────────*/
void TravelAgencyUI::openBooking(int row,int)
{
    if(row<0 || row>=int(currentBookings.size())) return;
    showBookingDetails(currentBookings[row]);
}

/*───────────────────────────────────────────────────────────────────*/
void TravelAgencyUI::showBookingDetails(const BookingPtr& b)
{
    QDialog dlg(this);
    dlg.setWindowTitle("Détails de la réservation");

    /* layout racine ---------------------------------------------*/
    auto *root = new QVBoxLayout(&dlg);

    /* (1) champs communs éditables ------------------------------*/
    QLineEdit *priceEdit,*fromEdit,*toEdit;
    {
        auto *common = new QFormLayout;
        priceEdit = new QLineEdit(QString::number(b->getPrice(),'f',2));
        fromEdit  = new QLineEdit(QString::fromStdString(b->getFromDate()));
        toEdit    = new QLineEdit(QString::fromStdString(b->getToDate()));
        common->addRow("Prix (€) :",priceEdit);
        common->addRow("Du :",       fromEdit);
        common->addRow("Au :",       toEdit);
        root->addLayout(common);
    }

    /* (2) onglets spécifiques -----------------------------------*/
    auto *tabs = new QTabWidget;
    tabs->setIconSize(QSize(20,20));
    root->addWidget(tabs);

    /* ---- vol ---- */
    if (auto f = std::dynamic_pointer_cast<FlightBooking>(b)){
        QWidget *page=new QWidget;
        auto *form=new QFormLayout(page);
        form->addRow("De :",new QLabel(QString::fromStdString(f->getFrom())));
        form->addRow("À :", new QLabel(QString::fromStdString(f->getTo())));
        form->addRow("Compagnie :",new QLabel(QString::fromStdString(f->getAirline())));
        form->addRow("Classe :",new QLabel(QString::fromStdString(f->bookingClassText())));
        tabs->addTab(page,QIcon("/media/sf_yohann/PG2/bild/myflug.jpeg"),"Vol");
    }
    /* ---- hotel ---- */
    else if (auto h = std::dynamic_pointer_cast<HotelBooking>(b)){
        QWidget *page=new QWidget;
        auto *form=new QFormLayout(page);
        form->addRow("Hôtel :",new QLabel(QString::fromStdString(h->getHotelName())));
        form->addRow("Ville :",new QLabel(QString::fromStdString(h->getCity())));
        form->addRow("Type :",new QLabel(QString::fromStdString(h->roomTypeText())));
        tabs->addTab(page,QIcon("/media/sf_yohann/PG2/bild/hotel.png"),"Hôtel");
    }
    /* ---- voiture ---- */
    else if (auto r = std::dynamic_pointer_cast<RentalCarReservation>(b)){
        QWidget *page=new QWidget;
        auto *form=new QFormLayout(page);
        form->addRow("Agence :",new QLabel(QString::fromStdString(r->getCompany())));
        form->addRow("Catégorie :",new QLabel(QString::fromStdString(r->getVehicleClass())));
        form->addRow("Retrait :",new QLabel(QString::fromStdString(r->getPickupLocation())));
        form->addRow("Retour :",new QLabel(QString::fromStdString(r->getReturnLocation())));
        tabs->addTab(page,QIcon("/media/sf_yohann/PG2/bild/mycar.jpeg"),"Voiture");
    }
    /* ---- train ---- */
    else if (auto t = std::dynamic_pointer_cast<TrainTicket>(b)){
        QWidget *page=new QWidget;
        auto *form=new QFormLayout(page);
        form->addRow("De :", new QLabel(QString::fromStdString(t->getFrom())));
        form->addRow("Départ :",new QLabel(QString::fromStdString(t->getDepartureTime())));
        form->addRow("À :", new QLabel(QString::fromStdString(t->getTo())));
        form->addRow("Arrivée :",new QLabel(QString::fromStdString(t->getArrivalTime())));
        form->addRow("Ticket :",
                     new QLabel(QString::fromStdString(
                         TrainTicket::classDescription(t->getBookingClass()))));

        auto *stops = new QListWidget;
        for (const std::string& s : t->getStopovers())
            stops->addItem(QString::fromStdString(s));
        form->addRow("Via :",stops);

        tabs->addTab(page,QIcon("/media/sf_yohann/PG2/bild/zug.jpg"),"Train");
    }

    /* (3) boutons --------------------------------------------------*/
    QPushButton *save = new QPushButton("Enregistrer");
    QPushButton *cancel= new QPushButton("Annuler");
    save->setEnabled(false);    // activé si champ édité
    auto enable=[&]{ save->setEnabled(true); };
    connect(priceEdit,&QLineEdit::textEdited,enable);
    connect(fromEdit ,&QLineEdit::textEdited,enable);
    connect(toEdit   ,&QLineEdit::textEdited,enable);

    auto *hl=new QHBoxLayout;
    hl->addWidget(save);
    hl->addWidget(cancel);
    root->addLayout(hl);

    connect(save,&QPushButton::clicked,[&]{
        b->setPrice(priceEdit->text().toDouble());
        b->setFromDate(fromEdit->text().toStdString());
        b->setToDate  (toEdit  ->text().toStdString());
        dlg.accept();
        markChanged();
        fillBookingTable(currentTravels[travelTable->currentRow()]);
    });
    connect(cancel,&QPushButton::clicked,[&]{ dlg.reject(); });

    dlg.exec();
}

/*───────────────────────────────────────────────────────────────────*/
void TravelAgencyUI::markChanged()
{
    dataChanged=true;
    actSave->setEnabled(true);
    setWindowModified(true);
}
