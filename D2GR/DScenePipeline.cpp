#include "DScenePipeline.h"

DScenePipeline* DScenePipeline::SCNPIPELINE = nullptr;

DScenePipeline::DScenePipeline()
{
	S_INSTANCES = new SIGNATURE<StaticInstance>*[MAX_S_INSTANCES];	//INIT SIGNATURES ARRAY

	/*
	TO-DO

	AO INVES DE DEIXAR O ARRAY COMO nullptr POR SLOT, ATRIBUA UMA SIGNATURE PARA CADA.
	
	*/
}

void DScenePipeline::Initialize()
{
}

void DScenePipeline::Close()
{
	for (size_t i = 0; i < S_INSTANCES_SIZE; i++) {
		delete S_INSTANCES[i];
		S_INSTANCES[i] = nullptr;
	}

	delete[] S_INSTANCES;
}

void DScenePipeline::InstanceCreate(DISPOSABLE<StaticInstance>* ptr)
{
	if (SIGNATURES_LOCKED) return;
	if (ptr->HasSignature()) return;

	SIGNATURES_LOCKED = true;

	if (SIGNATURES_PLACES.size() < 1 && S_INSTANCES_SIZE < MAX_S_INSTANCES) {
		S_INSTANCES[S_INSTANCES_SIZE++] =				new SIGNATURE<StaticInstance>;
		S_INSTANCES[S_INSTANCES_SIZE]->SetSignature		(SIGNATURE_PASSWORD);
		S_INSTANCES[S_INSTANCES_SIZE]->SetPtrwFrom		(new StaticInstance, SIGNATURE_PASSWORD);
		S_INSTANCES[S_INSTANCES_SIZE]->SetINDX			(S_INSTANCES_SIZE - 1, SIGNATURE_PASSWORD);

		//S_INSTANCES[S_INSTANCES_SIZE]->INDX = S_INSTANCES_SIZE - 1;
		//S_INSTANCES[S_INSTANCES_SIZE]->ptrw = new StaticInstance;

		ptr->SetSignature								(S_INSTANCES[S_INSTANCES_SIZE]);
	}
	else {
		if (SIGNATURES_PLACES.size() <= MAX_S_INSTANCES) {
			S_INSTANCES[SIGNATURES_PLACES.front()] =					new SIGNATURE<StaticInstance>;
			S_INSTANCES[SIGNATURES_PLACES.front()]->SetSignature		(SIGNATURE_PASSWORD);
			S_INSTANCES[SIGNATURES_PLACES.front()]->SetPtrwFrom			(new StaticInstance, SIGNATURE_PASSWORD);
			S_INSTANCES[SIGNATURES_PLACES.front()]->SetINDX				(SIGNATURES_PLACES.front(), SIGNATURE_PASSWORD);

			//S_INSTANCES[S_INSTANCES_SIZE]->INDX = SIGNATURES_PLACES.front();
			//S_INSTANCES[S_INSTANCES_SIZE]->ptrw = new StaticInstance;

			SIGNATURES_PLACES.erase										(SIGNATURES_PLACES.begin());
			ptr->SetSignature											(S_INSTANCES[SIGNATURES_PLACES.front()]);
		}
	}

	SIGNATURES_LOCKED = false;
}

void DScenePipeline::InstanceDelete(DISPOSABLE<StaticInstance>* ptr)
{
	if (SIGNATURES_LOCKED) return;
	if (!ptr->HasSignature()) return;

	SIGNATURES_LOCKED = true;

	if (ptr->GetSignature().GetINDX() >= S_INSTANCES_SIZE) return;

	delete S_INSTANCES[ptr->GetSignature().GetINDX()];
	S_INSTANCES[ptr->GetSignature().GetINDX()] = nullptr;

	SIGNATURES_PLACES.insert(SIGNATURES_PLACES.begin(), ptr->GetSignature().GetINDX());

	--S_INSTANCES_SIZE;

	SIGNATURES_LOCKED = false;
}


/*void DScenePipeline::InstanceDeleteWithINDX(const uint32_t INDX)
{
	if (SIGNATURESLOCK) return;

	SIGNATURESLOCK = true;

	if (INDX >= S_INSTANCES_SIZE) return;
	
	delete S_INSTANCES[INDX];
	S_INSTANCES[INDX] = nullptr;

	SIGNATURES_PLACES.insert(SIGNATURES_PLACES.begin(), INDX);

	--S_INSTANCES_SIZE;

	SIGNATURESLOCK = false;
}*/

StaticInstance* DScenePipeline::GetInstanceWithINDX(const uint32_t INDX)
{
	return &S_INSTANCES[INDX]->GetPtrw();
}

void DScenePipeline::SetScenePipelineSingleton(DScenePipeline* ptr)
{
	if (SCNPIPELINE) {
		SCNPIPELINE->Close();
		
		delete SCNPIPELINE;
	}

	SCNPIPELINE = ptr;
}

DScenePipeline* DScenePipeline::GetScenePipelineSingleton()
{
	return SCNPIPELINE;
}
