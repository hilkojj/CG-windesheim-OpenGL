#pragma once

#include "../graphics/Model.h"

namespace model_loader
{

	void loadIntoModel(Model &, const char *objPath, const char *texturePathPrefix="assets/models/");

};

