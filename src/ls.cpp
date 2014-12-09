#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <pwd.h>

using namespace std;


//Global Variable(s)
//==================================================================
const string flag_a = "-a";
const string flag_l = "-l";
const string flag_R = "-R";
const string flag_a_l = "-al";
const string flag_a_R = "-aR";
const string flag_l_a = "-la";
const string flag_l_R = "-lR";
const string flag_R_a = "-Ra";
const string flag_R_l = "-Rl";
//==================================================================


// Additional Function(s)
//==================================================================

void print( vector<string>& list ) // Prints basic ls
{
	int row_count = 0;

	for( unsigned int i = 0; i < list.size(); ++i ) 
	{

		if( row_count == 4 ) // Determines the number of rows in output
		{
			cout << endl;
		}

		// The line below gives a modified output of all files in the list
		cout << setw(20) << left << list.at(i) << "  ";

		++row_count;
	}

	cout << endl;

}

//==================================================================

void print_l( vector<string>& list ) // Prints ls -l
{
	struct stat status;
	int intake;	

	string temp;
	string curr;

	ssize_t size = 0;

	for( unsigned int i = 0; i < list.size(); ++i ) 
	{
		curr = list[i];

		if( stat(curr.c_str(), &status) == -1 ) 
		{
			perror("stat");
			exit(1);
		}

		size += status.st_size;
	}

	cout << "total " << size << endl;	

	for( unsigned int i = 0; i < list.size(); ++i ) 
	{
		temp = list[i];

		if( stat(temp.c_str(), &status) == -1 ) 
		{
			perror("stat");
			exit(1);
		}

		if( S_ISDIR(status.st_mode) )
		{
			cout << "d";
		}

		else
		{
			cout << "-";
		}

		if( (intake = access(temp.c_str(), R_OK) ) == 0 )
		{
			if( intake == -1 )
			{
				perror( "access" );
				exit(1);
			}

			cout << "r";
		}

		else
		{
			cout << "-";
		}

		if( (intake = access(temp.c_str(), W_OK) ) == 0 )
		{
			if( intake == -1 )
			{
				perror( "access" );
				exit(1);
			}

			cout << "w";
		}

		else
		{
			cout << "-";
		}

		if( (intake = access(temp.c_str(), X_OK) ) == 0 )
		{
			if( intake == -1 )
			{
				perror( "access" );
				exit(1);
			}

			cout << "x";
		}

		else
		{
			cout << "-";
		}

		cout << "------" << " ";		


		cout << status.st_nlink << " ";
		cout << status.st_uid << " ";
		cout << status.st_gid << " ";
		cout << setw(5) << status.st_size << " ";
		cout << setw(5) << status.st_mtime << "sec ";			
		cout << left << setw(0) << list[i] << endl;
	}

}

//==================================================================

void list_sort( vector<string>& list ) // Sorts the list of files for ls
{

	vector<string> temp;

	for( unsigned int i = 0; i < list.size(); ++i )
	{
		if( list[i][0] >= 'A' && list[i][0] <= 'Z' )
		{
			temp.push_back( list[i] );
		}
	}

	sort( list.begin(), list.end() );

}

//==================================================================

void file_search() // Basic ls
{
	vector<string> file_list;

	DIR *directory;

	struct dirent *entry;

	char *curr;

	if( ( directory=opendir(".") ) == NULL ) 
	{
		perror( "opendir" );
		return;
	}

	do 
	{
		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			curr = entry->d_name;						

			if( curr[0] != '.' ) 
			{
				file_list.push_back( entry->d_name );;
			}

		}	

	} while(entry != NULL);

	list_sort( file_list );
	print( file_list );

	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}

}

//==================================================================

