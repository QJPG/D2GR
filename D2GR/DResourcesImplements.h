#pragma once

#include <iostream>

/*
NÃO DELETE UMA ASSINATURA SEM ANTES DELETAR SUA REFERENCIA!

*/

template<class T>
struct SIGNATURE {
	T&			GetPtrw();
	uint32_t	GetINDX() const;

	void SetPtrwFrom		(T*			ptr_from,		  const uint32_t password);
	void SetINDX			(const uint32_t		index,    const uint32_t password);
	void SetSignature		(const uint32_t		password, const uint32_t old_password = NULL);

	bool HasPtrw() const;

	~SIGNATURE();

private:
	bool SIGNATURE_LOCKED = false;

	T*			ptrw = nullptr;		//PTR FROM SIGNED OBJECT
	uint32_t	INDX = NULL;		//SIGNATURE DYNAMIC IDX
	uint32_t	PSSW = NULL;		//PASSWORD FOR MODIFY ASSIGNATURE
};

template<class T>
struct DISPOSABLE {
	void SetSignature(SIGNATURE<T>* ptr_from);
	
	SIGNATURE<T>& GetSignature();

	bool HasSignature() const;		//USE BEFORE GET A SIGNATURE!

	~DISPOSABLE() = default;		//DONT DESTROY A SIGNATURE PTR!!!!!!!

private:
	SIGNATURE<T>* ptrw = nullptr;
};

/*SIGNATURE IMPLEMENTATION*/

template<class T>
inline T& SIGNATURE<T>::GetPtrw()
{
	if (!ptrw)
		throw std::runtime_error("ptrw is nullptr");
		
	return *ptrw;
}

template<class T>
inline uint32_t SIGNATURE<T>::GetINDX() const
{
	return INDX;
}

template<class T>
inline void SIGNATURE<T>::SetPtrwFrom(T* ptr_from, const uint32_t password)
{
	if (SIGNATURE_LOCKED && PSSW != password) return;

	if (ptrw)
		delete ptrw;

	ptrw = ptr_from;
}

template<class T>
inline void SIGNATURE<T>::SetINDX(const uint32_t index, const uint32_t password)
{
	if (SIGNATURE_LOCKED && PSSW != password) return;

	INDX = index;
}

template<class T>
inline void SIGNATURE<T>::SetSignature(const uint32_t password, const uint32_t old_password)
{
	if (!SIGNATURE_LOCKED || SIGNATURE_LOCKED && PSSW == old_password) {
		PSSW = password;
		
		return;
	}
}

template<class T>
inline bool SIGNATURE<T>::HasPtrw() const
{
	if (ptrw)
		return true;

	return false;
}

template<class T>
inline SIGNATURE<T>::~SIGNATURE()
{
	SetPtrwFrom(nullptr, PSSW);		//DESTROY T PTR. ITS is VALID!
}

/*DISPOSABLE IMPLEMENTATION*/

template<class T>
inline void DISPOSABLE<T>::SetSignature(SIGNATURE<T>* ptr_from)
{
	if (ptrw) return;

	ptrw = ptr_from;
}

template<class T>
inline SIGNATURE<T>& DISPOSABLE<T>::GetSignature()
{
	if (!ptrw)
		throw std::runtime_error("ptrw is nullptr");

	return *ptrw;
}

template<class T>
inline bool DISPOSABLE<T>::HasSignature() const
{
	if (ptrw)
		return true;

	return false;
}
