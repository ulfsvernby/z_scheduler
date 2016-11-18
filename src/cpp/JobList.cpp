/*
 * JobList.cpp
 *
 *  Created on: Jun 29, 2016
 *      Author: s22708
 */
#include <list>
#include <sstream>
#include "JobList.h"

JobList::JobList(string jobFileName)
{
	JobList::job_filename = jobFileName;
}

JobList::JobList()
{
	JobList("");
}

JobList::~JobList()
{
	job_list.clear();
}

bool JobList::add(Job& new_job)
{
	if(new_job.getOrderNumber() == 0 )
		return false;
	// Insert in sorted order
	for (list<Job>::iterator it = job_list.begin(); it != job_list.end(); it++)
	{
		if (it->getOrderNumber() == new_job.getOrderNumber())
			return false;
		if (it->getOrderNumber() > new_job.getOrderNumber())
		{
			job_list.insert(it, new_job);
			return true;
		}
	}
	job_list.push_back(new_job);
	return true;
}

bool JobList::remove(Job& j)
{
	job_list.remove(j);
	return "";
}

bool JobList::remove(int order_number)
{
	for (auto it : job_list)
	{
		if (it.getOrderNumber() == order_number)
		{
			job_list.remove(it);
			return true;
		}
	}
	return false;
}

bool JobList::get(Job& j, int order_number)
{
	for (auto it : job_list)
	{
		if (it.getOrderNumber() == order_number)
		{
			j = it;
			return true;
		}
	}
	return false;
}

bool JobList::getNext(Job& j, int order_number)
{
	bool number_found = false;
	if(order_number == 0)
		number_found=true;		// First iteration
	for (list<Job>::iterator it = job_list.begin(); it != job_list.end(); it++)
	{
		if (number_found)
		{
			j = *it;
			return true;
		}
		if (it->getOrderNumber() >= order_number)
		{
			number_found = true;
		}
	}
	return false;
}

bool JobList::set(Job& j)
{
	for (list<Job>::iterator it = job_list.begin(); it != job_list.end(); it++)

//	for (auto it : job_list)
	{

		if (it->getOrderNumber() == j.getOrderNumber())
		{
			it->set(j);
			return true;
		}
	}
	return false;

}

int JobList::getNumberOfJobs()
{
	return job_list.size();
}

string JobList::getJobFileName()
{
	return job_filename;
}

