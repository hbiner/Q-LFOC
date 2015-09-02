#ifndef HUANGBIN_H
#define HUANGBIN_H

#include <iostream>
#include <stdlib.h>
#include <io.h>
#include <math.h>
#include <stdio.h>

#include <QThread>
#include <qdatetime.h>
#include "Vmware/controller/commonfactory.h"

#define STATE_NUM 9					//状态数,分别为s1:<2,2>, s2:<2,3>, s3:<2,4>, s4:<3,2>, s5:<3,3>, s6:<3,4>, s7:<4,2>, s8:<4,3>, s9:<4,4>
#define ACTION_NUM 4                //动作数,分别为a1:<1,1>, a2:<1,-1>, a3:<-1,1>, a4:<-1,-1>
#define PARAMETER_NUM 2				//资源变量数目(vCPU1, vCPU2)
#define BASE_ACTION_NUM 2			//基本动作数目increase / decrease
#define INITIAL_STATE 1				//初始状态
#define PARAMETER_RANGE_NUM 2		//界定变量范围所需个数
#define LOOPTIMES 2000				//循环次数
#define INDICATOR_NUM 2				//指标数目
#define SLA_RES	100					//响应时间SLA
#define REF_THRPT 150				//最大吞吐量

class Huangbin
{
public:
	Huangbin();
	~Huangbin();
	int FindMaxQAction(double d[3]);
	int ActionSelection(int iCurrentState);
	void WaitForData(int sec);
	void ReinforcementLearning();
	void ReinforcementLearning_RandomAction();
	void Test();
	void ShowQTable();
	void waitForData(int sec);
	void HandleInTheEnd();
	void ShowInfor(int iState, int iNextState, int iAct, double reward);
	void GetPlatformImmediateInfor();
	void SetPlatformConfiguration(int state);
	
	double RewardFunction();
	double RewardFunctionReturnRandomValue();
	double MaxQValueOfThisState(double q[ACTION_NUM + 1]);

	void GeneratePolicy();      //Generate the policy of current Q table
	bool IsBestPolicy();        //Judging the policy is the best or not
	void ShowPolicy();          //Show the policy

protected:
	void run();

private:
	FILE *m_fpFinalFile, *m_fpProcessFile;
	int m_arrnPolicy[STATE_NUM + 1], m_arrnPolicyVCPU1[STATE_NUM + 1], m_arrnPolicyVCPU2[STATE_NUM + 1];
	VMInfoStruct* vmAPPStruct, vmDBStruct;
};

#endif // HUANGBIN_H
