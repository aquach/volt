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
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);
}

void GLWidget::resizeGL (int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h); // set origin to bottom left corner
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL () {
    dynamic_cast<Editor*>(parent())->RenderScene();
}

void GLWidget::mousePressEvent (QMouseEvent *event) {

}
void GLWidget::mouseMoveEvent (QMouseEvent *event) {
}

void GLWidget::keyPressEvent (QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    default:
        event->ignore();
        break;
    }
}
