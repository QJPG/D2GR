#pragma once
#include <iostream>

typedef float FLOAT32;
typedef uint32_t UINT;
//typedef unsigned long LUINT;
//typedef bool BOOL;
typedef int INT;

#ifdef _GLES3
#include <glad/glad.h>

typedef GLfloat FLOAT32;
typedef GLuint UINT;
//typedef GLsizeiptr LUINT;
//typedef GLboolean BOOL;
typedef GLint INT;

#endif

class DBufferPipeline
{
public:
	enum BufferUsageMode {
		Unchanged, Changed, FastChanged
	};

	enum BufferLayoutFormat {
		RGB_FLOAT = 3,	//fvec3
		RG_FLOAT  = 2,	//fvec2
		RGBA_FLOAT= 4,	//fvec4

		RGB_INT = 3,	//ivec3
		RG_INT  = 2,	//ivec2
		RGBA_INT= 4,	//ivec4

		RGB_UINT = 3,	//uvec3
		RG_UINT  = 2,	//uvec2
		RGBA_UINT= 4,	//uvec3
	};

	struct BufferResource {
		BufferUsageMode		UsageMode;
		unsigned long long	ByteSize;
		void*				AllocData = nullptr;
	};

	struct VertexLayout {
		BufferLayoutFormat	LayoutFormat;
		UINT				Location;
	};

	struct VertexBufferResource : BufferResource {
		uint32_t		LayoutCount = 16;
		VertexLayout	VLayouts[31];


#ifdef _GLES3
		GLuint arrayb_id;
		GLuint vertex_id;
#endif
	};

	struct IndexBufferResource : BufferResource {
		UINT IndicesCount = 0;
#ifdef _GLES3
		GLuint index_id;
#endif
	};
	
	struct GeometryBufferResource {
		VertexBufferResource*	VertexBuffer = nullptr;
		IndexBufferResource*	IndexBuffer = nullptr;
	};

	struct FrameBufferResource {

	};

