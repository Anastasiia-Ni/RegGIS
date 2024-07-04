#include "mainwindow.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::shared_ptr<Map> map = std::make_shared<Map>();
    QString layersDir = "/layers/";
    QString styleFile = "/add/Layers.json";
    map->loadStyle(styleFile);
    map->loadMap(layersDir);

    MainWindow w;
    w.setMap(map);
    w.setWindowTitle("Territory of Advanced Social and Economic Development (TOSED)");
    w.show();

    return a.exec();
}
