#include "tabledialog.h"


// Constructor initializes the dialog with a list widget and sets the layout.
TableDialog::TableDialog(QWidget *parent) : QDialog(parent), tableWidget(nullptr)
{
    listWidget = new QListWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(listWidget);
    setLayout(layout);
    setWindowTitle("Layer List");

}

TableDialog::~TableDialog() {}


// Sets the layer names in the list widget.
void TableDialog::setLayerList(const QStringList &layerNames)
{
    listWidget->clear();
    listWidget->addItems(layerNames);
}


// Returns the pointer to the list widget.
QListWidget *TableDialog::getListWidget() const
{
    return listWidget;
}
