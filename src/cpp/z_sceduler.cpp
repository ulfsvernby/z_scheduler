/*
 * z_sceduler.cpp
 *
 *  Created on: Jun 28, 2016
 *      Author: s22708
 */
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <list>
#include "Job.h"

char* zked_cli_parse(string& cmd_str);
int main(int argc, char **argv)
{
	bool dont_stop = true;
	while (dont_stop)
	{
		cout << "ZSCHED> ";
		char buf[1024];
		int len = sizeof(buf) - 1;
		cin.getline(buf, len);
		string cmd = buf;
		char* r =  zked_cli_parse(cmd);
		if( r != nullptr)
		{
			cout << r << endl;
		}
	}
}

