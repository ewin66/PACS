#pragma once

//��ͼ���õĴ�������
typedef struct GraphData
{
	unsigned int clientID;	//�ͻ��˼���
	double efficiency;	//Ч�ʣ�ƽ������ʱ�䣬��secondΪ��λ
	double transRate;	//�����ʣ�����������/ʱ�䣬��Byte/secondΪ��λ
}GraphData;

class DrawGraph
{
public:
	DrawGraph(void);
	~DrawGraph(void);

//	��ͼ����������
public:
	// ��ͼ����
	void drawGraph(GraphData graphData);
	//void drawGraph();
	// ���Ƴ�ʼ���
	void drawFrame();
	// ��ͼ����ʼ����
	double x;
	double y;
	// ��ͼ����С
	double width;
	double height;
	// �̶ȵȷ�
	double  xinterval;
	double yinterval;
	// ��ͼ��ʼ�㣺ʱ��
	double xpoint1;
	double ypoint1;
	// ��ͼ��ʼ�㣺������
	double xpoint2;
	double ypoint2;
};
