#pragma once
#include <vector>
#include "DResourcesImplements.h"

typedef float		 FLOAT32;
typedef unsigned int UINT;

#ifdef GLES3

#include <glad/glad.h>
typedef GLfloat FLOAT32;
typedef GLuint  UINT;

#endif


struct STATICVERTEX {
	FLOAT32 POSITION	[3];
	FLOAT32 NORMAL		[3];
	FLOAT32 TANGENT		[3];
	FLOAT32 TEXCOORD	[2];
};

struct StaticInstance {};


class DScenePipeline
{
	const size_t			MAX_S_INSTANCES = 2048;
	static DScenePipeline*	SCNPIPELINE;						//SINGLETON

	SIGNATURE<StaticInstance>**		S_INSTANCES = nullptr;		//SIGNATURES ARRAY
	size_t							S_INSTANCES_SIZE = 0;		//SIG CURRENT SIZE
	
	bool							SIGNATURES_LOCKED = false;	//LOCK INSTANCES ARRAY
	std::vector<size_t>				SIGNATURES_PLACES;			//VECTOR OF EMPTY INSTANCES INDICES

	uint32_t SIGNATURE_PASSWORD = 9910300;						//TEST PASSWORD FOR SIGNATURES EDIT

public:
	DScenePipeline();
	~DScenePipeline() = default;

	void					Initialize					();
	void					Close						();
	void					InstanceCreate				(DISPOSABLE<StaticInstance>* ptr);
	void					InstanceDelete				(DISPOSABLE<StaticInstance>* ptr);
	//void					InstanceDeleteWithINDX		(const uint32_t INDEX);

	StaticInstance*			GetInstanceWithINDX			(const uint32_t INDX);
	
	static void				SetScenePipelineSingleton	(DScenePipeline* ptr);
	static DScenePipeline*	GetScenePipelineSingleton	();
};



