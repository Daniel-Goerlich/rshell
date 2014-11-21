#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

using namespace std;


//Global Variable(s)
//==================================================================
const int MAX_ARR_SIZE = 1024;
//==================================================================


// Additional Function(s)
//==================================================================
int is_digit(char *curr)
{
	return ( *curr >= '0' && *curr <= '9' );
}
//==================================================================
int mod_command(char command[], char **argv)
{
        int arg_number = 0;
        
        argv[arg_number] = strtok(command, " \t\n");
        
        while ( argv[arg_number] != NULL )
        {
            ++arg_number;
            argv[arg_number] = strtok(NULL, " \t\n");
        }
        
        argv[arg_number + 1] = NULL;  /* ending by null character */
        return arg_number; 
}
//==================================================================
int command_redirection(char userinput[], char **argv)
{
	int mod_flag;
	int in_val = 0;
	int out_val = 1;
    
    char *filein;
    char *curr_in;
    char *curr_out;
    
    char temp_in[MAX_ARR_SIZE];
    char temp_out[MAX_ARR_SIZE];
	char file_out[MAX_ARR_SIZE];
	char dmsg[MAX_ARR_SIZE];
	char c_endline[2];
	
	int got_msg = 0;
	
	char *buffer;	
	buffer = userinput;
	
	for( int i = 0; i < 10; ++i )
	{
    	mod_flag = 0;
    	out_val = 1;
    	
    	if ( ( curr_in = strstr(buffer, "<<<") ) != NULL)
    	{
    		char *begin;
    		char *last;
    		
    		begin = strstr(curr_in + 3 , "\"");
    		last = strstr(begin + 1, "\"");
    		strncpy(dmsg, begin + 1, last - begin - 1);
    		dmsg[last - begin - 1] = 0;
    		got_msg = 1;
    	}
    	
    	else if ( ( curr_in = strstr(buffer,"<") ) != NULL ) 
    	{
		    filein = NULL; 
    	    strcpy(temp_in, curr_in+1);
         	filein = strtok(temp_in, " \n\t><");
         	
         	if ( filein == NULL ) 
         	{
        	  	cout << "Error: Could not find " << userinput << endl;
        	}
     		
     		if ( is_digit(curr_in - 1) )
     		{
    	  		char value[2];
    	  		strncpy(value, curr_in - 1, 1);
    	  		value[1] = 0;
    	  		in_val = atoi(value);
    	  		*(curr_in - 1) = ' ';
     		}
		    
		    buffer = curr_in + 1;
	        int new_stdin;
    		new_stdin = -1; 

    		if ( filein != NULL ) 
    		{
            		new_stdin = open (filein, O_RDONLY);
    		}
    		
    		if ( new_stdin != -1 ) 
    		{
            		close ( in_val );
            		dup( new_stdin );
          			close ( new_stdin );
    		}
    	}
    
    	if ( ( curr_out = strstr(buffer, ">") ) != NULL)
    	{
       		if( *( curr_out + 1 ) == '>' )
       		{
    			strcpy(temp_out, curr_out + 2);
        		mod_flag = 1;
    			*curr_out = 0;
    			buffer = curr_out + 2;
       		}
       		
       		else
       		{
                strcpy(temp_out, curr_out + 1);
                buffer = curr_out + 1;
    	   	}
    	   	
    		*curr_out = 0;
    		
    		char *pbuff;
    		
    		pbuff = strtok(temp_out, " \n\t><");
    		
    		strcpy(file_out, pbuff);
    		
       		if ( is_digit(curr_out - 1) )
       		{
    	 	 	char value[2];
    	  		strncpy(value, curr_out - 1, 1);
    	  		value[1] = 0;
    	  		out_val = atoi(value);
    	  		*(curr_out - 1) = ' ';
       		}
    		
    		int new_stdout;
    		
    		if( mod_flag == 0 )
    		{
                new_stdout = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 
                				S_IRWXU | S_IRWXG | S_IRWXO);
    		}
    		
    		else if( mod_flag == 1 )
    		{
                new_stdout = open(file_out, O_WRONLY | O_CREAT | O_APPEND, 
                				S_IRWXU | S_IRWXG | S_IRWXO);
    		}
    		
    		if( new_stdout != -1 )
    		{
    			close(out_val);
    			dup(new_stdout);
    			close(new_stdout);
    		}
    
    	}
    	
    	if ( curr_in  != NULL )
    	{
        	*curr_in  = 0;
    	}
    	
    	if ( curr_out != NULL )
    	{ 
    	    *curr_out = 0;
    	}
    	
    }
	
	if( got_msg == 1 )
	{
		int fd[2];
		
		pipe(fd);
		
		int pid;
		
		pid = write( fd[1], dmsg, strlen(dmsg) );
		
		if( pid < 0 )
		{
			perror("write");
		}
		
		strcpy(c_endline, "\n");
		
		c_endline[1] = 0 ;
		
		pid = write( fd[1], c_endline, strlen(c_endline) );
		
		if( pid < 0 )
		{
			perror("write");
		}
		
		if( dup2(fd[0], 0) == -1)
		{
			perror("dup2");
			exit (1);
		}
		
		close(fd[1]);
	}
	
        int arg_number = mod_command(userinput, argv);

       return arg_number;      
}
//==================================================================
int command_parse(char userinput[], char **argv)
{
	char **command;
	char *start;
	
	int new_command_point = 0;
    int command_point = 0;
	
	int b_flag = 0;

	command = new char *[50];

	char *curr;
	
	if( ( curr = strstr(userinput, "&") ) != NULL )
	{
	    b_flag = 1;
	    *curr = ' ';
	}

    start = userinput;
    
	int nlen = strlen(userinput);
	
        for( int i = 0; i < nlen ; ++i) 
        {
            if( strncmp( &userinput[i], "|", 1 ) == 0 ) 
            {
                 command[command_point] = start;
                 start = &userinput[i + 1];
                 userinput[i] = 0;
                 ++command_point;
            }
        }
        
    command[command_point] = start;
	new_command_point = command_point;

	int pid;
	int status = 0;
	
	int newfd[2];
    int  fd[2];

	if(new_command_point == 0)
	{

        pid = fork ();
        
        if ( pid == -1 ) 
        {
              perror("fork");
        } 
        
        else if ( pid == 0 ) 
        {
              command_redirection( command[0], argv );
              
              if ( execvp( argv[0], argv ) == -1 )
              {
                  perror("execvp");
              }
              
              exit(1);
        } 
        
        else if ( pid > 0 ) 
        {
            if( b_flag == 0 )
            {
              	wait(&status);
                return status;
            }
            
            else
            {
                return 0;
            }
        }

	}

	else
	{	
		pipe(fd);
		
		switch( pid = fork() )
		{
            case 0: 
            command_redirection( command[0], argv );
            
            if( dup2(fd[1], 1) == -1)
			{
				perror("dup2");
				exit (1);
			}
		
            close( fd[0] );
            
            if ( execvp ( argv[0], argv ) == -1 )
            {
                perror ("execvp");
                exit(1);
            }
            
            case -1:
            perror("fork");
            exit(1);
		}
		
		for( int j = 1; j < new_command_point ; ++j )
		{
			pipe(newfd);
			
			switch( pid = fork() )
			{
                case 0:
                command_redirection(command[j], argv);
                
                if( dup2(fd[0], 0) == -1)
				{
					perror("dup2");
					exit (1);
				}
		
                close( fd[1] );
                
                if( dup2(fd[1], 1) == -1)
				{
					perror("dup2");
					exit (1);
				}
		
                close( newfd[0] ); 
                
                if ( execvp ( argv[0], argv ) == -1 )
                {
                    perror ("exevp");
                    exit (1);
			    }
                
                case -1:
                perror("fork");
                exit(1);
			}
			
			close( fd[0] );
			close( fd[1] );
			
			fd[0] = newfd[0];
			fd[1] = newfd[1];
		}
		
		switch( pid = fork() ) 
		{
            case 0:
            command_redirection( command[new_command_point], argv );
            
            dup2( fd[0], 0 );
            close( fd[1] );
            
            if ( execvp ( argv[0], argv ) == -1 )
            {
                perror ("exevp");
                exit (1);
            }
            
            case -1:
            perror("fork");
            exit(1);
		}
		
		close( fd[0] );
		close( fd[1] );
		
		if( b_flag == 0 )
		{
			while( ( pid = wait(&status) ) != -1 );
			{
			    return status;
			}
		}
		
		else
		{
			return 0;
		}
	}

	return status;
}
//==================================================================


