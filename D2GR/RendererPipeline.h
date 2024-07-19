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

	static inline void RenderDrawStaticGeometryInstance(DGeometryPipeline::StaticGeometryInstance* geometryInstance) {
		if (geometryInstance->geometryBuffer->VertexBuffer && \
			geometryInstance->geometryBuffer->IndexBuffer && \
			geometryInstance->hasValidOverrideMaterial()) {

			DBufferPipeline::GeometryBufferBindVertexBuffer(geometryInstance->geometryBuffer);

#ifdef _GLES3
			if (geometryInstance->hasValidOverlayMaterial()) {
				glUseProgram(geometryInstance->OverlayMaterial->GeometryShader->program_id);
			}

			glUseProgram(geometryInstance->OverrideMaterial->GeometryShader->program_id);

			glDrawElements(GL_TRIANGLES, geometryInstance->geometryBuffer->IndexBuffer->IndicesCount, GL_UNSIGNED_INT, NULL);

			if (geometryInstance->hasValidOverlayMaterial()) {
				glUseProgram(NULL);
			}

			glUseProgram(NULL);
#endif
			DBufferPipeline::GeometryBufferBindVertexBuffer(geometryInstance->geometryBuffer);
		}
	};

	static inline void RendererDestroy() {};
};

