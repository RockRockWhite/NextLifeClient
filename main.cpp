//The project is a 3D game.
//EMMMMMM...

#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/Application.h>

#include <Urho3D/Engine/EngineDefs.h>

#include <Urho3D/Engine/EngineEvents.h>

#include<Urho3D/Input/InputEvents.h>


using namespace Urho3D;

class NextLife :public Application
{
public:
	NextLife(Context* context) :
		Application(context)
	{
	}
	virtual void Setup()
	{
		engineParameters_[EP_FULL_SCREEN] = false;
		engineParameters_[EP_WINDOW_TITLE] = "NextLife Base V1.0.8.25";
		engineParameters_[EP_WINDOW_WIDTH] = 1280;
		engineParameters_[EP_WINDOW_HEIGHT] = 720;

	}
	virtual void Start()
	{
		SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(NextLife, onKeyDown));
	}
	virtual void Stop()
	{
	}
private:
	void onKeyDown(StringHash type, VariantMap &data)
	{
		//Deal with KEYDOWN events;
		using namespace KeyDown;
		if (data[P_KEY].GetInt() == KEY_ESCAPE)//Exit when the key ESC down;
		{
			engine_->Exit();
		}
	}
};

URHO3D_DEFINE_APPLICATION_MAIN(NextLife)