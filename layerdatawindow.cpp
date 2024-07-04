#include "layerdatawindow.h"

// Constructs a LayerDataWindow object to display layer data from a DBF file.
// This constructor initializes the dialog with data extracted from the specified DBF file.
// It sets up a QTableWidget to display the attribute table of the layer.

LayerDataWindow::LayerDataWindow(const QString &curPath, const QString &layerName, QWidget *parent)
    : QDialog(parent), tableWidget(nullptr)
{
    // Construct the full file path
    QString fileName = layerName + ".dbf";
    QString fullPath = curPath + fileName;

    // Open the GDAL dataset
    GDALAllRegister();
    GDALDataset *poDS = (GDALDataset *) GDALOpenEx(fullPath.toStdString().c_str(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    if (poDS == nullptr) {
        qDebug() << "Failed to open file:" << fileName;
        return;
    }

    // Get the layer from the dataset
    OGRLayer *poLayer = poDS->GetLayer(0);
    if (poLayer == nullptr) {
        qDebug() << "Failed to get layer from file:" << fileName;
        GDALClose(poDS);
        return;
    }

    // Reset reading and get the first feature
    poLayer->ResetReading();
    OGRFeature *poFeature = poLayer->GetNextFeature();
    if (poFeature == nullptr) {
        qDebug() << "No features in layer:" << fileName;
        GDALClose(poDS);
        return;
    }

    // Get feature definition to extract field information
    OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
    int columnCount = poFDefn->GetFieldCount();

    // Create new QTableWidget and set layout
    if (tableWidget != nullptr) {
        delete tableWidget;
    }
    tableWidget = new QTableWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    setLayout(layout);

    // Set column count and headers
    tableWidget->setColumnCount(columnCount);
    QStringList headers;
    for (int i = 0; i < columnCount; ++i) {
        headers << poFDefn->GetFieldDefn(i)->GetNameRef();
    }
    tableWidget->setHorizontalHeaderLabels(headers);

    // Populate tableWidget with data from features
    int row = 0;
    while (poFeature != nullptr) {
        tableWidget->insertRow(row);
        for (int col = 0; col < columnCount; ++col) {
            QString value = poFeature->GetFieldAsString(col);
            QTableWidgetItem *item = new QTableWidgetItem(value);
            item->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(row, col, item);
        }
        ++row;
        OGRFeature::DestroyFeature(poFeature);
        poFeature = poLayer->GetNextFeature();
    }

    GDALClose(poDS);

    // Configure tableWidget properties
    tableWidget->setSortingEnabled(true);
    tableWidget->resizeColumnsToContents();
    tableWidget->resizeRowsToContents();
    tableWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color:lightgray }");
    tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section { background-color:lightgray }");

    setWindowTitle(layerName + " Data");
    resize(800, 600);
}

LayerDataWindow::~LayerDataWindow()
{
    delete tableWidget;
}
