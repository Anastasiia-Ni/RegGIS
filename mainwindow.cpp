#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myOpenGLWidget = new MyWidget(this);
    setCentralWidget(myOpenGLWidget);

    tableDialog = new TableDialog(this);
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections() {
    connect(ui->checkBoxBordersRegion, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxMunicipalBorders, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxMunicipalDistrictsBorders, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxSettlementsBorders, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxLakes, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxRivers, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxHealthcareFacilities, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxEducationalInstitutions, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxSchools, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxKindergartens, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxAirports, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxLandingSites, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxPortsPiers, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxRoads, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxRailways, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxRailwayStations, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxBorderCheckpoints, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxPowerLines, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxElectricityGeneration, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxElectricDistributionSubstations, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxNationalParksBorders, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxTouristRoutes, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxMonuments, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxCampings, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxLandPlotsOfTOSED, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxInvestmentProjects, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));
    connect(ui->checkBoxAllowablePlacementZones, SIGNAL(stateChanged(int)), this, SLOT(on_checkBoxState_changed(int)));

    connect(tableDialog->getListWidget(), &QListWidget::itemDoubleClicked, this, &MainWindow::onLayerItemDoubleClicked);
}

void MainWindow::on_actionZoomIn_triggered()
{
    myOpenGLWidget->zoomIn();
}


void MainWindow::on_actionZoomOut_triggered()
{
    myOpenGLWidget->zoomOut();
}


void MainWindow::on_actionZoomFullExtent_triggered()
{
    myOpenGLWidget->setZoomLevel(1.0f);
    myOpenGLWidget->setOffsets(0.0f, 0.0f);
}


void MainWindow::on_actionQuit_triggered()
{
    qDebug() << "Application closed";
    QApplication::quit();
}


void MainWindow::on_checkBoxState_changed(int state)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    if (checkBox) {
        QString name = checkBox->objectName();
        myOpenGLWidget->changeVisibility(name.section("checkBox", 1), state);
    }
}


void MainWindow::on_actionOpenTable_triggered()
{
    QStringList layerNames;
    QVector<QString> listLayers = myOpenGLWidget->map->getLayersNames();

    for (const QString &layer : listLayers) {
        layerNames << formatLayerName(layer);
    }
    tableDialog->setLayerList(layerNames);
    tableDialog->show();
}

QString MainWindow::formatLayerName(QString layerName) {
    const QRegularExpression re("(?<!^)(?=[A-Z])");
    layerName = layerName.replace(re, " ");
    layerName.replace("T O S E D", "TOSED");
    return layerName;
}

void MainWindow::onLayerItemDoubleClicked(QListWidgetItem *item)
{
    QString layerName = item->text();
    layerName.remove(" ");
    QString fullPath = myOpenGLWidget->map->getCurrentPath() + myOpenGLWidget->map->getLayersDir();
    LayerDataWindow *layerDataWindow = new LayerDataWindow(fullPath, layerName, this);
    layerDataWindow->setAttribute(Qt::WA_DeleteOnClose);
    layerDataWindow->show();

}
