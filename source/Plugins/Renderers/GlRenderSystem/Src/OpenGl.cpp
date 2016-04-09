#include "OpenGl.hpp"

#include "GlRenderSystem.hpp"

#include <Rectangle.hpp>
#include <Logger.hpp>
#include <Utils.hpp>

#include <GlslWriter.hpp>

using namespace Castor3D;
using namespace Castor;

#define DEF_USE_DIRECT_STATE_ACCESS 0

namespace GlRender
{
	template< typename T >
	inline void GetFunction( T & p_function, String const & p_name, String const & p_extension )
	{
		if ( !gl_api::GetFunction( p_name, p_function ) )
		{
			if ( !gl_api::GetFunction( p_name + p_extension, p_function ) )
			{
				Logger::LogWarning( cuT( "Unable to retrieve function " ) + p_name );
			}
		}
	}

	//*************************************************************************************************

	TexFunctionsBase::TexFunctionsBase( OpenGl & p_gl )
		: Holder( p_gl )
	{
	}

	//*************************************************************************************************

	TexFunctions::TexFunctions( OpenGl & p_gl )
		: TexFunctionsBase( p_gl )
	{
		m_pfnBindTexture = &glBindTexture;
		m_pfnTexSubImage1D = &glTexSubImage1D;
		m_pfnTexSubImage2D = &glTexSubImage2D;
		m_pfnTexImage1D = &glTexImage1D;
		m_pfnTexImage2D = &glTexImage2D;
		m_pfnTexParameteri = &glTexParameteri;
		m_pfnTexParameterf = &glTexParameterf;
		m_pfnTexParameteriv = &glTexParameteriv;
		m_pfnTexParameterfv = &glTexParameterfv;
		m_pfnGetTexImage = &glGetTexImage;
		GetFunction( m_pfnTexImage3D, cuT( "glTexImage3D" ), cuT( "EXT" ) );
		GetFunction( m_pfnTexSubImage3D, cuT( "glTexSubImage3D" ), cuT( "EXT" ) );
		GetFunction( m_pfnGenerateMipmap, cuT( "glGenerateMipmap" ), cuT( "EXT" ) );
	}

	//*************************************************************************************************

	TexFunctionsDSA::TexFunctionsDSA( OpenGl & p_gl )
		: TexFunctionsBase( p_gl )
		, m_uiTexture( 0 )
		, m_pfnTextureSubImage1D()
		, m_pfnTextureSubImage2D()
		, m_pfnTextureSubImage3D()
		, m_pfnTextureImage1D()
		, m_pfnTextureImage2D()
		, m_pfnTextureImage3D()
	{
		GetFunction( m_pfnTextureSubImage1D, cuT( "glTextureSubImage1D" ), cuT( "EXT" ) );
		GetFunction( m_pfnTextureSubImage2D, cuT( "glTextureSubImage2D" ), cuT( "EXT" ) );
		GetFunction( m_pfnTextureSubImage3D, cuT( "glTextureSubImage3D" ), cuT( "EXT" ) );
		GetFunction( m_pfnTextureImage1D, cuT( "glTextureImage1D" ), cuT( "EXT" ) );
		GetFunction( m_pfnTextureImage2D, cuT( "glTextureImage2D" ), cuT( "EXT" ) );
		GetFunction( m_pfnTextureImage3D, cuT( "glTextureImage3D" ), cuT( "EXT" ) );
		GetFunction( m_pfnTextureParameteri, cuT( "glTextureParameteri" ), cuT( "EXT" ) );
		GetFunction( m_pfnTextureParameterf, cuT( "glTextureParameterf" ), cuT( "EXT" ) );
		GetFunction( m_pfnTextureParameteriv, cuT( "glTextureParameteriv" ), cuT( "EXT" ) );
		GetFunction( m_pfnTextureParameterfv, cuT( "glTextureParameterfv" ), cuT( "EXT" ) );
		GetFunction( m_pfnGetTextureImage, cuT( "glGetTextureImage" ), cuT( "EXT" ) );
		GetFunction( m_pfnGenerateTextureMipmap, cuT( "glGenerateTextureMipmap" ), cuT( "EXT" ) );
	}

	//*************************************************************************************************

	BufFunctionsBase::BufFunctionsBase( OpenGl & p_gl )
		: Holder( p_gl )
	{
		if ( p_gl.HasExtension( NV_shader_buffer_load ) )
		{
			GetFunction( m_pfnMakeBufferResident, cuT( "glMakeBufferResident" ), cuT( "NV" ) );
			GetFunction( m_pfnMakeBufferNonResident, cuT( "glMakeBufferNonResident" ), cuT( "NV" ) );
			GetFunction( m_pfnIsBufferResident, cuT( "glIsBufferResident" ), cuT( "NV" ) );
			GetFunction( m_pfnMakeNamedBufferResident, cuT( "glMakeNamedBufferResident" ), cuT( "NV" ) );
			GetFunction( m_pfnMakeNamedBufferNonResident, cuT( "glMakeNamedBufferNonResident" ), cuT( "NV" ) );
			GetFunction( m_pfnIsNamedBufferResident, cuT( "glIsNamedBufferResident" ), cuT( "NV" ) );
			GetFunction( m_pfnGetBufferParameterui64v, cuT( "glGetBufferParameterui64v" ), cuT( "NV" ) );
			GetFunction( m_pfnGetNamedBufferParameterui64v, cuT( "glGetNamedBufferParameterui64v" ), cuT( "NV" ) );
		}

		if ( p_gl.HasExtension( NV_vertex_buffer_unified_memory ) )
		{
			GetFunction( m_pfnBufferAddressRange, cuT( "glBufferAddressRange" ), cuT( "NV" ) );
			GetFunction( m_pfnVertexFormat, cuT( "glVertexFormat" ), cuT( "NV" ) );
			GetFunction( m_pfnNormalFormat, cuT( "glNormalFormat" ), cuT( "NV" ) );
			GetFunction( m_pfnColorFormat, cuT( "glColorFormat" ), cuT( "NV" ) );
			GetFunction( m_pfnIndexFormat, cuT( "glIndexFormat" ), cuT( "NV" ) );
			GetFunction( m_pfnTexCoordFormat, cuT( "glTexCoordFormat" ), cuT( "NV" ) );
			GetFunction( m_pfnEdgeFlagFormat, cuT( "glEdgeFlagFormat" ), cuT( "NV" ) );
			GetFunction( m_pfnSecondaryColorFormat, cuT( "glSecondaryColorFormat" ), cuT( "NV" ) );
			GetFunction( m_pfnFogCoordFormat, cuT( "glFogCoordFormat" ), cuT( "NV" ) );
			GetFunction( m_pfnVertexAttribFormat, cuT( "glVertexAttribFormat" ), cuT( "NV" ) );
			GetFunction( m_pfnVertexAttribIFormat, cuT( "glVertexAttribIFormat" ), cuT( "NV" ) );
		}
	}

	//*************************************************************************************************

	BufFunctions::BufFunctions( OpenGl & p_gl )
		: BufFunctionsBase( p_gl )
	{
		GetFunction( m_pfnBindBuffer, cuT( "glBindBuffer" ), cuT( "ARB" ) );
		GetFunction( m_pfnBufferData, cuT( "glBufferData" ), cuT( "ARB" ) );
		GetFunction( m_pfnBufferSubData, cuT( "glBufferSubData" ), cuT( "ARB" ) );
		GetFunction( m_pfnGetBufferParameteriv, cuT( "glGetBufferParameteriv" ), cuT( "ARB" ) );
		GetFunction( m_pfnMapBuffer, cuT( "glMapBuffer" ), cuT( "ARB" ) );
		GetFunction( m_pfnUnmapBuffer, cuT( "glUnmapBuffer" ), cuT( "ARB" ) );
		GetFunction( m_pfnMapBufferRange, cuT( "glMapBufferRange" ), cuT( "" ) );
		GetFunction( m_pfnFlushMappedBufferRange, cuT( "glFlushMappedBufferRange" ), cuT( "" ) );
	}

	//*************************************************************************************************

	BufFunctionsDSA::BufFunctionsDSA( OpenGl & p_gl )
		: BufFunctionsBase( p_gl )
		, m_uiBuffer( 0 )
	{
		GetFunction( m_pfnNamedBufferData, cuT( "glNamedBufferData" ), cuT( "EXT" ) );
		GetFunction( m_pfnNamedBufferSubData, cuT( "glNamedBufferSubData" ), cuT( "EXT" ) );
		GetFunction( m_pfnGetNamedBufferParameteriv, cuT( "glGetNamedBufferParameteriv" ), cuT( "EXT" ) );
		GetFunction( m_pfnMapNamedBuffer, cuT( "glMapNamedBuffer" ), cuT( "EXT" ) );
		GetFunction( m_pfnUnmapNamedBuffer, cuT( "glUnmapNamedBuffer" ), cuT( "EXT" ) );
		GetFunction( m_pfnMapNamedBufferRange, cuT( "glMapNamedBufferRange" ), cuT( "EXT" ) );
		GetFunction( m_pfnFlushMappedNamedBufferRange, cuT( "glFlushMappedNamedBufferRange" ), cuT( "EXT" ) );
	}

	//*************************************************************************************************

	#define CASTOR_DBG_WIN32 0

