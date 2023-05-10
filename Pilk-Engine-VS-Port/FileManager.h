#pragma once

class Entity;

/// Original Author: Thomas Beet
/// <summary>
/// File manager declarations
/// </summary>
class FileManager final
{
public:
	explicit FileManager() = delete;
	static std::vector<Entity*> LoadEntities(const std::string& p_filePath);
};