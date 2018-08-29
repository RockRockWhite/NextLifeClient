#pragma once
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Resource/ResourceCache.h>

using namespace Urho3D;

class GUI
{
public:
	GUI(Context* context,UIElement* UIRoot, ResourceCache* cache):
		context_(context), root_(UIRoot),cache_(cache)
	{
		root_->SetDefaultStyle(cache_->GetResource<XMLFile>("UI/DefaultStyle.xml"));
		CreateMenu();
	}
	void OnClicked(String name)
	{
		if (name == "CancelButton")
			MenuWindow->SetVisible(false);
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
	Context* context_;
	UIElement* root_;
	ResourceCache* cache_;
	SharedPtr<Window> MenuWindow;
};

