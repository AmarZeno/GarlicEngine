#include "Renderer.h"
#include "Engine.h"

namespace Renderer {

	RendererInfo::RendererInfo(StrongPointer<GameObject> i_pStrPtr)
	{
		m_pWeakPtr = i_pStrPtr;
		Engine::GetMasterObjects()->GetRendererInfoList().push_back(m_pWeakPtr);
	}

	RendererInfo::~RendererInfo()
	{
		//delete pSprite;
	}

	void Draw(RendererInfo *rendererInfo)
	{
		StrongPointer<GameObject> strngPtr = rendererInfo->m_pWeakPtr.Acquire();

		if (rendererInfo->m_pWeakPtr.IsValid()) {
			GLib::Point2D spriteOffset = { static_cast<float>(rendererInfo->m_pWeakPtr.Acquire()->GetPosition().x()), static_cast<float>(rendererInfo->m_pWeakPtr.Acquire()->GetPosition().y()) };
			// Tell GLib to render this sprite at our calculated location
			GLib::Sprites::RenderSprite(*(rendererInfo->pSprite), spriteOffset, 0.0f);
		}

		return;
	}

}



