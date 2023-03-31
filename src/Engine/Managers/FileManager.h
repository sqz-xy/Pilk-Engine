#pragma once

#include "../PilkEngineCommon.h"
#include "../Objects/Entity.h"

class FileManager final
{
public:
	explicit FileManager() = delete;
	static std::vector<Entity> LoadEntities(const std::string& p_filePath);
};