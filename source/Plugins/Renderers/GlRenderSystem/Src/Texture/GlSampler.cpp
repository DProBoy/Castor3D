#include "Texture/GlSampler.hpp"

#include "Common/OpenGl.hpp"
#include "Render/GlRenderSystem.hpp"

using namespace Castor3D;
using namespace Castor;

namespace GlRender
{
	GlSampler::GlSampler( OpenGl & p_gl, GlRenderSystem * renderSystem, Castor::String const & p_name )
		: Sampler( *renderSystem->GetEngine(), p_name )
		, ObjectType( p_gl,
					  "GlSampler",
					  std::bind( &OpenGl::GenSamplers, std::ref( p_gl ), std::placeholders::_1, std::placeholders::_2 ),
					  std::bind( &OpenGl::DeleteSamplers, std::ref( p_gl ), std::placeholders::_1, std::placeholders::_2 ),
					  std::bind( &OpenGl::IsSampler, std::ref( p_gl ), std::placeholders::_1 )
					)
	{
	}

	GlSampler::~GlSampler()
	{
	}

	bool GlSampler::Initialise()
	{
		bool result = IsValid();

		if ( !result )
		{
			result = ObjectType::Create();

			if ( result )
			{
				GlInterpolationMode minMode = GetOpenGl().Get( GetInterpolationMode( InterpolationFilter::eMin ) );
				GlInterpolationMode mipMode = GetOpenGl().Get( GetInterpolationMode( InterpolationFilter::eMip ) );
				DoAdjustMinMipModes( minMode, mipMode );

				GetOpenGl().SetSamplerParameter( GetGlName(), GlSamplerParameter::eLODBias, float( GetLodBias() ) );
				GetOpenGl().SetSamplerParameter( GetGlName(), GlSamplerParameter::eUWrap, int( GetOpenGl().Get( GetWrappingMode( TextureUVW::eU ) ) ) );
				GetOpenGl().SetSamplerParameter( GetGlName(), GlSamplerParameter::eVWrap, int( GetOpenGl().Get( GetWrappingMode( TextureUVW::eV ) ) ) );
				GetOpenGl().SetSamplerParameter( GetGlName(), GlSamplerParameter::eWWrap, int( GetOpenGl().Get( GetWrappingMode( TextureUVW::eW ) ) ) );
				GetOpenGl().SetSamplerParameter( GetGlName(), GlSamplerParameter::eMinFilter, int( minMode ) );
				GetOpenGl().SetSamplerParameter( GetGlName(), GlSamplerParameter::eMagFilter, int( GetOpenGl().Get( GetInterpolationMode( InterpolationFilter::eMag ) ) ) );
				GetOpenGl().SetSamplerParameter( GetGlName(), GlSamplerParameter::eBorderColour, GetBorderColour().const_ptr() );
				GetOpenGl().SetSamplerParameter( GetGlName(), GlSamplerParameter::eMaxAnisotropy, float( GetMaxAnisotropy() ) );
				GetOpenGl().SetSamplerParameter( GetGlName(), GlSamplerParameter::eCompareMode, int( GetOpenGl().Get( GetComparisonMode() ) ) );
				GetOpenGl().SetSamplerParameter( GetGlName(), GlSamplerParameter::eCompareFunc, int( GetOpenGl().Get( GetComparisonFunc() ) ) );
			}
		}

		return result;
	}

	void GlSampler::Cleanup()
	{
		ObjectType::Destroy();
	}

	void GlSampler::Bind( uint32_t p_index )const
	{
		glTrackSampler( GetGlName(), p_index );
		GetOpenGl().BindSampler( p_index, GetGlName() );
	}

	void GlSampler::Unbind( uint32_t p_index )const
	{
		GetOpenGl().BindSampler( p_index, 0u );
		glTrackSampler( 0u, p_index );
	}

	void GlSampler::DoAdjustMinMipModes( GlInterpolationMode & p_min, GlInterpolationMode & p_mip )
	{
		if ( p_mip != GlInterpolationMode::eNearest )
		{
			if ( p_min == GlInterpolationMode::eLinear )
			{
				if ( p_mip == GlInterpolationMode::eLinear )
				{
					p_min = GlInterpolationMode::eLinearMipmapLinear;
				}
				else
				{
					p_min = GlInterpolationMode::eLinearMipmapNearest;
				}
			}
			else
			{
				if ( p_mip == GlInterpolationMode::eLinear )
				{
					p_min = GlInterpolationMode::eNearestMipmapLinear;
				}
				else
				{
					p_min = GlInterpolationMode::eNearestMipmapNearest;
				}
			}
		}
	}
}
