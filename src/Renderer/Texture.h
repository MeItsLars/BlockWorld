#pragma once
#include <string>

class Texture {
public:
    int width = 0;
    int height = 0;
    int nrChannels = 0;

    ~Texture();

    void load(const std::string &path);

    [[nodiscard]] unsigned char *getData() const {
        return data;
    }

private:
    unsigned char *data = nullptr;
};
