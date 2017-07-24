﻿/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.html)
Copyright (c) 2016 dragonjoker59@hotmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef ___C3D_FxaaPostEffect_H___
#define ___C3D_FxaaPostEffect_H___

#include "FxaaUbo.hpp"

#include <Mesh/Buffer/BufferDeclaration.hpp>
#include <PostEffect/PostEffect.hpp>
#include <Texture/TextureUnit.hpp>
#include <Render/Viewport.hpp>
#include <Shader/MatrixUbo.hpp>

namespace fxaa
{
	class PostEffect
		: public Castor3D::PostEffect
	{
	public:
		PostEffect( Castor3D::RenderTarget & p_renderTarget, Castor3D::RenderSystem & renderSystem, Castor3D::Parameters const & p_param );
		~PostEffect();
		static Castor3D::PostEffectSPtr Create( Castor3D::RenderTarget & p_renderTarget, Castor3D::RenderSystem & renderSystem, Castor3D::Parameters const & p_param );
		/**
		 *\copydoc		Castor3D::PostEffect::Initialise
		 */
		bool Initialise() override;
		/**
		 *\copydoc		Castor3D::PostEffect::Cleanup
		 */
		void Cleanup() override;
		/**
		 *\copydoc		Castor3D::PostEffect::Apply
		 */
		bool Apply( Castor3D::FrameBuffer & p_framebuffer ) override;

		inline void SetSubpixShift( float p_value )
		{
			m_subpixShift = p_value;
		}

		inline void SetMaxSpan( float p_value )
		{
			m_spanMax = p_value;
		}

		inline void SetReduceMul( float p_value )
		{
			m_reduceMul = p_value;
		}

	private:
		/**
		 *\copydoc		Castor3D::PostEffect::DoWriteInto
		 */
		bool DoWriteInto( Castor::TextFile & p_file ) override;

	public:
		static Castor::String Type;
		static Castor::String Name;

	private:
		Castor3D::PushUniform1sSPtr m_mapDiffuse;
		Castor3D::SamplerSPtr m_sampler;
		Castor3D::RenderPipelineSPtr m_pipeline;
		PostEffectSurface m_surface;
		Castor3D::MatrixUbo m_matrixUbo;
		FxaaUbo m_fxaaUbo;
		float m_subpixShift{ 1.0f / 4.0f };
		float m_spanMax{ 8.0f };
		float m_reduceMul{ 1.0f / 8.0f };
	};
}

#endif
