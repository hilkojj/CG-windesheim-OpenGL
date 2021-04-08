#pragma once

#include "../graphics/Texture.h"

namespace texture_loader
{

	SharedTexture load(const char *path);

	SharedTexture getOrLoad(const char* path);

};

