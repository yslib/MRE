
#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_
#include <vector>
#include <VMat/geometry.h>
#include "actor.h"
namespace vm
{
class Actor;
class VISUALMAN_EXPORT_IMPORT AbstraSceneManager
{
public:
	AbstraSceneManager() = default;
	virtual ~AbstraSceneManager() = default;
	Bound3f BoundingBox() const { return bound; }
	void SetBoundingBox( const Bound3f &bound ) { this->bound = bound; }
	virtual void ExtractActors( std::vector<VMRef<Actor>> &actorArray ) = 0;
	virtual std::vector<VMRef<Actor>> Actors() = 0;

protected:
	Bound3f bound;
};
}  // namespace vm

#endif