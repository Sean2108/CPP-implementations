#pragma once

#include <memory>
#include <string>
#include <vector>

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
			std::vector<File> files;

			Directory(const std::string& name);

			bool isDirectory() override;
		};

		std::shared_ptr<Directory> m_root;
		// present working directory
		std::shared_ptr<Directory> m_pwd;
	public:
		FileSystem();

		std::shared_ptr<Directory> getRoot() const;
		std::shared_ptr<Directory> getPwd() const;
	};
}


