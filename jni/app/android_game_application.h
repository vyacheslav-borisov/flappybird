#ifndef PEGAS_APP_ANDROID_GAME_APPLICATION_H_
#define PEGAS_APP_ANDROID_GAME_APPLICATION_H_

#include "base_game_application.h"

namespace pegas
{
	class AndroidGameApplication: public BaseGameApplication,
								  public IActivityHandler,
								  public IInputHandler
	{
	public:
		AndroidGameApplication(android_app* context);

		virtual bool init();
	public:
		//IActivityHandler
		virtual status onActivate();
		virtual void onDeactivate();
		virtual status onStep();

		virtual void onStart();
		virtual void onResume();
		virtual void onPause();
		virtual void onStop();
		virtual void onDestroy();

		virtual void onSaveState(void** data, size_t* size);
		virtual void onConfigurationChanged();
		virtual void onLowMemory();

		virtual void onCreateWindow();
		virtual void onDestroyWindow();
		virtual void onGainFocus();
		virtual void onLostFocus();

	public:
		//IInputHandler
		virtual bool onTouchEvent(AInputEvent* event);
		virtual bool onKeyEvent(AInputEvent* event);

	private:
		android_app* 			 m_androidAppContext;
		std::set<IInputHandler*> m_inputHandlers;
	};
}

#endif /* APP_ANDROID_GAME_APPLICATION_H_ */
