
#ifndef _ABSTRARENDERSTATE_H_
#define _ABSTRARENDERSTATE_H_
#include "graphictype.h"
#include "camera.h"
#include <vector>

namespace vm
{
class RenderContext;
class GLSLProgram;

class VISUALMAN_EXPORT_IMPORT RenderState : public std::enable_shared_from_this<RenderState>
{
public:
	RenderState() = delete;
	RenderState( RenderStateType type ) :
	  type( type ) {}
	virtual void Apply( int index, const Camera *camera, RenderContext *context ) const = 0;
	RenderStateType Type() const { return type; }
	virtual ~RenderState() = default;

private:
	RenderStateType type;
};

class VISUALMAN_EXPORT_IMPORT RenderStateNonIndexed : public RenderState
{
public:
	RenderStateNonIndexed( RenderStateType type ) :
	  RenderState( type ) {}
};

class VISUALMAN_EXPORT_IMPORT RenderStateIndexed : public RenderState
{
public:
	RenderStateIndexed( RenderStateType type ) :
	  RenderState( type ) {}
	//int Index()const { return index; }
	//void SetIndex(int index) { this->index = index; }
private:
	//int index = -1;
};

class RenderStateBox
{
public:
	RenderStateBox() = default;
	RenderStateBox( VMRef<RenderState> state, int i ) :
	  rawRenderState( std::move( state ) ), index( i ) {}
	RenderStateType StateType() const
	{
		if ( index < 0 )
			return rawRenderState->Type();
		return RenderStateType( rawRenderState->Type() + index );
	}

	/**
			 * \brief  index<0 indicates it is not a indexed render state
			 */
	virtual void Apply( const Camera *camera, RenderContext *context ) const
	{
		rawRenderState->Apply( index, camera, context );
	}
	VMRef<RenderState> rawRenderState;
	int index = -1;
};

}  // namespace vm

#endif