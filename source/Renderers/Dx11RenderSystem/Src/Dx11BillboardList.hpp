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
#ifndef ___DX11_BILLBOARD_LIST_H___
#define ___DX11_BILLBOARD_LIST_H___

#include "Dx11RenderSystemPrerequisites.hpp"
#include <BillboardList.hpp>

namespace Dx11Render
{
	class DxBillboardList
		:	public Castor3D::BillboardList
	{
	public:
		DxBillboardList( Castor3D::SceneSPtr p_pScene, DxRenderSystem * p_pRenderSystem );
		virtual ~DxBillboardList();

	private:
		virtual bool DoInitialise();
		virtual Castor3D::ShaderProgramBaseSPtr DoGetProgram( uint32_t p_flags );
	};
}

#endif