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

	std::shared_ptr<FileSystem::Directory> FileSystem::getDirectory(SplitPath&& splitPath, const bool shouldCreateMissingDirectories) const {
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
				else if (shouldCreateMissingDirectories) {
					currentDir->childDirs.emplace(token, std::make_shared<Directory>(token, currentDir));
					currentDir = currentDir->childDirs[token];
				}
				else {
					throw std::invalid_argument(token + " is not recognised");
				}
			}
		}
		return currentDir;
	}

	std::shared_ptr<FileSystem::File> FileSystem::makeFile(std::string&& pathToNewDir, const bool isDirectory, const bool shouldCreateMissingDirectories) {
		SplitPath splitPath = tokenisePath(std::move(pathToNewDir));
		const std::string fileToCreate = splitPath.back();
		if (fileToCreate.empty()) {
			throw std::invalid_argument("Create path is invalid");
		}
		splitPath.pop_back();
		std::shared_ptr<Directory> dirToCreateIn = getDirectory(std::move(splitPath), shouldCreateMissingDirectories);
		if (dirToCreateIn->childDirs.find(fileToCreate) == dirToCreateIn->childDirs.cend()
			&& dirToCreateIn->files.find(fileToCreate) == dirToCreateIn->files.cend()) {
			if (isDirectory) {
				dirToCreateIn->childDirs.emplace(fileToCreate, make_shared<FileSystem::Directory>(fileToCreate, dirToCreateIn));
				return dirToCreateIn->childDirs[fileToCreate];
			}
			else {
				dirToCreateIn->files.emplace(fileToCreate, make_shared<FileSystem::File>(fileToCreate));
				return dirToCreateIn->files[fileToCreate];
			}
		}
		throw std::invalid_argument(fileToCreate + " already exists");
	}

	std::shared_ptr<FileSystem::Directory> FileSystem::changeDirectory(std::string&& path) {
		return m_pwd = getDirectory(tokenisePath(std::move(path)), false);
	}

	std::shared_ptr<FileSystem::File> FileSystem::removeFile(std::string&& pathToRemove) {
		SplitPath splitPath = tokenisePath(std::move(pathToRemove));
		const std::string fileToRemove = splitPath.back();
		if (fileToRemove.empty()) {
			throw std::invalid_argument("Remove path is invalid");
		}
		splitPath.pop_back();
		std::shared_ptr<Directory> dirToRemoveFrom = getDirectory(std::move(splitPath), false);
		const auto& removeDirIt = dirToRemoveFrom->childDirs.find(fileToRemove);
		if (removeDirIt != dirToRemoveFrom->childDirs.cend()) {
			const std::shared_ptr<Directory> removedDir = removeDirIt->second;
			dirToRemoveFrom->childDirs.erase(removeDirIt);
			return removedDir;
		}
		const auto& removeFileIt = dirToRemoveFrom->files.find(fileToRemove);
		if (removeFileIt != dirToRemoveFrom->files.cend()) {
			const std::shared_ptr<File> removedFile = removeFileIt->second;
			dirToRemoveFrom->files.erase(removeFileIt);
			return removedFile;
		}
		throw std::invalid_argument(fileToRemove + " does not exist");
	}

	std::shared_ptr<FileSystem::File> FileSystem::copyFile(std::string&& sourcePath, std::string&& destPath, const bool shouldRemoveOriginal) {
		SplitPath splitPath = tokenisePath(std::move(destPath));
		const std::string destFile = splitPath.back();
		if (destFile.empty()) {
			throw std::invalid_argument("Move destination path is invalid");
		}
		splitPath.pop_back();
		std::shared_ptr<Directory> moveDestDir = getDirectory(std::move(splitPath), false);
		if (moveDestDir->childDirs.find(destFile) == moveDestDir->childDirs.cend()
			&& moveDestDir->files.find(destFile) == moveDestDir->files.cend()) {
			const std::shared_ptr<File> copiedFile = 
				shouldRemoveOriginal ? removeFile(std::move(sourcePath)) : getDirectory(tokenisePath(std::move(sourcePath)), false);
			copiedFile->name = destFile;
			if (copiedFile->isDirectory()) {
				moveDestDir->childDirs.emplace(destFile, dynamic_pointer_cast<Directory>(copiedFile));
			}
			else {
				moveDestDir->files.emplace(destFile, copiedFile);
			}
			return copiedFile;
		}
		else {
			throw std::invalid_argument(destFile + " already exists");
		}
	}

	std::shared_ptr<FileSystem::File> FileSystem::copyFile(std::string&& sourcePath, std::string&& destPath) {
		return copyFile(std::move(sourcePath), std::move(destPath), false);
	}

	std::shared_ptr<FileSystem::File> FileSystem::moveFile(std::string&& sourcePath, std::string&& destPath) {
		return copyFile(std::move(sourcePath), std::move(destPath), true);
	}
}