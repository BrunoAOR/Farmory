#include <maz/globals.h>
#include "CImGuiService.h"
#include <maz/CGlfwHandler.h>

#include <external/ImGui/imgui.h>
#include <external/ImGui/imgui_impl_glfw.h>
#include <external/ImGui/imgui_impl_opengl3.h>


namespace maz
{

static const char* kGlslShaderVersion = "#version 330";

CImGuiService::CImGuiService() { ; }

CImGuiService::~CImGuiService() { ; }


bool CImGuiService::Init()
{
    bool lOk = true;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    MAZ_ASSERT(CGlfwHandler::GetWindow() != nullptr, "[CImGuiService]::Init - Call to CGlfwHandler::GetWindow returned a nullptr!");
    lOk = lOk && ImGui_ImplGlfw_InitForOpenGL(CGlfwHandler::GetWindow(), true);
    MAZ_ASSERT(lOk, "[CImGuiService]::Init - Call to ImGui_ImplGlfw_InitForOpenGL failed!");
    lOk = lOk && ImGui_ImplOpenGL3_Init(kGlslShaderVersion);
    MAZ_ASSERT(lOk, "[CImGuiService]::Init - Call to ImGui_ImplOpenGL3_Init failed! (Ignore if ImGui_ImplGlfw_InitForOpenGL failed before)");

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    return lOk;
}


void CImGuiService::End()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


bool CImGuiService::IsOk() const
{
    return true;
}


void CImGuiService::PreUpdate()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


void CImGuiService::Update()
{
    static bool lShowDemoWindow = false;
    static bool lShowOpenGlWindow = false;
    float lMainMenuBarHeight;

    if (ImGui::BeginMainMenuBar())
    {
        lMainMenuBarHeight = ImGui::GetWindowHeight();
        
        if (ImGui::BeginMenu("Main"))
        {
            ImGui::MenuItem("Show demo window", nullptr, &lShowDemoWindow);
            ImGui::Separator();

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Configuration"))
        {
            ImGui::MenuItem("OpenGL", nullptr, &lShowOpenGlWindow);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (lShowDemoWindow)
    {
        ImGui::ShowDemoWindow(&lShowDemoWindow);
    }

    if (lShowOpenGlWindow)
    {
        //ShowOpenGLWindow(lMainMenuBarHeight, &lShowOpenGlWindow);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void CImGuiService::PostUpdate()
{
}

} // maz
