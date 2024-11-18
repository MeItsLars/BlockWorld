#include "GUI.h"

#include "ObjectChecker.h"
#include "../Game/BlockWorldGame.h"

GUI::GUI(BlockWorldGame *blockWorldGame) {
    spdlog::info("Creating GUI instance");
    this->game = blockWorldGame;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    game->getWindow()->initUI();

    // Setup style
    // setStyle(io);

    // Position and size
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(300, displaySize.y));
}

void GUI::setStyle(ImGuiIO &io) {
    ImGuiStyle *style = &ImGui::GetStyle();

    auto &colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
    colors[ImGuiCol_MenuBarBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Border
    colors[ImGuiCol_Border] = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
    colors[ImGuiCol_BorderShadow] = ImVec4{0.0f, 0.0f, 0.0f, 0.24f};

    // Text
    colors[ImGuiCol_Text] = ImVec4{1.0f, 1.0f, 1.0f, 1.0f};
    colors[ImGuiCol_TextDisabled] = ImVec4{0.5f, 0.5f, 0.5f, 1.0f};

    // Headers
    colors[ImGuiCol_Header] = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_HeaderActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_CheckMark] = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};

    // Popups
    colors[ImGuiCol_PopupBg] = ImVec4{0.1f, 0.1f, 0.13f, 0.92f};

    // Slider
    colors[ImGuiCol_SliderGrab] = ImVec4{0.44f, 0.37f, 0.61f, 0.54f};
    colors[ImGuiCol_SliderGrabActive] = ImVec4{0.74f, 0.58f, 0.98f, 0.54f};

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{0.13f, 0.13, 0.17, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_FrameBgActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TabHovered] = ImVec4{0.24, 0.24f, 0.32f, 1.0f};
    colors[ImGuiCol_TabActive] = ImVec4{0.2f, 0.22f, 0.27f, 1.0f};
    colors[ImGuiCol_TabUnfocused] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
    colors[ImGuiCol_ScrollbarGrab] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{0.24f, 0.24f, 0.32f, 1.0f};

    // Separator
    colors[ImGuiCol_Separator] = ImVec4{0.44f, 0.37f, 0.61f, 1.0f};
    colors[ImGuiCol_SeparatorHovered] = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};
    colors[ImGuiCol_SeparatorActive] = ImVec4{0.84f, 0.58f, 1.0f, 1.0f};

    // Resize Grip
    colors[ImGuiCol_ResizeGrip] = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
    colors[ImGuiCol_ResizeGripHovered] = ImVec4{0.74f, 0.58f, 0.98f, 0.29f};
    colors[ImGuiCol_ResizeGripActive] = ImVec4{0.84f, 0.58f, 1.0f, 0.29f};

    // Docking
    style->TabRounding = 4;
    style->ScrollbarRounding = 9;
    style->WindowRounding = 7;
    style->GrabRounding = 3;
    style->FrameRounding = 3;
    style->PopupRounding = 4;
    style->ChildRounding = 4;

    // Fonts
    font = io.Fonts->AddFontFromFileTTF("res/fonts/Roboto-Regular.ttf", 16.0f);
}

GUI::~GUI() {
    game->getWindow()->destroyUI();
    ImGui::DestroyContext();
}

