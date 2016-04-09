#include "ComTexture.hpp"
#include "ComLogger.hpp"

namespace CastorCom
{
	static const Castor::String ERROR_UNINITIALISED = cuT( "The sampler must be initialised" );

	CTexture::CTexture()
	{
	}

	CTexture::~CTexture()
	{
	}

	STDMETHODIMP CTexture::Initialise()
	{
		HRESULT hr = E_POINTER;

		if ( m_internal )
		{
			hr = m_internal->Initialise() ? S_OK : E_FAIL;
		}
		else
		{
			hr = CComError::DispatchError(
					 E_FAIL,						// This represents the error
					 IID_ITexture,				// This is the GUID of component throwing error
					 cuT( "Initialise" ),			// This is generally displayed as the title
					 ERROR_UNINITIALISED.c_str(),	// This is the description
					 0,								// This is the context in the help file
					 NULL );
		}

		return hr;
	}

	STDMETHODIMP CTexture::Cleanup()
	{
		HRESULT hr = E_POINTER;

		if ( m_internal )
		{
			m_internal->Cleanup();
			hr = S_OK;
		}
		else
		{
			hr = CComError::DispatchError(
					 E_FAIL,						// This represents the error
					 IID_ITexture,				// This is the GUID of component throwing error
					 cuT( "Cleanup" ),				// This is generally displayed as the title
					 ERROR_UNINITIALISED.c_str(),	// This is the description
					 0,								// This is the context in the help file
					 NULL );
		}

		return hr;
	}
}