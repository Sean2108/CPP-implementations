#include "filesystem.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace implementations {
	FileSystem::File::File(const std::string& name)
		: name(name) {}

	inline bool FileSystem::File::isDirectory() {
		return false;
	}

	FileSystem::Directory::Directory(const std::string& name, const std::shared_ptr<Directory>& parent)
		: File(name)
		, parent(parent)
		, files()
		, childDirs() {}

	inline bool FileSystem::Directory::isDirectory() {
		return true;
	}

	FileSystem::FileSystem()
		: m_root(std::make_shared<Directory>("/", nullptr))
		, m_pwd(m_root) {}

	const std::shared_ptr<FileSystem::Directory> FileSystem::getRoot() const {
		return m_root;
	}

	std::shared_ptr<FileSystem::Directory> FileSystem::getPwd() const {
		return m_pwd;
	}

	FileSystem::SplitPath FileSystem::tokenisePath(std::string&& path) const {
		std::stringstream stream(path);
		std::string token;
		if (path.back() == '/') {
			path.pop_back();
		}
		SplitPath splitPath(std::count(path.cbegin(), path.cend(), '/') + 1, "");
		size_t index = 0;
		while (std::getline(stream, token, '/')) {
			splitPath[index++] = token;
		}
		return splitPath;
	}

	std::shared_ptr<FileSystem::Directory> FileSystem::getDirectory(SplitPath&& splitPath) const {
		if (splitPath.empty()) {
			return m_pwd;
		}
		std::shared_ptr<FileSystem::Directory> currentDir = splitPath[0].empty() ? m_root : m_pwd;
		for (const std::string& token : splitPath) {
			if (token.empty() || token == ".") {
				continue;
			}
			else if (token == "..") {
				if (!currentDir->parent) {
					throw std::invalid_argument(currentDir->name + " does not have a parent directory");
				}
				currentDir = currentDir->parent;
			}
			else {
				const auto nextDirIt = currentDir->childDirs.find(token);
				if (nextDirIt != currentDir->childDirs.cend()) {
					if (!nextDirIt->second->isDirectory()) {
						throw std::invalid_argument(nextDirIt->first + " is not a directory");
					}
					currentDir = nextDirIt->second;
				}
				else {
					throw std::invalid_argument(token + " is not recognised");
				}
			}
		}
		return currentDir;
	}

	std::shared_ptr<FileSystem::File> FileSystem::makeFile(std::string&& pathToNewDir, const bool isDirectory) {
		SplitPath splitPath = tokenisePath(std::move(pathToNewDir));
		const std::string fileToCreate = splitPath.back();
		splitPath.pop_back();
		std::shared_ptr<Directory> dirToCreateIn = getDirectory(std::move(splitPath));
		if (dirToCreateIn->childDirs.find(fileToCreate) == dirToCreateIn->childDirs.cend()
			&& dirToCreateIn->files.find(fileToCreate) == dirToCreateIn->files.cend()) {
			if (isDirectory) {
				const auto createdDir = make_shared<FileSystem::Directory>(fileToCreate, dirToCreateIn);
				dirToCreateIn->childDirs.emplace(fileToCreate, createdDir);
				return createdDir;
			}
			else {
				const auto createdFile = make_shared<FileSystem::File>(fileToCreate);
				dirToCreateIn->files.emplace(fileToCreate, createdFile);
				return createdFile;
			}
		}
		throw std::invalid_argument(fileToCreate + " already exists");
	}

	std::shared_ptr<FileSystem::Directory> FileSystem::changeDirectory(std::string&& path) {
		return m_pwd = getDirectory(tokenisePath(std::move(path)));
	}
}