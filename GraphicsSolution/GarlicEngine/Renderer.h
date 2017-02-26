#pragma once
#include "GameObject.h"
#include "WeakPointer.h"
#include "GLib.h"
namespace Renderer
{
	class RendererInfo {
	public:
		RendererInfo(StrongPointer<GameObject> i_pStrPtr);
		~RendererInfo();

		GLib::Sprites::Sprite * pSprite;
		WeakPointer<GameObject> m_pWeakPtr;
		
		// TODO Add MeshInfo, Textures & Material
	};

	void Draw(RendererInfo *rendererInfo);
};