void file_search(char *curr_directory) // ls w/ directory passed in
{

	vector<string> file_list;

	DIR *directory;

	struct dirent *entry;

	char *curr;

	if( ( directory = opendir(curr_directory) ) == NULL ) 
	{
		perror("opendir");
		return;
	}

	do {

		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			curr = entry->d_name;						

			if( curr[0] == '.' ) 
				continue;

			file_list.push_back(entry->d_name);
		}	

	} while( entry != NULL );

	list_sort(file_list);
	print(file_list);

	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}	

}

//==================================================================

void file_search_a() // ls -a
{
	vector<string> file_list;

	DIR *directory;

	struct dirent *entry;

	if( ( directory = opendir(".") ) == NULL ) 
	{
		perror( "opendir" );
		return;
	}

	errno = 0;

	if( ( entry = readdir(directory) ) != NULL ) 
	{	
		file_list.push_back( entry->d_name );
	}

	while(entry != NULL) 
	{
		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			file_list.push_back( entry->d_name );
		}	

	} 

	list_sort( file_list );
	print( file_list );

	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}

}

//==================================================================

void file_search_a(char *curr_directory) // ls -a w/ directory passed in
{

	vector<string> file_list;

	DIR *directory;

	struct dirent *entry;

	if( ( directory = opendir(curr_directory) ) == NULL ) 
	{
		perror( "opendir" );
		return;
	}

	errno = 0;

	if( ( entry = readdir(directory) ) != NULL ) 
	{	
		file_list.push_back( entry->d_name );
	}

	while(entry != NULL) 
	{
		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			file_list.push_back( entry->d_name );
		}	

	} 

	list_sort( file_list );
	print( file_list );

	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}

}

//==================================================================

void file_search_R() // ls -R
{

	DIR *directory;

	struct dirent *entry;

	char *curr;

	if( ( directory = opendir(".") ) == NULL ) 
	{
		perror("opendir");
		return;
	}

	do {

		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			if( entry->d_type == DT_DIR )
			{						
				curr = entry->d_name;

				if( curr[0] == '.' && curr[1] == '.' )
					continue;

				cout << curr << ":" << endl;
				file_search(curr);
				cout << endl;

			}
		}	

	} while( entry != NULL );


	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}

}

//==================================================================

void file_search_R(char *curr_directory) // ls -R w/ directory passed in
{

	DIR *directory;

	struct dirent *entry;

	char *curr;

	if( ( directory = opendir(curr_directory) ) == NULL ) 
	{
		perror("opendir");
		return;
	}

	do {

		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			if( entry->d_type == DT_DIR )
			{						
				curr = entry->d_name;

				if( curr[0] == '.' && curr[1] == '.' )
					continue;

				cout << curr << ":" << endl;
				file_search(curr);
				cout << endl;

			}
		}	

	} while( entry != NULL );


	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}

}

//==================================================================

void file_search_l() // ls -l
{

	vector<string> file_list;

	DIR *directory;

	struct dirent *entry;

	char *curr;

	if( ( directory = opendir(".") ) == NULL ) 
	{
		perror( "opendir" );
		return;
	}

	do 
	{
		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			curr = entry->d_name;						

			if( curr[0] != '.' ) 
			{
				file_list.push_back( entry->d_name );;
			}
		}	

	} while(entry != NULL);


	list_sort( file_list );
	print_l( file_list );

	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}	

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}

}

//==================================================================

void file_search_l(char *curr_directory) // ls -l w/ directory passed in
{

	vector<string> file_list;

	DIR *directory;

	struct dirent *entry;

	char *curr;

	if( ( directory = opendir(".") ) == NULL ) 
	{
		perror( "opendir" );
		return;
	}

	do 
	{
		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			curr = entry->d_name;						

			if( curr[0] != '.' ) 
			{
				file_list.push_back( entry->d_name );;
			}
		}	

	} while(entry != NULL);


	list_sort( file_list );
	print_l( file_list );

	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}	

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}

}

//==================================================================

