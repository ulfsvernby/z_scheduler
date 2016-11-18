/*
 * zked_cli.cpp
 *
 *  Created on: Jul 1, 2016
 *      Author: s22708
 */
#include <cstring>
#include <iostream>
#include <sstream>
#include "Job.h"
#include "JobList.h"
extern "C"
{
#include "ge_cli.h"
}

using namespace std;

JobList cur_job_list;

/*
 * Command Language Definitions
 */
static char *cli_cb(GE_CLI_REPORT *cmd_report);

typedef enum obj_enum
{
	NO_OBJ, JOB_OBJ = 100, LIST_OBJ,
} OBJ_ENUM;

typedef enum cmd_enum
{
	NO_CMD, ADD_CMD = 200, EXIT_CMD, SET_CMD, RUN_CMD, SHOW_CMD, QUIT_CMD,
} CMD_ENUM;

typedef enum joptenum
{
	NO_OPT,
	JOPT_ORDER_NUMBER = 300,
	JOPT_FUNC_NAME,
	JOPT_COMMAND,
	JOPT_LAST_DONE,
	JOPT_IS_DCL,
	JOPT_GROUP_ACCOUNT,
} JOPT_ENUM;

static GE_CLI_VAL job_key_def[] =
{
{ "ORDER_NUMBER", GE_CLI_INT, JOPT_ORDER_NUMBER, 0, 10000000,
GE_CLI_IS_REQ_VAL, 0 },
{ 0, GE_CLI_UNDEFINED_VAL_TYP, 0, 0, 0, 0, 0 }, };

static GE_CLI_VAL job_opt_def[] =
{
{ "FUNC_NAME", GE_CLI_STRING0, JOPT_FUNC_NAME, 1, 30,
GE_CLI_IS_RECORD_INFO, 0 },
{ "COMMAND", GE_CLI_STRING0, JOPT_COMMAND, 1, 100,
GE_CLI_IS_RECORD_INFO, 0 },
{ "LAST_DONE", GE_CLI_STRING0, JOPT_LAST_DONE, 0, 10,
GE_CLI_IS_RECORD_INFO, 0 },
{ "GROUP_ACCOUNT", GE_CLI_STRING0, JOPT_GROUP_ACCOUNT, 0, 10,
GE_CLI_IS_RECORD_INFO, 0 },
{ "IS_DCL", GE_CLI_STRING0, JOPT_IS_DCL, 0, 10,
GE_CLI_IS_RECORD_INFO, 0 },
{ 0, GE_CLI_UNDEFINED_VAL_TYP, 0, 0, 0, 0, 0 }, };

static GE_CLI_SYNTAX show_job_syntax =
{ "SET", SET_CMD, "JOB", JOB_OBJ, job_key_def, job_opt_def, nullptr, cli_cb };

static GE_CLI_SYNTAX set_job_syntax =
{ "SHOW", SHOW_CMD, "JOB", JOB_OBJ, job_key_def, job_opt_def, nullptr, cli_cb };

static GE_CLI_SYNTAX add_job_syntax =
{ "ADD", ADD_CMD, "JOB", JOB_OBJ, job_key_def, job_opt_def, nullptr, cli_cb };

static GE_CLI_SYNTAX run_syntax =
{ "RUN", RUN_CMD, nullptr, 0, nullptr, 0, 0, cli_cb };

static GE_CLI_SYNTAX exit_syntax =
{ "EXIT", EXIT_CMD, nullptr, 0, nullptr, 0, 0, cli_cb };

static GE_CLI_SYNTAX quit_syntax =
{ "QUIT", QUIT_CMD, nullptr, 0, nullptr, 0, 0, cli_cb };

static bool is_init = false;

static bool to_bool(char *param)
{
	return (*param == '0' || *param == 'F') ? false : true;
}
/*
 ******************************************************************************
 **
 **  FUNCTION:
 **
 **		clki_init
 **
 **  FUNCTIONAL DESCRIPTION:
 **
 **
 **  AUTHOR:
 **
 **      Ulf Svernby
 **
 ******************************************************************************
 */
static void cli_init()
{

	char *err_txt;
	/*
	 **  define commands
	 */
	err_txt = ge_cli_add_syntax(&show_job_syntax);
	if (err_txt == nullptr)
		err_txt = ge_cli_add_syntax(&set_job_syntax);
	if (err_txt == nullptr)
		err_txt = ge_cli_add_syntax(&add_job_syntax);
	if (err_txt == nullptr)
		err_txt = ge_cli_add_syntax(&run_syntax);
	if (err_txt == nullptr)
		err_txt = ge_cli_add_syntax(&exit_syntax);
	if (err_txt == nullptr)
		err_txt = ge_cli_add_syntax(&quit_syntax);

	if (err_txt != nullptr)
	{
		puts(err_txt);
		exit(0);
	}
	is_init = true;
}

