/*
 * Job.cpp
 *
 *  Created on: Jun 29, 2016
 *      Author: s22708
 */
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <ctime>
#include "Job.h"

using namespace std;

static void date_set_today(char* d)
{
	time_t yesterday = time(NULL) - ( 60*60*24 );
	struct tm* seiko = localtime(&yesterday);
	sprintf(d, "%4d%02d%02d", seiko->tm_year+1900, seiko->tm_mon+1, seiko->tm_mday);
}

void Job::setDefaultValues()
{
	is_dcl = 1;
	date_set_today(this->last_done);
}

void Job::set(Job& j)
{
	strcpy(this->command, j.command);
	strcpy(this->func_name, j.func_name);
	strcpy(this->command, j.command);
	strcpy(this->group_account, j.group_account);
	strcpy(this->last_done, j.last_done);
	this->is_dcl = j.is_dcl;
}

bool Job::operator==(const Job j)
{
	return this->order_number == j.order_number;
}

int Job::getOrderNumber()
{
	return order_number;
}

string Job::getCommand()
{
	return command;
}

string Job::getFuncName()
{
	return func_name;
}

string Job::getLastdone()
{
	return last_done;
}

bool Job::getIsDCL()
{
	return is_dcl;
}

string Job::getGroupAccount()
{
	return group_account;
}

void Job::setOrderNumber(int n)
{
	order_number = n;
}

void Job::setCommand(string cmd)
{
	if (cmd.length() >= sizeof(command))
		throw length_error("Command max length is " + sizeof(command) - 1);
	strcpy(command, cmd.c_str());
}

void Job::setFuncName(string fn)
{
	if (fn.length() >= sizeof(Job::func_name))
		throw length_error(
				"FuncName max length is " + sizeof(Job::command) - 1);
	strcpy(Job::func_name, fn.c_str());
}

void Job::setGroupAccount(string fn)
{
	if (fn.length() >= sizeof(Job::group_account))
		throw length_error(
				"GroupAccount max length is " + sizeof(Job::group_account) - 1);
	strcpy(Job::group_account, fn.c_str());
}

void Job::setIsDCL(bool b)
{
	Job::is_dcl = b;
}

void Job::setLastDone(string date)
{
	if (date.length() >= sizeof(Job::last_done))
		throw length_error(
				"LastDone max length is " + sizeof(Job::last_done) - 1);
	strcpy(Job::last_done, date.c_str());
}

string Job::to_string(bool oneLine)
{
	ostringstream sout;
	if (oneLine)
	{
		sout << "order_no" << order_number << " func=" << func_name << " cmd="
				<< "l ast_done=" << last_done << command << "gr_acc="
				<< group_account << " is_dcl=" << is_dcl;
	}
	else
	{
		const char* margin = "    ";
		sout << order_number //
				<< endl << margin << "func_name: " << this->func_name //
				<< endl << margin << "command: " << this->command //
				<< endl << margin << "last_done: " << this->last_done //
				<< endl << margin << "group_account: " << this->group_account //
				<< endl << margin << "is_dcl: " << (int) this->is_dcl;
	}
	return sout.str();
}

char* Job::to_binary()
{
	static char bin_buf[sizeof(Job::command)];
	return bin_buf;
}

