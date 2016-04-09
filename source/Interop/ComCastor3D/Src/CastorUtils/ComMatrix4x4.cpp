#include "ComMatrix4x4.hpp"

namespace CastorCom
{
	CMatrix4x4::CMatrix4x4()
	{
	}

	CMatrix4x4::~CMatrix4x4()
	{
	}

	STDMETHODIMP CMatrix4x4::Transpose()
	{
		Castor::Matrix4x4r::transpose();
		return S_OK;
	}

	STDMETHODIMP CMatrix4x4::Invert()
	{
		Castor::Matrix4x4r::invert();
		return S_OK;
	}
}