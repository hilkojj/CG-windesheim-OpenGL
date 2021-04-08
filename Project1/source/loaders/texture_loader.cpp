#include "texture_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"
#include <stdexcept>
#include <unordered_map>
#include <iostream>

SharedTexture texture_loader::load(const char* path)
{
    std::cout << "Loading " << path << std::endl;
	
    int width, height, channels;
    unsigned char* imgData = stbi_load(path, &width, &height, &channels, 0);

    if (imgData == NULL)
        throw std::runtime_error("Could not load image using STB: " + std::string(path));

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // todo: shouldnt be hardcoded
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // todo: mipmapping stuff
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
    else if (channels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
    else
        throw std::runtime_error("Loaded an image with unsupported amount of channels (" + std::to_string(channels) + ").");

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imgData);
    return SharedTexture(new Texture(id));
}

SharedTexture texture_loader::getOrLoad(const char* path)
{
    static std::unordered_map<std::string, SharedTexture> sharedTextures;

    if (!sharedTextures[path])
        sharedTextures[path] = load(path);

    return sharedTextures[path];
}
