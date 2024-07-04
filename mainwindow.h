#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QHeaderView>

#include "mywidget.h"
#include "map.h"
#include "tabledialog.h"
#include "layerdatawindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setMap(std::shared_ptr<Map> map) { myOpenGLWidget->map = map; }

private slots:
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionZoomFullExtent_triggered();
    void on_actionQuit_triggered();
    void on_checkBoxState_changed(int state);
    void on_actionOpenTable_triggered();
    void onLayerItemDoubleClicked(QListWidgetItem *item);

private:
    void handleLayerStateChanged(int state, QString layerName);
    void setupConnections();
    QString formatLayerName(QString layerName);

    Ui::MainWindow *ui;
    MyWidget *myOpenGLWidget;
    TableDialog *tableDialog;
    QTableWidget *tableWidget;
};
#endif // MAINWINDOW_H
