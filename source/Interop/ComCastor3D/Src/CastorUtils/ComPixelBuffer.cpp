#include "ComPixelBuffer.hpp"
#include "ComSize.hpp"
#include "ComUtils.hpp"

namespace CastorCom
{
	static const Castor::String ERROR_UNINITIALISED_BUFFER = cuT( "The buffer must be initialised" );
	static const Castor::String ERROR_INITIALISED_BUFFER = cuT( "The buffer is already initialised" );

	CPixelBuffer::CPixelBuffer()
	{
	}

	CPixelBuffer::~CPixelBuffer()
	{
	}

	STDMETHODIMP CPixelBuffer::Flip()
	{
		HRESULT hr = E_POINTER;

		if ( m_buffer )
		{
			m_buffer->flip();
			hr = S_OK;
		}
		else
		{
			hr = CComError::DispatchError(
					 E_FAIL,								// This represents the error
					 IID_IPixelBuffer,						// This is the GUID of component throwing error
					 cuT( "Flip" ),							// This is generally displayed as the title
					 ERROR_UNINITIALISED_BUFFER.c_str(),	// This is the description
					 0,										// This is the context in the help file
					 NULL );
		}

		return hr;
	}

	STDMETHODIMP CPixelBuffer::Mirror()
	{
		HRESULT hr = E_POINTER;

		if ( m_buffer )
		{
			m_buffer->mirror();
			hr = S_OK;
		}
		else
		{
			hr = CComError::DispatchError(
					 E_FAIL,								// This represents the error
					 IID_IPixelBuffer,						// This is the GUID of component throwing error
					 cuT( "Mirror" ),						// This is generally displayed as the title
					 ERROR_UNINITIALISED_BUFFER.c_str(),	// This is the description
					 0,										// This is the context in the help file
					 NULL );
		}

		return hr;
	}

	STDMETHODIMP CPixelBuffer::Initialise( /* [in] */ ISize * size, /* [in] */ ePIXEL_FORMAT format )
	{
		HRESULT hr = E_FAIL;

		if ( !m_buffer )
		{
			m_buffer = Castor::PxBufferBase::create( *static_cast< CSize * >( size ), Castor::ePIXEL_FORMAT( format ) );
			hr = S_OK;
		}
		else
		{
			hr = CComError::DispatchError(
					 E_FAIL,							// This represents the error
					 IID_IPixelBuffer,					// This is the GUID of component throwing error
					 cuT( "Initialise" ),				// This is generally displayed as the title
					 ERROR_INITIALISED_BUFFER.c_str(),	// This is the description
					 0,									// This is the context in the help file
					 NULL );
		}

		return hr;
	}
}