#include "ReinforcementLearning_vCPU1_vCPU2.h"
using namespace std;

const int g_nParameterRange[PARAMETER_NUM + 1][PARAMETER_RANGE_NUM + 1] = 	//������Χ
{
	{ -8, -8, -8 },		//�ڱ�
	{ -8, 2, 4 },		//��һ������vCPU1
	{ -8, 2, 4 }		//�ڶ�������vCPU2
};
int g_nAction[ACTION_NUM + 1][PARAMETER_NUM + 1] =		//����������廯
{
	{ -8, -8, -8 },		//�ڱ�
	{ -8, 1, 1 },		//a1
	{ -8, 1, -1 },
	{ -8, -1, 1 },
	{ -8, -1, -1 }		//a4
};
int g_nStateConfig[STATE_NUM + 1][PARAMETER_NUM + 1] =	//״̬��Ӧ�ĸ���Դ����
{
	//
	{ -8, -8, -8 },		//�ڱ�
	{ -8, 2, 2 },		//s1
	{ -8, 2, 3 },
	{ -8, 2, 4 },
	{ -8, 3, 2 },
	{ -8, 3, 3 },
	{ -8, 3, 4 },
	{ -8, 4, 2 },
	{ -8, 4, 3 },
	{ -8, 4, 4 }		//s9
};
int g_nNextState[STATE_NUM + 1][ACTION_NUM + 1] = 		//״̬ת����
{
	{ -8, -8, -8, -8, -8 },	//�ڱ�
	{ -8, 5, 0, 0, 0 },		//s1
	{ -8, 6, 4, 0, 0 },		//s2
	{ -8, 0, 5, 0, 0 },		//s3
	{ -8, 8, 0, 2, 0 },		//s4
	{ -8, 9, 7, 3, 1 },		//s5
	{ -8, 0, 8, 0, 2 },		//s6
	{ -8, 0, 0, 2, 0 },		//s7
	{ -8, 0, 0, 3, 4 },		//s8
	{ -8, 0, 0, 0, 5 }		//s9
};
double Q[STATE_NUM + 1][ACTION_NUM + 1] = 		//Qֵ��
{
	{ -8, -8, -8, -8, -8 },			//�ڱ�
	{ -8, 0.0, 0.0, 0.0, 0.0 },		//s1��action��Qֵ
	{ -8, 0.0, 0.0, 0.0, 0.0 },
	{ -8, 0.0, 0.0, 0.0, 0.0 },
	{ -8, 0.0, 0.0, 0.0, 0.0 },
	{ -8, 0.0, 0.0, 0.0, 0.0 },
	{ -8, 0.0, 0.0, 0.0, 0.0 },
	{ -8, 0.0, 0.0, 0.0, 0.0 },
	{ -8, 0.0, 0.0, 0.0, 0.0 },
	{ -8, 0.0, 0.0, 0.0, 0.0 }		//s9��action��Qֵ
};
int g_nCurrentState = 1;			//��ǰ����״̬
const double g_dGama = 0.9;			//gamaΪ�ۿ�����
const double g_dAlph = 0.05;		//alphΪѧϰ��
const int g_nBaseActionType[BASE_ACTION_NUM + 1] = { -8, -1, 1 };		//������������
double g_dIndicator[INDICATOR_NUM + 1] = { -8, 70.0, 90.0 };			//����ָ�꣨��Ҷ�ʺ���Ӧʱ�䣩

//�ļ���������ʽ:PATH_DATE_LOOPTIMES_STEP_INITSTATE_K_g_a_Pressure_ThisDayTestTimes_FinalOrProcess
const char g_strFinalFile[] = "D:\\final.txt";
const char g_strProcessFile[] = "D:\\process.txt";


/*
*	���캯��
*/
Huangbin::Huangbin()
{
	cout << "���캯��!!!!!!" << endl;
	cout << "Class Initiating..." << endl;
	srand(100);				//���������
	GetPlatformImmediateInfor();

	//�����쳣�������ļ������ɣ��´β��Խ���ɾ����
	if (_access(g_strProcessFile, 0) == 0)
	{
		if (remove(g_strProcessFile) == 0)
		{
			//qDebug() << processName << "Be Removed Successfully!!!!!!!!!!!!!!";
			cout << g_strProcessFile << "Be Removed Successfully!!!!!!" << endl;
		}
		else
		{
			//qDebug() << processName << processName << "Cannot Be Removed!!!!!!!!!!!!!!";
			cout << g_strProcessFile << "Cannot Be Removed!!!!!!" << endl;
		}
	}
	m_fpProcessFile = fopen(g_strProcessFile, "a");;
	if (m_fpProcessFile == NULL)
	{
		//qDebug() << "File creation error!!!!!!";
		cout << "Process File creation error!!!!!!" << endl;
		return;
	}
}

