#ifndef LAYERSINFO_H
#define LAYERSINFO_H

#include <QString>
#include <QMap>
#include <QColor>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>


// Structure to hold visual information for a layer.
// This structure stores color, transparency (alpha), point size, and line width
// information for a layer.

struct lInfo {
    QColor  color;
    int     alpha;
    float   pointSize;
    float   lineWidth;
};


// This class manages and provides access to visual styles and information
// for multiple layers.
class layersInfo
{
    public:
        layersInfo();

        void setInfo(QByteArray &fileData);
        const lInfo getInfo(const QString &layerName);

    private:
        QMap<QString, lInfo> layersStyles;  //Map storing layer names and their associated visual information

};

#endif // LAYERSINFO_H
