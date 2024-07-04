#include "layerdatawindow.h"

LayerDataWindow::LayerDataWindow(const QString &curPath, const QString &layerName, QWidget *parent) : QDialog(parent), tableWidget(nullptr)
{
    QString fileName = layerName + ".dbf";
    QString fullPath = curPath + fileName;

    GDALAllRegister();
    GDALDataset *poDS = (GDALDataset *) GDALOpenEx(fullPath.toStdString().c_str(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    if (poDS == nullptr) {
        qDebug() << "Failed to open file:" << fileName;
        return;
    }

    OGRLayer *poLayer = poDS->GetLayer(0);
    if (poLayer == nullptr) {
        qDebug() << "Failed to get layer from file:" << fileName;
        GDALClose(poDS);
        return;
    }

    poLayer->ResetReading();
    OGRFeature *poFeature = poLayer->GetNextFeature();
    if (poFeature == nullptr) {
        qDebug() << "No features in layer:" << fileName;
        GDALClose(poDS);
        return;
    }

    OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
    int columnCount = poFDefn->GetFieldCount();

    if (tableWidget != nullptr) {
        delete tableWidget;
    }

    tableWidget = new QTableWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    setLayout(layout);

    tableWidget->setColumnCount(columnCount);

    QStringList headers;
    for (int i = 0; i < columnCount; ++i) {
        headers << poFDefn->GetFieldDefn(i)->GetNameRef();
    }
    tableWidget->setHorizontalHeaderLabels(headers);

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