/*
*	��������
*/
Huangbin::~Huangbin()
{
	m_fpFinalFile = fopen(g_strFinalFile, "w");
	if (m_fpFinalFile == NULL)
	{
		//qDebug() << "File creation error!!!!!!";
		cout << "Final File creation error!!!!!!" << endl;
		return;
	}
	for (int i = 1; i <= STATE_NUM; i++)
	{
		for (int j = 1; j <= ACTION_NUM; j++)
		{
			printf("Q[%d][%d]=%f\n", i, j, Q[i][j]);
			fprintf(m_fpFinalFile, "Q[%d][%d]=%f\n", i, j, Q[i][j]);
		}
	}
	fprintf(m_fpFinalFile, "\nEnding!!!!!!\n");
	fclose(m_fpFinalFile);
	fclose(m_fpProcessFile);
	cout << "��������!!!!!!" << endl;
}

/*
*   ��ȡ��ǰ״̬�£��ĸ�����������Qֵ
*/
double Huangbin::MaxQValueOfThisState(double q[ACTION_NUM + 1])		//�õ�����Qֵ
{
	double dMaxQValueOfThisState = q[1];
	for (int i = 2; i <= ACTION_NUM; i++)
	{
		if (q[i] > dMaxQValueOfThisState)
		{
			dMaxQValueOfThisState = q[i];
		}
	}
	return dMaxQValueOfThisState;
}

/*
*	����ѡ��
*/
int Huangbin::ActionSelection(int iCurrentState)
{
	int vCPU1, vCPU2, iCPU1Act, iCPU2Act;		//��ǰ����״̬vCPU1��С��vCPU2��С��vCPU1Ӧѡ������vCPU2Ӧѡ����
	vCPU1 = g_nStateConfig[iCurrentState][1];
	vCPU2 = g_nStateConfig[iCurrentState][2];
	//ΪvCPU1ѡ����
	if (vCPU1 == 2)
	{
		iCPU1Act = 1;
	}
	else if (vCPU1 == 4)
	{
		iCPU1Act = -1;
	}
	else
	{
		iCPU1Act = g_nBaseActionType[(rand() % 2) + 1];
	}
	//ΪvCPU2ѡ����
	if (vCPU2 == 2)
	{
		iCPU2Act = 1;
	}
	else if (vCPU2 == 4)
	{
		iCPU2Act = -1;
	}
	else
	{
		iCPU2Act = g_nBaseActionType[(rand() % 2) + 1];
	}
	//���ض����±�
	for (int i = 1; i < ACTION_NUM + 1; i++)
	{
		if (g_nAction[i][1] == iCPU1Act && g_nAction[i][2] == iCPU2Act)
			return i;
	}
}

/*
*   ���㽱��ֵ
*	TODO:��ȡ������g_dIndicator[][]
*/
double Huangbin::RewardFunction()
{
	double thrpt, res_time, reward;
	GetPlatformImmediateInfor();
	thrpt = g_dIndicator[1];
	res_time = g_dIndicator[2];
	if (res_time < SLA_RES)
	{
		reward = thrpt / REF_THRPT;
	}
	else
	{
		reward = -1.0;
	}
	fprintf(m_fpProcessFile, "thrpt=%f\n", thrpt);
	fprintf(m_fpProcessFile, "resp=%f\n", res_time);
	fprintf(m_fpProcessFile, "reward=%f\n", reward);
	return reward;
}

/*
*   ���ⷵ��[0,1]��Ľ���ֵ
*/
double Huangbin::RewardFunctionReturnRandomValue()
{
	double x = 8.0;
	fprintf(m_fpProcessFile, "thrpt=%f\n", x);
	fprintf(m_fpProcessFile, "resp=%f\n", x);
	fprintf(m_fpProcessFile, "reward=%f\n", x);
	return double (rand() % 100) / 100;
}

/*
*	��ʾQֵ��
*/
void Huangbin::ShowQTable()
{
	for (int i = 1; i <= STATE_NUM; i++)
	{
		for (int j = 1; j <= ACTION_NUM; j++)
		{
			cout << "Q[" << i << "][" << j << "]: " << Q[i][j] << endl;
		}
	}
}

/*
*	��ʾһЩ��Ϣ
*/
void Huangbin::ShowInfor(int iState, int iNextState, int iAct, double dReward)
{
	cout << "Current State:" << iState << endl;
	cout << "Select Action:" << iAct << endl;
	cout << "Next State:" << iNextState << endl;
	cout << "Reward<" << iState << "," << iAct << ">: " << dReward << endl;
	cout << "Q[" << iState << "][" << iAct << "]: " << Q[iState][iAct] << endl << endl;

	fprintf(m_fpProcessFile, "curr_state=%d\n", iState);
	fprintf(m_fpProcessFile, "select_act=%d\n", iAct);
	fprintf(m_fpProcessFile, "Q[%d][%d]=%f\n", iState, iAct, dReward);
	fprintf(m_fpProcessFile, "next_state=%d\n\n", iNextState);
}

