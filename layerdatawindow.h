#ifndef LAYERDATAWINDOW_H
#define LAYERDATAWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>

#include <gdal.h>
#include <ogrsf_frmts.h>



// Dialog window for displaying layer data in a table.
// This class represents a dialog window used to display layer data in a table format.

class LayerDataWindow : public QDialog
{
    Q_OBJECT
public:
    explicit LayerDataWindow(const QString &curPath, const QString &layerName, QWidget *parent = nullptr);
    ~LayerDataWindow();

private:
    QTableWidget *tableWidget;
};

#endif // LAYERDATAWINDOW_H
