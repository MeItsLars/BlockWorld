#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <fstream>

#include "stb_image.h"
#include "spdlog/spdlog.h"

Texture::~Texture() {
    if (data) {
        stbi_image_free(data);
    }
}

void Texture::load(const std::string &path) {
    // Check if file exists
    std::ifstream file(path);
    if (!file.good()) {
        spdlog::error("Failed to load texture: {}", path);
        return;
    }

    spdlog::info(R"([load] texture: "{}")", path);
    if (data) {
        stbi_image_free(data);
    }

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        spdlog::error("Failed to load texture: {}", path);
        return;
    }
    this->data = data;
    this->width = width;
    this->height = height;
    this->nrChannels = nrChannels;
}

