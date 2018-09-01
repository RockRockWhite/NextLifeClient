#pragma once
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/ProgressBar.h>
#include <Urho3D/UI/LineEdit.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Resource/ResourceCache.h>

using namespace Urho3D;

class GUI
{
public:
	GUI(Context* context, SharedPtr<Engine> engine, UIElement* UIRoot, ResourceCache* cache ) :
		context_(context), engine_(engine), root_(UIRoot), cache_(cache)
	{
		engine_ = engine;
		root_->SetDefaultStyle(cache_->GetResource<XMLFile>("UI/DefaultStyle.xml"));
		CreateLoginWindow();
		CreateMainUI();
		CreateMenu();
	}

	void OnClicked(String name)
	{
		if (name == "CancelButton")
		{
			MenuWindow->SetVisible(false);
			engine_->GetSubsystem<Input>()->SetMouseVisible(false);
		
		}
			
		if (name == "ExitButton")
			engine_->Exit();
	}
	void CreateLoginWindow()
	{
	
		engine_->GetSubsystem<Input>()->SetMouseVisible(true);
		
		//LoginWindow
		auto* LoginWindow = root_->CreateChild<Window>("LoginWindow");
		LoginWindow->SetSize(350, 200);
		LoginWindow->SetStyleAuto();
		LoginWindow->SetAlignment(HA_CENTER, VA_CENTER);

		//LoginWindowText
		auto* LoginWindowText = LoginWindow->CreateChild<Text>("LoginWindowText");
		LoginWindowText->SetText("Login");
		LoginWindowText->SetStyleAuto();
		LoginWindowText->SetAlignment(HA_CENTER, VA_TOP);
		LoginWindowText->SetFontSize(40);

		//UserText
		auto* UserText = LoginWindow->CreateChild<Text>("UserText");
		UserText->SetText("User:");
		UserText->SetStyleAuto();
		UserText->SetFontSize(13);
		UserText->SetPosition(5, 60);

		//UserLineEdit
		auto* UserLineEdit = LoginWindow->CreateChild<LineEdit>("UserLineEdit");
		UserLineEdit->SetStyleAuto();
		UserLineEdit->SetSize(230,30) ;
		UserLineEdit->SetPosition(100,60);


		//PasswordsText
		auto* PasswordsText = LoginWindow->CreateChild<Text>("PasswordsText");
		PasswordsText->SetText("Passwords:");
		PasswordsText->SetStyleAuto();
		PasswordsText->SetFontSize(13);
		PasswordsText->SetPosition(5, 100);

		//PasswordsLineEdit
		auto* PasswordsLineEdit = LoginWindow->CreateChild<LineEdit>("PasswordsLineEdit");
		PasswordsLineEdit->SetStyleAuto();
		PasswordsLineEdit->SetSize(230, 30);
		PasswordsLineEdit->SetPosition(100, 100);
		PasswordsLineEdit->SetEchoCharacter(42);
	}
	void CreateMainUI()
	{

		/***********************************************************************
		TestText
		***********************************************************************/
		auto* TestText = root_->CreateChild<Text>("TestText");
		TestText->SetText("NextLife Client Base V1.0.9.01");
		TestText->SetStyleAuto();
		TestText->SetAlignment(HA_LEFT, VA_BOTTOM);
		TestText->SetPosition(5, 0);

		/***********************************************************************
		StateWindow
		***********************************************************************/
		//Create StateWindow 
		auto* StateWindow = new Window(context_);
		StateWindow->SetName("StateWindow");
		root_->AddChild(StateWindow);
		StateWindow->SetSize(300,130);
		StateWindow->SetAlignment(HA_LEFT, VA_TOP);
		StateWindow->SetStyleAuto();

		//UserNameText
		auto* UserNameText = StateWindow->CreateChild<Text>("UserNameText");
		UserNameText->SetText("Rock White");
		UserNameText->SetStyleAuto();
		UserNameText->SetFontSize(14);
		UserNameText->SetAlignment(HA_CENTER, VA_TOP);

		//Health Text
		auto* HealthText = StateWindow->CreateChild<Text>("HealthText");
		HealthText->SetText("Health:");
		HealthText->SetStyleAuto();
		HealthText->SetFontSize(12);
		HealthText->SetPosition(5, 30);

		//Health Bar
		auto* HealthBar = StateWindow->CreateChild<ProgressBar>("HealthBar");
		HealthBar->SetStyleAuto();
		HealthBar->SetSize(200, 20);
		HealthBar->SetValue(100.0f);
		HealthBar->SetPosition(90, 30);

		//FoodBar Text
		auto* FoodText = StateWindow->CreateChild<Text>("FoodText");
		FoodText->SetText("Food:");
		FoodText->SetStyleAuto();
		FoodText->SetFontSize(12);
		FoodText->SetPosition(5, 60);

		//Food Bar
		auto* FoodBar = StateWindow->CreateChild<ProgressBar>("FoodBar");
		FoodBar->SetStyleAuto();
		FoodBar->SetSize(200, 20);
		FoodBar->SetValue(100.0f);
		FoodBar->SetPosition(90, 60);

		//IntellectBar Text
		auto* IntellectText = StateWindow->CreateChild<Text>("IntellectText");
		IntellectText->SetText("Intellect:");
		IntellectText->SetStyleAuto();
		IntellectText->SetFontSize(12);
		IntellectText->SetPosition(5, 90);

		//Intellect Bar
		auto* IntellectBar = StateWindow->CreateChild<ProgressBar>("IntellectBar");
		IntellectBar->SetStyleAuto();
		IntellectBar->SetSize(200, 20);
		IntellectBar->SetValue(100.0f);
		IntellectBar->SetPosition(90, 90);

		/***********************************************************************
		MapWindow
		***********************************************************************/
		//Create MapWindow
		auto* MapWindow = root_->CreateChild<Window>("MapWindow");
		MapWindow->SetStyleAuto();
		MapWindow->SetSize(300, 330);
		MapWindow->SetAlignment(HA_RIGHT, VA_BOTTOM);

		//TODO Text
		auto* TODOText = MapWindow->CreateChild<Text>("TODOText");
		TODOText->SetText("Map to be here.");
		TODOText->SetAlignment(HA_CENTER, VA_CENTER);
		TODOText->SetStyleAuto();
		TODOText->SetFontSize(25);


	    //TimeText
		auto* TimeText = MapWindow->CreateChild<Text>("TimeText");
		TimeText->SetText("Alive: 3 days     Time: 11:15");
		TimeText->SetStyleAuto();
		TimeText->SetAlignment(HA_CENTER, VA_BOTTOM);

	}