static void handle_run_cmd()
{
	Job j;
	int cnt = 0, o_num = 0;
	while (cur_job_list.getNext(j, o_num))
	{
		o_num = j.getOrderNumber();
		cnt++;
		cout << "Starting Job " << j.getOrderNumber() << "  " << j.getFuncName()
				<< endl;
		string cmd = "dcl -c \"";
		cmd += j.getCommand();
		cmd += "\"";
		int sts = system(cmd.c_str());
		cout << "    " << sts << " returned from command " << j.getCommand()
				<< endl;
		cout << "End of Job " << j.getOrderNumber() << "  " << j.getFuncName()
				<< endl << endl;
	}
	cout << cnt << " jobs executed" << endl;
}

static void handle_cli_report(GE_CLI_REPORT *cmd_report, ostringstream& result,
		bool& stop_this_process)
{
	Job j;
	int verb = cmd_report->syntax->verb_id_enum;
	switch (verb)
	{
	case EXIT_CMD:
	case QUIT_CMD:
		stop_this_process = true;
		return;
	case ADD_CMD:
	case SET_CMD:
		if (verb == ADD_CMD)
		{
			j = Job();
			j.setDefaultValues();
		}
		else
		{
			if (!cur_job_list.get(j, atol(cmd_report->value_str_array[0])))
			{
				result
						<< "%ZKED-E-ERADJOB, Job not changed - Check if order_number "
						<< j.getOrderNumber() << " exist";
				return;
			}
		}
		// Set Option values
		switch (cmd_report->syntax->obj_id_enum)
		{
		case JOB_OBJ:
			j.setOrderNumber(atol(cmd_report->value_str_array[0]));
			for (int i = 0; i < cmd_report->no_of_options; i++)
			{
				char *p1 = cmd_report->option_value_str_array[i];
				switch (cmd_report->option_def_array[i]->enum_id)
				{
				case JOPT_COMMAND:
					j.setCommand(p1);
					break;
				case JOPT_FUNC_NAME:
					j.setFuncName(p1);
					break;
				case JOPT_LAST_DONE:
					j.setLastDone(p1);
					break;
				case JOPT_GROUP_ACCOUNT:
					j.setGroupAccount(p1);
					break;
				case JOPT_IS_DCL:
					j.setIsDCL(to_bool(p1));
					break;
				default:
					printf("%s/%d: program error, %d unimpemented obj_id %s\n",
							__func__, __LINE__, cmd_report->syntax->obj_id_enum,
							cmd_report->syntax->obj_id);
					exit(20);
				}
			}
			if (verb == ADD_CMD)
			{
				if (!cur_job_list.add(j))
				{
					result
							<< "%ZKED-E-ERADJOB, Job not added - Check if order_number "
							<< j.getOrderNumber() << " already exist";
				}
			}
			else
				cur_job_list.set(j);
			break;
		default:
			printf("%s: program error, %d undefined obj_id_enum\n", __func__,
					cmd_report->syntax->obj_id_enum);
			exit(20);
		}
		break;

	case RUN_CMD:

		handle_run_cmd();
		break;
	case SHOW_CMD:
		switch (cmd_report->syntax->obj_id_enum)
		{
		case JOB_OBJ:
		{
			bool is_ok = cur_job_list.get(j,
					atol(cmd_report->value_str_array[0]));
			if (is_ok)
				result << j.to_string(false);
			else
				result << "%ZKED-E-JNF, job \""
						<< cmd_report->value_str_array[0] << "\" not found";
			break;
		}
		default:
			printf("%s: program error, %d undefined obj_id_enum\n", __func__,
					cmd_report->syntax->obj_id_enum);
			exit(20);
		}
		break;

	default:
		printf("%s/%d: program error, %d undefined verb_id_enum\n", __func__,
		__LINE__, cmd_report->syntax->obj_id_enum);
		exit(20);
	}
}

/*
 ******************************************************************************
 **
 **  FUNCTION:
 **
 **	cli_cb
 **
 **  FUNCTIONAL DESCRIPTION:
 **
 **	This routine is called when a valid command has been received to the
 **	Command Language Interpreter.
 **
 **
 **  AUTHOR:
 **
 **      Ulf Svernby
 **
 ******************************************************************************
 */
static char* cli_cb(GE_CLI_REPORT *cmd_report)
{
	bool stop_this_process = false;
	ostringstream result;
	handle_cli_report(cmd_report, result, stop_this_process);
	if (stop_this_process)
	{
		cout << endl << result.str() << endl;
		cout << "program stopped" << endl;
		exit(0);
	}
	static char tmp[1024];
	strcpy(tmp, result.str().c_str());
	return tmp;
}

char* zked_cli_parse(string& cmd_str)
{
	if (!is_init)
		cli_init();
	return ge_cli_parse_str((char*) cmd_str.c_str());
}