	OpenGl::OpenGl( GlRenderSystem & p_renderSystem )
		: m_bHasAnisotropic( false )
		, m_bHasInstancedDraw( false )
		, m_bHasInstancedArrays( false )
		, m_pTexFunctions( nullptr )
		, m_pBufFunctions( nullptr )
		, m_pfnReadPixels()
		, m_pfnBlitFramebuffer()
		, m_pfnTexImage2DMultisample()
		, m_pfnGetActiveUniform()
		, m_pfnVertexAttribPointer()
		, m_bBindVboToGpuAddress( false )
		, m_renderSystem( p_renderSystem )
	{
		uint32_t l_index = 0;
		GlslStrings[l_index++] = cuT( "[e00] GLSL is not available!" );
		GlslStrings[l_index++] = cuT( "[e01] Not a valid program object!" );
		GlslStrings[l_index++] = cuT( "[e02] Not a valid object!" );
		GlslStrings[l_index++] = cuT( "[e03] Out of memory!" );
		GlslStrings[l_index++] = cuT( "[e04] Unknown compiler error!" );
		GlslStrings[l_index++] = cuT( "[e05] Linker log is not available!" );
		GlslStrings[l_index++] = cuT( "[e06] Compiler log is not available!" );
		GlslStrings[l_index] = cuT( "[Empty]" );

		l_index = 0;
		GlslErrors[l_index++] = cuT( "[500] Invalid Enum !" );
		GlslErrors[l_index++] = cuT( "[501] Invalid Value !" );
		GlslErrors[l_index++] = cuT( "[502] Invalid Operation !" );
		GlslErrors[l_index++] = cuT( "[503] Stack Overflow !" );
		GlslErrors[l_index++] = cuT( "[504] Stack Underflow !" );
		GlslErrors[l_index++] = cuT( "[505] Out of memory !" );
		GlslErrors[l_index++] = cuT( "[506] Invalid frame buffer operation" );
		GlslErrors[l_index] = cuT( "[Empty] Unknown Error" );

		PrimitiveTypes[eTOPOLOGY_POINTS] = eGL_PRIMITIVE_POINTS;
		PrimitiveTypes[eTOPOLOGY_LINES] = eGL_PRIMITIVE_LINES;
		PrimitiveTypes[eTOPOLOGY_LINE_LOOP] = eGL_PRIMITIVE_LINE_LOOP;
		PrimitiveTypes[eTOPOLOGY_LINE_STRIP] = eGL_PRIMITIVE_LINE_STRIP;
		PrimitiveTypes[eTOPOLOGY_TRIANGLES] = eGL_PRIMITIVE_TRIANGLES;
		PrimitiveTypes[eTOPOLOGY_TRIANGLE_STRIPS] = eGL_PRIMITIVE_TRIANGLE_STRIP;
		PrimitiveTypes[eTOPOLOGY_TRIANGLE_FAN] = eGL_PRIMITIVE_TRIANGLE_FAN;
		PrimitiveTypes[eTOPOLOGY_QUADS] = eGL_PRIMITIVE_QUADS;
		PrimitiveTypes[eTOPOLOGY_QUAD_STRIPS] = eGL_PRIMITIVE_QUAD_STRIP;
		PrimitiveTypes[eTOPOLOGY_POLYGON] = eGL_PRIMITIVE_POLYGON;

		TextureDimensions[eTEXTURE_TYPE_BUFFER] = eGL_TEXDIM_BUFFER;
		TextureDimensions[eTEXTURE_TYPE_1D] = eGL_TEXDIM_1D;
		TextureDimensions[eTEXTURE_TYPE_1DARRAY] = eGL_TEXDIM_1D_ARRAY;
		TextureDimensions[eTEXTURE_TYPE_2D] = eGL_TEXDIM_2D;
		TextureDimensions[eTEXTURE_TYPE_2DARRAY] = eGL_TEXDIM_2D_ARRAY;
		TextureDimensions[eTEXTURE_TYPE_2DMS] = eGL_TEXDIM_2DMS;
		TextureDimensions[eTEXTURE_TYPE_2DMSARRAY] = eGL_TEXDIM_2DMS_ARRAY;
		TextureDimensions[eTEXTURE_TYPE_3D] = eGL_TEXDIM_3D;
		TextureDimensions[eTEXTURE_TYPE_CUBE] = eGL_TEXDIM_CUBE;
		TextureDimensions[eTEXTURE_TYPE_CUBEARRAY] = eGL_TEXDIM_CUBE_ARRAY;

		AlphaFuncs[eALPHA_FUNC_ALWAYS] = eGL_FUNC_ALWAYS;
		AlphaFuncs[eALPHA_FUNC_LESS] = eGL_FUNC_LESS;
		AlphaFuncs[eALPHA_FUNC_LESS_OR_EQUAL] = eGL_FUNC_LEQUAL;
		AlphaFuncs[eALPHA_FUNC_EQUAL] = eGL_FUNC_EQUAL;
		AlphaFuncs[eALPHA_FUNC_NOT_EQUAL] = eGL_FUNC_NOTEQUAL;
		AlphaFuncs[eALPHA_FUNC_GREATER_OR_EQUAL] = eGL_FUNC_GEQUAL;
		AlphaFuncs[eALPHA_FUNC_GREATER] = eGL_FUNC_GREATER;
		AlphaFuncs[eALPHA_FUNC_NEVER] = eGL_FUNC_NEVER;

		TextureWrapMode[eWRAP_MODE_REPEAT] = eGL_WRAP_MODE_REPEAT;
		TextureWrapMode[eWRAP_MODE_MIRRORED_REPEAT] = eGL_WRAP_MODE_MIRRORED_REPEAT;
		TextureWrapMode[eWRAP_MODE_CLAMP_TO_BORDER] = eGL_WRAP_MODE_CLAMP_TO_BORDER;
		TextureWrapMode[eWRAP_MODE_CLAMP_TO_EDGE] = eGL_WRAP_MODE_CLAMP_TO_EDGE;

		TextureInterpolation[eINTERPOLATION_MODE_UNDEFINED] = eGL_INTERPOLATION_MODE_NEAREST;
		TextureInterpolation[eINTERPOLATION_MODE_NEAREST] = eGL_INTERPOLATION_MODE_NEAREST;
		TextureInterpolation[eINTERPOLATION_MODE_LINEAR] = eGL_INTERPOLATION_MODE_LINEAR;

		BlendFactors[eBLEND_ZERO] = eGL_BLEND_FACTOR_ZERO;
		BlendFactors[eBLEND_ONE] = eGL_BLEND_FACTOR_ONE;
		BlendFactors[eBLEND_SRC_COLOUR] = eGL_BLEND_FACTOR_SRC_COLOR;
		BlendFactors[eBLEND_INV_SRC_COLOUR] = eGL_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		BlendFactors[eBLEND_DST_COLOUR] = eGL_BLEND_FACTOR_DST_COLOR;
		BlendFactors[eBLEND_INV_DST_COLOUR] = eGL_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		BlendFactors[eBLEND_SRC_ALPHA] = eGL_BLEND_FACTOR_SRC_ALPHA;
		BlendFactors[eBLEND_INV_SRC_ALPHA] = eGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		BlendFactors[eBLEND_DST_ALPHA] = eGL_BLEND_FACTOR_DST_ALPHA;
		BlendFactors[eBLEND_INV_DST_ALPHA] = eGL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		BlendFactors[eBLEND_CONSTANT] = eGL_BLEND_FACTOR_CONSTANT;
		BlendFactors[eBLEND_INV_CONSTANT] = eGL_BLEND_FACTOR_ONE_MINUS_CONSTANT;
		BlendFactors[eBLEND_SRC_ALPHA_SATURATE] = eGL_BLEND_FACTOR_SRC_ALPHA_SATURATE;
		BlendFactors[eBLEND_SRC1_COLOUR] = eGL_BLEND_FACTOR_SRC1_COLOR;
		BlendFactors[eBLEND_INV_SRC1_COLOUR] = eGL_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
		BlendFactors[eBLEND_SRC1_ALPHA] = eGL_BLEND_FACTOR_SRC1_ALPHA;
		BlendFactors[eBLEND_INV_SRC1_ALPHA] = eGL_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;

		TextureArguments[eBLEND_SOURCE_TEXTURE] = eGL_BLEND_SOURCE_TEXTURE;
		TextureArguments[eBLEND_SOURCE_TEXTURE0] = eGL_BLEND_SOURCE_TEXTURE0;
		TextureArguments[eBLEND_SOURCE_TEXTURE1] = eGL_BLEND_SOURCE_TEXTURE1;
		TextureArguments[eBLEND_SOURCE_TEXTURE2] = eGL_BLEND_SOURCE_TEXTURE2;
		TextureArguments[eBLEND_SOURCE_TEXTURE3] = eGL_BLEND_SOURCE_TEXTURE3;
		TextureArguments[eBLEND_SOURCE_CONSTANT] = eGL_BLEND_SOURCE_CONSTANT;
		TextureArguments[eBLEND_SOURCE_DIFFUSE] = eGL_BLEND_SOURCE_PRIMARY_COLOR;
		TextureArguments[eBLEND_SOURCE_PREVIOUS] = eGL_BLEND_SOURCE_PREVIOUS;

		RgbBlendFuncs[eRGB_BLEND_FUNC_NONE] = eGL_BLEND_FUNC_MODULATE;
		RgbBlendFuncs[eRGB_BLEND_FUNC_FIRST_ARG] = eGL_BLEND_FUNC_REPLACE;
		RgbBlendFuncs[eRGB_BLEND_FUNC_ADD] = eGL_BLEND_FUNC_ADD;
		RgbBlendFuncs[eRGB_BLEND_FUNC_ADD_SIGNED] = eGL_BLEND_FUNC_ADD_SIGNED;
		RgbBlendFuncs[eRGB_BLEND_FUNC_MODULATE] = eGL_BLEND_FUNC_MODULATE;
		RgbBlendFuncs[eRGB_BLEND_FUNC_INTERPOLATE] = eGL_BLEND_FUNC_INTERPOLATE;
		RgbBlendFuncs[eRGB_BLEND_FUNC_SUBTRACT] = eGL_BLEND_FUNC_SUBTRACT;
		RgbBlendFuncs[eRGB_BLEND_FUNC_DOT3_RGB] = eGL_BLEND_FUNC_DOT3_RGB;
		RgbBlendFuncs[eRGB_BLEND_FUNC_DOT3_RGBA] = eGL_BLEND_FUNC_DOT3_RGBA;

		AlphaBlendFuncs[eALPHA_BLEND_FUNC_NONE] = eGL_BLEND_FUNC_MODULATE;
		AlphaBlendFuncs[eALPHA_BLEND_FUNC_FIRST_ARG] = eGL_BLEND_FUNC_REPLACE;
		AlphaBlendFuncs[eALPHA_BLEND_FUNC_ADD] = eGL_BLEND_FUNC_ADD;
		AlphaBlendFuncs[eALPHA_BLEND_FUNC_ADD_SIGNED] = eGL_BLEND_FUNC_ADD_SIGNED;
		AlphaBlendFuncs[eALPHA_BLEND_FUNC_MODULATE] = eGL_BLEND_FUNC_MODULATE;
		AlphaBlendFuncs[eALPHA_BLEND_FUNC_INTERPOLATE] = eGL_BLEND_FUNC_INTERPOLATE;
		AlphaBlendFuncs[eALPHA_BLEND_FUNC_SUBSTRACT] = eGL_BLEND_FUNC_SUBTRACT;

		BlendOps[eBLEND_OP_ADD] = eGL_BLEND_OP_ADD;
		BlendOps[eBLEND_OP_SUBSTRACT] = eGL_BLEND_OP_SUBTRACT;
		BlendOps[eBLEND_OP_REV_SUBSTRACT] = eGL_BLEND_OP_REV_SUBTRACT;
		BlendOps[eBLEND_OP_MIN] = eGL_BLEND_OP_MIN;
		BlendOps[eBLEND_OP_MAX] = eGL_BLEND_OP_MAX;

		PixelFormats[ePIXEL_FORMAT_L8] = PixelFmt( eGL_FORMAT_RED, eGL_INTERNAL_R8, eGL_TYPE_UNSIGNED_BYTE );
		PixelFormats[ePIXEL_FORMAT_L16F32F] = PixelFmt( eGL_FORMAT_RED, eGL_INTERNAL_R16F, eGL_TYPE_FLOAT );
		PixelFormats[ePIXEL_FORMAT_L32F] = PixelFmt( eGL_FORMAT_RED, eGL_INTERNAL_R32F, eGL_TYPE_FLOAT );
		PixelFormats[ePIXEL_FORMAT_A8L8] = PixelFmt( eGL_FORMAT_RG, eGL_INTERNAL_RG16, eGL_TYPE_UNSIGNED_BYTE );
		PixelFormats[ePIXEL_FORMAT_AL16F32F] = PixelFmt( eGL_FORMAT_RG, eGL_INTERNAL_RG16F, eGL_TYPE_FLOAT );
		PixelFormats[ePIXEL_FORMAT_AL32F] = PixelFmt( eGL_FORMAT_RG, eGL_INTERNAL_RG32F, eGL_TYPE_FLOAT );
		PixelFormats[ePIXEL_FORMAT_A1R5G5B5] = PixelFmt( eGL_FORMAT_BGRA, eGL_INTERNAL_RGB5_A1, eGL_TYPE_UNSIGNED_SHORT_1_5_5_5_REV );
		PixelFormats[ePIXEL_FORMAT_A4R4G4B4] = PixelFmt( eGL_FORMAT_BGRA, eGL_INTERNAL_RGBA4, eGL_TYPE_UNSIGNED_SHORT_4_4_4_4_REV );
		PixelFormats[ePIXEL_FORMAT_R5G6B5] = PixelFmt( eGL_FORMAT_BGR, eGL_INTERNAL_RGB16, eGL_TYPE_UNSIGNED_SHORT_5_6_5_REV );
		PixelFormats[ePIXEL_FORMAT_R8G8B8] = PixelFmt( eGL_FORMAT_BGR, eGL_INTERNAL_RGB8, eGL_TYPE_UNSIGNED_BYTE );
		PixelFormats[ePIXEL_FORMAT_B8G8R8] = PixelFmt( eGL_FORMAT_RGB, eGL_INTERNAL_RGB8, eGL_TYPE_UNSIGNED_BYTE );
		PixelFormats[ePIXEL_FORMAT_A8R8G8B8] = PixelFmt( eGL_FORMAT_BGRA, eGL_INTERNAL_RGBA8, eGL_TYPE_UNSIGNED_BYTE );
		PixelFormats[ePIXEL_FORMAT_A8B8G8R8] = PixelFmt( eGL_FORMAT_RGBA, eGL_INTERNAL_RGBA8, eGL_TYPE_UNSIGNED_BYTE );
		PixelFormats[ePIXEL_FORMAT_RGB16F] = PixelFmt( eGL_FORMAT_BGR, eGL_INTERNAL_RGB16F, eGL_TYPE_HALF_FLOAT );
		PixelFormats[ePIXEL_FORMAT_ARGB16F] = PixelFmt( eGL_FORMAT_BGRA, eGL_INTERNAL_RGBA16F, eGL_TYPE_HALF_FLOAT );
		PixelFormats[ePIXEL_FORMAT_RGB16F32F] = PixelFmt( eGL_FORMAT_BGR, eGL_INTERNAL_RGB16F, eGL_TYPE_FLOAT );
		PixelFormats[ePIXEL_FORMAT_ARGB16F32F] = PixelFmt( eGL_FORMAT_BGRA, eGL_INTERNAL_RGBA16F, eGL_TYPE_FLOAT );
		PixelFormats[ePIXEL_FORMAT_RGB32F] = PixelFmt( eGL_FORMAT_BGR, eGL_INTERNAL_RGB32F, eGL_TYPE_FLOAT );
		PixelFormats[ePIXEL_FORMAT_ARGB32F] = PixelFmt( eGL_FORMAT_BGRA, eGL_INTERNAL_RGBA32F, eGL_TYPE_FLOAT );
		PixelFormats[ePIXEL_FORMAT_DXTC1] = PixelFmt( eGL_FORMAT_BGR, eGL_INTERNAL_COMPRESSED_RGBA_S3TC_DXT1, eGL_TYPE_DEFAULT );
		PixelFormats[ePIXEL_FORMAT_DXTC3] = PixelFmt( eGL_FORMAT_BGRA, eGL_INTERNAL_COMPRESSED_RGBA_S3TC_DXT3, eGL_TYPE_DEFAULT );
		PixelFormats[ePIXEL_FORMAT_DXTC5] = PixelFmt( eGL_FORMAT_RGBA, eGL_INTERNAL_COMPRESSED_RGBA_S3TC_DXT5, eGL_TYPE_DEFAULT );
		PixelFormats[ePIXEL_FORMAT_YUY2] = PixelFmt( eGL_FORMAT( 0 ), eGL_INTERNAL( 0 ), eGL_TYPE_DEFAULT );
		PixelFormats[ePIXEL_FORMAT_DEPTH16] = PixelFmt( eGL_FORMAT_DEPTH, eGL_INTERNAL_DEPTH_COMPONENT16, eGL_TYPE_UNSIGNED_BYTE );
		PixelFormats[ePIXEL_FORMAT_DEPTH24] = PixelFmt( eGL_FORMAT_DEPTH, eGL_INTERNAL_DEPTH_COMPONENT24, eGL_TYPE_UNSIGNED_BYTE );
		PixelFormats[ePIXEL_FORMAT_DEPTH24S8] = PixelFmt( eGL_FORMAT_DEPTH_STENCIL, eGL_INTERNAL_DEPTH24_STENCIL8, eGL_TYPE_UNSIGNED_INT_24_8 );
		PixelFormats[ePIXEL_FORMAT_DEPTH32] = PixelFmt( eGL_FORMAT_DEPTH, eGL_INTERNAL_DEPTH_COMPONENT32, eGL_TYPE_UNSIGNED_BYTE );
		PixelFormats[ePIXEL_FORMAT_DEPTH32F] = PixelFmt( eGL_FORMAT_DEPTH, eGL_INTERNAL_DEPTH_COMPONENT32, eGL_TYPE_FLOAT );
		PixelFormats[ePIXEL_FORMAT_STENCIL1] = PixelFmt( eGL_FORMAT_STENCIL, eGL_INTERNAL_STENCIL_INDEX1, eGL_TYPE_UNSIGNED_BYTE );
		PixelFormats[ePIXEL_FORMAT_STENCIL8] = PixelFmt( eGL_FORMAT_STENCIL, eGL_INTERNAL_STENCIL_INDEX8, eGL_TYPE_UNSIGNED_BYTE );

		ShaderTypes[eSHADER_TYPE_VERTEX] = eGL_SHADER_TYPE_VERTEX;
		ShaderTypes[eSHADER_TYPE_HULL] = eGL_SHADER_TYPE_TESS_CONTROL;
		ShaderTypes[eSHADER_TYPE_DOMAIN] = eGL_SHADER_TYPE_TESS_EVALUATION;
		ShaderTypes[eSHADER_TYPE_GEOMETRY] = eGL_SHADER_TYPE_GEOMETRY;
		ShaderTypes[eSHADER_TYPE_PIXEL] = eGL_SHADER_TYPE_FRAGMENT;
		ShaderTypes[eSHADER_TYPE_COMPUTE] = eGL_SHADER_TYPE_COMPUTE;

		Internals[ePIXEL_FORMAT_L8] = eGL_INTERNAL_FORMAT_R16;
		Internals[ePIXEL_FORMAT_L16F32F] = eGL_INTERNAL_FORMAT_R16F;
		Internals[ePIXEL_FORMAT_L32F] = eGL_INTERNAL_FORMAT_R32F;
		Internals[ePIXEL_FORMAT_A8L8] = eGL_INTERNAL_FORMAT_RG16;
		Internals[ePIXEL_FORMAT_AL16F32F] = eGL_INTERNAL_FORMAT_RG16F;
		Internals[ePIXEL_FORMAT_AL32F] = eGL_INTERNAL_FORMAT_RG32F;
		Internals[ePIXEL_FORMAT_A1R5G5B5] = eGL_INTERNAL_FORMAT_RGBA16UI;
		Internals[ePIXEL_FORMAT_A4R4G4B4] = eGL_INTERNAL_FORMAT_RGBA16UI;
		Internals[ePIXEL_FORMAT_R5G6B5] = eGL_INTERNAL_FORMAT_RGB16UI;
		Internals[ePIXEL_FORMAT_R8G8B8] = eGL_INTERNAL_FORMAT_RGBA32UI;
		Internals[ePIXEL_FORMAT_B8G8R8] = eGL_INTERNAL_FORMAT_RGBA32UI;
		Internals[ePIXEL_FORMAT_A8R8G8B8] = eGL_INTERNAL_FORMAT_RGBA32UI;
		Internals[ePIXEL_FORMAT_A8B8G8R8] = eGL_INTERNAL_FORMAT_RGBA32UI;
		Internals[ePIXEL_FORMAT_RGB16F] = eGL_INTERNAL_FORMAT_RGBA16F;
		Internals[ePIXEL_FORMAT_ARGB16F] = eGL_INTERNAL_FORMAT_RGBA16F;
		Internals[ePIXEL_FORMAT_RGB16F32F] = eGL_INTERNAL_FORMAT_RGBA16F;
		Internals[ePIXEL_FORMAT_ARGB16F32F] = eGL_INTERNAL_FORMAT_RGBA16F;
		Internals[ePIXEL_FORMAT_RGB32F] = eGL_INTERNAL_FORMAT_RGBA32F;
		Internals[ePIXEL_FORMAT_ARGB32F] = eGL_INTERNAL_FORMAT_RGBA32F;

		Attachments[eATTACHMENT_POINT_NONE] = eGL_TEXTURE_ATTACHMENT_NONE;
		Attachments[eATTACHMENT_POINT_COLOUR] = eGL_TEXTURE_ATTACHMENT_COLOR0;
		Attachments[eATTACHMENT_POINT_DEPTH] = eGL_TEXTURE_ATTACHMENT_DEPTH;
		Attachments[eATTACHMENT_POINT_STENCIL] = eGL_TEXTURE_ATTACHMENT_STENCIL;

		FramebufferModes[eFRAMEBUFFER_TARGET_DRAW] = eGL_FRAMEBUFFER_MODE_DRAW;
		FramebufferModes[eFRAMEBUFFER_TARGET_READ] = eGL_FRAMEBUFFER_MODE_READ;
		FramebufferModes[eFRAMEBUFFER_TARGET_BOTH] = eGL_FRAMEBUFFER_MODE_DEFAULT;

		RboAttachments[eATTACHMENT_POINT_NONE] = eGL_RENDERBUFFER_ATTACHMENT_NONE;
		RboAttachments[eATTACHMENT_POINT_COLOUR] = eGL_RENDERBUFFER_ATTACHMENT_COLOR0;
		RboAttachments[eATTACHMENT_POINT_DEPTH] = eGL_RENDERBUFFER_ATTACHMENT_DEPTH;
		RboAttachments[eATTACHMENT_POINT_STENCIL] = eGL_RENDERBUFFER_ATTACHMENT_STENCIL;

		RboStorages[ePIXEL_FORMAT_L8] = eGL_RENDERBUFFER_STORAGE_L8;
		RboStorages[ePIXEL_FORMAT_L16F32F] = eGL_RENDERBUFFER_STORAGE_L16F;
		RboStorages[ePIXEL_FORMAT_L32F] = eGL_RENDERBUFFER_STORAGE_L32F;
		RboStorages[ePIXEL_FORMAT_A8L8] = eGL_RENDERBUFFER_STORAGE_A8L8;
		RboStorages[ePIXEL_FORMAT_AL16F32F] = eGL_RENDERBUFFER_STORAGE_AL16F;
		RboStorages[ePIXEL_FORMAT_AL32F] = eGL_RENDERBUFFER_STORAGE_AL32F;
		RboStorages[ePIXEL_FORMAT_A1R5G5B5] = eGL_RENDERBUFFER_STORAGE_RGB5_A1;
		RboStorages[ePIXEL_FORMAT_A4R4G4B4] = eGL_RENDERBUFFER_STORAGE_RGBA4;
		RboStorages[ePIXEL_FORMAT_R5G6B5] = eGL_RENDERBUFFER_STORAGE_RGB565;
		RboStorages[ePIXEL_FORMAT_R8G8B8] = eGL_RENDERBUFFER_STORAGE_RGB8;
		RboStorages[ePIXEL_FORMAT_B8G8R8] = eGL_RENDERBUFFER_STORAGE_RGB8;
		RboStorages[ePIXEL_FORMAT_A8R8G8B8] = eGL_RENDERBUFFER_STORAGE_RGBA8;
		RboStorages[ePIXEL_FORMAT_A8B8G8R8] = eGL_RENDERBUFFER_STORAGE_RGBA8;
		RboStorages[ePIXEL_FORMAT_RGB16F] = eGL_RENDERBUFFER_STORAGE_RGB16F;
		RboStorages[ePIXEL_FORMAT_ARGB16F] = eGL_RENDERBUFFER_STORAGE_RGBA16F;
		RboStorages[ePIXEL_FORMAT_RGB16F32F] = eGL_RENDERBUFFER_STORAGE_RGB16F;
		RboStorages[ePIXEL_FORMAT_ARGB16F32F] = eGL_RENDERBUFFER_STORAGE_RGBA16F;
		RboStorages[ePIXEL_FORMAT_RGB32F] = eGL_RENDERBUFFER_STORAGE_RGB32F;
		RboStorages[ePIXEL_FORMAT_ARGB32F] = eGL_RENDERBUFFER_STORAGE_RGBA32F;
		RboStorages[ePIXEL_FORMAT_DXTC1] = eGL_RENDERBUFFER_STORAGE_DXTC1;
		RboStorages[ePIXEL_FORMAT_DXTC3] = eGL_RENDERBUFFER_STORAGE_DXTC3;
		RboStorages[ePIXEL_FORMAT_DXTC5] = eGL_RENDERBUFFER_STORAGE_DXTC5;
		RboStorages[ePIXEL_FORMAT_YUY2] = eGL_RENDERBUFFER_STORAGE( 0 );
		RboStorages[ePIXEL_FORMAT_DEPTH16] = eGL_RENDERBUFFER_STORAGE_DEPTH16;
		RboStorages[ePIXEL_FORMAT_DEPTH24] = eGL_RENDERBUFFER_STORAGE_DEPTH24;
		RboStorages[ePIXEL_FORMAT_DEPTH24S8] = eGL_RENDERBUFFER_STORAGE_DEPTH24;
		RboStorages[ePIXEL_FORMAT_DEPTH32] = eGL_RENDERBUFFER_STORAGE_DEPTH32;
		RboStorages[ePIXEL_FORMAT_DEPTH32F] = eGL_RENDERBUFFER_STORAGE_DEPTH32F;
		RboStorages[ePIXEL_FORMAT_STENCIL1] = eGL_RENDERBUFFER_STORAGE_STENCIL1;
		RboStorages[ePIXEL_FORMAT_STENCIL8] = eGL_RENDERBUFFER_STORAGE_STENCIL8;

		Buffers[eBUFFER_NONE] = eGL_BUFFER_NONE;
		Buffers[eBUFFER_FRONT_LEFT] = eGL_BUFFER_FRONT_LEFT;
		Buffers[eBUFFER_FRONT_RIGHT] = eGL_BUFFER_FRONT_RIGHT;
		Buffers[eBUFFER_BACK_LEFT] = eGL_BUFFER_BACK_LEFT;
		Buffers[eBUFFER_BACK_RIGHT] = eGL_BUFFER_BACK_RIGHT;
		Buffers[eBUFFER_FRONT] = eGL_BUFFER_FRONT;
		Buffers[eBUFFER_BACK] = eGL_BUFFER_BACK;
		Buffers[eBUFFER_LEFT] = eGL_BUFFER_LEFT;
		Buffers[eBUFFER_RIGHT] = eGL_BUFFER_RIGHT;
		Buffers[eBUFFER_FRONT_AND_BACK] = eGL_BUFFER_FRONT_AND_BACK;

		Faces[eFACE_NONE] = eGL_FACE( 0 );
		Faces[eFACE_FRONT] = eGL_FACE_FRONT;
		Faces[eFACE_BACK] = eGL_FACE_BACK;
		Faces[eFACE_FRONT_AND_BACK] = eGL_FACE_FRONT_AND_BACK;

		FillModes[eFILL_MODE_POINT] = eGL_FILL_MODE_POINT;
		FillModes[eFILL_MODE_LINE] = eGL_FILL_MODE_LINE;
		FillModes[eFILL_MODE_SOLID] = eGL_FILL_MODE_FILL;

		DepthFuncs[eDEPTH_FUNC_NEVER] = eGL_FUNC_NEVER;
		DepthFuncs[eDEPTH_FUNC_LESS] = eGL_FUNC_LESS;
		DepthFuncs[eDEPTH_FUNC_EQUAL] = eGL_FUNC_LEQUAL;
		DepthFuncs[eDEPTH_FUNC_LEQUAL] = eGL_FUNC_EQUAL;
		DepthFuncs[eDEPTH_FUNC_GREATER] = eGL_FUNC_NOTEQUAL;
		DepthFuncs[eDEPTH_FUNC_NOTEQUAL] = eGL_FUNC_GEQUAL;
		DepthFuncs[eDEPTH_FUNC_GEQUAL] = eGL_FUNC_GREATER;
		DepthFuncs[eDEPTH_FUNC_ALWAYS] = eGL_FUNC_ALWAYS;

		WriteMasks[eWRITING_MASK_ZERO] = false;
		WriteMasks[eWRITING_MASK_ALL] = true;

		StencilFuncs[eSTENCIL_FUNC_NEVER] = eGL_FUNC_NEVER;
		StencilFuncs[eSTENCIL_FUNC_LESS] = eGL_FUNC_LESS;
		StencilFuncs[eSTENCIL_FUNC_EQUAL] = eGL_FUNC_LEQUAL;
		StencilFuncs[eSTENCIL_FUNC_LEQUAL] = eGL_FUNC_EQUAL;
		StencilFuncs[eSTENCIL_FUNC_GREATER] = eGL_FUNC_NOTEQUAL;
		StencilFuncs[eSTENCIL_FUNC_NOTEQUAL] = eGL_FUNC_GEQUAL;
		StencilFuncs[eSTENCIL_FUNC_GEQUAL] = eGL_FUNC_GREATER;
		StencilFuncs[eSTENCIL_FUNC_ALWAYS] = eGL_FUNC_ALWAYS;

		StencilOps[eSTENCIL_OP_KEEP] = eGL_STENCIL_OP_KEEP;
		StencilOps[eSTENCIL_OP_ZERO] = eGL_STENCIL_OP_ZERO;
		StencilOps[eSTENCIL_OP_REPLACE] = eGL_STENCIL_OP_REPLACE;
		StencilOps[eSTENCIL_OP_INCR] = eGL_STENCIL_OP_INCR;
		StencilOps[eSTENCIL_OP_INCR_WRAP] = eGL_STENCIL_OP_INCR_WRAP;
		StencilOps[eSTENCIL_OP_DECR] = eGL_STENCIL_OP_DECR;
		StencilOps[eSTENCIL_OP_DECR_WRAP] = eGL_STENCIL_OP_DECR_WRAP;
		StencilOps[eSTENCIL_OP_INVERT] = eGL_STENCIL_OP_INVERT;

		BuffersTA[eGL_TEXTURE_ATTACHMENT_NONE] = eGL_BUFFER_NONE;
		BuffersTA[eGL_TEXTURE_ATTACHMENT_COLOR0] = eGL_BUFFER_COLOR0;

		BuffersRBA[eGL_RENDERBUFFER_ATTACHMENT_NONE] = eGL_BUFFER_NONE;
		BuffersRBA[eGL_RENDERBUFFER_ATTACHMENT_COLOR0] = eGL_BUFFER_COLOR0;

		Queries[eQUERY_TYPE_TIME_ELAPSED] = eGL_QUERY_TIME_ELAPSED;
		Queries[eQUERY_TYPE_SAMPLES_PASSED] = eGL_QUERY_SAMPLES_PASSED;
		Queries[eQUERY_TYPE_ANY_SAMPLES_PASSED] = eGL_QUERY_ANY_SAMPLES_PASSED;
		Queries[eQUERY_TYPE_PRIMITIVES_GENERATED] = eGL_QUERY_PRIMITIVES_GENERATED;
		Queries[eQUERY_TYPE_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN] = eGL_QUERY_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN;
		Queries[eQUERY_TYPE_ANY_SAMPLES_PASSED_CONSERVATIVE] = eGL_QUERY_ANY_SAMPLES_PASSED_CONSERVATIVE;
		Queries[eQUERY_TYPE_TIMESTAMP];

		QueryInfos[eQUERY_INFO_RESULT] = eGL_QUERY_INFO_RESULT;
		QueryInfos[eQUERY_INFO_RESULT_AVAILABLE] = eGL_QUERY_INFO_RESULT_AVAILABLE;
		QueryInfos[eQUERY_INFO_RESULT_NO_WAIT] = eGL_QUERY_INFO_RESULT_NO_WAIT;

		Cleanup();

	#if defined( _WIN32 )

		m_pfnMakeCurrent = &wglMakeCurrent;
		m_pfnSwapBuffers = &::SwapBuffers;
		m_pfnCreateContext = &wglCreateContext;
		m_pfnDeleteContext = &wglDeleteContext;

	#elif defined( __linux__ )

		m_pfnMakeCurrent = &glXMakeCurrent;
		m_pfnSwapBuffers = &glXSwapBuffers;
		m_pfnCreateContext = &glXCreateContext;
		m_pfnDeleteContext = &glXDestroyContext;

	#else

	#	error "Yet unsupported OS"

	#endif
	}

