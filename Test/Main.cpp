#include <SDL.h>
#include "../../_engine_.h"
#include "../D2GR/RendererPipeline.h"


int main(int argc, char* argv[]) {
	FLOAT32 vertices[] = {
		0.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 1.f, 0.f
	};
	UINT indices[] = {
		0, 1, 2
	};

	FLOAT32 color[4] = {
		1.f, 0.f, 1.f, 1.f
	};

	DBufferPipeline::VertexBufferResource vbuffer;
	vbuffer.UsageMode = DBufferPipeline::Unchanged;
	vbuffer.ByteSize = sizeof(vertices);
	vbuffer.LayoutCount = 1;
	vbuffer.AllocData = &vertices;

	vbuffer.VLayouts[0].Location = 0;
	vbuffer.VLayouts[0].LayoutFormat = DBufferPipeline::RGB_FLOAT;

	DBufferPipeline::IndexBufferResource ibuffer;
	ibuffer.UsageMode = DBufferPipeline::Unchanged;
	ibuffer.ByteSize = sizeof(indices);
	ibuffer.AllocData = &indices;
	ibuffer.IndicesCount = 3;

	DBufferPipeline::UniformBufferResource ubuffer;
	ubuffer.LayoutCount = 1;
	ubuffer.UsageMode = DBufferPipeline::Unchanged;
	ubuffer.ByteSize = sizeof(color);
	ubuffer.AllocData = &color;
	
	ubuffer.UniformBlocks[0].BlockName = "VertexBlock";
	ubuffer.UniformBlocks[0].Location = 0;

	DBufferPipeline::GeometryBufferResource geoBuffer;
	geoBuffer.VertexBuffer = &vbuffer;
	geoBuffer.IndexBuffer = &ibuffer;
	geoBuffer.UniformBuffer = &ubuffer;

	DGeometryPipeline::GeometryShaderResource sh;
	sh.VertexSource = R"(#version 140

#ifdef GL_ES
precision mediump float;
#endif


in vec3 POSITION;
in vec3 NORMAL;
in vec3 TANGENT;
in vec2 TEXCOORD;

uniform VertexBlock {
	vec4 COLOR;
};

//in vec4 VColor;

//varying vec4 fColor;

out vec4 fColor;

//out vec2 fCoord;

void main()
{
	gl_Position = vec4(POSITION, 1.0);
	
	fColor = COLOR;
	
	//fCoord = VTexCoord;
}
)";
	sh.FragmentSource = R"(#version 140

#ifdef GL_ES
precision mediump float;
#endif

in vec4 fColor;
//in vec2 fCoord;

//uniform sampler2D ScreenTexture;

out vec4 FragColor;

void main()
{
	FragColor = fColor;
})";

	DGeometryPipeline::StaticGeometryMaterial mat;
	mat.GeometryShader = &sh;

	DGeometryPipeline::StaticGeometryInstance instance;
	instance.geometryBuffer = &geoBuffer;
	instance.OverrideMaterial = &mat;


	SDL_Window* window;
	SDL_Event event;
	SDL_GLContext context;

	std::cout << "D2GR version " << _D2GR_VERSION_NAME_ << std::endl;

	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("D2GR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 420, SDL_WINDOW_OPENGL);

	context = SDL_GL_CreateContext(window);

	gladLoadGLLoader(SDL_GL_GetProcAddress);

	DBufferPipeline::GeometryBufferInitialize(&geoBuffer);
	
	DGeometryPipeline::StaticGeometryMaterialInitialize(&mat);
	
	DGeometryPipeline::StaticGeometryInstanceInitialize(&instance);
	
	DRendererPipeline::RendererInitialize();

	bool running = true;

	while (running) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			}
		}

		DRendererPipeline::RendererBeginFrame();
		DGeometryPipeline::StaticGeometryInstanceRenderInstance(&instance);
		DRendererPipeline::RendererEndFrame();

		SDL_GL_SwapWindow(window);
	}

	DRendererPipeline::RendererDestroy();
	DGeometryPipeline::StaticGeometryInstanceDestroy(&instance);
	DBufferPipeline::GeometryBufferDestroy(&geoBuffer);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}