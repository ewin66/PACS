//Protocol.h
//�������˺Ϳͻ���ͨ��Э��
//����˺Ϳͻ��˹���

//Լ��ͨ�Ŷ˿ں�
#ifndef PRE_AGREED_PORT
#define PRE_AGREED_PORT 8888
#endif

//Լ��ÿ�δ����ͼƬ��Ŀ���˴�Ϊ3
#ifndef DCM_NUM
#define DCM_NUM 3
#endif

//Լ����Ϣ�ṹ��
////Ҫ��Э�̽��е�DIMSE��Ϣ////
typedef struct ASSOCIATE_PDU
{
	TCHAR PDUName[20];//PDU���ƣ�A-ASSOCIATE-RQ, A-ASSOCIATE-RJ, A-ASSOCIATE-AC
	TCHAR SOPClassUID[100];	//SOP��UID
	TCHAR SOPInstanceUID[100];	//SOPʵ��UID
	TCHAR transferSyntaxUID[100];	//����䷨
	//TCHAR DIMSEname[20]; //Լ��DIMSE��Ϣ
}ASSOCIATE_PDU;

////ҵ������////
//PDU���ƣ�C-FIND-RQ, C-STORE-RQ, C-MOVE-RQ, C-FIND-RSP, C-STORE-RSP, C-MOVE-RSP
typedef struct C_MOVE_PDU
{
	TCHAR PDUname[20];	//PDU
	TCHAR msgID[20];	//��ϢID
	TCHAR SOPClassUID[100];	//Ӱ���SOP��UID
	//TCHAR data[1024];	//����
}C_MOVE_PDU;

////�ͷ�����////
typedef struct RELEASE_PDU
{
	TCHAR PDUName[20];	//PDU���ƣ�A-Release-RQ, A-Release-RP
	TCHAR serviceName[20];	//����������
}RELEASE_PDU;