/*
*	��������
*/
void Huangbin::HandleInTheEnd()
{
	cout << "Q Table Update Endly!!!!!!" << endl;
	ShowQTable();
	//this->sleep(REFRESH_STRUCT_TIME / 1000 + 5);
	//CommonFactory::getAlgorithm2_XenInstance()->exitWhenFinish();
}

/*
*   �����㷨����
*/
void Huangbin::ReinforcementLearning()
{
	int iState = INITIAL_STATE;	//״̬���,��ʼ��Ϊs1
	int iNextState = 0;         //��һ״̬���
	int iAct = 1;				//��ѡ����
	double dNextMaxQ = 0;     	//��һ״̬����Qֵ

	//Qѧϰ����
	for (int i = 0; i < LOOPTIMES; i++)
	{
		iAct = ActionSelection(iState);						//ѡ����
		iNextState = g_nNextState[iState][iAct];			//��һ״̬
		dNextMaxQ = MaxQValueOfThisState(Q[iNextState]);	//��һ״̬������Qֵ
		SetPlatformConfiguration(iNextState);				//��<s,act>���û�ƽ̨�����ȴ�һ��ʱ�䣨����ʱ��,����15s��
		double dReward = RewardFunction();	//��ȡ<s,act>�Ľ���ֵ
		Q[iState][iAct] = (1 - g_dAlph) * Q[iState][iAct] + g_dAlph * dReward + g_dAlph * g_dGama * dNextMaxQ;  //����Q���е�Q[s][act]ֵ
		GeneratePolicy();									//���ɲ���
		ShowPolicy();										//��ʾ����
		ShowInfor(iState, iNextState, iAct, dReward);		//��ʾһЩ��Ϣ
		iState = iNextState;								//��ǰ״̬תΪ��һ״̬
	}
	HandleInTheEnd();
}

/*
*   ��ƽ̨��ȡ������Ӧʱ��,������,CPU�����ʺ��ڴ������ʵ�
*/
void Huangbin::GetPlatformImmediateInfor()
{
	//��ȡg_dIndicator[INDICATOR_NUM]
	vmAPPStruct = CommonFactory::getAlgorithm2_XenInstance()->getVMInfoStruct();
	vmDBStruct = CommonFactory::getAlgorithm2_XenInstance()->getDatabaseStruct();
	g_dIndicator[1] = vmAPPStruct->vm_throughputs;
	g_dIndicator[2] = vmAPPStruct->vm_response_time;

	QDateTime curDateTime = QDateTime::currentDateTime();
	QString strCurDateTime = curDateTime.toString("yyyy-MM-dd hh:mm:ss ddd");
	qDebug() << "CurrentTime: " << strCurDateTime;
}

/*
*   ����һ״̬���ø����������
*/
void Huangbin::SetPlatformConfiguration(int state)
{
	vmAPPStruct->vm_cpu_cores = g_nStateConfig[state][1];
	vmDBStruct->vm_cpu_cores = g_nStateConfig[state][1];
	CommonFactory::getAlgorithm2_XenInstance()->reflesh();
}

/*
*   ���ɲ���
*/
void Huangbin::GeneratePolicy()
{
	//qDebug() << "In GeneratePolicy";
	for (int i = 1; i <= STATE_NUM; i++)
	{
		int iQLargerAction = 1;
		for (int j = 2; j < ACTION_NUM; j++)
		{
			if (Q[i][j] > Q[i][iQLargerAction])
			{
				iQLargerAction = j;
			}
		}
		switch (iQLargerAction)
		{
		case 1:
			m_arrnPolicy[i] = 1;
			break;
		case 2:
			m_arrnPolicy[i] = 2;
			break;
		case 3:
			m_arrnPolicy[i] = 3;
			break;
		case 4:
			m_arrnPolicy[i] = 4;
			break;
		default:
			cout << "Policy Generation Error!!!!!!";
			return;
		}
	}

	m_arrnPolicy[1] = 1;
	m_arrnPolicy[2] = 1;
	m_arrnPolicy[3] = 1;
	m_arrnPolicy[4] = 1;
	m_arrnPolicy[5] = 2;
	m_arrnPolicy[6] = 2;
	m_arrnPolicy[7] = 2;
	m_arrnPolicy[8] = 2;
	m_arrnPolicy[9] = 2;

	for (int j = 1; j <= STATE_NUM; j++)
	{
		m_arrnPolicyVCPU1[j] = g_nAction[m_arrnPolicy[j]][1];
		m_arrnPolicyVCPU2[j] = g_nAction[m_arrnPolicy[j]][2];
	}

	//cout << "m_arrnPolicy:" << endl;
	//for (int j = 1; j <= STATE_NUM; j++)
	//{
	//	cout << "" << m_arrnPolicy[j] << ":" << m_arrnPolicyVCPU1[j] << ", " << m_arrnPolicyVCPU2[j] << endl;
	//}
}