void GUI::drawGUI() {
    // Start the Dear ImGui frame
    game->getWindow()->beginUIFrame();
    ImGui::NewFrame();

    // Load font
    // ImGui::PushFont(font);

    // Main window
    ImGui::Begin("BlockWorld", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // if (ImGui::CollapsingHeader("Models")) {
    //     ImGui::Text("Number of models: %llu", renderer->scene->models.size());
    //     for (int i = 0; i < static_cast<int>(renderer->scene->models.size()); i++) {
    //         Model &model = renderer->scene->models[i];
    //         std::string modelName = model.name;
    //
    //         if (ImGui::TreeNodeEx(modelName.c_str())) {
    //             ImGui::Text("Vertices: %llu - Faces: %llu", model.getVertexCount(), model.getFaceCount());
    //
    //             for (int j = 0; j < static_cast<int>(model.materialGroups.size()); j++) {
    //                 MaterialGroup &materialGroup = model.materialGroups[j];
    //                 std::string materialGroupName = "Material Group '" + materialGroup.name + "'";
    //
    //                 if (ImGui::TreeNodeEx(materialGroupName.c_str())) {
    //                     ImGui::Text("Start index: %u - Count: %u", materialGroup.startIndex, materialGroup.count);
    //                     ImGui::ColorEdit3("Ambient", glm::value_ptr(materialGroup.material.ambient));
    //                     ImGui::ColorEdit3("Diffuse", glm::value_ptr(materialGroup.material.diffuse));
    //                     ImGui::ColorEdit3("Specular", glm::value_ptr(materialGroup.material.specular));
    //                     ImGui::SliderFloat("Shininess", &materialGroup.material.shininess, 0.0f, 100.0f, "%1.0f", ImGuiSliderFlags_AlwaysClamp);
    //                     ImGui::TreePop();
    //                 }
    //             }
    //
    //             // Texture
    //             ImGui::Checkbox("Use two part mapping", &model.useTwoPartMapping);
    //             if (ImGui::Button(("Texture: " + model.texturePath).c_str())) {
    //                 fileDialog.OpenDialog("ChooseTextureDlgKey", "Choose texture file", ".jpg,.png,.bmp", ".");
    //             }
    //             if (fileDialog.Display("ChooseTextureDlgKey")) {
    //                 if (fileDialog.IsOk()) {
    //                     model.texturePath = fileDialog.GetFilePathName();
    //                     renderer->getModelRenderer()->updateTexture(&model);
    //                 }
    //                 fileDialog.Close();
    //             }
    //
    //             // Buttons
    //             ImGui::SameLine();
    //             if (ImGui::Button("Reset")) {
    //                 model.modelMatrix = glm::mat4(1.0f);
    //                 float scale = 1.0f / renderer->scene->scale;
    //                 model.scale(scale, scale, scale);
    //             }
    //             ImGui::SameLine();
    //             if (ImGui::Button("Teleport")) {
    //                 glm::vec4 modelPositionObject = glm::vec4(model.center, 1.0f);
    //                 glm::vec4 modelPositionWorld = model.modelMatrix * modelPositionObject;
    //                 renderer->scene->camera.position = glm::vec3(modelPositionWorld);
    //                 renderer->scene->camera.updateViewMatrix();
    //                 renderer->update(UpdateType::VIEW_MATRIX);
    //             }
    //             ImGui::TreePop();
    //         }
    //     }
    // }
    //
    // if (ImGui::CollapsingHeader("Projection")) {
    //     const char* items[] = {"Perspective", "Parallel" };
    //
    //     bool projChanged = ImGui::Combo("Projection Type", &renderer->scene->camera.projectionType, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items));
    //     switch (renderer->scene->camera.projectionType) {
    //         case Camera::PERSPECTIVE:
    //             projChanged |= ImGui::SliderFloat("Field of view",&renderer->scene->camera.fov, 20.0f, 160.0f, "%1.0f", ImGuiSliderFlags_AlwaysClamp);
    //             projChanged |= ImGui::SliderFloat("Far",&renderer->scene->camera.farPlane, 1.0f, 1000.0f, "%1.0f", ImGuiSliderFlags_AlwaysClamp);
    //             break;
    //         case Camera::PARALLEL:
    //             projChanged |= ImGui::SliderFloat("Top",&renderer->scene->camera.top, 1.0f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
    //             projChanged |= ImGui::SliderFloat("Far",&renderer->scene->camera.farPlane, 1.0f, 1000.0f, "%1.0f", ImGuiSliderFlags_AlwaysClamp);
    //             projChanged |= ImGui::SliderFloat("Oblique scale",&renderer->scene->camera.obliqueScale, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
    //             projChanged |= ImGui::SliderFloat("Oblique angle", &renderer->scene->camera.obliqueAngle, 15, 75, "%1.0f", ImGuiSliderFlags_AlwaysClamp);
    //             break;
    //         default:
    //             std::cout << "Invalid projection type" << std::endl;
    //             break;
    //     }
    //
    //     if (projChanged) {
    //         renderer->scene->camera.updateProjectionMatrix();
    //         renderer->update(UpdateType::PROJECTION_MATRIX);
    //     }
    // }
    //
    // if (ImGui::CollapsingHeader("Ambient Light")) {
    //     if (ImGui::ColorEdit3("Ambient Light Color", glm::value_ptr(renderer->scene->ambientLight.color))) {
    //         renderer->update(UpdateType::AMBIENT_LIGHT);
    //     }
    // }
    //
    // if (ImGui::CollapsingHeader("Light")) {
    //     bool lightChanged = false;
    //     lightChanged |= ImGui::SliderFloat("X", &renderer->scene->light.position.x, -20.0f, 20.0f, "%1.0f", ImGuiSliderFlags_AlwaysClamp);
    //     lightChanged |= ImGui::SliderFloat("Y", &renderer->scene->light.position.y, -20.0f, 20.0f, "%1.0f", ImGuiSliderFlags_AlwaysClamp);
    //     lightChanged |= ImGui::SliderFloat("Z", &renderer->scene->light.position.z, -20.0f, 20.0f, "%1.0f", ImGuiSliderFlags_AlwaysClamp);
    //     lightChanged |= ImGui::ColorEdit3("Light Color", glm::value_ptr(renderer->scene->light.color));
    //     if (lightChanged) {
    //         renderer->update(UpdateType::LIGHT);
    //     }
    // }

    if (ImGui::CollapsingHeader("Game Info", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("FPS: %.2f", game->getFPS());
        ImGui::Text("Memory: %lu MB", getUsedMemory() / 1024 / 1024);
        std::string vsyncText = "VSync: " + std::string(game->getWindow()->isVSync() ? "On" : "Off");
        if (ImGui::Button(vsyncText.c_str())) {
            game->getWindow()->setVSync(!game->getWindow()->isVSync());
        }
        ImGui::Text("Vertex count: %lu", game->getVertexCount());
    }

    int renderDistance = game->getActiveWorld()->getRenderDistance();
    if (ImGui::CollapsingHeader("World", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("WSPS: %.2f", game->getActiveWorld()->getChunkManager()->getWsps());
        ImGui::Text("Chunks loaded: %lu", game->getActiveWorld()->getChunkManager()->getActiveChunkCount());
        if (ImGui::SliderInt("Render distance", &renderDistance, 1, 76)) {
            game->getActiveWorld()->setRenderDistance(renderDistance);
        }
        ImGui::Text("OC Chunks Loaded: %d", ObjectChecker::getChunkCount());
        ImGui::Text("OC SubChunks Loaded: %d", ObjectChecker::getSubChunkCount());
        ImGui::Text("OC Renderables Loaded: %d", ObjectChecker::getRenderableCount());
        ImGui::Text("OC Meshes Loaded: %d", ObjectChecker::getMeshCount());
    }

    if (ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_DefaultOpen)) {
        Player *player = game->getActiveWorld()->getPlayer();
        glm::vec3 location = player->getLocation();
        glm::vec3 direction = player->getDirection();
        float speed = player->getSpeed();

        ImGui::Text("Location: (%.2f, %.2f, %.2f)", location.x, location.y, location.z);
        ImGui::Text("Direction: (%.2f, %.2f, %.2f)", direction.x, direction.y, direction.z);
        if (ImGui::SliderFloat("Speed", &speed, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp)) {
            player->setSpeed(speed);
        }
    }

    if (ImGui::CollapsingHeader("Camera")) {
        Camera *camera = game->getCamera();
        glm::vec3 position = camera->getPosition();
        glm::vec3 direction = camera->getDirection();
        glm::vec3 up = camera->getUp();

        ImGui::Text("Position: (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
        ImGui::Text("Direction: (%.2f, %.2f, %.2f)", direction.x, direction.y, direction.z);
        ImGui::Text("Up: (%.2f, %.2f, %.2f)", up.x, up.y, up.z);
        if (ImGui::SliderFloat("Near", &camera->nearPlane, 0.01f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp)
            || ImGui::SliderFloat("Far", &camera->farPlane, 100.0f, 10000.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp)) {
            camera->updateProjectionMatrix();
        }
        if (ImGui::Button("Reset camera")) {
            camera->reset();
            camera->updateViewMatrix();
        }
    }

    ImGui::End();

    // Unload font
    // ImGui::PopFont();

    // Rendering
    ImGui::Render();
    game->getWindow()->endUIFrame();
}
