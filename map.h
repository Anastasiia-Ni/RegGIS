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
        Map();
        ~Map();

        double minX, maxX, minY, maxY;

        void loadStyle(QString filePath);
        void loadMap(QString layersDir);

        QVector<QString> getLayersNames() const;

        QVector<iPoint>& getPointLayers();
        QVector<iLine>& getLineLayers();
        QVector<iPolygon>& getPolygonLayers();
        QVector<iPolygon>& getBorderLayers();
        QString getCurrentPath() const;
        QString getLayersDir() const;

    private:
        QString m_currentPath;
        QString m_layersDir;

        std::shared_ptr<layersInfo> layersStyle;

        QVector<iPoint> m_pointLayers;
        QVector<iLine> m_lineLayers;
        QVector<iPolygon> m_poligonLayers;
        QVector<iPolygon> m_borderLayers;

        void layaerDistribution(OGRLayer* ogrLayer, QString layerName);
        void loadPoint(OGRLayer* layer, const QString &name);
        void loadLine(OGRLayer* layer, const QString &name);
        void loadPolygon(OGRLayer* layer, const QString &name, bool isCovered);
        void loadMultiPolygon(OGRLayer* layer, const QString &name, bool isCovered); 
};

#endif // MAP_H
