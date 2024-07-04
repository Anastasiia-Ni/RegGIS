#ifndef ILAYER_H
#define ILAYER_H

#include <QVector>
#include <QColor>

// Structure representing a point with coordinates.
struct Point {
    double x;
    double y;

    Point(double x, double y) : x(x), y(y) {}
};


// Structure representing a layer of points.
struct iPoint {
    QVector<Point> points;
    QColor  color;
    QString name;
    float   pointSize;
    bool    isVisible;

    void addPoint(const Point& point) {
        points.append(point);
    }

    int size() {
        return points.size();
    }
};

// Structure representing a layer of lines.
struct iLine {
    QVector<QVector<Point>> lines;
    QColor  color;
    QString name;
    float   lineWidth;
    bool    isVisible;

    void addLine(const QVector<Point>& line) {
        lines.append(line);
    }

    int size() {
        return lines.size();
    }
};


// Structure representing a layer of polygons.
struct iPolygon {
    QVector<QVector<Point>> polygons;
    QColor  color;
    QString name;
    float   lineWidth;
    bool    isMulti;
    bool    isVisible;

    void addPolygon(const QVector<Point>& polygon) {
        polygons.append(polygon);
    }

    int size() {
        return polygons.size();
    }
};

#endif // ILAYER_H
