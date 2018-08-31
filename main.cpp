//The project is a 3D game.
//EMMMMMM...

#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Engine/EngineEvents.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/Controls.h>
#include <Urho3D/Core/CoreEvents.h>

#include "Character.h"
#include "Scene.h"
#include "GUI.h"

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
		engineParameters_[EP_WINDOW_TITLE] = "NextLife Base V1.0.8.31";
		engineParameters_[EP_WINDOW_WIDTH] = 1280;
		engineParameters_[EP_WINDOW_HEIGHT] = 720;
	}
	virtual void Start()
	{
		//Initialize scene.
		scene_ = new scene(context_, GetSubsystem<ResourceCache>(), GetSubsystem<Renderer>());
		character_ = new Character(context_, scene_->GetScene(), scene_->GetCameraNode() , GetSubsystem<ResourceCache>());
		controls_ = new Controls();
		ui_ = new GUI(context_,engine_, GetSubsystem<UI>()->GetRoot(),GetSubsystem<ResourceCache>());

		SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(NextLife, OnKeyDown));
		SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(NextLife, Update));
		SubscribeToEvent(E_UIMOUSECLICK, URHO3D_HANDLER(NextLife, OnUIClicked));
	}
	virtual void Stop()
	{
		//Free.
		delete scene_;
		scene_ = 0;
		delete character_;
		character_ = 0;
		delete controls_;
		controls_ = 0;
		delete ui_;
		ui_ = 0;
	}
private:
	scene* scene_;
	Character* character_;
	Controls* controls_;
	GUI* ui_;

	void Update(StringHash type, VariantMap& data)
	{
		//Deal with Update events
		using namespace Update;
		float timeStep = data[P_TIMESTEP].GetFloat();

		//Clear
		controls_->Set(CTRL_FORWARD | CTRL_BACK | CTRL_LEFT | CTRL_RIGHT | CTRL_JUMP,false);

		//Get controls
		Input* input=GetSubsystem<Input>();
		
		controls_->Set(CTRL_FORWARD, input->GetKeyDown(KEY_W));
		controls_->Set(CTRL_BACK, input->GetKeyDown(KEY_S));
		controls_->Set(CTRL_LEFT, input->GetKeyDown(KEY_A));
		controls_->Set(CTRL_RIGHT, input->GetKeyDown(KEY_D));
		controls_->Set(CTRL_JUMP, input->GetKeyDown(KEY_SPACE));

		controls_->yaw_ += input->GetMouseMoveX()*MOUSE_SENSITIVITY;
		controls_->pitch_ += input->GetMouseMoveY()*MOUSE_SENSITIVITY;

		character_->Update(timeStep, controls_);
		scene_->Update(timeStep);

	}

	void OnKeyDown(StringHash type, VariantMap &data)
	{
		//Deal with KEYDOWN events
		using namespace KeyDown;
		if (data[P_KEY].GetInt() == KEY_ESCAPE)//Exit when the key ESC down;
		{
			ui_->SetMenuVisible(!ui_->GetMenuVisible());
			GetSubsystem<Input>()->SetMouseVisible(ui_->GetMenuVisible());
		}
	}

	void OnUIClicked(StringHash type, VariantMap& data)
	{
		using namespace Click;
		auto clicked = static_cast<UIElement* >(data[P_ELEMENT].GetPtr());
		if (clicked)
		{
			String name = clicked->GetName();
			ui_->OnClicked(name);
		}
	}

};

URHO3D_DEFINE_APPLICATION_MAIN(NextLife)