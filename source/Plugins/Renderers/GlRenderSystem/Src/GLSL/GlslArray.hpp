/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.htm)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GLSL_ARRAY_H___
#define ___GLSL_ARRAY_H___

#include "GlslBool.hpp"

namespace GlRender
{
	namespace GLSL
	{
		template< typename T >
		struct Array
				: public T
		{
			Array( GlslWriter * p_writer, const Castor::String & p_name, uint32_t p_dimension );
			template< typename U > T & operator[]( U const & p_offset );
			uint32_t m_dimension;
		};
	}
}

#include "GlslArray.inl"

#endif
