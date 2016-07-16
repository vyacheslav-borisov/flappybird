#pragma once

#include "interfaces.h"

namespace pegas
{
	class BaseScreenLayer: public IScreenLayer, public IKeyboardController, public IMouseController
	{
	public:
		BaseScreenLayer(const LayerType& type, const LayerId& id, bool modal): 
		  IScreenLayer(type, id), m_modal(modal)  
		  {
			  setActivity(true);
		  }
		virtual ~BaseScreenLayer() {}

		//IScreenLayer default implementation
		virtual void update(IPlatformContext* context) {};
		virtual void render(IPlatformContext* context) {};
		
		virtual void create(IPlatformContext* context) 
		{
			//setActivity(true);
		}

		virtual void destroy(IPlatformContext* context) {};

		virtual void onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags);
		virtual void onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags);
		virtual void onMouseMove(float x, float y, MouseFlags flags);
		virtual void onMouseWheel(NumNothes wheel, MouseFlags flags);

		virtual void onKeyDown(KeyCode key, KeyFlags flags);
		virtual void onKeyUp(KeyCode key, KeyFlags flags);
		virtual void onChar(tchar ch);


		void setModal(bool modal) { m_modal = modal; }
		bool isModal() { return m_modal; }

	protected:
		bool m_modal;
	};

	typedef SmartPointer<BaseScreenLayer> BaseScreenLayerPtr;

	class DefaultGameState: public IGameState, public IKeyboardController, public IMouseController

	{
	public:
		DefaultGameState(const GameStateID& id): IGameState(id) {}
		virtual ~DefaultGameState() {}	

		virtual void enter(IPlatformContext* context);
		virtual void leave(IPlatformContext* context);
		virtual void update(IPlatformContext* context);
		virtual void render(IPlatformContext* context);

		virtual void onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags);
		virtual void onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags);
		virtual void onMouseMove(float x, float y, MouseFlags flags);
		virtual void onMouseWheel(NumNothes wheel, MouseFlags flags);

		virtual void onKeyDown(KeyCode key, KeyFlags flags);
		virtual void onKeyUp(KeyCode key, KeyFlags flags);
		virtual void onChar(tchar ch);


	public:
		void pushLayer(BaseScreenLayerPtr layer);
		BaseScreenLayerPtr getLayer(const LayerId& id);

	protected:
		std::list<BaseScreenLayerPtr> m_layers;
	};
}
