//���ļ���һ�����ܱ����ļ� 0:���������  1:������� This file is a function compilation file 0: not involved in compilation 1: involved in compilation

#ifndef __SWITCH_FUNCTION_H_
#define __SWITCH_FUNCTION_H_


#define DEBUG_USARTx USART1

//�������߲���ͬʱʹ�� The following two cannot be used at the same time
#define IR_TRACK_SWITCH 0  //4·ѭ������ 4-way tracking switch
#define ELE_SWITCH      0  //���Ѳ�߿���  Electromagnetic line patrol switch
#define CCD_SWITCH      0 //CCD���� CCD switch

#define K210_SWITCH     0 //k210��ʼ��. k210 initialization.

//ʹ���״�ע������ //Precautions for using radar
//1.�Ѳ��õ����趼���ͷŵ� �ر��ǳ������Ͷ�ʱ�� ��Сcpu���� //1. Release all unused peripherals, especially ultrasonic and timers, to reduce CPU burden
//2.�״����ݴ�����Ҫ��main�������洦��ɣ������ж�ռ��ʱ�� //2. Radar data processing needs to be processed in the main function to reduce interruption time
#define Lidar_SWITCH    0 //�״￪�� // Radar switch

#endif

