
#include "vmqtwindow.h"
#include <QMouseEvent>

namespace vm
{
VMQtWindow::VMQtWindow( QWidget *parent ) :
  QOpenGLWidget( parent )
{
}

void VMQtWindow::initializeGL()
{
	InitContext();
	RenderContext::DispatchInitEvent();
}

void VMQtWindow::resizeGL( int w, int h )
{
	RenderContext::DispatchResizeEvent( w, h );
}

void VMQtWindow::paintGL()
{
	RenderContext::DispatchUpdateEvent();
}

void VMQtWindow::MakeCurrent()
{
	makeCurrent();
}

void VMQtWindow::SwapBuffer()
{
	frameSwapped();
}

void VMQtWindow::Update()
{
	update();
}

void VMQtWindow::mousePressEvent( QMouseEvent *event )
{
	int buttons = 0;
	if ( event->buttons() & Qt::LeftButton )
		buttons |= Mouse_Left;
	if ( event->buttons() & Qt::RightButton )
		buttons |= Mouse_Right;
	RenderContext::DispatchMousePressedEvent( (MouseButton)buttons, event->pos().x(), event->pos().y() );
	Update();
}

void VMQtWindow::mouseMoveEvent( QMouseEvent *event )
{
	// Note that the returned value of event->button() is always Qt::NoButton for mouse move events.
	int buttons = 0;
	if ( event->buttons() & Qt::LeftButton )
		buttons |= Mouse_Left;
	if ( event->buttons() & Qt::RightButton )
		buttons |= Mouse_Right;
	//std::cout << button << std::endl;
	RenderContext::DispatchMouseMoveEvent( (MouseButton)buttons, event->pos().x(), event->pos().y() );
	Update();
}

void VMQtWindow::mouseReleaseEvent( QMouseEvent *event )
{
	int buttons = 0;
	if ( event->buttons() & Qt::LeftButton )
		buttons |= Mouse_Left;
	if ( event->buttons() & Qt::RightButton )
		buttons |= Mouse_Right;
	RenderContext::DispatchMouseReleasedEvent( (MouseButton)buttons, event->pos().x(), event->pos().y() );
	Update();
}
}  // namespace vm
