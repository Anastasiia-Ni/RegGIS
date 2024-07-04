#include "map.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"

Map::Map() {
    m_currentPath = QString(PROJECT_ROOT_DIR);

    minX = 180.0f;
    maxX = 0.0f;
    minY = 180.0f;
    maxY = 0.0f;
}

void Map::loadStyle(QString filePath) {

    QFile file(m_currentPath + filePath);

    if (!file.exists()) {
        qDebug() << "File does not exist:" << filePath;
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "File is not readable:" << filePath;
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    layersStyle = std::make_shared<layersInfo>();
    layersStyle->setInfo(fileData);

}

void Map::loadMap(QString layersDir){
    GDALAllRegister();

    m_layersDir = layersDir;
    QDir directory(m_currentPath + m_layersDir);
    QStringList filters;
    filters << "*.shp";
    directory.setNameFilters(filters);

    QFileInfoList fileList = directory.entryInfoList();

    for (const QFileInfo &fileInfo : fileList) {
        QString filePath = fileInfo.absoluteFilePath();
        QString layerName = fileInfo.completeBaseName();

        if (!fileInfo.exists() || !fileInfo.isFile()) {
            qDebug() << "File does not exist or is not a file:" << layerName;
            continue;
        }
        if (!fileInfo.isReadable()) {
            qDebug() << "File is not readable:" << layerName;
            continue;
        }

        GDALDataset *dataset = static_cast<GDALDataset*>(GDALOpenEx(filePath.toStdString().c_str(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr));

        if (!dataset) {
            qDebug() << "Failed to open file: " + layerName + ".shp";
            continue;
        }

        OGRLayer* ogrLayer = dataset->GetLayer(0);
        layaerDistribution(ogrLayer, layerName);

        GDALClose(dataset);
    }

}

void Map::layaerDistribution(OGRLayer* ogrLayer, QString layerName) {

    OGRFeature* feature;
    ogrLayer->ResetReading();

    if ((feature = ogrLayer->GetNextFeature()) != nullptr) {
        OGRGeometry* geometry = feature->GetGeometryRef();
        QString layerType;
        ogrLayer->ResetReading();

        if (!geometry) {
            return;
        }

        OGRwkbGeometryType type = wkbFlatten(geometry->getGeometryType());
        layerType = OGRGeometryTypeToName(type);

        if (layerType == "Point") {
            loadPoint(ogrLayer, layerName);
        }
        else if (layerType == "Polygon") {
            if (layerName.contains("border", Qt::CaseInsensitive)) {
                loadPolygon(ogrLayer, layerName, false);
            }
            else {
                loadPolygon(ogrLayer, layerName, true);
            }
        }
        else if (layerType == "Multi Polygon") {
            if (layerName.contains("border", Qt::CaseInsensitive)) {
                loadMultiPolygon(ogrLayer, layerName, false);
            }
            else {
                loadMultiPolygon(ogrLayer, layerName, true);
            }
        }
        else if (layerType == "Line String") {
            loadLine(ogrLayer, layerName);
        }
        else {
            qDebug() << "Layer type: " << layerType << " is unknown.";
        }
    }
}


void Map::loadPoint(OGRLayer* layer, const QString &name) {
    iPoint points;
    OGRFeature *feature;

    while ((feature = layer->GetNextFeature()) != nullptr) {
        OGRGeometry *geometry = feature->GetGeometryRef();
        if (geometry && wkbFlatten(geometry->getGeometryType()) == wkbPoint) {
            OGRPoint *ogrPoint = static_cast<OGRPoint*>(geometry);
            points.addPoint(Point(ogrPoint->getX(), ogrPoint->getY()));
        }
        OGRFeature::DestroyFeature(feature);
    }

    points.name = name;
    lInfo info = layersStyle->getInfo(name);
    points.color = info.color;
    points.color.setAlpha(info.alpha);
    points.pointSize = info.pointSize ? info.pointSize : 5.0f;
    points.isVisible = true;
    m_pointLayers.push_back(points);
}


void Map::loadLine(OGRLayer* layer, const QString &name) {
    iLine lineObj;
    OGRFeature *feature;

    while ((feature = layer->GetNextFeature()) != nullptr) {
        OGRGeometry *geometry = feature->GetGeometryRef();
        if (geometry && wkbFlatten(geometry->getGeometryType()) == wkbLineString) {
            OGRLineString *ogrLine = static_cast<OGRLineString*>(geometry);
            QVector<Point> line;
            for (int i = 0; i < ogrLine->getNumPoints(); i++) {
                line.append(Point(ogrLine->getX(i), ogrLine->getY(i)));
            }
            lineObj.addLine(line);
        }
        OGRFeature::DestroyFeature(feature);
    }

    lineObj.name = name;
    lInfo info = layersStyle->getInfo(name);
    lineObj.color = info.color;
    lineObj.color.setAlpha(info.alpha);
    lineObj.lineWidth = info.lineWidth ? info.lineWidth : 1.0f;
    lineObj.isVisible = true;

    m_lineLayers.push_back(lineObj);
}


void Map::loadPolygon(OGRLayer* layer, const QString &name, bool isCovered) {
    iPolygon polygonObj;
    OGRFeature *feature;

    while ((feature = layer->GetNextFeature()) != nullptr) {
        OGRGeometry *geometry = feature->GetGeometryRef();
        if (geometry && wkbFlatten(geometry->getGeometryType()) == wkbPolygon) {
            OGRPolygon *ogrPolygon = static_cast<OGRPolygon*>(geometry);
            QVector<Point> polygon;
            OGRLinearRing *externalRing = ogrPolygon->getExteriorRing();
            if (externalRing != nullptr) {
                for (int i = 0; i < externalRing->getNumPoints(); i++) {
                    double x = externalRing->getX(i);
                    double y = externalRing->getY(i);
                    polygon.append(Point(x, y));

                    if (!isCovered) {
                        if (x < minX) minX = x;
                        if (x > maxX) maxX = x;
                        if (y < minY) minY = y;
                        if (y > maxY) maxY = y;
                    }
                }
                polygonObj.addPolygon(polygon);
            }
        }
        OGRFeature::DestroyFeature(feature);
    }

    polygonObj.name = name;
    lInfo info = layersStyle->getInfo(name);
    polygonObj.color = info.color;
    polygonObj.color.setAlpha(info.alpha);
    polygonObj.lineWidth = info.lineWidth ? info.lineWidth : 1.0f;
    polygonObj.isVisible = true;
    polygonObj.isMulti = false;

    if (isCovered) {
        m_poligonLayers.push_back(polygonObj);
    }
    else {
        m_borderLayers.push_back(polygonObj);
    }
}


void Map::loadMultiPolygon(OGRLayer* layer, const QString &name, bool isCovered) {
    iPolygon polygonObj;
    OGRFeature *feature;

    while ((feature = layer->GetNextFeature()) != nullptr) {
        OGRGeometry *geometry = feature->GetGeometryRef();
        if (geometry && wkbFlatten(geometry->getGeometryType()) == wkbMultiPolygon) {
            OGRMultiPolygon *multiPolygon = static_cast<OGRMultiPolygon*>(geometry);

            for (int i = 0; i < multiPolygon->getNumGeometries(); i++) {
                OGRPolygon *ogrPolygon = static_cast<OGRPolygon*>(multiPolygon->getGeometryRef(i));
                QVector<Point> polygon;
                OGRLinearRing *externalRing = ogrPolygon->getExteriorRing();
                if (externalRing != nullptr) {
                    for (int j = 0; j < externalRing->getNumPoints(); j++) {
                        polygon.append(Point(externalRing->getX(j), externalRing->getY(j)));
                    }
                    polygonObj.addPolygon(polygon);
                }

                for (int j = 0; j < ogrPolygon->getNumInteriorRings(); j++) {
                    QVector<Point> hole;
                    OGRLinearRing *interiorRing = ogrPolygon->getInteriorRing(j);
                    for (int k = 0; k < interiorRing->getNumPoints(); k++) {
                        hole.append(Point(interiorRing->getX(k), interiorRing->getY(k)));
                    }
                    polygonObj.addPolygon(hole);
                }
            }
        }
        OGRFeature::DestroyFeature(feature);
    }

    polygonObj.name = name;
    lInfo info = layersStyle->getInfo(name);
    polygonObj.color = info.color;
    polygonObj.color.setAlpha(info.alpha);
    polygonObj.lineWidth = info.lineWidth ? info.lineWidth : 1.0f;
    polygonObj.isVisible = true;
    polygonObj.isMulti = true;
    if (isCovered) {
        m_poligonLayers.push_back(polygonObj);
    }
    else {
        m_borderLayers.push_back(polygonObj);
    }
}


QVector<QString> Map::getLayersNames() {
    QVector<QString> layersNames;

    for (iPoint layer: m_pointLayers) {
        layersNames.push_back(layer.name);
    }

    for (iLine layer: m_lineLayers) {
        layersNames.push_back(layer.name);
    }

    for (iPolygon layer: m_poligonLayers) {
        layersNames.push_back(layer.name);
    }

    for (iPolygon layer: m_borderLayers) {
        layersNames.push_back(layer.name);
    }

    return layersNames;
}
