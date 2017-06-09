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
#ifndef ___GLSL_Utils_H___
#define ___GLSL_Utils_H___

#include "GlslFunction.hpp"

namespace GLSL
{
	class Utils
	{
	public:
		GlslWriter_API Utils( GlslWriter & p_writer );
		GlslWriter_API void DeclareCalcTexCoord();
		GlslWriter_API void DeclareCalcVSPosition();
		GlslWriter_API void DeclareCalcVSDepth();
		GlslWriter_API void DeclareCalcWSPosition();
		GlslWriter_API void DeclareApplyGamma();
		GlslWriter_API void DeclareRemoveGamma();
		GlslWriter_API void DeclareLineariseDepth();
		GlslWriter_API Vec2 CalcTexCoord();
		GlslWriter_API Vec3 CalcVSPosition( Vec2 const & p_uv
			, Mat4 const & p_invProj );
		GlslWriter_API Float CalcVSDepth( Vec2 const & p_uv
			, Mat4 const & p_proj );
		GlslWriter_API Vec3 CalcWSPosition( Vec2 const & p_uv
			, Mat4 const & p_invViewProj );
		GlslWriter_API Vec3 ApplyGamma( Float const & p_gamma
			, Vec3 const & p_HDR );
		GlslWriter_API Vec3 RemoveGamma( Float const & p_gamma
			, Vec3 const & p_sRGB );
		GlslWriter_API Float LineariseDepth( Float const & p_depth
			, Mat4 const & p_invProj );

	private:
		GlslWriter & m_writer;
		Function< Vec2 > m_calcTexCoord;
		Function< Vec3, InVec2, InMat4 > m_calcVSPosition;
		Function< Float, InVec2, InMat4 > m_calcVSDepth;
		Function< Vec3, InVec2, InMat4 > m_calcWSPosition;
		Function< Vec3, InFloat, InVec3 > m_applyGamma;
		Function< Vec3, InFloat, InVec3 > m_removeGamma;
		Function< Float, InFloat, InMat4 > m_lineariseDepth;
	};
}

#endif
