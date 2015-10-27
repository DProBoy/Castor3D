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
#ifndef ___DX11_WINDOW_RENDERER_H___
#define ___DX11_WINDOW_RENDERER_H___

#include "Dx11RenderSystemPrerequisites.hpp"

#include <RenderWindow.hpp>

namespace Dx11Render
{
	class DxRenderWindow
		: public Castor3D::RenderWindow
	{
	public:
		DxRenderWindow( DxRenderSystem * p_renderSystem );
		virtual ~DxRenderWindow();

	private:
		/**
		 *\copydoc		Castor3D::RenderWindow::DoInitialise
		 */
		virtual bool DoInitialise();
		/**
		 *\copydoc		Castor3D::RenderWindow::DoCleanup
		 */
		virtual void DoCleanup();
		/**
		 *\copydoc		Castor3D::RenderWindow::DoBeginScene
		 */
		virtual void DoBeginScene();
		/**
		 *\copydoc		Castor3D::RenderWindow::DoEndScene
		 */
		virtual void DoEndScene();
	};
}

#endif
