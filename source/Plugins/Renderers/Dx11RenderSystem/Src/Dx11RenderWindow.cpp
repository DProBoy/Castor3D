#include "Dx11RenderWindow.hpp"

#include "Dx11RenderSystem.hpp"

using namespace Castor3D;
using namespace Castor;

namespace Dx11Render
{
	DxRenderWindow::DxRenderWindow( DxRenderSystem * p_renderSystem )
		: RenderWindow( *p_renderSystem->GetOwner() )
	{
	}

	DxRenderWindow::~DxRenderWindow()
	{
	}

	void DxRenderWindow::DoBeginScene()
	{
	}

	void DxRenderWindow::DoEndScene()
	{
	}
}