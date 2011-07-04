#include "Editor/Editor/GLWidget.h"
#include <QtGui/QMouseEvent>
#include "Editor/Editor/Editor.h"

GLWidget::GLWidget (QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);
}

int GLWidget::width () const {
    return QWidget::width();
}

int GLWidget::height () const {
    return QWidget::height();
}

void GLWidget::initializeGL () {
}

void GLWidget::resizeGL (int w, int h) {
    Graphics::Set2D(w, h);
}

void GLWidget::paintGL () {
    dynamic_cast<Editor*>(parent())->RenderScene();
}

void GLWidget::mousePressEvent (QMouseEvent *event) {

}
void GLWidget::mouseMoveEvent (QMouseEvent *event) {
}

void GLWidget::keyPressEvent (QKeyEvent* event) {
    //if (event->isAutoRepeat())
    //    event->ignore();
    LOG(INFO) << "EVENT";
    switch(event->key()) {
        case Qt::Key_Left:
            LOG(INFO) << "HEY";
            break;
        default:
            event->ignore();
            break;
    }
}

void GLWidget::keyReleaseEvent (QKeyEvent *event) {
    if (event->isAutoRepeat())
        event->ignore();
        
    switch(event->key()) {
        case Qt::Key_Left:
            LOG(INFO) << "HEY STOP";
            break;
        default:
            event->ignore();
            break;
    }
}

