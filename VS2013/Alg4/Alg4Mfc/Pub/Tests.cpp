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
	��VS2013Ϊ׼����עx��Ϊvs2015ȥ���ĺ���
	x	basename
	����ָ��·���� ���塣
	Returns the stem of a specified path.
	x	complete
	������Դ�����ָ���� Path ֵ������·����
	Creates a full path from specified Path values, if possible.
	copy_file
	����һ�������Ժ����ݶ�λ��һ���ļ�������ճ����
	Copies the attributes and contents in one file and pastes them in another.
	create_directories
	������һ��ָ�����ļ�����Ŀ¼��
	���½��м�Ŀ¼����Ҫ��
	Creates a directory from a specified file name. Also creates intermediate directories as required.
	create_directory
	������һ��ָ�����ļ�����Ŀ¼��
	Creates a directory from a specified file name.
	create_hard_link
	���� Ӳ���� �������ļ���
	Creates a hard link to an existing file.
	create_symlink
	���� �������� �������ļ���
	Creates a symbolic link to an existing file.
	current_path
	���û������ǰ����Ŀ¼��
	Sets or retrieves the current working directory.
	equivalent
	ָ�����ļ����Ƿ�������ͬ���ļ���
	Specifies whether two file names reference the same file.
	exists
	ָ���ļ��Ƿ���ڡ�	Specifies whether a file exists.
	extension
	������ָ�����ļ�����չ����
	Retrieves the file name extension of a specified file.
	file_size
	���صĴ�С�����ֽ�Ϊ����ָ�����Ƶ��ļ���
	Returns the size, in bytes, of a file that has a specified name.
	initial_path
	���ص�ǰ����Ŀ¼�ڳ���������
	Returns the current working directory at program startup.
	is_directory
	ָʾָ���� file_status ���ļ����Ƿ��ʾĿ¼��
	Indicates whether the specified file_status or file name represents a directory.
	is_empty
	ָ��ָʾ·���Ƿ����ÿ��ļ���Ŀ¼��
	Indicates whether a specified path refers to an empty file or directory.
	is_other
	ָʾָ���� file_status ���ļ����Ƿ��ʾ�����ļ���Ŀ¼��������ӵ������ļ���
	Indicates whether the specified file_status or file name represents an existing file that is not a file, a directory, or a symbolic link.
	is_regular
	ָʾָ���� file_status ���ļ����Ƿ��ʾ�ļ���
	Indicates whether the specified file_status or file name represents a file.
	is_regular_file
	ָʾָ���� file_status ���ļ����Ƿ��ʾ�ļ���
	Indicates whether the specified file_status or file name represents a file.
	is_symlink
	ָʾָ���� file_status ���ļ����Ƿ��ʾ�������ӡ�
	Indicates whether the specified file_status or file name represents a symbolic link.
	last_write_time
	��ȷ��ָ�����ļ����ϴ�д��ʱ�䡣
	Determines the last write time for a specified file.
	remove
	ɾ������ָ��·�����ļ���
	Removes a file that has the specified path.
	remove_all
	ɾ������ָ��·�����ļ���Ŀ¼��
	Removes a file or directory that has the specified path.
	x 	remove_directory
	ɾ������ָ��·����Ŀ¼��
	Removes a directory that has the specified path.
	x	remove_filename
	ɾ������ָ��·�����ļ���
	Removes a file that has the specified path.
	rename
	�����������ļ���
	Renames an existing file.
	resize_file (VS2015)
	�޸ĸ��ļ���С��ɾ���ļ������ݣ��������Ҫ������ʽ���ֽڡ�
	Modifies the size of the file, either removing data at the of the file, or adding zero-formatted bytes as necessary.
	replace_extension
	�滻��ָ���ļ����ļ���չ����
	Replaces the file name extension of the specified file.
	space
	�йؼ���ָ������������Ϣ��
	Retrieves information about a specified volume.
	status
	������ָ�����ļ���״̬��
	Retrieves the status of the specified file.
	status_known
	ָ���ض��ļ�״̬�Ƿ��˽⡣
	Specifies whether a given file status is known.
	stem
	����ָ��·���� ���塣
	Returns the stem of the specified path.
	swap (STL)
	������ basic_path ����洢���ļ�����
	Exchanges the stored file names of two basic_path objects.
	symlink_status
	������ָ�����ļ����ɷ�������״̬��
	Retrieves the symbolic link status of a specified file.
	system_complete
	����޷�ת���������ļ����������ļ�����
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
