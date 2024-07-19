#pragma once
#include "BufferPipeline.h"


class DGeometryPipeline
{
public:
	struct GeometryShaderResource {
		const char* FragmentSource;
		const char* VertexSource;

#ifdef _GLES3
		GLuint program_id;
#endif
	};

	struct GeometryMaterial {
		GeometryShaderResource* GeometryShader = nullptr;
	};

	struct StaticGeometryMaterial : GeometryMaterial {
		
	};
	
	struct StaticGeometryInstance {
		DBufferPipeline::GeometryBufferResource* geometryBuffer = nullptr;
		StaticGeometryMaterial* OverrideMaterial = nullptr;
		StaticGeometryMaterial* OverlayMaterial = nullptr;
	};

	static inline void StaticGeometryInstanceRenderInstance(StaticGeometryInstance* geometryInstance) {
		if (geometryInstance->geometryBuffer->VertexBuffer) {
			DBufferPipeline::GeometryBufferBindVertexBuffer(geometryInstance->geometryBuffer);
#ifdef _GLES3
			glUseProgram(geometryInstance->OverrideMaterial->GeometryShader->program_id);

			glDrawElements(GL_TRIANGLES, geometryInstance->geometryBuffer->IndexBuffer->IndicesCount, GL_UNSIGNED_INT, NULL);

			glUseProgram(NULL);
#endif
			DBufferPipeline::GeometryBufferBindVertexBuffer(geometryInstance->geometryBuffer);
		}
	};

	static inline void StaticGeometryMaterialInitialize(StaticGeometryMaterial* geometryMaterial) {
		if (!geometryMaterial->GeometryShader) throw "Geometry Shader is NullPtr";

#ifdef _GLES3
		geometryMaterial->GeometryShader->program_id = glCreateProgram();
		
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

		GLint	glOk;
		GLchar	glString[512];

		glShaderSource	(vs, 1, &geometryMaterial->GeometryShader->VertexSource, NULL);
		glCompileShader	(vs);
		glGetShaderiv	(vs, GL_COMPILE_STATUS, &glOk);

		if (!glOk) {
			glGetShaderInfoLog(vs, 512, NULL, glString);

			throw glString;
		}

		glShaderSource	(fs, 1, &geometryMaterial->GeometryShader->FragmentSource, NULL);
		glCompileShader	(fs);
		glGetShaderiv	(fs, GL_COMPILE_STATUS, &glOk);

		if (!glOk) {
			glGetShaderInfoLog(fs, 512, NULL, glString);

			throw glString;
		}

		glAttachShader(geometryMaterial->GeometryShader->program_id, vs);
		glAttachShader(geometryMaterial->GeometryShader->program_id, fs);

		glBindAttribLocation(geometryMaterial->GeometryShader->program_id, 0, "POSITION");
		glBindAttribLocation(geometryMaterial->GeometryShader->program_id, 1, "NORMAL");
		glBindAttribLocation(geometryMaterial->GeometryShader->program_id, 2, "TANGENT");
		glBindAttribLocation(geometryMaterial->GeometryShader->program_id, 3, "TEXCOORD");
		//glBindAttribLocation(geometryMaterial->GeometryShader->program_id, 4, "COLOR");

		glLinkProgram	(geometryMaterial->GeometryShader->program_id);
		glGetProgramiv	(geometryMaterial->GeometryShader->program_id, GL_LINK_STATUS, &glOk);

		if (!glOk) {
			glGetProgramInfoLog(geometryMaterial->GeometryShader->program_id, 512, NULL, glString);

			throw glString;
		}

		//glShAtPosition = glGetAttribLocation	(glShProgrm, "v_POSITION");
		//glShAtUV = glGetAttribLocation			(glShProgrm, "v_UV");
		//glShAtNormal = glGetAttribLocation		(glShProgrm, "v_NORMAL");
		//glShAtColor = glGetAttribLocation		(glShProgrm, "v_COLOR");

		//printf("Material v_POSITION at %i\n", glGetAttribLocation(glShProgrm, "v_POSITION"));
		//printf("Material v_NORMAL at %i\n", glGetAttribLocation(glShProgrm, "v_NORMAL"));
		//printf("Material v_UV at %i\n", glGetAttribLocation(glShProgrm, "v_UV"));
		//printf("Material v_COLOR at %i\n", glGetAttribLocation(glShProgrm, "v_COLOR"));

		glDeleteShader(vs);
		glDeleteShader(fs);

#endif
	}

	static inline void StaticGeometryInstanceDestroy(StaticGeometryInstance* instance) {
#ifdef _GLES3
		glDeleteProgram(instance->OverrideMaterial->GeometryShader->program_id);

		instance->OverrideMaterial->GeometryShader = nullptr;
		instance->OverrideMaterial = nullptr;

		//delete instance->OverrideMaterial->GeometryShader;
		//delete instance->OverrideMaterial;
		//delete instance;
		
#endif
	}
};

