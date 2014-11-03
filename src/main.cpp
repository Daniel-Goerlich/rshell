#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;


//Global Variable(s)
//==================================================================

//==================================================================


// Additional Function(s)
//==================================================================

//==================================================================


int main()
{

	// Variable(s)
	//==================================================================
	string user_commandline; // user input
	vector<string> user_command; // seperated user input
	string temp; // temp string to sperate user input
	int size; // stores the size of user_command
	//==================================================================


	// Output of prompt and gathering of user commands
	//==================================================================
	cout << "$ ";
	getline(cin, user_commandline);
	//==================================================================


	// Seprates user input
	//==================================================================
	istringstream iss;
	iss.str(user_commandline);

	while( iss >> temp )
	{
		user_command.push_back(temp);
	}

	size = user_command.size();

	char **mod_command; // modified command to use with execvp
	mod_command = new char*[ size * sizeof(char*) ];

	for( int i = 0; i < size; ++i )
	{
		mod_command[i] = new char[ temp.length() + 1 ];
		strcpy( mod_command[i], temp.c_str() );
	}

	//==================================================================


	// The code below keeps the program cycling until user enters "exit"
	//==================================================================
	while( user_command.at(0) != "exit" )
	{

		int pid = fork();

		if( pid == -1 ) // Checks for failure in fork
		{
			perror("fork");
			exit(1);
		}

		else if ( pid == 0 ) // Child of the fork
		{
			int pid_3 = execvp( mod_command[0], mod_command );

			if ( pid_3 == -1) // Checks for failure in execvp
			{
				perror("execvp");
				exit(1); 
			}

		}

		else // Parent of the fork
		{

			while( !user_command.empty() )
			{
				user_command.pop_back();
			}

			int pid_2 = wait(NULL); // forces the Child to run first

			if ( pid_2 == -1) // Checks for failure in wait
			{
				perror("wait");
				exit(1);
			}

			cout << "$ ";
			getline(cin, user_commandline);
			istringstream iss_2;
			iss_2.str(user_commandline);

			while( iss_2 >> temp )
			{
				user_command.push_back(temp);
			}

			if( user_command.at(0) == "exit" )
			{
				break;
			}

			size = user_command.size();

			mod_command = new char*[ size * sizeof(char*) ];

			for( int i = 0; i < size; ++i )
			{
				mod_command[i] = new char[ temp.length() + 1 ];
				strcpy( mod_command[i], temp.c_str() );
			}

		}

	}
	//==================================================================


	return 0;

}