int main()
{
	
	while(true) // Main Loop that Runs the Program
	{
		cout << "$ "; // Output of Prompt
		
		char command[MAX_ARR_SIZE]; // Note: Only stores 2^10 chars
		char buff[MAX_ARR_SIZE]; // Buffer for Command
		
		char *start;
		char *next;
		
		int stat;
		bool is_stat = false;
	
		cin.getline(command, MAX_ARR_SIZE);
		
	//===================Removes Input After Comment====================
		unsigned int count = 0; 
		
		while( count < strlen(command) )
		{
			if( command[count] == '#' )
			{
				command[count] = '\0'; 
				break;
			}
			
			++count;
		}
	//==================================================================


	//======================Kills Program===============================
		if( !strcmp(command, "exit") ) 
		{
		    return 0;
		}
	//==================================================================	
		
		
	//=============Handles Various Operators in Commands================
	
		char **argv;
		argv = new char *[50];

		if( strstr(command, ";") != NULL )
		{
			start = command;
			next = strstr(start, ";");
			
			while(next != NULL)
			{
				for( int i = 0; i < MAX_ARR_SIZE; ++i )
				{ 
				    buff[i]=0;
				}
				
				strncpy(buff, start, next - start);	
				stat = command_parse(buff, argv);
				start = next + 1;
				next = strstr(start, ";");
			}

			for(int i = 0; i < MAX_ARR_SIZE; ++i)
			{ 
			    buff[i]=0;
			}
			
			strcpy(buff, start);
			stat = command_parse(buff, argv);
		}

		else if( strstr(command, "&&") != NULL )
		{
			start = command;
			is_stat = true;
			next = strstr(start, "&&");
			
			while( next != NULL && is_stat )
			{
				for( int i = 0; i < MAX_ARR_SIZE; ++i )
				{ 
				    buff[i] = 0;
				}
				
				strncpy(buff, start, next - start);	
				stat = command_parse(buff, argv);
				
				if(stat != 0) 
				{
					is_stat = false;
				}
				
				start = next + 2;
				next = strstr(start, "&&");
			}
			
			if( is_stat )
			{
				for( int i = 0; i < MAX_ARR_SIZE; ++i )
				{ 
				    buff[i] = 0;
				}
				
				strcpy(buff, start);
				stat = command_parse(buff, argv);
			}
		}

		else if( strstr(command, "||") != NULL )
		{
			start = command;
			is_stat = true;
			next = strstr(start, "||");
			
			while(next != NULL && is_stat)
			{
				for(int i =0; i < MAX_ARR_SIZE; ++i) 
				{
					buff[i] = 0;
				}
				
				strncpy(buff, start, next - start);	
				
				stat = command_parse(buff, argv);
				
				if(stat == 0) 
				{
					is_stat = false;
				}
				
				start = next + 2;
				
				next = strstr(start, "||");
			}
			
			if( is_stat )
			{
				for( int i = 0; i < MAX_ARR_SIZE; ++i )
				{ 
				    buff[i] = 0;	
				}
				
				strcpy(buff, start);
				
				stat = command_parse(buff, argv);
			}
		}
	//==================================================================

		
	//=====================Single Arguement Commands====================
		else
		{
			stat = command_parse(command, argv);
		}
	//==================================================================		
		
		delete[] argv;
		
	}

}