	OpenGl::~OpenGl()
	{
	}

	bool OpenGl::PreInitialise( String const & p_strExtensions )
	{
		char const * l_extensions = ( char const * )glGetString( GL_EXTENSIONS );
		m_extensions = string::string_cast< xchar >( l_extensions ) + p_strExtensions;
		m_vendor = string::string_cast< xchar >( ( char const * )glGetString( GL_VENDOR ) );
		m_renderer = string::string_cast< xchar >( ( char const * )glGetString( GL_RENDERER ) );
		m_version = string::string_cast< xchar >( ( char const * )glGetString( GL_VERSION ) );
		double l_version;
		StringStream l_stream( m_version );
		l_stream >> l_version;
		m_iVersion = int( l_version * 10 );

		if ( m_iVersion >= 43 )
		{
			m_iGlslVersion = 430;
		}
		else if ( m_iVersion >= 42 )
		{
			m_iGlslVersion = 420;
		}
		else if ( m_iVersion >= 41 )
		{
			m_iGlslVersion = 410;
		}
		else if ( m_iVersion >= 40 )
		{
			m_iGlslVersion = 400;
		}
		else if ( m_iVersion >= 33 )
		{
			m_iGlslVersion = 330;
		}
		else if ( m_iVersion >= 32 )
		{
			m_iGlslVersion = 150;
		}
		else if ( m_iVersion >= 31 )
		{
			m_iGlslVersion = 140;
		}
		else if ( m_iVersion >= 30 )
		{
			m_iGlslVersion = 130;
		}
		else if ( m_iVersion >= 21 )
		{
			m_iGlslVersion = 120;
		}
		else if ( m_iVersion >= 20 )
		{
			m_iGlslVersion = 110;
		}

#if defined( _WIN32 )

		if ( HasExtension( ARB_create_context ) )
		{
			gl_api::GetFunction( cuT( "wglCreateContextAttribsARB" ), m_pfnCreateContextAttribs );
		}

#elif defined( __linux__ )

		if ( HasExtension( ARB_create_context ) )
		{
			gl_api::GetFunction( cuT( "glXCreateContextAttribsARB" ), m_pfnCreateContextAttribs );
		}
		else
		{
			gl_api::GetFunction( cuT( "glXCreateContextAttribsARB" ), m_pfnCreateContextAttribs );
		}

#else

#	error "Yet unsupported OS"

#endif

		return true;
	}

