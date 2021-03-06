
#ifndef _RENDERABLE_H_
#define _RENDERBALE_H_
#include "graphictype.h"
namespace vm
{
class Shading;
class RenderContext;
class Camera;
class RenderState;
class Actor;

class VISUALMAN_EXPORT_IMPORT Renderable
{
public:
	Renderable() = default;
	virtual void Render( const Actor *actor, const Shading *shading, const Camera *camera, RenderContext *context );
	virtual void UpdateDirtyBufferObject( BufferObjectUpdateMode mode ) = 0;

	void SetBufferObjectDirty( bool dirty ) { renderableDirty = dirty; }
	bool GetBufferObjectDirty() const { return renderableDirty; }

	void SetBufferObjectEnabled( bool enable ) { enableBufferObject = enable; }
	bool GetBufferObjectEnabled() const { return enableBufferObject; }

	virtual ~Renderable() = default;

protected:
	virtual void Render_Implement( const Actor *actor, const Shading *shading, const Camera *camera, RenderContext *context ) const = 0;

private:
	bool renderableDirty = true;
	bool enableBufferObject = true;
};
}  // namespace vm

#endif