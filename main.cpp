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
		engineParameters_[EP_WINDOW_TITLE] = "NextLife Base V1.0.8.28";
		engineParameters_[EP_WINDOW_WIDTH] = 1280;
		engineParameters_[EP_WINDOW_HEIGHT] = 720;
	}
	virtual void Start()
	{
		//Initialize scene.
		scene_ = new scene(context_, GetSubsystem<ResourceCache>(), GetSubsystem<Renderer>());
		character_ = new Character(context_, scene_->GetScene(), scene_->GetCameraNode() , GetSubsystem<ResourceCache>());
		controls_ = new Controls();

		SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(NextLife, onKeyDown));
		SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(NextLife, Update));
	}
	virtual void Stop()
	{
		//Free.
		delete scene_;
		scene_ = 0;
		delete character_;
		character_ = 0;
	}
private:
	scene* scene_;
	Character* character_;
	Controls* controls_;
	float yaw_=0;
	float pitch_=0;



	void MoveCamera(float timeStep)
	{
		//Move contorl.
		const float MOVE_SPEED = 20.0f;
		const float MOUSE_SENSITIVITY = 0.1f;

		Input* input = GetSubsystem<Input>();

		yaw_+= MOUSE_SENSITIVITY*input->GetMouseMove().x_;
		pitch_+= MOUSE_SENSITIVITY*input->GetMouseMove().y_;
		//Limit pitch.
		pitch_ = Clamp(pitch_, -90.0f, 90.0f);
		scene_->GetCameraNode()->SetRotation(Quaternion(pitch_, yaw_,0.0f));

		if (input->GetKeyDown(KEY_W))
		{
			scene_->GetCameraNode()->Translate(Vector3().FORWARD*MOVE_SPEED*timeStep);
		}
		if (input->GetKeyDown(KEY_S))
		{
			scene_->GetCameraNode()->Translate(Vector3().BACK*MOVE_SPEED*timeStep);
		}
		if (input->GetKeyDown(KEY_A))
		{
			scene_->GetCameraNode()->Translate(Vector3().LEFT*MOVE_SPEED*timeStep);
		}
		if (input->GetKeyDown(KEY_D))
		{
			scene_->GetCameraNode()->Translate(Vector3().RIGHT*MOVE_SPEED*timeStep);
		}
	}

	void Update(StringHash type, VariantMap& data)
	{
		//Deal with Update events
		using namespace Update;
		float timeStep = data[P_TIMESTEP].GetFloat();
		
		//Clear controls
		controls_->Set(CTRL_CLEAR);
		//Get controls;
		Input* input = GetSubsystem<Input>();
		controls_->Set(CTRL_FORWARD, input->GetKeyDown(KEY_W));
		controls_->Set(CTRL_BACK, input->GetKeyDown(KEY_S));
		controls_->Set(CTRL_LEFT, input->GetKeyDown(KEY_A));
		controls_->Set(CTRL_RIGHT, input->GetKeyDown(KEY_D));
		controls_->Set(CTRL_JUMP, input->GetKeyDown(KEY_SPACE));

		controls_->yaw_ += input->GetMouseMove().x_*MOUSE_SENSITIVITY;
		controls_->pitch_+= input->GetMouseMove().y_*MOUSE_SENSITIVITY;

		//MoveCamera(timeStep);
		scene_->Update(timeStep);
		character_->Update(timeStep, controls_);
	}
	void onKeyDown(StringHash type, VariantMap &data)
	{
		//Deal with KEYDOWN events
		using namespace KeyDown;
		if (data[P_KEY].GetInt() == KEY_ESCAPE)//Exit when the key ESC down;
		{
			engine_->Exit();
		}
	}

};

URHO3D_DEFINE_APPLICATION_MAIN(NextLife)