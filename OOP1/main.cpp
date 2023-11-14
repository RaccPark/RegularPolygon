#include <iostream>
#include <cstdlib> // include rand()
#include <Windows.h>
#include <conio.h>
#include "GameObject.h"
#include "Utils.h"
#include "InputManager.h"


//
int main()
{
	GameObject::Initialize();
    InputManager input;
	bool exit_flag = false;
    
	while (exit_flag == false)
	{
		GameObject::ClearFrame();
		input.readEveryFrame();

		GameObject::ProcessInput(exit_flag, input);

		GameObject::ExamineCollision();

		GameObject::UpdateAll(input);
		
		GameObject::DrawAll();

		GameObject::RenderFrame();
	}

    GameObject::Deinitialize();

	return 0;
}

