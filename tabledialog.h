#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QFile>
#include <QFileInfo>
#include <QString>

// This class inherits QDialog and provides functionality to display a list of layers.
// It allows navigation to another window for viewing layer tables.

class TableDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TableDialog(QWidget *parent = nullptr);
    ~TableDialog();
    void setLayerList(const QStringList &layerList);
    QListWidget *getListWidget() const;

private:
    QListWidget *listWidget;
    QTableWidget *tableWidget;

};

#endif // TABLEDIALOG_H
