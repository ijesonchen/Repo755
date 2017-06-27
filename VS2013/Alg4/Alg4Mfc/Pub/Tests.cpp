#include "..\stdafx.h"
#include "tests.h"
#include <iostream>
#include <locale>
#include <filesystem>
#include <sstream>

using namespace std;
using namespace std::tr2;

//////////////////////////////////////////////////////////////////////////
// test for tr2::filesystem header
void TestFilesystem(void)
{
	string strPath("C:\\bin\\Python27\\Tools");
	sys::path sysPath(strPath);
	if (sys::is_directory(sysPath))
	{
		cout << strPath << " is directory" << endl;
	}

	/*
	sys::
	以VS2013为准，标注x的为vs2015去除的函数
	x	basename
	返回指定路径的 主体。
	Returns the stem of a specified path.
	x	complete
	如果可以创建从指定的 Path 值的完整路径。
	Creates a full path from specified Path values, if possible.
	copy_file
	在另一副本特性和内容都位于一个文件并将其粘贴。
	Copies the attributes and contents in one file and pastes them in another.
	create_directories
	创建从一个指定的文件名的目录。
	并新建中间目录根据要求。
	Creates a directory from a specified file name. Also creates intermediate directories as required.
	create_directory
	创建从一个指定的文件名的目录。
	Creates a directory from a specified file name.
	create_hard_link
	创建 硬链接 到现有文件。
	Creates a hard link to an existing file.
	create_symlink
	创建 符号链接 到现有文件。
	Creates a symbolic link to an existing file.
	current_path
	设置或检索当前工作目录。
	Sets or retrieves the current working directory.
	equivalent
	指定两文件名是否引用相同的文件。
	Specifies whether two file names reference the same file.
	exists
	指定文件是否存在。	Specifies whether a file exists.
	extension
	检索已指定的文件的扩展名。
	Retrieves the file name extension of a specified file.
	file_size
	返回的大小，以字节为具有指定名称的文件。
	Returns the size, in bytes, of a file that has a specified name.
	initial_path
	返回当前工作目录在程序启动。
	Returns the current working directory at program startup.
	is_directory
	指示指定的 file_status 或文件名是否表示目录。
	Indicates whether the specified file_status or file name represents a directory.
	is_empty
	指定指示路径是否引用空文件或目录。
	Indicates whether a specified path refers to an empty file or directory.
	is_other
	指示指定的 file_status 或文件名是否表示不是文件、目录或符号链接的现有文件。
	Indicates whether the specified file_status or file name represents an existing file that is not a file, a directory, or a symbolic link.
	is_regular
	指示指定的 file_status 或文件名是否表示文件。
	Indicates whether the specified file_status or file name represents a file.
	is_regular_file
	指示指定的 file_status 或文件名是否表示文件。
	Indicates whether the specified file_status or file name represents a file.
	is_symlink
	指示指定的 file_status 或文件名是否表示符号链接。
	Indicates whether the specified file_status or file name represents a symbolic link.
	last_write_time
	来确定指定的文件的上次写入时间。
	Determines the last write time for a specified file.
	remove
	删除具有指定路径的文件。
	Removes a file that has the specified path.
	remove_all
	删除具有指定路径的文件或目录。
	Removes a file or directory that has the specified path.
	x 	remove_directory
	删除具有指定路径的目录。
	Removes a directory that has the specified path.
	x	remove_filename
	删除具有指定路径的文件。
	Removes a file that has the specified path.
	rename
	重命名现有文件。
	Renames an existing file.
	resize_file (VS2015)
	修改该文件大小，删除文件的数据，或根据需要添加零格式的字节。
	Modifies the size of the file, either removing data at the of the file, or adding zero-formatted bytes as necessary.
	replace_extension
	替换中指定文件的文件扩展名。
	Replaces the file name extension of the specified file.
	space
	有关检索指定的数量的信息。
	Retrieves information about a specified volume.
	status
	检索已指定的文件的状态。
	Retrieves the status of the specified file.
	status_known
	指定特定文件状态是否了解。
	Specifies whether a given file status is known.
	stem
	返回指定路径的 主体。
	Returns the stem of the specified path.
	swap (STL)
	交换两 basic_path 对象存储的文件名。
	Exchanges the stored file names of two basic_path objects.
	symlink_status
	检索已指定的文件生成符号链接状态。
	Retrieves the symbolic link status of a specified file.
	system_complete
	如果无法转换给定的文件名。完整文件名。
	Converts a given file name to a complete file name, if possible.
	*/

	// part of path: 
	// C: / bin Python27 Tools
	for (auto it : sysPath)
	{
		cout << it << " ";
	}
	cout << endl;

	// list file in path
	// C:/bin/Python27/Tools/i18n C:/bin/Python27/Tools/pynche ...
	for (auto it = sys::directory_iterator(sysPath), end = sys::directory_iterator();
		it != end; ++it)
	{
		sys::directory_entry en = *it;
		sys::path subpath = it->path();
		cout << subpath.string() << " ";
	}
	cout << endl;

	// list all file in path, include sub directory
	/*
	0 C:/bin/Python27/Tools/i18n
	0 C:/bin/Python27/Tools/pynche
	1 C:/bin/Python27/Tools/pynche/websafe.txt
	1 C:/bin/Python27/Tools/pynche/X
	2 C:/bin/Python27/Tools/pynche/X/rgb.txt
	2 C:/bin/Python27/Tools/pynche/X/xlicense.txt
	*/
	for (auto it = sys::recursive_directory_iterator(sysPath), end = sys::recursive_directory_iterator();
		it != end; ++it)
	{
		int level = it.level();
		sys::path subpath = it->path();
		cout << level << " " << subpath.string() << endl;
	}
	cout << endl;
}
