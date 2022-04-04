#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace implementations {
	class FileSystem
	{
		struct File {
			std::string name;

			File(const std::string& name);

			virtual bool isDirectory();
		};

		struct Directory : public File {
			std::shared_ptr<Directory> parent;
			std::unordered_map<std::string, std::shared_ptr<File>> files;
			std::unordered_map<std::string, std::shared_ptr<Directory>> childDirs;

			Directory(const std::string& name, const std::shared_ptr<Directory>& parent);

			bool isDirectory() override;
		};

		const std::shared_ptr<Directory> m_root;
		// present working directory
		std::shared_ptr<Directory> m_pwd;

		using SplitPath = std::vector<std::string>;

		SplitPath tokenisePath(std::string&& path) const;
		std::shared_ptr<Directory> getDirectory(SplitPath&& splitPath, const bool shouldCreateMissingDirectories) const;
		std::shared_ptr<File> copyFile(std::string&& sourcePath, std::string&& destPath, const bool shouldRemoveOriginal);
	public:
		FileSystem();

		const std::shared_ptr<Directory> getRoot() const;
		std::shared_ptr<Directory> getPwd() const;

		std::shared_ptr<File> makeFile(std::string&& pathToNewFile, const bool isDirectory, const bool shouldCreateMissingDirectories = false);
		std::shared_ptr<Directory> changeDirectory(std::string&& path);
		std::shared_ptr<File> removeFile(std::string&& pathToRemove);
		std::shared_ptr<File> moveFile(std::string&& sourcePath, std::string&& destPath);
		std::shared_ptr<File> copyFile(std::string&& sourcePath, std::string&& destPath);
	};
}