	static inline void GeometryBufferInitialize(GeometryBufferResource* geometryBuffer) {
		if (!geometryBuffer->VertexBuffer) throw "Vertex Buffer is NullPtr!";
		if (!geometryBuffer->IndexBuffer)  throw "Index Buffer is NullPtr!";

#ifdef _GLES3
		glGenVertexArrays	(1, &geometryBuffer->VertexBuffer->arrayb_id);
		glGenBuffers		(1, &geometryBuffer->VertexBuffer->vertex_id);
		glGenBuffers		(1, &geometryBuffer->IndexBuffer->index_id);

		GLsizeiptr buffer_usage = NULL;

		switch (geometryBuffer->VertexBuffer->UsageMode) {
			case BufferUsageMode::Changed:
				buffer_usage = GL_DYNAMIC_DRAW;
				break;

			case BufferUsageMode::Unchanged:
				buffer_usage = GL_STATIC_DRAW;
				break;

			case BufferUsageMode::FastChanged:
				buffer_usage = GL_STREAM_DRAW;
				break;
		};

		switch (geometryBuffer->IndexBuffer->UsageMode) {
			case BufferUsageMode::Changed:
				buffer_usage = GL_DYNAMIC_DRAW;
				break;

			case BufferUsageMode::Unchanged:
				buffer_usage = GL_STATIC_DRAW;
				break;

			case BufferUsageMode::FastChanged:
				buffer_usage = GL_STREAM_DRAW;
				break;
		};

		glBindVertexArray	(geometryBuffer->VertexBuffer->arrayb_id);
		glBindBuffer		(GL_ARRAY_BUFFER, geometryBuffer->VertexBuffer->vertex_id);
		glBufferData		(GL_ARRAY_BUFFER, geometryBuffer->VertexBuffer->ByteSize, geometryBuffer->VertexBuffer->AllocData, buffer_usage);

		glBindBuffer		(GL_ELEMENT_ARRAY_BUFFER, geometryBuffer->IndexBuffer->index_id);
		glBufferData		(GL_ELEMENT_ARRAY_BUFFER, geometryBuffer->IndexBuffer->ByteSize, geometryBuffer->IndexBuffer->AllocData, buffer_usage);

		/*Calc. Stride*/

		size_t stride = 0;

		for (size_t _l = 0; _l < geometryBuffer->VertexBuffer->LayoutCount; _l++) {
			stride += geometryBuffer->VertexBuffer->VLayouts[_l].LayoutFormat;
		}

		/*Attrib Layouts*/

		size_t offset = 0;

		for (size_t _l = 0; _l < geometryBuffer->VertexBuffer->LayoutCount; _l++) {
			VertexLayout& v = geometryBuffer->VertexBuffer->VLayouts[_l];

			UINT format;
			
			if (v.LayoutFormat == BufferLayoutFormat::RGBA_FLOAT || v.LayoutFormat == BufferLayoutFormat::RGB_FLOAT || v.LayoutFormat == BufferLayoutFormat::RG_FLOAT) {
				format = GL_FLOAT;
				glVertexAttribPointer(v.Location, v.LayoutFormat, format, GL_FALSE, sizeof(FLOAT32) * stride, (void*)(offset * sizeof(FLOAT32)));
			}

			if (v.LayoutFormat == BufferLayoutFormat::RGBA_INT   || v.LayoutFormat == BufferLayoutFormat::RG_INT    || v.LayoutFormat == BufferLayoutFormat::RGB_INT) {
				format = GL_INT;
				glVertexAttribPointer(v.Location, v.LayoutFormat, format, GL_FALSE, sizeof(INT) * stride, (void*)(offset * sizeof(INT)));
			}

			if (v.LayoutFormat == BufferLayoutFormat::RGBA_UINT  || v.LayoutFormat == BufferLayoutFormat::RG_UINT   || v.LayoutFormat == BufferLayoutFormat::RGB_UINT) {
				format = GL_UNSIGNED_INT;
				glVertexAttribPointer(v.Location, v.LayoutFormat, format, GL_FALSE, sizeof(UINT) * stride, (void*)(offset * sizeof(UINT)));
			}

			offset += v.LayoutFormat;
			
			glEnableVertexAttribArray	(v.Location);
		}
		

		glBindBuffer		(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray	(NULL);
#endif
	};
	
	static inline void GeometryBufferBindVertexBuffer(GeometryBufferResource* geometryBuffer) {
#ifdef _GLES3
		glBindVertexArray(geometryBuffer->VertexBuffer->arrayb_id);
		glBindBuffer(GL_ARRAY_BUFFER, geometryBuffer->VertexBuffer->vertex_id);
#endif
	};
	
	static inline void GeometryBufferUnbindVertexBuffer(GeometryBufferResource* geometryBuffer) {
#ifdef _GLES3
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray(NULL);
#endif
	};

	static inline void GeometryBufferCopyDataToVertexBuffer(GeometryBufferResource* geometryBuffer, void* data, UINT offset) {
#ifdef _GLES3
		glBindVertexArray(geometryBuffer->VertexBuffer->arrayb_id);
		glBindBuffer(GL_ARRAY_BUFFER, geometryBuffer->VertexBuffer->vertex_id);

		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(data), data);

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray(NULL);
#endif
	};
	static inline void GeometryBufferCopyDataToIndexBuffer(GeometryBufferResource* geometryBuffer, void* data, UINT offset) {
#ifdef _GLES3
		glBindVertexArray(geometryBuffer->VertexBuffer->arrayb_id);

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeof(data), data);

		glBindVertexArray(NULL);
#endif
	};
	
	static inline void GeometryBufferDestroy(GeometryBufferResource* geometryBuffer) {
#ifdef _GLES3
		glDeleteBuffers			(1, &geometryBuffer->IndexBuffer->index_id);
		glDeleteBuffers			(1, &geometryBuffer->VertexBuffer->vertex_id);
		glDeleteVertexArrays	(1, &geometryBuffer->VertexBuffer->arrayb_id);

		geometryBuffer->VertexBuffer = nullptr;
		geometryBuffer->IndexBuffer = nullptr;
		
		//delete geometryBuffer;
#endif
	};
};

