#ifndef MYWIDGET_H
#define MYWIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>

#include <map.h>
#include <tabledialog.h>

class MyWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
    private:
        float   zoomLevel;
        float   offsetX;
        float   offsetY;
        bool    dragging;
        QPoint  lastMousePos;

    public:
        MyWidget(QWidget *parent = nullptr);
        std::shared_ptr<Map> map;

    protected:
        void initializeGL() override;
        void resizeGL(int width, int height) override;
        void paintGL() override;
        void wheelEvent(QWheelEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;


    public slots:
        void setZoomLevel(float zoom);
        void setOffsets(float x, float y);
        void zoomIn();
        void zoomOut();
        void panLeft();
        void panRight();
        void panUp();
        void panDown();
        void changeVisibility(QString layerName, bool status);
};

#endif // MYWIDGET_H
