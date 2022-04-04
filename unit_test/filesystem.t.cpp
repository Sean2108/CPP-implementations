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

TEST(FileSystemTest, CreateFileAbsolutePath) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	filesystem.makeFile("/test", false);
	const auto pwd = filesystem.getPwd();

	// THEN
	EXPECT_EQ("/", pwd->name);
	ASSERT_EQ(1, pwd->files.size());
	ASSERT_TRUE(pwd->files.find("test") != pwd->files.cend());
	EXPECT_FALSE(pwd->files["test"]->isDirectory());
	EXPECT_THROW(filesystem.makeFile("test", true), std::invalid_argument);
}

TEST(FileSystemTest, CreateDirectoryRelativePath) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	filesystem.makeFile("test", true);
	filesystem.makeFile("test/file.txt", false);
	const auto pwd = filesystem.getPwd();

	// THEN
	EXPECT_EQ("/", pwd->name);
	ASSERT_EQ(1, pwd->childDirs.size());
	EXPECT_TRUE(pwd->childDirs["test"]->isDirectory());
	ASSERT_EQ(1, pwd->childDirs["test"]->files.size());
	EXPECT_EQ("file.txt", pwd->childDirs["test"]->files["file.txt"]->name);
}

TEST(FileSystemTest, ChangeToSiblingDirectory) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	filesystem.makeFile("test", true);
	filesystem.makeFile("test/inner", true);
	filesystem.makeFile("test/inner2", true);
	filesystem.changeDirectory("test/inner");
	const auto pwd = filesystem.getPwd();
	const auto siblingPwd = filesystem.changeDirectory("../inner2");

	// THEN
	EXPECT_EQ("inner", pwd->name);
	EXPECT_EQ("inner2", siblingPwd->name);
	EXPECT_EQ("test", pwd->parent->name);
	EXPECT_EQ("test", siblingPwd->parent->name);
}