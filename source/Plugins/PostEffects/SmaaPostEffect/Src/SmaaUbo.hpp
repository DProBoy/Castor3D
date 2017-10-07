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
#ifndef ___C3DSMAA_SmaaUbo_H___
#define ___C3DSMAA_SmaaUbo_H___

#include "Shader/UniformBuffer.hpp"

namespace smaa
{
	namespace constants
	{
		extern const castor::String PixelSize;
		extern const castor::String Threshold;
		extern const castor::String PredicationScale;
		extern const castor::String PredicationThreshold;
		extern const castor::String PredicationStrength;
		extern const castor::String MaxSearchSteps;
		extern const castor::String MaxSearchStepsDiag;
		extern const castor::String SubsampleIndices;
		extern const castor::String AreaTexPixelSize;
		extern const castor::String AreaTexSubtexSize;
		extern const castor::String AreaTexMaxDistance;
		extern const castor::String AreaTexMaxDistanceDiag;
		extern const castor::String CornerRounding;
	}
}

#endif