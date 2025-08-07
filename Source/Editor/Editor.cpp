#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Input/Input.h>
#include <Editor/EditorApplication.h>
#include <Script.Squirrel/ScriptFile.h>

using namespace Urho3D;

void Foundation_SquirrelFileTypes(Context* context, Project* project)
{
    project->AddAnalyzeFileCallback([](ResourceFileDescriptor& desc, const AnalyzeFileContext& ctx)
        {
            if (desc.HasExtension({".nut"}))
                desc.AddObjectType<ScriptFile>();
        });
}

class CustomEditorApplication : public EditorApplication
{
public:
    using EditorApplication::EditorApplication;

    /// Implement EditorApplication.
    /// @{
    void Setup() override
    {
        editorPluginManager_->AddPlugin("Foundation.SquirrelFileTypes", &Foundation_SquirrelFileTypes);

        EditorApplication::Setup();
        LinkedPlugins::RegisterStaticPlugins();

        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(CustomEditorApplication, HandleKeyDown));
    }

    void HandleKeyDown(StringHash eventType, VariantMap& eventData)
    {
        auto input = GetSubsystem<Input>();
        bool isMouseFree = input->GetMouseMode() == MM_FREE;
        bool isMouseVisible = input->IsMouseVisible();

        const auto key = static_cast<Key>(eventData[KeyDown::P_KEY].GetUInt());
        if (key == KEY_ESCAPE)
        {
            if (isMouseFree && isMouseVisible)
            {
                input->SetMouseMode(MM_RELATIVE);
                input->SetMouseVisible(false);
            }
            else
            {
                input->SetMouseMode(MM_FREE);
                input->SetMouseVisible(true);
            }
        }
    }
    /// @}
};

URHO3D_DEFINE_APPLICATION_MAIN(CustomEditorApplication);
