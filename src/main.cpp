/****** main.cpp
 * IndoPanel UI (SDL + ImGui)
 * Mike Teehan <mike.teehan@gmail.com>
 * Copyright 2024
 * License: GPL (see LICENSE for details)
 ******/

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include <cstdio>
#include <string>
#include <format>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_render.h>

#include "plasma.h"

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

// Main code
int main()
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    SDL_Surface* logoimg = IMG_Load("res/logo.png");
    if(logoimg == NULL)
        SDL_Quit();
    SDL_Texture* logotex = SDL_CreateTextureFromSurface(renderer, logoimg);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);
    ImFontConfig fontcfg;
    fontcfg.GlyphExtraSpacing.x = 1.0f;
    fontcfg.GlyphExtraSpacing.y = 2.0f;
    io.Fonts->AddFontFromFileTTF("res/Inconsolata-Black.ttf", 20.0f, &fontcfg);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;
    Plasma plasma;

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        int xoff = 0, yoff = 0;
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        int ww = 0, wh = 0;
        SDL_GetWindowSize(window, &ww, &wh);
        float xgut = (ww * 0.04f);
        float ygut = (wh * 0.05f);

        {
            ImGui::SetNextWindowPos(ImVec2(xgut, ygut));
            ImGui::SetNextWindowSize(ImVec2(ww - (xgut * 2), wh - (ygut * 2)));

            int winflags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            ImGui::Begin("IndoPanel", NULL, winflags);
            auto w = ImGui::GetCurrentWindow();
            ImGui::SetWindowFontScale(1.8);
            ImGui::BeginTable("button-table", 4, flags);
            ImGuiTable* table = ImGui::GetCurrentTable();
            if(table != nullptr)
            {
                ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("C++", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("DDD", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();

                for(int row = 0; row < 4; row++)
                {
                    ImGui::TableNextRow();
                    for(int col = 0; col < 4; col++)
                        {
                            ImGui::TableSetColumnIndex(col);
                            std::string btntxt = std::format("Button #{}", (row * 4) + col + 1);
                            ImGui::PushItemWidth(ImGui::GetWindowSize().x * 0.25f);
                            ImGui::Button(btntxt.c_str(), ImVec2(-1.0f, 0.0f));
                    }
                }
                ImGui::EndTable();
            }
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        SDL_Surface* plassurf = SDL_CreateRGBSurfaceWithFormat(0, ww, wh, 32, SDL_PIXELFORMAT_RGB888);
        SDL_Texture* plastex = SDL_CreateTextureFromSurface(renderer, plasma.frame(plassurf));
        SDL_Rect po = SDL_Rect((ww - 700) / 2, (wh - 700) / 2, 700, 700);
        SDL_RenderCopy(renderer, plastex, NULL, NULL);
        SDL_RenderCopy(renderer, logotex, NULL, &po);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
        SDL_FreeSurface(plassurf);
        SDL_DestroyTexture(plastex);
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
