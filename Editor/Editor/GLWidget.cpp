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

void GLWidget::enterEvent (QEvent* event) {
    setFocus();
}

void GLWidget::mouseReleaseEvent (QMouseEvent* event) {
    dynamic_cast<Editor*>(parent())->OnViewportMouseRelease(event);
}

void GLWidget::mousePressEvent (QMouseEvent* event) {
    dynamic_cast<Editor*>(parent())->OnViewportMousePress(event);
}

void GLWidget::mouseMoveEvent (QMouseEvent* event) {
    dynamic_cast<Editor*>(parent())->OnViewportMouseMove(event);
}

void GLWidget::keyPressEvent (QKeyEvent* event) {
    if (event->isAutoRepeat())
        event->ignore();

    Editor* editor = dynamic_cast<Editor*>(parent());
    
    switch(event->key()) {
        case Qt::Key_Left:
            editor->MoveHorizontal(-1);
            break;
        case Qt::Key_Right:
            editor->MoveHorizontal(1);
            break;
        case Qt::Key_Up:
            editor->MoveVertical(-1);
            break;
        case Qt::Key_Down:
            editor->MoveVertical(1);
            break;
        default:
            event->ignore();
            break;
    }
}

void GLWidget::keyReleaseEvent (QKeyEvent* event) {
    if (event->isAutoRepeat())
        event->ignore();

    Editor* editor = dynamic_cast<Editor*>(parent());
        
    switch(event->key()) {
        case Qt::Key_Left:
        case Qt::Key_Right:
            editor->MoveHorizontal(0);
            break;
        case Qt::Key_Up:
        case Qt::Key_Down:
            editor->MoveVertical(0);
            break;
        default:
            event->ignore();
            break;
    }
}