void file_search_l_a() // ls -l -a
{
	vector<string> file_list;

	DIR *directory;

	struct dirent *entry;

	if( ( directory = opendir(".") ) == NULL ) 
	{
		perror( "opendir" );
		return;
	}

	errno = 0;

	if( ( entry = readdir(directory) ) != NULL ) 
	{	
		file_list.push_back( entry->d_name );
	}

	while(entry != NULL) 
	{
		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			file_list.push_back( entry->d_name );
		}	

	} 

	list_sort( file_list );
	print_l( file_list );

	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}
}

//==================================================================

void file_search_l_a(char *curr_directory) // ls -l -a w/ directory passed in
{
	vector<string> file_list;

	DIR *directory;

	struct dirent *entry;

	if( ( directory = opendir(curr_directory) ) == NULL ) 
	{
		perror( "opendir" );
		return;
	}

	errno = 0;

	if( ( entry = readdir(directory) ) != NULL ) 
	{	
		file_list.push_back( entry->d_name );
	}

	while(entry != NULL) 
	{
		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			file_list.push_back( entry->d_name );
		}	

	} 

	list_sort( file_list );
	print_l( file_list );

	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}
}

//==================================================================

void file_search_a_R() // ls -a -R
{
	DIR *directory;

	struct dirent *entry;

	char *curr;

	if( ( directory = opendir(".") ) == NULL ) 
	{
		perror("opendir");
		return;
	}

	do {

		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			if( entry->d_type == DT_DIR )
			{						
				curr = entry->d_name;

				if( curr[0] == '.' && curr[1] == '.' )
					continue;

				cout << curr << ":" << endl;
				file_search_a(curr);
				cout << endl;

			}
		}	

	} while( entry != NULL );


	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}
}

//==================================================================

void file_search_a_R(char *curr_directory) // ls -a -R w/ directory passed in
{
	DIR *directory;

	struct dirent *entry;

	char *curr;

	if( ( directory = opendir(curr_directory) ) == NULL ) 
	{
		perror("opendir");
		return;
	}

	do {

		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			if( entry->d_type == DT_DIR )
			{						
				curr = entry->d_name;

				if( curr[0] == '.' && curr[1] == '.' )
					continue;

				cout << curr << ":" << endl;
				file_search_a(curr);
				cout << endl;

			}
		}	

	} while( entry != NULL );


	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}
}

//==================================================================

void file_search_l_R() // ls -l -R
{
	DIR *directory;

	struct dirent *entry;

	char *curr;

	if( ( directory = opendir(".") ) == NULL ) 
	{
		perror("opendir");
		return;
	}

	do {

		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			if( entry->d_type == DT_DIR )
			{						
				curr = entry->d_name;

				if( curr[0] == '.' && curr[1] == '.' )
					continue;

				cout << curr << ":" << endl;
				file_search_l(curr);
				cout << endl;

			}
		}	

	} while( entry != NULL );


	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}
}

//==================================================================

void file_search_l_R(char *curr_directory) // ls -l -R w/ directory passed in
{
	DIR *directory;

	struct dirent *entry;

	char *curr;

	if( ( directory = opendir(curr_directory) ) == NULL ) 
	{
		perror("opendir");
		return;
	}

	do {

		errno = 0;

		if( ( entry = readdir(directory) ) != NULL ) 
		{	
			if( entry->d_type == DT_DIR )
			{						
				curr = entry->d_name;

				if( curr[0] == '.' && curr[1] == '.' )
					continue;

				cout << curr << ":" << endl;
				file_search_l(curr);
				cout << endl;

			}
		}	

	} while( entry != NULL );


	if( closedir(directory) == -1)
	{
		perror( "closedir" );
		exit(1);
	}

	if(errno != 0)
	{
		perror( "Failed in reading directory" );
		return;
	}
}

//==================================================================


