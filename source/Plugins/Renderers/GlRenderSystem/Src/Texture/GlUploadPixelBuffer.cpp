#include "Texture/GlUploadPixelBuffer.hpp"

#include "Common/OpenGl.hpp"

using namespace Castor3D;
using namespace Castor;

namespace GlRender
{
	GlUploadPixelBuffer::GlUploadPixelBuffer( OpenGl & p_gl, GlRenderSystem * p_renderSystem, uint8_t * p_pixels, uint32_t p_pixelsSize )
		: GlGpuIoBuffer( p_gl, p_renderSystem, p_pixels, p_pixelsSize, GlBufferTarget::ePixelUnpack, BufferAccessType::Stream, BufferAccessNature::Draw )
	{
	}

	GlUploadPixelBuffer::GlUploadPixelBuffer( OpenGl & p_gl, GlRenderSystem * p_renderSystem, PxBufferBaseSPtr p_pixels )
		: GlGpuIoBuffer( p_gl, p_renderSystem, p_pixels->ptr(), p_pixels->size(), GlBufferTarget::ePixelUnpack, BufferAccessType::Stream, BufferAccessNature::Draw )
	{
	}

	GlUploadPixelBuffer::~GlUploadPixelBuffer()
	{
	}

	bool GlUploadPixelBuffer::Initialise()
	{
		return Fill( nullptr, m_pixelsSize );
	}
}
