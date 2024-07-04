#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent) : QOpenGLWidget(parent), zoomLevel(1.0f), dragging(false) {
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

void MyWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void MyWidget::resizeGL(int width,int height){
    float aspectRatio = float(width) / float(height);
    float left = -aspectRatio * zoomLevel + offsetX;
    float right = aspectRatio * zoomLevel + offsetX;
    float bottom = -zoomLevel + offsetY;
    float top = zoomLevel + offsetY;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void MyWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    for (int i = 0; i < map->m_poligonLayers.size(); i++) {
        if (!map->m_poligonLayers[i].isVisible) continue;


        QColor &color = map->m_poligonLayers[i].color;
        glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());

        for (const auto &polygon : map->m_poligonLayers[i].polygons) {
            if (map->m_poligonLayers[i].isMulti) {
                glEnable(GL_STENCIL_TEST);
                glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
            }
            glBegin(GL_POLYGON);
            for (const auto &point : polygon) {
                float normalizedX = (point.x - map->minX) / (map->maxX - map->minX) * 2 - 1;
                float normalizedY = (point.y - map->minY) / (map->maxY - map->minY) * 2 - 1;
                glVertex2f(normalizedX * zoomLevel + offsetX, normalizedY * zoomLevel + offsetY);

            }
            glEnd();
        }
    }

    for (int i = 0; i < map->m_lineLayers.size(); i++) {
        if (!map->m_lineLayers[i].isVisible) continue;

        const auto& layer = map->m_lineLayers[i];
        QColor &color = map->m_lineLayers[i].color;
        glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        glLineWidth(map->m_lineLayers[i].lineWidth);

        for (const auto& line : layer.lines) {
            glBegin(GL_LINE_STRIP);
            for (const auto& point : line) {
                float normalizedX = (point.x - map->minX) / (map->maxX - map->minX) * 2 - 1;
                float normalizedY = (point.y - map->minY) / (map->maxY - map->minY) * 2 - 1;
                glVertex2f(normalizedX * zoomLevel + offsetX, normalizedY * zoomLevel + offsetY);
            }
            glEnd();
        }
    }

    for (int i = 0; i < map->m_borderLayers.size(); i++) {
        if (!map->m_borderLayers[i].isVisible) continue;

        QColor &color = map->m_borderLayers[i].color;
        glColor3f(color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f);
        glLineWidth(map->m_borderLayers[i].lineWidth);

        for (const auto &polygon : map->m_borderLayers[i].polygons) {
            glBegin(GL_LINE_STRIP);
            for (const auto &point : polygon) {
                float normalizedX = (point.x - map->minX) / (map->maxX - map->minX) * 2 - 1;
                float normalizedY = (point.y - map->minY) / (map->maxY - map->minY) * 2 - 1;
                glVertex2f(normalizedX * zoomLevel + offsetX, normalizedY * zoomLevel + offsetY);

            }
            glEnd();
        }
    }

    for (int i = 0; i < map->m_pointLayers.size(); i++) {
        if (!map->m_pointLayers[i].isVisible) continue;

        QColor &color = map->m_pointLayers[i].color;
        glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        glPointSize(map->m_pointLayers[i].pointSize);

        glBegin(GL_POINTS);
        for (const auto &point : map->m_pointLayers[i].points) {
            float normalizedX = (point.x - map->minX) / (map->maxX - map->minX) * 2 - 1;
            float normalizedY = (point.y - map->minY) / (map->maxY - map->minY) * 2 - 1;
            glVertex2f(normalizedX * zoomLevel + offsetX, normalizedY * zoomLevel + offsetY);
        }
        glEnd();
    }

    glFlush();
}


void MyWidget::changeVisibility(QString layerName, bool status) {
    for (auto& layer : map->m_pointLayers) {
        if (layer.name == layerName) {
            layer.isVisible = status;
            update();
            return;
        }
    }

    for (auto& layer : map->m_lineLayers) {
        if (layer.name == layerName) {
            layer.isVisible = status;
            update();
            return;
        }
    }

    for (auto& layer : map->m_poligonLayers) {
        if (layer.name == layerName) {
            layer.isVisible = status;
            update();
            return;
        }
    }

    for (auto& layer : map->m_borderLayers) {
        if (layer.name == layerName) {
            layer.isVisible = status;
            update();
            return;
        }
    }

    qDebug() << "Layer with name" << layerName << "not found.";
}


void MyWidget::setZoomLevel(float zoom) {
    zoomLevel = zoom;
    resizeGL(width(), height());
    update();
}

void MyWidget::setOffsets(float x, float y) {
    offsetX = x;
    offsetY = y;
}

void MyWidget::zoomIn() {
    zoomLevel *= 1.1f;
    resizeGL(width(), height());
    update();
}

void MyWidget::zoomOut() {
    zoomLevel /= 1.1f;
    resizeGL(width(), height());
    update();
}

void MyWidget::panLeft() {
    offsetX += 0.1f;
    update();
}

void MyWidget::panRight() {
    offsetX -= 0.1f;
    update();
}

void MyWidget::panUp() {
    offsetY -= 0.1f;
    update();
}

void MyWidget::panDown() {
    offsetY += 0.1f;
    update();
}

void MyWidget::wheelEvent(QWheelEvent *event) {
    QPoint globalCursorPos = QCursor::pos();
    QPoint widgetCursorPos = mapFromGlobal(globalCursorPos);

    float glX = float(widgetCursorPos.x()) / float(width()) * 2.0f - 1.0f;
    float glY = -float(widgetCursorPos.y()) / float(height()) * 2.0f + 1.0f;

    float originalZoomLevel = zoomLevel;

    if (event->angleDelta().y() > 0) {
        zoomLevel *= 1.1f;
    } else {
        zoomLevel /= 1.1f;
    }

    offsetX = glX - (glX - offsetX) * (zoomLevel / originalZoomLevel);
    offsetY = glY - (glY - offsetY) * (zoomLevel / originalZoomLevel);

    resizeGL(width(), height());
    update();
}

void MyWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}


void MyWidget::mouseMoveEvent(QMouseEvent *event) {
    if (dragging) {
        QPointF delta = event->pos() - lastMousePos;
        lastMousePos = event->pos();

        float deltaX = float(delta.x()) / width() * 2.0f;
        float deltaY = -float(delta.y()) / height() * 2.0f;

        offsetX += deltaX;
        offsetY += deltaY;

        update();
    }
}

void MyWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        setCursor(Qt::ArrowCursor);
    }
}

void MyWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QPoint widgetCursorPos = event->pos();

    float glX = float(widgetCursorPos.x()) / float(width()) * 2.0f - 1.0f;
    float glY = -float(widgetCursorPos.y()) / float(height()) * 2.0f + 1.0f;

    float originalZoomLevel = zoomLevel;
    zoomLevel *= 1.4f;

    offsetX = glX - (glX - offsetX) * (zoomLevel / originalZoomLevel);
    offsetY = glY - (glY - offsetY) * (zoomLevel / originalZoomLevel);

    resizeGL(width(), height());
    update();
}

void MyWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        panLeft();
        break;
    case Qt::Key_Right:
        panRight();
        break;
    case Qt::Key_Up:
        panUp();
        break;
    case Qt::Key_Down:
        panDown();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Equal:
        zoomIn();
        break;
    default:
        QOpenGLWidget::keyPressEvent(event);
    }
}

