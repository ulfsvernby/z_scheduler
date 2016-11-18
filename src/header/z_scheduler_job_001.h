/*
 * z_scheduler_job_001.h
 *
 *  Created on: Jun 28, 2016
 *      Author: s22708
 */

#ifndef HEADER_Z_SCHEDULER_JOB_001_H_
#define HEADER_Z_SCHEDULER_JOB_001_H_

typedef struct _z_job_001
{
	int			order_number;
	char			func_name[31];
	char 		command[101];
	bool			dismiss_flag;
	char			group_account[11];
	char			phase[31];
	char			last_done[9];
	char			revision;
	bool			only_on_bank_days_flag;
	int			dissmiss_day_flag;
	bool			stop_on_failure;
	char			dissmiss_single_date[9];
}Z_JOB_001;


#endif /* HEADER_Z_SCHEDULER_JOB_001_H_ */
