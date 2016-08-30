#include "Texture/TestTexture.hpp"

#include "Render/TestRenderSystem.hpp"

#include <Log/Logger.hpp>

using namespace Castor3D;
using namespace Castor;

namespace TestRender
{
	TestTexture::TestTexture( TestRenderSystem & p_renderSystem, TextureType p_type, AccessType p_cpuAccess, AccessType p_gpuAccess )
		: TextureLayout{ p_renderSystem, p_type, p_cpuAccess, p_gpuAccess }
	{
	}

	TestTexture::~TestTexture()
	{
	}

	bool TestTexture::Create()
	{
		return true;
	}

	void TestTexture::Destroy()
	{
	}

	void TestTexture::GenerateMipmaps()const
	{
	}

	bool TestTexture::DoBind( uint32_t p_index )const
	{
		return true;
	}

	void TestTexture::DoUnbind( uint32_t p_index )const
	{
	}
}
