#include "filesystem.h"

namespace implementations {
	FileSystem::File::File(const std::string& name)
		: name(name) {}

	inline bool FileSystem::File::isDirectory() {
		return false;
	}

	FileSystem::Directory::Directory(const std::string& name)
		: File(name)
		, parent()
		, files() {}

	inline bool FileSystem::Directory::isDirectory() {
		return true;
	}

	FileSystem::FileSystem()
		: m_root(std::make_shared<Directory>("/"))
		, m_pwd(m_root) {}

	std::shared_ptr<FileSystem::Directory> FileSystem::getRoot() const {
		return m_root;
	}

	std::shared_ptr<FileSystem::Directory> FileSystem::getPwd() const {
		return m_pwd;
	}
}