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

#define STATE_NUM 9					//״̬��,�ֱ�Ϊs1:<2,2>, s2:<2,3>, s3:<2,4>, s4:<3,2>, s5:<3,3>, s6:<3,4>, s7:<4,2>, s8:<4,3>, s9:<4,4>
#define ACTION_NUM 4                //������,�ֱ�Ϊa1:<1,1>, a2:<1,-1>, a3:<-1,1>, a4:<-1,-1>
#define PARAMETER_NUM 2				//��Դ������Ŀ(vCPU1, vCPU2)
#define BASE_ACTION_NUM 2			//����������Ŀincrease / decrease
#define INITIAL_STATE 1				//��ʼ״̬
#define PARAMETER_RANGE_NUM 2		//�綨������Χ�������
#define LOOPTIMES 2000				//ѭ������
#define INDICATOR_NUM 2				//ָ����Ŀ
#define SLA_RES	100					//��Ӧʱ��SLA
#define REF_THRPT 150				//���������

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
