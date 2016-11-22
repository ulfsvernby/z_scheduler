/*
 * z_sceduler.cpp
 *
 *  Created on: Jun 28, 2016
 *      Author: s22708
 */

#include <readline/history.h>
#include <readline/readline.h>
#include <iostream>
using namespace std;
char* zked_cli_parse(char *cmd_str);
int main(int argc, char **argv)
{
	using_history();
	char *prompt = "ZSCHED> ";

	for (;;)
	{
		char *cmd = readline(prompt);
		if (cmd)
		{
			char* r = zked_cli_parse(cmd);
			if (r != nullptr)
			{
				cout << r << endl;
			}
			add_history(cmd);
			free(cmd);
		}
		else
			cout << "cmd is empty" << endl;
	}
}

