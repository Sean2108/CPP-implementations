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

TEST(FileSystemTest, CreateFileRecursively) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	filesystem.makeFile("test/test1/test2.txt", false, true);
	const auto pwd = filesystem.getPwd();

	// THEN
	EXPECT_EQ("/", pwd->name);
	ASSERT_EQ(1, pwd->childDirs.size());
	EXPECT_TRUE(pwd->childDirs["test"]->isDirectory());
	ASSERT_EQ(1, pwd->childDirs["test"]->childDirs.size());
	EXPECT_EQ("test1", pwd->childDirs["test"]->childDirs["test1"]->name);
	ASSERT_EQ(1, pwd->childDirs["test"]->childDirs["test1"]->files.size());
	EXPECT_EQ("test2.txt", pwd->childDirs["test"]->childDirs["test1"]->files["test2.txt"]->name);
}

TEST(FileSystemTest, RemoveFile) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	filesystem.makeFile("test/test1.txt", false, true);
	filesystem.makeFile("test/test2.txt", false, false);
	const auto pwd = filesystem.getPwd();
	ASSERT_EQ(1, pwd->childDirs.size());
	EXPECT_TRUE(pwd->childDirs["test"]->isDirectory());
	EXPECT_EQ(2, pwd->childDirs["test"]->files.size());
	filesystem.removeFile("test/test2.txt");

	// THEN
	EXPECT_EQ(1, pwd->childDirs["test"]->files.size());
	EXPECT_TRUE(pwd->childDirs["test"]->files.find("test1.txt") != pwd->childDirs["test"]->files.cend());
	EXPECT_TRUE(pwd->childDirs["test"]->files.find("test2.txt") == pwd->childDirs["test"]->files.cend());
}