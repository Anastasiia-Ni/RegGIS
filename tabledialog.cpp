#include "tabledialog.h"

TableDialog::TableDialog(QWidget *parent) : QDialog(parent), tableWidget(nullptr)
{
    listWidget = new QListWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(listWidget);
    setLayout(layout);
    setWindowTitle("Layer List");

}

TableDialog::~TableDialog() {}

void TableDialog::setLayerList(const QStringList &layerNames)
{
    listWidget->clear();
    listWidget->addItems(layerNames);
}


QListWidget *TableDialog::getListWidget() const
{
    return listWidget;
}

