#include "pch.h"

#include "../implementations/filesystem.cpp"

using namespace implementations;

TEST(FileSystemTest, CreateBlankFileSystem) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	const auto pwd = filesystem.getPwd();

	// THEN
	EXPECT_EQ("/", pwd->name);
	EXPECT_TRUE(pwd->files.empty());
}