	bool OpenGl::Initialise()
	{
		m_pfnGetError = &glGetError;
		m_pfnClearColor = &glClearColor;
		m_pfnShadeModel = &glShadeModel;
		m_pfnViewport = &glViewport;
		m_pfnClear = &glClear;
		m_pfnDrawArrays = &glDrawArrays;
		m_pfnDrawElements = &glDrawElements;
		m_pfnEnable = &glEnable;
		m_pfnDisable = &glDisable;
		m_pfnGenTextures = &glGenTextures;
		m_pfnDeleteTextures = &glDeleteTextures;
		m_pfnVertexPointer = &glVertexPointer;
		m_pfnColorPointer = &glColorPointer;
		m_pfnNormalPointer = &glNormalPointer;
		m_pfnTexCoordPointer = &glTexCoordPointer;
		m_pfnTexEnvi = &glTexEnvi;
		m_pfnTexEnviv = &glTexEnviv;
		m_pfnTexEnvf = &glTexEnvf;
		m_pfnTexEnvfv = &glTexEnvfv;
		m_pfnBlendFunc = &glBlendFunc;
		m_pfnAlphaFunc = &glAlphaFunc;
		m_pfnIsTexture = &glIsTexture;
		m_pfnTexGeni = &glTexGeni;
		m_pfnCullFace = &glCullFace;
		m_pfnFrontFace = &glFrontFace;
		m_pfnMaterialf = &glMaterialf;
		m_pfnMaterialfv = &glMaterialfv;
		m_pfnSelectBuffer = &glSelectBuffer;
		m_pfnGetIntegerv = &glGetIntegerv;
		m_pfnRenderMode = &glRenderMode;
		m_pfnReadBuffer = &glReadBuffer;
		m_pfnReadPixels = &glReadPixels;
		m_pfnDrawBuffer = &glDrawBuffer;
		m_pfnDrawPixels = &glDrawPixels;
		m_pfnPixelStorei = &glPixelStorei;
		m_pfnPixelStoref = &glPixelStoref;
		m_pfnDepthFunc = &glDepthFunc;
		m_pfnDepthMask = &glDepthMask;
		m_pfnColorMask = &glColorMask;
		m_pfnPolygonMode = &glPolygonMode;
		m_pfnHint = &glHint;
		m_pfnPolygonOffset = &glPolygonOffset;
		m_pfnStencilOp = &glStencilOp;
		m_pfnStencilFunc = &glStencilFunc;
		m_pfnStencilMask = &glStencilMask;
		GetFunction( m_pfnBlendEquation, cuT( "glBlendEquation" ), cuT( "EXT" ) );
		GetFunction( m_pfnBlendFuncSeparate, cuT( "glBlendFuncSeparate" ), cuT( "EXT" ) );
		GetFunction( m_pfnBlendColor, cuT( "glBlendColor" ), cuT( "EXT" ) );
		GetFunction( m_pfnStencilOpSeparate, cuT( "glStencilOpSeparate" ), cuT( "ATI" ) );
		GetFunction( m_pfnStencilFuncSeparate, cuT( "glStencilFuncSeparate" ), cuT( "ATI" ) );
		GetFunction( m_pfnStencilMaskSeparate, cuT( "glStencilMaskSeparate" ), cuT( "" ) );
		GetFunction( m_pfnActiveTexture, cuT( "glActiveTexture" ), cuT( "ARB" ) );
		GetFunction( m_pfnClientActiveTexture, cuT( "glClientActiveTexture" ), cuT( "ARB" ) );

#if defined( _WIN32 )

		if ( HasExtension( ARB_create_context ) )
		{
			gl_api::GetFunction( cuT( "wglCreateContextAttribsARB" ), m_pfnCreateContextAttribs );
		}

		if ( HasExtension( EXT_swap_control ) )
		{
			gl_api::GetFunction( cuT( "wglSwapIntervalEXT" ), m_pfnSwapInterval );
		}

#elif defined( __linux__ )

		if ( HasExtension( ARB_create_context ) )
		{
			gl_api::GetFunction( cuT( "glXCreateContextAttribsARB" ), m_pfnCreateContextAttribs );
		}
		else
		{
			gl_api::GetFunction( cuT( "glXCreateContextAttribsARB" ), m_pfnCreateContextAttribs );
		}

		if ( HasExtension( EXT_swap_control ) )
		{
			gl_api::GetFunction( cuT( "glXSwapIntervalEXT" ), m_pfnSwapInterval );
		}
		else
		{
			gl_api::GetFunction( cuT( "glXSwapIntervalEXT" ), m_pfnSwapInterval );
		}

#else

#	error "Yet unsupported OS"

#endif
#if DEF_USE_DIRECT_STATE_ACCESS

		if ( HasExtension( EXT_direct_state_access ) )
		{
			m_pTexFunctions = new TexFunctionsDSA( *this );
			m_pBufFunctions = new BufFunctionsDSA( *this );
		}
		else
#endif
		{
			m_pTexFunctions = new TexFunctions( *this );
			m_pBufFunctions = new BufFunctions( *this );
		}

		if ( HasExtension( NV_shader_buffer_load ) )
		{
			GetFunction( m_pfnGetIntegerui64v, cuT( "glGetIntegerui64v" ), cuT( "NV" ) );
		}

		m_bBindVboToGpuAddress = HasExtension( NV_shader_buffer_load ) && HasExtension( NV_vertex_buffer_unified_memory );

		if ( m_iVersion >= 40 || HasExtension( ARB_draw_buffers_blend ) )
		{
			GetFunction( m_pfnBlendEquationi, cuT( "glBlendEquationi" ), cuT( "ARB" ) );
			GetFunction( m_pfnBlendFuncSeparatei, cuT( "glBlendFuncSeparatei" ), cuT( "ARB" ) );
		}
		else if ( HasExtension( AMD_draw_buffers_blend ) )
		{
			gl_api::GetFunction( cuT( "glBlendEquationIndexedAMD" ), m_pfnBlendEquationi );
			gl_api::GetFunction( cuT( "glBlendFuncIndexedAMD" ), m_pfnBlendFuncSeparatei );
		}

		m_bHasDepthClipping = HasExtension( EXT_clip_volume_hint );
		m_bHasNonPowerOfTwoTextures = HasExtension( ARB_texture_non_power_of_two );

		if ( HasExtension( ARB_texture_multisample ) )
		{
			GetFunction( m_pfnTexImage2DMultisample, cuT( "glTexImage2DMultisample" ), cuT( "" ) );
			GetFunction( m_pfnSampleCoverage, cuT( "glSampleCoverage" ), cuT( "ARB" ) );
		}

		if ( HasExtension( ARB_imaging ) )
		{
		}

		if ( HasExtension( ARB_debug_output ) )
		{
			GetFunction( m_pfnDebugMessageCallback, cuT( "glDebugMessageCallback" ), cuT( "ARB" ) );
		}
		else if ( HasExtension( AMDX_debug_output ) )
		{
			GetFunction( m_pfnDebugMessageCallbackAMD, cuT( "glDebugMessageCallbackAMD" ), cuT( "" ) );
		}

		if ( HasExtension( ARB_vertex_buffer_object ) )
		{
			m_bHasVbo = true;
			GetFunction( m_pfnGenBuffers, cuT( "glGenBuffers" ), cuT( "ARB" ) );
			GetFunction( m_pfnDeleteBuffers, cuT( "glDeleteBuffers" ), cuT( "ARB" ) );
			GetFunction( m_pfnIsBuffer, cuT( "glIsBuffer" ), cuT( "ARB" ) );

			if ( HasExtension( EXT_coordinate_frame ) )
			{
				GetFunction( m_pfnTangentPointer, cuT( "glTangentPointer" ), cuT( "EXT" ) );
				GetFunction( m_pfnBinormalPointer, cuT( "glBinormalPointer" ), cuT( "EXT" ) );
			}

			if ( HasExtension( ARB_pixel_buffer_object ) )
			{
				m_bHasPbo = true;
			}
		}

		if ( HasExtension( ARB_vertex_array_object ) )
		{
			m_bHasVao = true;
			GetFunction( m_pfnGenVertexArrays, cuT( "glGenVertexArrays" ), cuT( "ARB" ) );
			GetFunction( m_pfnBindVertexArray, cuT( "glBindVertexArray" ), cuT( "ARB" ) );
			GetFunction( m_pfnIsVertexArray, cuT( "glIsVertexArray" ), cuT( "ARB" ) );
			GetFunction( m_pfnDeleteVertexArrays, cuT( "glDeleteVertexArrays" ), cuT( "ARB" ) );
		}

		if ( HasExtension( ARB_texture_buffer_object ) )
		{
			m_bHasTbo = true;
			GetFunction( m_pfnTexBuffer, cuT( "glTexBuffer" ), cuT( "EXT" ) );
		}

		if ( HasExtension( ARB_framebuffer_object ) )
		{
			m_bHasFbo = true;
			GetFunction( m_pfnDrawBuffers, cuT( "glDrawBuffers" ), cuT( "ARB" ) );
			GetFunction( m_pfnBlitFramebuffer, cuT( "glBlitFramebuffer" ), cuT( "ARB" ) );
			GetFunction( m_pfnGenRenderbuffers, cuT( "glGenRenderbuffers" ), cuT( "ARB" ) );
			GetFunction( m_pfnDeleteRenderbuffers, cuT( "glDeleteRenderbuffers" ), cuT( "ARB" ) );
			GetFunction( m_pfnIsRenderbuffer, cuT( "glIsRenderbuffer" ), cuT( "ARB" ) );
			GetFunction( m_pfnBindRenderbuffer, cuT( "glBindRenderbuffer" ), cuT( "ARB" ) );
			GetFunction( m_pfnRenderbufferStorage, cuT( "glRenderbufferStorage" ), cuT( "ARB" ) );
			GetFunction( m_pfnRenderbufferStorageMultisample, cuT( "glRenderbufferStorageMultisample" ), cuT( "ARB" ) );
			GetFunction( m_pfnGetRenderbufferParameteriv, cuT( "glGetRenderbufferParameteriv" ), cuT( "ARB" ) );
			GetFunction( m_pfnFramebufferRenderbuffer, cuT( "glFramebufferRenderbuffer" ), cuT( "ARB" ) );
			GetFunction( m_pfnGenFramebuffers, cuT( "glGenFramebuffers" ), cuT( "ARB" ) );
			GetFunction( m_pfnDeleteFramebuffers, cuT( "glDeleteFramebuffers" ), cuT( "ARB" ) );
			GetFunction( m_pfnIsFramebuffer, cuT( "glIsFramebuffer" ), cuT( "ARB" ) );
			GetFunction( m_pfnBindFramebuffer, cuT( "glBindFramebuffer" ), cuT( "ARB" ) );
			GetFunction( m_pfnFramebufferTexture, cuT( "glFramebufferTexture" ), cuT( "ARB" ) );
			GetFunction( m_pfnFramebufferTexture1D, cuT( "glFramebufferTexture1D" ), cuT( "ARB" ) );
			GetFunction( m_pfnFramebufferTexture2D, cuT( "glFramebufferTexture2D" ), cuT( "ARB" ) );
			GetFunction( m_pfnFramebufferTexture3D, cuT( "glFramebufferTexture3D" ), cuT( "ARB" ) );
			GetFunction( m_pfnFramebufferTextureLayer, cuT( "glFramebufferTextureLayer" ), cuT( "ARB" ) );
			GetFunction( m_pfnCheckFramebufferStatus, cuT( "glCheckFramebufferStatus" ), cuT( "ARB" ) );
		}
		else if ( HasExtension( EXT_framebuffer_object ) )
		{
			m_bHasFbo = true;
			GetFunction( m_pfnDrawBuffers, cuT( "glDrawBuffers" ), cuT( "EXT" ) );
			GetFunction( m_pfnBlitFramebuffer, cuT( "glBlitFramebuffer" ), cuT( "EXT" ) );
			GetFunction( m_pfnGenRenderbuffers, cuT( "glGenRenderbuffers" ), cuT( "EXT" ) );
			GetFunction( m_pfnDeleteRenderbuffers, cuT( "glDeleteRenderbuffers" ), cuT( "EXT" ) );
			GetFunction( m_pfnIsRenderbuffer, cuT( "glIsRenderbuffer" ), cuT( "EXT" ) );
			GetFunction( m_pfnBindRenderbuffer, cuT( "glBindRenderbuffer" ), cuT( "EXT" ) );
			GetFunction( m_pfnRenderbufferStorage, cuT( "glRenderbufferStorage" ), cuT( "EXT" ) );
			GetFunction( m_pfnRenderbufferStorageMultisample, cuT( "glRenderbufferStorageMultisample" ), cuT( "EXT" ) );
			GetFunction( m_pfnGetRenderbufferParameteriv, cuT( "glGetRenderbufferParameteriv" ), cuT( "EXT" ) );
			GetFunction( m_pfnFramebufferRenderbuffer, cuT( "glFramebufferRenderbuffer" ), cuT( "EXT" ) );
			GetFunction( m_pfnGenFramebuffers, cuT( "glGenFramebuffers" ), cuT( "EXT" ) );
			GetFunction( m_pfnDeleteFramebuffers, cuT( "glDeleteFramebuffers" ), cuT( "EXT" ) );
			GetFunction( m_pfnIsFramebuffer, cuT( "glIsFramebuffer" ), cuT( "EXT" ) );
			GetFunction( m_pfnBindFramebuffer, cuT( "glBindFramebuffer" ), cuT( "EXT" ) );
			GetFunction( m_pfnFramebufferTexture, cuT( "glFramebufferTexture" ), cuT( "EXT" ) );
			GetFunction( m_pfnFramebufferTexture1D, cuT( "glFramebufferTexture1D" ), cuT( "EXT" ) );
			GetFunction( m_pfnFramebufferTexture2D, cuT( "glFramebufferTexture2D" ), cuT( "EXT" ) );
			GetFunction( m_pfnFramebufferTexture3D, cuT( "glFramebufferTexture3D" ), cuT( "EXT" ) );
			GetFunction( m_pfnFramebufferTextureLayer, cuT( "glFramebufferTextureLayer" ), cuT( "EXT" ) );
			GetFunction( m_pfnCheckFramebufferStatus, cuT( "glCheckFramebufferStatus" ), cuT( "EXT" ) );
		}

		if ( HasExtension( ARB_sampler_objects ) )
		{
			m_bHasSpl = true;
			GetFunction( m_pfnGenSamplers, cuT( "glGenSamplers" ), cuT( "" ) );
			GetFunction( m_pfnDeleteSamplers, cuT( "glDeleteSamplers" ), cuT( "" ) );
			GetFunction( m_pfnIsSampler, cuT( "glIsSampler" ), cuT( "" ) );
			GetFunction( m_pfnBindSampler, cuT( "glBindSampler" ), cuT( "" ) );
			GetFunction( m_pfnGetSamplerParameteruiv, cuT( "glGetSamplerParameteruiv" ), cuT( "" ) );
			GetFunction( m_pfnGetSamplerParameterfv, cuT( "glGetSamplerParameterfv" ), cuT( "" ) );
			GetFunction( m_pfnGetSamplerParameteriv, cuT( "glGetSamplerParameteriv" ), cuT( "" ) );
			GetFunction( m_pfnSamplerParameteruiv, cuT( "glSamplerParameteruiv" ), cuT( "" ) );
			GetFunction( m_pfnSamplerParameterf, cuT( "glSamplerParameterf" ), cuT( "" ) );
			GetFunction( m_pfnSamplerParameterfv, cuT( "glSamplerParameterfv" ), cuT( "" ) );
			GetFunction( m_pfnSamplerParameteri, cuT( "glSamplerParameteri" ), cuT( "" ) );
			GetFunction( m_pfnSamplerParameteriv, cuT( "glSamplerParameteriv" ), cuT( "" ) );

			if ( HasExtension( EXT_texture_filter_anisotropic ) )
			{
				m_bHasAnisotropic = true;
			}
		}

		if ( HasExtension( ARB_draw_instanced ) )
		{
			m_bHasInstancedDraw = true;
			GetFunction( m_pfnDrawArraysInstanced, cuT( "glDrawArraysInstanced" ), cuT( "ARB" ) );
			GetFunction( m_pfnDrawElementsInstanced, cuT( "glDrawElementsInstanced" ), cuT( "ARB" ) );

			if ( HasExtension( ARB_instanced_arrays ) )
			{
				m_bHasInstancedArrays = true;
				GetFunction( m_pfnVertexAttribDivisor, cuT( "glVertexAttribDivisor" ), cuT( "ARB" ) );
			}
		}
		else if ( HasExtension( EXT_draw_instanced ) )
		{
			m_bHasInstancedDraw = true;
			GetFunction( m_pfnDrawArraysInstanced, cuT( "glDrawArraysInstanced" ), cuT( "EXT" ) );
			GetFunction( m_pfnDrawElementsInstanced, cuT( "glDrawElementsInstanced" ), cuT( "EXT" ) );

			if ( HasExtension( EXT_instanced_arrays ) )
			{
				m_bHasInstancedArrays = true;
				GetFunction( m_pfnVertexAttribDivisor, cuT( "glVertexAttribDivisor" ), cuT( "EXT" ) );
			}
		}

		if ( HasExtension( ARB_vertex_program ) )
		{
			m_bHasVSh = true;
			GetFunction( m_pfnCreateShader, cuT( "glCreateShader" ), cuT( "" ) );
			GetFunction( m_pfnDeleteShader, cuT( "glDeleteShader" ), cuT( "" ) );
			GetFunction( m_pfnIsShader, cuT( "glIsShader" ), cuT( "ARB" ) );
			GetFunction( m_pfnAttachShader, cuT( "glAttachShader" ), cuT( "" ) );
			GetFunction( m_pfnDetachShader, cuT( "glDetachShader" ), cuT( "" ) );
			GetFunction( m_pfnCompileShader, cuT( "glCompileShader" ), cuT( "ARB" ) );
			GetFunction( m_pfnGetShaderiv, cuT( "glGetShaderiv" ), cuT( "" ) );
			GetFunction( m_pfnGetShaderInfoLog, cuT( "glGetShaderInfoLog" ), cuT( "" ) );
			GetFunction( m_pfnShaderSource, cuT( "glShaderSource" ), cuT( "ARB" ) );
			GetFunction( m_pfnCreateProgram, cuT( "glCreateProgram" ), cuT( "" ) );
			GetFunction( m_pfnDeleteProgram, cuT( "glDeleteProgram" ), cuT( "" ) );
			GetFunction( m_pfnLinkProgram, cuT( "glLinkProgram" ), cuT( "ARB" ) );
			GetFunction( m_pfnUseProgram, cuT( "glUseProgram" ), cuT( "" ) );
			GetFunction( m_pfnGetProgramiv, cuT( "glGetProgramiv" ), cuT( "ARB" ) );
			GetFunction( m_pfnGetProgramInfoLog, cuT( "glGetProgramInfoLog" ), cuT( "" ) );
			GetFunction( m_pfnGetAttribLocation, cuT( "glGetAttribLocation" ), cuT( "ARB" ) );
			GetFunction( m_pfnIsProgram, cuT( "glIsProgram" ), cuT( "ARB" ) );
			GetFunction( m_pfnEnableVertexAttribArray, cuT( "glEnableVertexAttribArray" ), cuT( "ARB" ) );
			GetFunction( m_pfnVertexAttribPointer, cuT( "glVertexAttribPointer" ), cuT( "ARB" ) );
			GetFunction( m_pfnVertexAttribIPointer, cuT( "glVertexAttribIPointer" ), cuT( "ARB" ) );
			GetFunction( m_pfnDisableVertexAttribArray, cuT( "glDisableVertexAttribArray" ), cuT( "ARB" ) );
			GetFunction( m_pfnProgramParameteri, cuT( "glProgramParameteri" ), cuT( "ARB" ) );
			GetFunction( m_pfnGetUniformLocation, cuT( "glGetUniformLocation" ), cuT( "ARB" ) );
			GetFunction( m_pfnGetActiveAttrib, cuT( "glGetActiveAttrib" ), cuT( "ARB" ) );

			if ( HasExtension( ARB_fragment_program ) )
			{
				m_bHasPSh = true;
				GetFunction( m_pfnUniform1i, cuT( "glUniform1i" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform2i, cuT( "glUniform2i" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform3i, cuT( "glUniform3i" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform4i, cuT( "glUniform4i" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform1iv, cuT( "glUniform1iv" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform2iv, cuT( "glUniform2iv" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform3iv, cuT( "glUniform3iv" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform4iv, cuT( "glUniform4iv" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform1ui, cuT( "glUniform1ui" ), cuT( "EXT" ) );
				GetFunction( m_pfnUniform2ui, cuT( "glUniform2ui" ), cuT( "EXT" ) );
				GetFunction( m_pfnUniform3ui, cuT( "glUniform3ui" ), cuT( "EXT" ) );
				GetFunction( m_pfnUniform4ui, cuT( "glUniform4ui" ), cuT( "EXT" ) );
				GetFunction( m_pfnUniform1uiv, cuT( "glUniform1uiv" ), cuT( "EXT" ) );
				GetFunction( m_pfnUniform2uiv, cuT( "glUniform2uiv" ), cuT( "EXT" ) );
				GetFunction( m_pfnUniform3uiv, cuT( "glUniform3uiv" ), cuT( "EXT" ) );
				GetFunction( m_pfnUniform4uiv, cuT( "glUniform4uiv" ), cuT( "EXT" ) );
				GetFunction( m_pfnUniform1f, cuT( "glUniform1f" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform2f, cuT( "glUniform2f" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform3f, cuT( "glUniform3f" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform4f, cuT( "glUniform4f" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform1fv, cuT( "glUniform1fv" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform2fv, cuT( "glUniform2fv" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform3fv, cuT( "glUniform3fv" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform4fv, cuT( "glUniform4fv" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniform1d, cuT( "glUniform1d" ), cuT( "" ) );
				GetFunction( m_pfnUniform2d, cuT( "glUniform2d" ), cuT( "" ) );
				GetFunction( m_pfnUniform3d, cuT( "glUniform3d" ), cuT( "" ) );
				GetFunction( m_pfnUniform4d, cuT( "glUniform4d" ), cuT( "" ) );
				GetFunction( m_pfnUniform1dv, cuT( "glUniform1dv" ), cuT( "" ) );
				GetFunction( m_pfnUniform2dv, cuT( "glUniform2dv" ), cuT( "" ) );
				GetFunction( m_pfnUniform3dv, cuT( "glUniform3dv" ), cuT( "" ) );
				GetFunction( m_pfnUniform4dv, cuT( "glUniform4dv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix2fv, cuT( "glUniformMatrix2fv" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniformMatrix2x3fv, cuT( "glUniformMatrix2x3fv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix2x4fv, cuT( "glUniformMatrix2x4fv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix3fv, cuT( "glUniformMatrix3fv" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniformMatrix3x2fv, cuT( "glUniformMatrix3x2fv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix3x4fv, cuT( "glUniformMatrix3x4fv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix4fv, cuT( "glUniformMatrix4fv" ), cuT( "ARB" ) );
				GetFunction( m_pfnUniformMatrix4x2fv, cuT( "glUniformMatrix4x2fv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix4x3fv, cuT( "glUniformMatrix4x3fv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix2dv, cuT( "glUniformMatrix2dv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix2x3dv, cuT( "glUniformMatrix2x3dv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix2x4dv, cuT( "glUniformMatrix2x4dv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix3dv, cuT( "glUniformMatrix3dv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix3x2dv, cuT( "glUniformMatrix3x2dv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix3x4dv, cuT( "glUniformMatrix3x4dv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix4dv, cuT( "glUniformMatrix4dv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix4x2dv, cuT( "glUniformMatrix4x2dv" ), cuT( "" ) );
				GetFunction( m_pfnUniformMatrix4x3dv, cuT( "glUniformMatrix4x3dv" ), cuT( "" ) );

				if ( HasExtension( ARB_uniform_buffer_object ) )
				{
					m_bHasUbo = m_iGlslVersion >= 140;
					GetFunction( m_pfnGetUniformBlockIndex, cuT( "glGetUniformBlockIndex" ), cuT( "" ) );
					GetFunction( m_pfnBindBufferBase, cuT( "glBindBufferBase" ), cuT( "EXT" ) );
					GetFunction( m_pfnUniformBlockBinding, cuT( "glUniformBlockBinding" ), cuT( "" ) );
					GetFunction( m_pfnGetUniformIndices, cuT( "glGetUniformIndices" ), cuT( "" ) );
					GetFunction( m_pfnGetActiveUniformsiv, cuT( "glGetActiveUniformsiv" ), cuT( "" ) );
					GetFunction( m_pfnGetActiveUniformBlockiv, cuT( "glGetActiveUniformBlockiv" ), cuT( "" ) );
				}

				if ( HasExtension( ARB_geometry_shader4 ) || HasExtension( EXT_geometry_shader4 ) )
				{
					m_bHasGSh = true;

					if ( HasExtension( ARB_tessellation_shader ) )
					{
						m_bHasTSh = true;
						GetFunction( m_pfnPatchParameteri, cuT( "glPatchParameteri" ), cuT( "ARB" ) );

						if ( HasExtension( ARB_compute_shader ) )
						{
							m_bHasCSh = true;
						}
					}
				}
			}
		}

		HasExtension( ARB_timer_query );
		GetFunction( m_pfnGenQueries, cuT( "glGenQueries" ), cuT( "ARB" ) );
		GetFunction( m_pfnDeleteQueries, cuT( "glDeleteQueries" ), cuT( "ARB" ) );
		GetFunction( m_pfnIsQuery, cuT( "glIsQuery" ), cuT( "ARB" ) );
		GetFunction( m_pfnBeginQuery, cuT( "glBeginQuery" ), cuT( "ARB" ) );
		GetFunction( m_pfnEndQuery, cuT( "glEndQuery" ), cuT( "ARB" ) );
		GetFunction( m_pfnQueryCounter, cuT( "glQueryCounter" ), cuT( "ARB" ) );
		GetFunction( m_pfnGetQueryObjectiv, cuT( "glGetQueryObjectiv" ), cuT( "ARB" ) );
		GetFunction( m_pfnGetQueryObjectuiv, cuT( "glGetQueryObjectuiv" ), cuT( "ARB" ) );
		GetFunction( m_pfnGetQueryObjecti64v, cuT( "glGetQueryObjecti64v" ), cuT( "ARB" ) );
		GetFunction( m_pfnGetQueryObjectui64v, cuT( "glGetQueryObjectui64v" ), cuT( "ARB" ) );

		if ( HasExtension( ARB_program_interface_query ) )
		{
			GetFunction( m_pfnGetProgramInterfaceiv, cuT( "glGetProgramInterfaceiv" ), cuT( "ARB" ) );
			GetFunction( m_pfnGetProgramResourceIndex, cuT( "glGetProgramResourceIndex" ), cuT( "ARB" ) );
			GetFunction( m_pfnGetProgramResourceLocation, cuT( "glGetProgramResourceLocation" ), cuT( "ARB" ) );
			GetFunction( m_pfnGetProgramResourceLocationIndex, cuT( "glGetProgramResourceLocationIndex" ), cuT( "ARB" ) );
			GetFunction( m_pfnGetProgramResourceName, cuT( "glGetProgramResourceName" ), cuT( "ARB" ) );
			GetFunction( m_pfnGetProgramResourceiv, cuT( "glGetProgramResourceiv" ), cuT( "ARB" ) );
		}

		return true;
	}

	void OpenGl::Cleanup()
	{
		delete m_pTexFunctions;
		m_pTexFunctions = nullptr;
		delete m_pBufFunctions;
		m_pBufFunctions = nullptr;
		m_bHasVao = false;
		m_bHasUbo = false;
		m_bHasPbo = false;
		m_bHasTbo = false;
		m_bHasFbo = false;
		m_bHasVbo = false;
		m_bHasVSh = false;
		m_bHasPSh = false;
		m_bHasGSh = false;
		m_bHasTSh = false;
		m_bHasCSh = false;
		m_bHasSpl = false;
		m_bHasDepthClipping = false;
		m_bHasNonPowerOfTwoTextures = false;
		m_bBindVboToGpuAddress = false;
		m_iGlslVersion = 0;
		m_iVersion = 0;
	}

	#if !defined( NDEBUG )

	void OpenGl::Track( void * p_object, std::string const & p_name, std::string const & p_file, int p_line )const
	{
		m_renderSystem.Track( p_object, p_name, p_file, p_line );
	}

	void OpenGl::UnTrack( void * p_object )const
	{
		m_renderSystem.Untrack( p_object );
	}

	#endif

	bool OpenGl::GlCheckError( std::string const & p_text )const
	{
		return DoGlCheckError( string::string_cast< xchar >( p_text ) );
	}

	bool OpenGl::GlCheckError( std::wstring const & p_text )const
	{
		return DoGlCheckError( string::string_cast< xchar >( p_text ) );
	}

	void OpenGl::StDebugLog( eGL_DEBUG_SOURCE source, eGL_DEBUG_TYPE type, uint32_t id, eGL_DEBUG_SEVERITY severity, int length, const char * message, void * userParam )
	{
		reinterpret_cast< OpenGl * >( userParam )->DebugLog( source, type, id, severity, length, message );
	}

	void OpenGl::StDebugLogAMD( uint32_t id, eGL_DEBUG_CATEGORY category, eGL_DEBUG_SEVERITY severity, int length, const char * message, void * userParam )
	{
		reinterpret_cast< OpenGl * >( userParam )->DebugLogAMD( id, category, severity, length, message );
	}

	bool OpenGl::DoGlCheckError( String const & p_text )const
	{
		bool l_return = true;
		uint32_t l_errorCode = GetError();

		if ( l_errorCode != GL_NO_ERROR )
		{
			String l_strSysError = System::GetLastErrorText();
			l_errorCode -= GL_INVALID_ENUM;
			String l_strError = p_text + cuT( " - " ) + GlslErrors[l_errorCode];

			if ( !l_strSysError.empty() )
			{
				l_strError += cuT( " - " ) + l_strSysError;
			}

			Logger::LogError( l_strError );
			l_return = false;
		}

		return l_return;
	}

	void OpenGl::DebugLog( eGL_DEBUG_SOURCE source, eGL_DEBUG_TYPE type, uint32_t id, eGL_DEBUG_SEVERITY severity, int CU_PARAM_UNUSED( length ), const char * message )const
	{
		if ( id != 131185 )
		{
			bool l_error = false;
			String l_toLog = cuT( "OpenGl Debug - " );

			switch ( source )
			{
			case eGL_DEBUG_SOURCE_API:
				l_toLog += cuT( "Source:OpenGL\t" );
				break;

			case eGL_DEBUG_SOURCE_WINDOW_SYSTEM:
				l_toLog += cuT( "Source:Windows\t" );
				break;

			case eGL_DEBUG_SOURCE_SHADER_COMPILER:
				l_toLog += cuT( "Source:Shader compiler\t" );
				break;

			case eGL_DEBUG_SOURCE_THIRD_PARTY:
				l_toLog += cuT( "Source:Third party\t" );
				break;

			case eGL_DEBUG_SOURCE_APPLICATION:
				l_toLog += cuT( "Source:Application\t" );
				break;

			case eGL_DEBUG_SOURCE_OTHER:
				l_toLog += cuT( "Source:Other\t" );
				break;
			}

			switch ( type )
			{
			case eGL_DEBUG_TYPE_ERROR:
				l_toLog += cuT( "Type:Error\t" );
				break;

			case eGL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				l_toLog += cuT( "Type:Deprecated behavior\t" );
				break;

			case eGL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				l_toLog += cuT( "Type:Undefined behavior\t" );
				break;

			case eGL_DEBUG_TYPE_PORTABILITY:
				l_toLog += cuT( "Type:Portability\t" );
				break;

			case eGL_DEBUG_TYPE_PERFORMANCE:
				l_toLog += cuT( "Type:Performance\t" );
				break;

			case eGL_DEBUG_TYPE_OTHER:
				l_toLog += cuT( "Type:Other\t" );
				break;
			}

			l_toLog += cuT( "ID:" ) + string::to_string( id ) + cuT( "\t" );

			switch ( severity )
			{
			case eGL_DEBUG_SEVERITY_HIGH:
				l_error = true;
				l_toLog += cuT( "Severity:High\t" );
				break;

			case eGL_DEBUG_SEVERITY_MEDIUM:
				l_toLog += cuT( "Severity:Medium\t" );
				break;

			case eGL_DEBUG_SEVERITY_LOW:
				l_toLog += cuT( "Severity:Low\t" );
				break;
			}

			if ( l_error )
			{
				Logger::LogError( l_toLog + cuT( "Message:" ) + string::string_cast< xchar >( message ) );
			}
			else
			{
				Logger::LogWarning( l_toLog + cuT( "Message:" ) + string::string_cast< xchar >( message ) );
			}
		}
	}

	void OpenGl::DebugLogAMD( uint32_t id, eGL_DEBUG_CATEGORY category, eGL_DEBUG_SEVERITY severity, int CU_PARAM_UNUSED( length ), const char * message )const
	{
		bool l_error = false;
		String l_toLog = cuT( "OpenGl Debug - " );

		switch ( category )
		{
		case eGL_DEBUG_CATEGORY_API_ERROR:
			l_toLog += cuT( "Category:OpenGL\t" );
			break;

		case eGL_DEBUG_CATEGORY_WINDOW_SYSTEM:
			l_toLog += cuT( "Category:Windows\t" );
			break;

		case eGL_DEBUG_CATEGORY_DEPRECATION:
			l_toLog += cuT( "Category:Deprecated behavior\t" );
			break;

		case eGL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR:
			l_toLog += cuT( "Category:Undefined behavior\t" );
			break;

		case eGL_DEBUG_CATEGORY_PERFORMANCE:
			l_toLog += cuT( "Category:Performance\t" );
			break;

		case eGL_DEBUG_CATEGORY_SHADER_COMPILER:
			l_toLog += cuT( "Category:Shader compiler\t" );
			break;

		case eGL_DEBUG_CATEGORY_APPLICATION:
			l_toLog += cuT( "Category:Application\t" );
			break;

		case eGL_DEBUG_CATEGORY_OTHER:
			l_toLog += cuT( "Category:Other\t" );
			break;
		}

		l_toLog += cuT( "ID:" ) + string::to_string( id ) + cuT( "\t" );

		switch ( severity )
		{
		case eGL_DEBUG_SEVERITY_HIGH:
			l_error = true;
			l_toLog += cuT( "Severity:High\t" );
			break;

		case eGL_DEBUG_SEVERITY_MEDIUM:
			l_toLog += cuT( "Severity:Medium\t" );
			break;

		case eGL_DEBUG_SEVERITY_LOW:
			l_toLog += cuT( "Severity:Low\t" );
			break;
		}

		if ( l_error )
		{
			Logger::LogError( l_toLog + cuT( "Message:" ) + string::string_cast< xchar >( message ) );
		}
		else
		{
			Logger::LogWarning( l_toLog + cuT( "Message:" ) + string::string_cast< xchar >( message ) );
		}
	}

	void OpenGl::DisplayExtensions()const
	{
		auto l_array = string::split( m_extensions, cuT( " " ), 0xFFFFFFFF, false );
		std::sort( l_array.begin(), l_array.end() );

		for ( auto l_extension : l_array )
		{
			Logger::LogDebug( l_extension );
		}
	}

//*************************************************************************************************
}