int main( int argc, char **argv )
{

	vector<string> flags;

	flags.push_back(flag_a);
	flags.push_back(flag_l);
	flags.push_back(flag_R);
	flags.push_back(flag_a_l);
	flags.push_back(flag_l_a);
	flags.push_back(flag_a_R);
	flags.push_back(flag_R_a);
	flags.push_back(flag_l_R);
	flags.push_back(flag_R_l);

	if( argc == 2 ) // ls w/ 1 arguement
	{

		if( argv[1] == flag_a )
		{
			file_search_a();
		}

		else if( argv[1] == flag_l )
		{
			file_search_l();
		}

		else if( argv[1] == flag_R )
		{
			file_search_R();
		}

		else if( argv[1] == flag_a_l || argv[1] == flag_l_a )
		{
			file_search_l_a();
		}

		else if( argv[1] == flag_a_R || argv[1] == flag_R_a )
		{
			file_search_a_R();
		}

		else if( argv[1] == flag_l_R || argv[1] == flag_R_l )
		{
			file_search_l_R();
		}

		else
		{
			file_search(argv[1]);
		}

	}

	else if( argc == 3 ) // ls w/ 2 arguements
	{

		for( unsigned int i = 0; i < flags.size(); ++i )
		{
			if( argv[1] == flags.at(i) )
			{
				break;
			}

			if( i == 8 )
			{
				cout << "Error: Invalid Arguement(s)" << endl;
				return 0;
			}
		}

		if( argv[1] == flag_a_l || argv[1] == flag_l_a )
		{
			file_search_l_a( argv[2] );
		}

		else if( argv[1] == flag_a_R || argv[1] == flag_R_a )
		{
			file_search_a_R( argv[2] );
		}

		else if( argv[1] == flag_l || argv[1] == flag_R )
		{
			file_search_l_R( argv[2] );
		}

		if( argv[1] == flag_a && argv[2] == flag_l )
		{
			file_search_l_a();
		}

		else if( argv[1] == flag_a && argv[2] == flag_R )
		{
			file_search_a_R();
		}

		if( argv[1] == flag_l && argv[2] == flag_a )
		{
			file_search_l_a();
		}

		else if( argv[1] == flag_l && argv[2] == flag_R )
		{
			file_search_l_R();
		}

		if( argv[1] == flag_R && argv[2] == flag_a )
		{
			file_search_a_R();
		}

		else if( argv[1] == flag_R && argv[2] == flag_l )
		{
			file_search_l_R();
		}

		if( argv[1] == flag_a )
		{
			file_search_a( argv[2] );
		}

		else if( argv[1] == flag_l )
		{
			file_search_l( argv[2] );
		}

		else if( argv[1] == flag_R )
		{
			file_search_R( argv[2] );
		}

	}

	else if( argc == 4 ) // ls w/ 3 arguements
	{

		for( unsigned int i = 0; i < flags.size(); ++i )
		{
			if( argv[1] == flags.at(i) )
			{
				break;
			}

			if( i == 8 )
			{
				cout << "Error: Invalid Arguement(s)" << endl;
				return 0;
			}
		}

		for( unsigned int i = 0; i < flags.size(); ++i )
		{
			if( argv[2] == flags.at(i) )
			{
				break;
			}

			if( i == 8 )
			{
				cout << "Error: Invalid Arguement(s)" << endl;
				return 0;
			}
		}

		if( argv[1] == flag_a && argv[2] == flag_l )
		{
			file_search_l_a(argv[3]);
		}

		else if( argv[1] == flag_a && argv[2] == flag_R )
		{
			file_search_a_R(argv[3]);
		}

		if( argv[1] == flag_l && argv[2] == flag_a )
		{
			file_search_l_a(argv[3]);
		}

		else if( argv[1] == flag_l && argv[2] == flag_R )
		{
			file_search_l_R(argv[3]);
		}

		if( argv[1] == flag_R && argv[2] == flag_a )
		{
			file_search_a_R(argv[3]);
		}

		else if( argv[1] == flag_R && argv[2] == flag_l )
		{
			file_search_l_R(argv[3]);
		}
		//===============================================
	}

	else // Default ls
	{
		file_search();
	}

	return 0;
}
