/*
 * JobList.h
 *
 *  Created on: Jun 29, 2016
 *      Author: s22708
 */

#ifndef HEADER_JOBLIST_H_
#define HEADER_JOBLIST_H_

#include <list>
#include <string>
#include "Job.h"

using namespace std;

class JobList {
public:

JobList(string jobFileName);
JobList();
~JobList();

bool add( Job& new_job );
bool remove( Job& J );
bool remove( int order_number );
bool get(Job& j,  int order_number );
bool getNext(Job& j,  int order_number );
bool set( Job& j);

int getNumberOfJobs();

string getJobFileName();


private:
list<Job>  job_list;
string job_filename;

};

#endif /* HEADER_JOBLIST_H_ */
