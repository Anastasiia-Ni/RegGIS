#ifndef MAP_H
#define MAP_H

#include <QTextStream>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "ogrsf_frmts.h"
#include "ilayer.h"
#include "layersinfo.h"


// This class manages spatial boundaries, layer counts, paths, styles, and collections
// of layers for points, lines, polygons, and borders.
class Map
{
    public:
        double minX, maxX, minY, maxY;
        // int m_layerCount;
        QString m_currentPath;
        QString m_layersDir;

        std::shared_ptr<layersInfo> layersStyle;

        QVector<iPoint> m_pointLayers;
        QVector<iLine> m_lineLayers;
        QVector<iPolygon> m_poligonLayers;
        QVector<iPolygon> m_borderLayers;

        Map();
        void loadStyle(QString filePath);
        void loadMap(QString layersDir);
        void layaerDistribution(OGRLayer* ogrLayer, QString layerName);
        void loadPoint(OGRLayer* layer, const QString &name);
        void loadLine(OGRLayer* layer, const QString &name);
        void loadPolygon(OGRLayer* layer, const QString &name, bool isCovered);
        void loadMultiPolygon(OGRLayer* layer, const QString &name, bool isCovered);
        QVector<QString> getLayersNames();
};

#endif // MAP_H
