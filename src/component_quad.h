#ifndef COMPONENT_QUAD_H
#define COMPONENT_QUAD_H

#include <insolence/insolence.h>
#include "render_manager_quads.h"

struct Quad : Component {};

struct SystemQuads : SystemBase<Quad> {
	RenderManagerQuads *renderer;

	SystemQuads(RenderManagerQuads *r) :
		renderer(r)
	{}

	void Manage(const GameTime &gametime, Entity *e, Quad *c)
	{
		/*
		 * Insolence has a Manage override that would let you manually
		 * iterate over the components a System holds and just perform
		 * this check once - but this is prettier.
		 */
		if(renderer == NULL)
			return;

		Transform *t = e->Get<Transform>();
		renderer->Add(t->GetPosXY(), t->GetScaleXY());
	}
};

#endif
