#include "layersinfo.h"

layersInfo::layersInfo() {}

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

const lInfo layersInfo::getInfo(const QString &layerName) {

    auto it = layersStyles.find(layerName);
    if (it != layersStyles.end()) {
        return it.value();
    }
    return lInfo();
}
