#include "layersinfo.h"

layersInfo::layersInfo() {}


// Parses the provided JSON data to populate layer information such as color,
// transparency, point size, and line width into layersStyles map.
void layersInfo::setInfo(QByteArray &fileData) {

    QJsonDocument document = QJsonDocument::fromJson(fileData);
    if (document.isNull()) {
        qDebug() << "Failed to create QJsonDocument.";
        return;
    }

    QJsonObject jsonObject = document.object();
    QJsonArray layersArray = jsonObject["layers"].toArray();

    for (const QJsonValue& value : layersArray) {
        QJsonObject layerObject = value.toObject();

        lInfo info;
        QString name = layerObject["name"].toString();

        info.color = QColor(layerObject["color"].toString());
        info.alpha = layerObject["alpha"].toString().toInt();
        info.pointSize = layerObject["size"].toString().toFloat();
        info.lineWidth = layerObject["width"].toString().toFloat();
        layersStyles[name] = info;
    }
}


// Searches the layersStyles map for information associated with
// the specified layer name
const lInfo layersInfo::getInfo(const QString &layerName) {

    auto it = layersStyles.find(layerName);
    if (it != layersStyles.end()) {
        return it.value();
    }
    return lInfo();
}
