#include "Game/Graphics/Graphics.h"
#include <QtOpenGL/QGLWidget>
#include "Volt/Graphics/Viewport.h"

class GLWidget : public QGLWidget, public Volt::Viewport {
    Q_OBJECT;
public:
    GLWidget (QWidget *parent = NULL);

    virtual int width () const;
    virtual int height () const;
    
protected:
    virtual void initializeGL ();
    virtual void resizeGL (int w, int h);
    virtual void paintGL ();

    virtual void enterEvent (QEvent* event);
    
    virtual void mousePressEvent (QMouseEvent* event);
    virtual void mouseMoveEvent (QMouseEvent* event);
    virtual void keyPressEvent (QKeyEvent* event);
    virtual void keyReleaseEvent (QKeyEvent* event);
};
