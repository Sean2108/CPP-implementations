#include "pch.h"

#include "../implementations/filesystem.cpp"

using namespace implementations;

TEST(FileSystemTest, CreateBlankFileSystem) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	const auto pwd = filesystem.getPwd();

	// THEN
	EXPECT_EQ("root", pwd->name);
	EXPECT_TRUE(pwd->files.empty());
}

TEST(FileSystemTest, CreateFileAbsolutePath) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	filesystem.makeFile("/test", false);
	const auto pwd = filesystem.getPwd();

	// THEN
	EXPECT_EQ("root", pwd->name);
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
	EXPECT_EQ("root", pwd->name);
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
	EXPECT_EQ("root", pwd->name);
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

TEST(FileSystemTest, MoveDirectory) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	filesystem.makeFile("test/test1.txt", false, true);
	filesystem.makeFile("test/subtest/test1.txt", false, true);
	filesystem.moveFile("test/subtest", "/test2");
	const auto pwd = filesystem.getPwd();

	// THEN
	ASSERT_EQ(2, pwd->childDirs.size());
	EXPECT_TRUE(pwd->childDirs["test"]->childDirs.empty());
	ASSERT_TRUE(pwd->childDirs.find("test2") != pwd->childDirs.cend());
	EXPECT_TRUE(pwd->childDirs["test2"]->files.find("test1.txt") != pwd->childDirs["test2"]->files.cend());
}

TEST(FileSystemTest, CopyDirectory) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	filesystem.makeFile("test/test1.txt", false, true);
	filesystem.makeFile("test/subtest/test1.txt", false, true);
	filesystem.copyFile("test/subtest", "/test2");
	const auto pwd = filesystem.getPwd();

	// THEN
	ASSERT_EQ(2, pwd->childDirs.size());
	ASSERT_EQ(1, pwd->childDirs["test"]->childDirs.size());
	EXPECT_TRUE(pwd->childDirs["test"]->childDirs.find("subtest") != pwd->childDirs["test"]->childDirs.cend());
	ASSERT_TRUE(pwd->childDirs.find("test2") != pwd->childDirs.cend());
	EXPECT_TRUE(pwd->childDirs["test2"]->files.find("test1.txt") != pwd->childDirs["test2"]->files.cend());
}

TEST(FileSystemTest, PrintTree) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	filesystem.makeFile("test/test1.txt", false, true);
	filesystem.makeFile("test/subtest/test1.txt", false, true);

	// THEN
	EXPECT_EQ("test\n\ttest1.txt\n\tsubtest\n\t\ttest1.txt", filesystem.printTree("test"));
	EXPECT_EQ("root\n\ttest\n\t\ttest1.txt\n\t\tsubtest\n\t\t\ttest1.txt", filesystem.printTree("/"));
}

TEST(FileSystemTest, PrintTreeParallel) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	filesystem.makeFile("test/test1.txt", false, true);
	filesystem.makeFile("test2/subtest/test2.txt", false, true);
	filesystem.makeFile("test3/subtest/test3.txt", false, true);

	// THEN
	EXPECT_EQ("root\n\ttest2\n\t\tsubtest\n\t\t\ttest2.txt\n\ttest\n\t\ttest1.txt\n\ttest3\n\t\tsubtest\n\t\t\ttest3.txt", filesystem.printTree("/"));
}

TEST(FileSystemTest, GetCurrentPath) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	filesystem.makeFile("a/b/c/d/e/f.txt", false, true);
	filesystem.changeDirectory("a/b/c/d/e");
	const auto currentPath = filesystem.getCurrentPath();

	// THEN
	EXPECT_EQ("/a/b/c/d/e", currentPath);
}

TEST(FileSystemTest, FindFile) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	filesystem.makeFile("a/b/c/d/e/f.txt", false, true);
	const auto foundPaths = filesystem.findFile("f.txt");

	// THEN
	ASSERT_EQ(1, foundPaths.size());
	EXPECT_EQ("/a/b/c/d/e/f.txt", foundPaths[0]);
}

TEST(FileSystemTest, FindFileParallel) {
	// GIVEN
	FileSystem filesystem;

	// WHEN
	filesystem.makeFile("a/b/c/d/e/f.txt", false, true);
	filesystem.makeFile("a/g/f.txt", false, true);
	filesystem.makeFile("a/g/k.txt", false, true);
	filesystem.makeFile("a/h/i/f.txt", false, true);
	filesystem.makeFile("a/h/i/n.txt", false, true);
	filesystem.makeFile("a/q.txt", false, true);
	auto foundPaths = filesystem.findFile("f.txt");

	// THEN
	ASSERT_EQ(3, foundPaths.size());
	sort(foundPaths.begin(), foundPaths.end());
	EXPECT_EQ("/a/b/c/d/e/f.txt", foundPaths[0]);
	EXPECT_EQ("/a/g/f.txt", foundPaths[1]);
	EXPECT_EQ("/a/h/i/f.txt", foundPaths[2]);
}