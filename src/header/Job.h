/*
 * Job.h
 *
 *  Created on: Jun 29, 2016
 *      Author: s22708
 */
#ifndef HEADER_JOB_H_
#define HEADER_JOB_H_
#include <string>

using namespace std;
class Job {

public:

	int getOrderNumber();
	string getCommand();
	string getFuncName();
	string getGroupAccount();
	bool getIsDCL();
	string getLastdone();

	void setCommand( string cmd );
	void setFuncName( string fn );
	void setOrderNumber( int n );
	void setGroupAccount(string acc);
	void setIsDCL(bool);
	void setLastDone(string date);
	void setDefaultValues();

	bool operator==(const Job j);
	void set( Job& j);
	string to_string(bool online);
	char*  to_binary();

private:
     int order_number;
     char command[101];
     char  func_name[31];
     char group_account[11];
     char  is_dcl;
     char last_done[9];
};

#endif /* HEADER_JOB_H_ */
