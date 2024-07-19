#pragma once
#include "GeometryPipeline.h"

class DRendererPipeline
{
public:
	static inline void RendererInitialize() {};

	static inline void RendererBeginFrame() {
#ifdef _GLES3
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
	};

	static inline void RendererEndFrame() {};

	static inline void RendererDestroy() {};
};