	void CreateMenu()
	{
		//Create Menu Window
		MenuWindow = new Window(context_);
		MenuWindow->SetName("MenuWindow");
		root_->AddChild(MenuWindow);
		MenuWindow->SetWidth(355);
		MenuWindow->SetHeight(250);
		MenuWindow->SetAlignment(HA_CENTER, VA_CENTER);
		MenuWindow->SetName("Menu");
		MenuWindow->SetStyleAuto();
		MenuWindow->SetVisible(false);

		//Create Menu Bar
		auto* MenuTitle = new Text(context_);
		MenuTitle->SetText("Menu");
		MenuWindow->AddChild(MenuTitle);
		MenuTitle->SetName("MenuTitle");
		MenuTitle->SetAlignment(HA_CENTER, VA_TOP);
		MenuTitle->SetStyleAuto();
		MenuTitle->SetFontSize(40);

		//Create SettingsButton
		auto* SettingsButton = new Button(context_);
		SettingsButton->SetName("SettingsButton");
		MenuWindow->AddChild(SettingsButton);
		auto* SettingsButtonText = SettingsButton->CreateChild<Text>("SettingsButtonText");
		SettingsButtonText->SetText("Settings");
		SettingsButtonText->SetStyleAuto();
		SettingsButtonText->SetFontSize(20);
		SettingsButtonText->SetAlignment(HA_CENTER, VA_CENTER);
		SettingsButton->SetStyleAuto();
		SettingsButton->SetAlignment(HA_CENTER,VA_CUSTOM);
		SettingsButton->SetPosition(0, 50);
		SettingsButton->SetMinHeight(50);
		SettingsButton->SetMinWidth(300);

		//Create CancelButton
		auto* CancelButton = new Button(context_);
		CancelButton->SetName("CancelButton");
		MenuWindow->AddChild(CancelButton);
		auto* CancelButtonText = CancelButton->CreateChild<Text>("CancelButtonText");
		CancelButtonText->SetText("Cancel");
		CancelButtonText->SetStyleAuto();
		CancelButtonText->SetFontSize(20);
		CancelButtonText->SetAlignment(HA_CENTER, VA_CENTER);
		CancelButton->SetStyleAuto();
		CancelButton->SetAlignment(HA_CENTER, VA_CUSTOM);
		CancelButton->SetPosition(0, 120);
		CancelButton->SetMinHeight(50);
		CancelButton->SetMinWidth(300);

		//Create ExitButton
		auto* ExitButton = new Button(context_);
		ExitButton->SetName("ExitButton");
		MenuWindow->AddChild(ExitButton);
		auto* ExitButtonText = ExitButton->CreateChild<Text>("ExitButtonText");
		ExitButtonText->SetText("Exit");
		ExitButtonText->SetStyleAuto();
		ExitButtonText->SetFontSize(20);
		ExitButtonText->SetAlignment(HA_CENTER, VA_CENTER);
		ExitButton->SetStyleAuto();
		ExitButton->SetAlignment(HA_CENTER, VA_CUSTOM);
		ExitButton->SetPosition(0, 190);
		ExitButton->SetMinHeight(50);
		ExitButton->SetMinWidth(300);

		//My Logo
		auto* WhiteButton = MenuWindow->CreateChild<Button>("White");
		WhiteButton->SetTexture(cache_->GetResource<Texture2D>("C:/Users/White/Pictures/White.jpg"));
		WhiteButton->SetPosition(50, 260);
		WhiteButton->SetSize(100, 100);
	}

	void SetMenuVisible(bool Visible)
	{
		MenuWindow->SetVisible(Visible);
	}
	bool GetMenuVisible()
	{
		return MenuWindow->IsVisible();
	};

private:
	SharedPtr<Engine> engine_;
	Context* context_;
	UIElement* root_;
	ResourceCache* cache_;

	SharedPtr<Window> MenuWindow;
	SharedPtr<Window> StateWindow;
};

