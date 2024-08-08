#include "CoreMinimal.h"

#include "HAL/Filesystem.h"
#include "HAL/ImpulseThread.h"

#include "App.h"
#include "Environment.h"

#include "Platform/PlatformEntryPoint.h"

class FEditorApplication : public IApplication
{
	DEFAULT_APP_BODY(FEditorApplication);

public:

	virtual void Run() override
	{
		FWindowPtr window = MakeWindow();
		FWindowDefinitionPtr windowDefinition = MakeWindowDefinition();

		windowDefinition->bIsRegularWindow = true;
		windowDefinition->bHasOSWindowBorder = true;
		windowDefinition->XDesiredPosition = 25;
		windowDefinition->YDesiredPosition = 25;
		windowDefinition->WidthDesiredSize = 1280;
		windowDefinition->HeightDesiredSize = 720;
		windowDefinition->Title = TEXT("Impulse Engine Editor");

		window->Init(windowDefinition, nullptr, true);

		while (window->IsOpen())
		{
			MSG message;
			while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
	}
};

IApplication* EntryPointGetApplication()
{
	FAppInfo info;

	info.Name = TEXT("Editor");
	info.Description = TEXT("Impulse Engine Editor");
	info.Version = MAKE_VERSION(1, 0, 0); // TODO: Get version from file

	return new FEditorApplication(info);
}