/*
*   �жϵ�ǰ�����Ƿ�����Ѳ���
*/
bool Huangbin::IsBestPolicy()
{
	bool bFlagVCPU1 = true;
	bool bFlagVCPU2 = true;
	for (int i = 1; i < STATE_NUM; i++)
	{
		bFlagVCPU1 = bFlagVCPU1 && (m_arrnPolicyVCPU1[i] >= m_arrnPolicyVCPU1[i + 1]);
		bFlagVCPU2 = bFlagVCPU2 && (m_arrnPolicyVCPU2[i] >= m_arrnPolicyVCPU2[i + 1]);
	}
	return  (bFlagVCPU1 && bFlagVCPU2);
}

/*
*   ��ʾ����
*/
void Huangbin::ShowPolicy()
{
	bool isBestPolicy = IsBestPolicy();
	cout << "��ǰ���ԣ�";
	for (int i = 1; i <= STATE_NUM; i++)
	{
		cout << m_arrnPolicy[i] << " ";
	}
	cout << endl << "vCPU1��\t  ";
	for (int j = 1; j <= STATE_NUM; j++)
	{
		if (m_arrnPolicyVCPU1[j] == 1)
			cout << "+ ";
		else
			cout << "- ";
	}
	cout << endl << "vCPU2��\t  ";
	for (int k = 1; k <= STATE_NUM; k++)
	{
		if (m_arrnPolicyVCPU2[k] == 1)
			cout << "+ ";
		else
			cout << "- ";
	}
	cout << endl;
	if (isBestPolicy)
	{
		int iLastUpIndex, iFirstDownIndex;
		cout << "����Ѳ��ԣ����vCPU��Ŀ�ǣ�";
		for (int j = 1; j < STATE_NUM; j++)
		{
			if (m_arrnPolicyVCPU1[j] == 1)
			{
				if (m_arrnPolicyVCPU1[j + 1] == -1)
				{
					iLastUpIndex = j;
					iFirstDownIndex = j + 1;
					cout << "vCPU1:" << iLastUpIndex << "��" << iFirstDownIndex << " && ";
					break;
				}
				else if (j == 8)
				{
					cout << "vCPU1:" << j + 1 << " && ";
				}
			}
		}
		
		for (int j = 1; j < STATE_NUM; j++)
		{
			if (m_arrnPolicyVCPU2[j] == 1)
			{
				if (m_arrnPolicyVCPU2[j + 1] == -1)
				{
					iLastUpIndex = j;
					iFirstDownIndex = j + 1;
					cout << "vCPU2:" << iLastUpIndex << "��" << iFirstDownIndex << "\t";
					break;
				}
				else if (j == 8)
				{
					cout << "vCPU2:" << j + 1 << "\t";
				}
			}
		}
		cout << endl;
	}
	else
	{
		cout << "������ò��ԣ��޷���ȡ���vCPU��Ŀ!!!" << endl;
	}
	cout.flush();
	fflush(stdout);
}

void Huangbin::run()
{
	Huangbin hb;
	hb.ReinforcementLearning();
	//    ReinforcementLearning_RandomAction();
}

void Huangbin::waitForData(int sec)
{
	this->sleep(sec);
}

void Huangbin::Test()
{
	//TEST:MaxQValueOfThisState	--->	OK
	/*for (int i = 1; i <= STATE_NUM; i++)
	{
	cout << "i=" << i << " max Q:" << hb.MaxQValueOfThisState(Q[i]) << endl;
	}*/

	//TEST:ActionSelection	--->	OK
	/*for (int i = 1; i <= 20; i++)
	{
	int act = hb.ActionSelection(9);
	cout << "��" << i << "����ѡ����:" << act << "ϸ��Ϊ" << g_nAction[act][1] << " , " << g_nAction[act][2] << endl;
	}*/

	//TEST:RewardFunction	--->	OK
	//cout << "Reward=" << hb.RewardFunction() << endl;

	//TEST:	--->
	for (int i = 0; i < 1000; i++)
	{
		fprintf(m_fpProcessFile, "%d\n", i);
	}
}

//int _tmain(int argc, _TCHAR* argv[])
//{
//	Huangbin hb;
//	hb.ReinforcementLearning();
//	system("pause");
//	return 0;
//}
