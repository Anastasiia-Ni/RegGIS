#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QFile>
#include <QFileInfo>
#include <QString>

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

signals:
};

#endif // TABLEDIALOG_H
