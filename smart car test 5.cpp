		/*
		 * Intellcarry.cpp
		 *
		 *  Created on: 2015-1-8
		 *      Author: fdk
		 */
		
		#include <Arduino.h>
		#include <Servo.h>
		#include <String.h>
		#include <string.h>
		
		#define otf 75    //135��
		#define otfR 90    //135��  
		#define nz 40     //90�� 
		#define ff 16     //45��
	
		
		#define Yellow  1    //��
		#define White  	2    //��
		#define Red    	3    //��
		#define Black  	4    //��
		#define Blue    5    //��
		//motor ���� 
		unsigned long time; 
		int i=0;
		unsigned int i1=0;
		char QTIS=0;
		int trac1 = A0; //�ӳ�ͷ��������ұ߿�ʼ����
		int trac2 = A1;
		int trac3 = A2;
		int trac4 = A3;
		//��������360���������
		Servo servo_left;
		Servo servo_right;
		
		int Led = 9;
		int times= millis();
		
		const int trig = 12;//������Trig����
		const int echo = 10;//������Echo����(��ʱ��1) 
		
		unsigned long pulsewidth=0;
		unsigned long millimetre=0;
		unsigned long dis=0;
		uint8_t Trigpin;
		uint8_t Echopin;
		
		//���������Ŷ���
		const int colorpin[6]={7,6,5,4,8,3};//��ɫ��������������
		
		//��ɫ����������
		int refer_time[3]={0,0,0};//��ƽ��õ��Ļ�׼ʱ��
		int clrpulses[3] = {0,0,0};
		int currentcolor=0;
		
		char recog_times=0;
		//char i=0;
		
		volatile int pulses=0;//������
		volatile int stdtime=0;//ʱ�������
		int s0,s1,s2,s3,led,out;//��ɫ����������
		volatile bool flag = false;
		
		
		int sekuai=5;
		int color;//ʵ����ɫ�ж�ʱ�õ�
		int color1;//��ɫʱ������
		char Idone=0;
		char process=1;
		char pointsth=0;//��ֱ���Ϸŵڼ���ɫ��
		unsigned int distance=0;//���������
		unsigned int distance1=0;
		extern volatile int stdtime2;
		
		unsigned char time_flag=0;
		volatile int stdtime2=0;//ʱ�������
		
		
		void Init_Timer2(void)
		{
		    TCCR2A |= (1<<WGM21);//CTC����
		    TCCR2B |= (1<<CS21); //8��Ƶ��ʱ������16/8=2MHz��0.5us
		    TCNT2 = 0;
		    OCR2A = 10;
		    sei();//�������ж�
		}
		
		SIGNAL(TIMER2_COMPA_vect)//TCNT2��OCR2A����ƥ��ʱ��TCNT2�Զ����㲢�����ж�
		{
			stdtime2++;
		}
		
		void PulseOut(uint8_t pin,int speed)
		{
		  digitalWrite(pin,HIGH);
		  delayMicroseconds(speed);
		  digitalWrite(pin,LOW);
		}
		
		void FastGoForward()
		{
		  servo_left.writeMicroseconds(2000);
		  servo_right.writeMicroseconds(1000);
		  delay(20);
		 
		}
		void SlowGoForward()
		{
		  servo_left.writeMicroseconds(1520);
		  servo_right.writeMicroseconds(1480);
		  delay(20);
		  
		}
		void GoBack(void)
		{
		  servo_left.writeMicroseconds(1460);
		  servo_right.writeMicroseconds(1540);
		  delay(20);
		  
		}
		void back(int pulses)
		{
		  while(pulses--)
		  {
		    GoBack();
		    delay(2);
		  }
		}
		/*
		 * ����ת
		 */
		void SpinLeft(void)
		{
		  servo_left.writeMicroseconds(1465);
		  servo_right.writeMicroseconds(1465);
		  delay(20);
		 
		}
		/*
		 * ����ת
		 */
		void SpinRight(void)
		{
		  servo_left.writeMicroseconds(1535);
		  servo_right.writeMicroseconds(1535);
		  delay(20);
		  
		}
		void TurnLeft(void)//����Сת
		{
		  servo_left.writeMicroseconds(1500);
		  servo_right.writeMicroseconds(1470);
		  delay(10);
		  
		}
		void TurnLeft_b(void)//�����ת
		{
		    int i;
		    for(i=0;i<=2;i++){
			servo_left.writeMicroseconds(1500);
		    servo_right.writeMicroseconds(1470);
		    delay(10);
			}
		    
		}
		
		void TurnRight(void)
		{
		  servo_left.writeMicroseconds(1530);
		  servo_right.writeMicroseconds(1500);
		  delay(10);
		}
		
		void TurnRight_b(void)
		{
		    int i;
			for(i=0;i<=2;i++){
			servo_left.writeMicroseconds(1530);
		    servo_right.writeMicroseconds(1500);
		    delay(10);
			} 
		
		}
		
		void Turn(int j)
		{
			int i;
			for(i=0;i<=j;i++)
			{
				servo_left.writeMicroseconds(1460);
		        servo_right.writeMicroseconds(1460);
		        delay(20);
			}
		}
		
		void Turn_180(void)
		{
		  int i;
		  for(i=0;i<=105;i++)
		  {
		    servo_left.writeMicroseconds(1460);
		     servo_right.writeMicroseconds(1460);
		    delay(20);
		  }
		}
		void stop(void)
		{
		  servo_left.writeMicroseconds(1500);
		  servo_right.writeMicroseconds(1500);
		  delay(20);
		}
		void TurnLeftAnyPulse(int pulses)
		{
			while(pulses--)
			{
				SpinLeft();
				delay(2);
			}
		}
		void TurnRightAnyPulse(int pulses)
		{
			while(pulses--)
			{
				SpinRight();
				delay(2);
			}
		}
		void Crossblk(int pulses)
		{
			while(pulses--)
			{
				FastGoForward();
				delay(2);
			}
		}
		void gofast(int pulses)
		{
			while(pulses--)
			{
				FastGoForward();
				delay(2);
			}
		}
		/*
		 * //����м���QTI�Ƿ��ں�����
		 */
		boolean IsMLeftQtiBlack(void){
			if(digitalRead(A1)==true)
			{
				delay(2);
				if(digitalRead(A1)==true)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		/*
		 * //����м���QTI�Ƿ��ں�����
		 */
		boolean IsMRightQtiBlack(void){
			if(digitalRead(A2)==true)
			{
				delay(2);
				if(digitalRead(A2)==true)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		
		void findLline(void)  //�������һ����
		{
			TurnLeftAnyPulse(3);
			while(1)
			{
				SpinLeft();
				if(IsMLeftQtiBlack())
				{
					i1++;
					if(i1==2)
					{
						i1=0;
						break;
					}
				}
			}
			while(1)
			{
				SpinLeft();
				if(IsMRightQtiBlack())
				{
					i1++;
					if(i1==2)
					{
						i1=0;
						break;
					}
				}
			}
			stop();
		}
		
		void findRline(void) //���ұ���һ����
		{
			TurnRightAnyPulse(3);
			while(1)
			{
				SpinRight();
		//		delay_nms(3000);
				if(IsMRightQtiBlack())
				{
					i1++;
					if(i1==2)
					{
						i1=0;
						break;
					}
				}
			}
			while(1)
			{
				SpinRight();
				if(IsMLeftQtiBlack())
				{
					i1++;
					if(i1==2)
					{
						i1=0;
						break;
					}
				}
			}
			stop();
		}
		
		void Robot_hunting2(void)
		{
		  int data[4];
		  data[0] = digitalRead(A0);
		  data[1] = digitalRead(A1);
		  data[2] = digitalRead(A2);
		  data[3] = digitalRead(A3);
		  QTIS = (1*data[3]+2*data[2]+4*data[1]+8*data[0]);
		      switch(QTIS)
		    {
		      case 4:TurnLeft_b();break;//��ת
		      case 8:TurnLeft();break;//��ת
		      case 12:TurnLeft();break;//��ת
		      case 1:TurnRight();break;//��ת
		      case 2:TurnRight_b();break;//��ת
		      case 3:TurnRight();break;//��ת
		      case 6:FastGoForward();break;//ֱ��
		      default:FastGoForward();break;//��һ�䲻��ʡ����ֹǰ���������û�е�ʱ��ִ�����
		    }
		    
		}
		void InitUltrasonic(uint8_t trig,uint8_t echo)
		{
			Trigpin = trig;
			Echopin = echo;
			pinMode(trig,OUTPUT);//PB10��Ϊ�����Trig
			pinMode(echo,INPUT);//PB12��Ϊ����,Echo
			digitalWrite(echo,LOW);
			digitalWrite(trig,LOW);
		}
		unsigned long GetDis(void)
		{
			//���ȸ�Trig����һ��10us����������
			//delayMicroseconds(5);
			digitalWrite(Trigpin,HIGH);
			delayMicroseconds(25);
			digitalWrite(Trigpin,LOW);
			//��ȡEcho���Ÿߵ�ƽ״̬����ʱ��,ָ����ʱʱ��Ϊ18.5ms��18500L��,��18.5*0.34m=6.29m, 3.145mm����
			//������������Χʱ������ֵΪ0
			pulsewidth = pulseIn(Echopin,HIGH,18500L);//pulsewidth��λΪus
			millimetre=pulsewidth*0.34;//ת���ɺ���
			return millimetre/2;//����ʵ�ʾ��루��λ��mm�������Ϊ3mm
			
		}
		unsigned long GetDis2(void)
		{
			int mini=0;
			int sum[5]={0};
			int num=0;
			int i=0,j=0,temp=0;
		    	for(i=0;i<5;i++)
		    	{
		      		mini = GetDis();
		      		sum[i]=mini;
		    	}
		    	for(i=0;i<5;i++)
		    	{
		      		for(j=i;j<5-i;j++)
		      		{
		        		if(sum[j]>sum[j+1])
		        		{
		          			temp=sum[j];
		          			sum[j]=sum[j+1];
		          			sum[j+1]=temp;
		        		}
		      		}
		    	}
		    	for(i=1;i<4;i++)
		    	{
		      		num+=sum[i];
		    	}
		      	num=num/3;
		      	return num;
		}
		
		void Init_INT1(void)
		{
			cli();//���������ж�
			EICRA |= (1<<ISC10)|(1<<ISC11); //�ⲿ�ж�1�����ش����ж�
		}
		void Init_Timer0(void)
		{
		    TCCR0A |= (1<<WGM01);//CTC����
		    TCCR0B |= (1<<CS01); //8��Ƶ��ʱ������16/8=2MHz��0.5us
		    TCNT0 = 0;
		    OCR0A = 10;
		    sei();//�������ж�
		}
		
		
		void OpenRedFilter(void){//��ɫ�˲���
			digitalWrite(s2,LOW);//S2--PD6
			digitalWrite(s3,LOW);//S3--PD7
		}
		void OpenBlueFilter(void){//��ɫ�˲���
			digitalWrite(s2,LOW);
			digitalWrite(s3,HIGH);
		}
		void OpenGreenFilter(void){//��ɫ�˲���
			digitalWrite(s2,HIGH);
			digitalWrite(s3,HIGH);
		}
		void ClosePower(void){//�رյ�Դ
			digitalWrite(s0,LOW);
			digitalWrite(s1,LOW);
		}
		void Out1than50(void){//�������1:50
			digitalWrite(s0,LOW);
			digitalWrite(s1,HIGH);
		}
		void Out1than5(void){//�������1:5
			digitalWrite(s0,HIGH);
			digitalWrite(s1,LOW);
		}
		void Out1than1(void){//�������1:1
			digitalWrite(s0,HIGH);
			digitalWrite(s1,HIGH);
		}
		bool ColorreCognt(int timestd[3],int value[3])
		{
			digitalWrite(led,HIGH);//����
			delay(100);
		
			//��ɫ
			pulses=0;
			stdtime=0;
			TCNT0 = 0;
			TIMSK0 |=(1<<OCIE0A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			Out1than1();
			OpenRedFilter();
			EIMSK |= (1<<INT1);      /*ʹ���ⲿ�ж�1����*/
			while(stdtime != timestd[0]);
			EIMSK &= ~(1<<INT1);
			TIMSK0 &= ~(1<<OCIE0A);
			ClosePower();
			if(pulses > 255){
				pulses = 255;
			}
			value[0] = pulses;
		
		
			//��ɫ
			pulses=0;
			stdtime=0;
			TCNT0 = 0;
			TIMSK0 |=(1<<OCIE0A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			Out1than1();
			OpenBlueFilter();
			EIMSK |= (1<<INT1);      /*ʹ���ⲿ�ж�1����*/
			while(stdtime != timestd[1]);
			EIMSK &= ~(1<<INT1);
			TIMSK0 &= ~(1<<OCIE0A);
			ClosePower();
			if(pulses > 255){
				pulses = 255;
			}
			value[1] = pulses;
		
			//��ɫ
			pulses=0;
			stdtime=0;
			TCNT0 = 0;
			TIMSK0 |=(1<<OCIE0A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			Out1than1();
			OpenGreenFilter();
			EIMSK |= (1<<INT1);      /*ʹ���ⲿ�ж�1����*/
			while(stdtime != timestd[2]);
			EIMSK &= ~(1<<INT1);
			TIMSK0 &= ~(1<<OCIE0A);
			ClosePower();
			if(pulses > 255){
				pulses = 255;
			}
			value[2] = pulses;
		
			digitalWrite(s0,LOW);
			digitalWrite(s1,LOW);
			digitalWrite(led,LOW);
		
			return true;
		}
		bool WhiteBalance(int refertime[3])
		{
			digitalWrite(led,HIGH);//LED
			delay(100);
		
			//��ɫ�˲���
			stdtime=0;
			pulses=0;	//��������
			TCNT0 = 0;
			TIMSK0 |=(1<<OCIE0A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			Out1than1();
			OpenRedFilter();
			EIMSK |= (1<<INT1);      /*ʹ���ⲿ�ж�1����*/
			while(!(flag&0x1));//�ȴ���ʱ��
			TIMSK0 &= ~(1<<OCIE0A);
			ClosePower();
			EIMSK &= ~(1<<INT1);
		
			refertime[0] = stdtime;//����������
			flag=false;
		
			//��ɫ
			stdtime=0;
			pulses=0;	//��������
			TCNT0 = 0;
			TIMSK0 |=(1<<OCIE0A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			Out1than1();
			OpenBlueFilter();
			EIMSK |= (1<<INT1);      /*ʹ���ⲿ�ж�1����*/
			while(!(flag&0x1));
			TIMSK0 &= ~(1<<OCIE0A);
			ClosePower();
			EIMSK &= ~(1<<INT1);
		
			refertime[1] = stdtime;
			flag=false;
		
			//��ɫ
			stdtime=0;
			pulses=0;	//��������
			TCNT0 = 0;
			TIMSK0 |=(1<<OCIE0A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			Out1than1();
			OpenGreenFilter();
			EIMSK |= (1<<INT1);      /*ʹ���ⲿ�ж�1����*/
			while(!(flag&0x1));
			TIMSK0 &= ~(1<<OCIE0A);
			ClosePower();
			EIMSK &= ~(1<<INT1);
		
			refertime[2] = stdtime;
			flag=false;
		
			digitalWrite(s0,LOW);//S0,�رյ�Դ
			digitalWrite(s1,LOW);//S1
			digitalWrite(led,LOW);//LED
		
			return true;
		}
		void InitColorSenor(const int pin[6])
		{
			//S0 = pin[0]
			//S1 = pin[1]
			//S2 = pin[2]
			//S3 = pin[3]
			//LED = pin[4]
			//OUT = pin[5],������PD3
			s0=pin[0];
			s1=pin[1];
			s2=pin[2];
			s3=pin[3];
			led=pin[4];
			out=pin[5];
			pinMode(s0,OUTPUT);
			pinMode(s1,OUTPUT);
			pinMode(s2,OUTPUT);
			pinMode(s3,OUTPUT);
			pinMode(led,OUTPUT);
			pinMode(out,INPUT);
		
			Init_INT1();//�ⲿ�ж�1
			Init_Timer0();//��ʱ��0
		}
		
		SIGNAL(INT1_vect)
		{
			pulses++;//���������
			if(pulses == 255)//��ɫ��Դʱ����ɫ�˲���ֵ��Ĭ����255
			{
				flag = true;
			}
		}
		SIGNAL(TIMER0_COMPA_vect)//TCNT0��OCR0A����ƥ��ʱ��TCNT0�Զ����㲢�����ж�
		{
			stdtime++;
		}
		int Robot_checkColor()
		{
			ColorreCognt(refer_time,clrpulses);
			if((clrpulses[0] - clrpulses[2] > 90) && (clrpulses[0] - clrpulses[1] > 80)){
				currentcolor = Red;
			}
			else if((abs(clrpulses[0] - clrpulses[2])<50)&&(abs(clrpulses[0] - clrpulses[1])<50)&& (clrpulses[0]>100)){
				currentcolor = White;
			}
			else if((abs(clrpulses[0] - clrpulses[2])<30)&&(abs(clrpulses[0] - clrpulses[1])<30)&& clrpulses[1]<40){
				currentcolor = Black;
			}
			else if((clrpulses[0] < clrpulses[1]) && (clrpulses[1] > 40)){
				currentcolor = Blue;
			}
			else if((clrpulses[2] > clrpulses[1]) && (clrpulses[0] - clrpulses[1] > 100)){
				currentcolor = Yellow;
			}
			return currentcolor;
		}
		
		void DownFirst(void)
		{
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
				while(stdtime2 <= 700)
				{
					Robot_hunting2();
				}
				TIMSK2 &= ~(1<<OCIE2A);
				back(45);//����
				Turn_180();//��ͷ
				pointsth++;
		}
		
		void DownSecond()
		{
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
				while(stdtime2 <= 600)
				{
					Robot_hunting2();
				}
				TIMSK2 &= ~(1<<OCIE2A);
				back(45);//����
				Turn_180();//
				
				pointsth++;
		}
		
		void Downthird()
		{
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
				while(stdtime2 <=500)
				{
					Robot_hunting2();
				}
				TIMSK2 &= ~(1<<OCIE2A);
				back(40);//����
				Turn_180();//
				
				pointsth++;
		}
		
		void DownFourth()
		{
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
				while(stdtime2 <= 400)
				{
					Robot_hunting2();
				}
				TIMSK2 &= ~(1<<OCIE2A);
				back(40);//����
				Turn_180();//
				
				pointsth++;
		}
		
		void DownFiveth(void)
		{
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//��ʱ��2����ж�ʹ��,ִ���жϷ�����
				while(stdtime2 <= 300)
				{
					Robot_hunting2();
				}
				TIMSK2 &= ~(1<<OCIE2A);
				pointsth++;
		}
		
		void GotoLine(void)
		{
			switch(pointsth)
			{
				case 0:DownFirst();
					break;
				case 1:DownSecond();
					break;
				case 2:Downthird();
					break;
				case 3:DownFourth();
					break;
				case 4:DownFiveth();
					break;
				default:
					break;
			}
		}
		
		void carryAtopoint()
		{
			do
			{
				Robot_hunting2();//��ת������Ѳ��
			}while(QTIS!=15);
			
			stop();
			Crossblk(13);
		//	delay(2000);
			TurnLeftAnyPulse(40);
			//äתĳ�Ƕ�
		//	delay(2000);
			findLline();//Ѱ����һ������
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);//�ҵ�ɫ��ǰһֱѲ��ǰ��
		//	delay(2000);
			Turn_180();
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			Crossblk(10);
		//	delay(2000);
			TurnRightAnyPulse(30);
			findRline();
			GotoLine();//��ת�ڷ�ɫ��ڷ�
			process=2;
		}
		
		void carryBtopoint()
		{
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			
			stop();
			Crossblk(13);
			
			TurnLeftAnyPulse(20);
		//	delay(2000);
			findLline();
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
			Turn_180();
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			Crossblk(12);
		//	delay(2000);
			TurnRightAnyPulse(15);
			GotoLine();
			process=3;
		}
		
		void carryCtopoint()
		{
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			Crossblk(13);
			stop();
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
			Turn_180();
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			Crossblk(10);
		//	delay(2000);
			GotoLine();
			process=4;
		}
		
		void carryDtopoint()
		{
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			Crossblk(13);
		//	delay(2000);
			stop();
			TurnRightAnyPulse(8);
		//	delay(2000);
			findRline();
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
			Turn_180();
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			Crossblk(16);
		//	delay(2000);
			findLline();
			GotoLine();
			process=5;
		}
		
		void carryEtopoint()
		{
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			Crossblk(13);
		//	delay(2000);
			stop();
			TurnRightAnyPulse(40);
		//	delay(2000);
			findRline();
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
			Turn_180();
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			Crossblk(14);
		//	delay(2000);
			TurnLeftAnyPulse(30);
			findLline();
			GotoLine();
			process=6;
		}
		
		void RedCarry(void)//���ɫɫ�鵽������
		{
				do
				{
					Robot_hunting2();
				}while(QTIS > 0);
				Crossblk(5);
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
				while(stdtime2 <= 300)
				{
					Robot_hunting2();
				}  //�Ӱ׵����ߵ�����������ʱ��550��ʱ����ߵĳ�
				TIMSK2 &= ~(1<<OCIE2A);
				Crossblk(32); 
				back(40);//����
				Turn_180();
				do
				{
					Robot_hunting2();
				}while(QTIS != 15);
				Crossblk(10);
		}
		void WhiteCarry(void)//���ɫɫ�鵽������
		{
				TurnLeftAnyPulse(11);
				findLline();
				delay(20);
				do
				{
					Robot_hunting2();
				}while(QTIS > 0);
				Crossblk(5);
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
				while(stdtime2 <= 300)
				{
					Robot_hunting2();
				}  //�Ӱ׵����ߵ�����������ʱ��2510��ʱ����ߵĳ�
				TIMSK2 &= ~(1<<OCIE2A);
				Crossblk(30);
				back(30);//����
				Turn_180();
				do
				{
					Robot_hunting2();
				}while(QTIS != 15);
				Crossblk(15);
//				TurnRightAnyPulse(ff);
				findRline();
				delay(200);
		}
		void YellowCarry(void)//���ɫɫ�鵽������
		{
				TurnLeftAnyPulse(43);
				delay(20);
				findLline();
				delay(200);
				do
				{
					Robot_hunting2();
				}while(QTIS > 0);
				Crossblk(5);
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
				while(stdtime2 <= 300)
				{
					Robot_hunting2();
				}    //�Ӱ׵����ߵ�����������ʱ��2510��ʱ����ߵĳ�
				TIMSK2 &= ~(1<<OCIE2A);
				Crossblk(28);
				back(30);//����
				Turn_180();
				do
				{
					Robot_hunting2();
				}while(QTIS != 15);
				Crossblk(14);
				TurnRightAnyPulse(30);
				findRline();
				delay(200);
		}
		void BlackCarry(void)//���ɫɫ�鵽������
		{
				TurnRightAnyPulse(30);
				findRline();
				delay(20);
				do
				{
					Robot_hunting2();
				}while(QTIS > 0);
				Crossblk(5);
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
				while(stdtime2 <= 300)
				{
					Robot_hunting2();
				}   //�Ӱ׵����ߵ�����������ʱ��2510��ʱ����ߵĳ�
				TIMSK2 &= ~(1<<OCIE2A);
				Crossblk(50);
				back(30);//����
				Turn_180();
				do
				{
					Robot_hunting2();
				}while(QTIS != 15);
				Crossblk(14);
				findLline();
				delay(200);
		}
		void BlueCarry(void)//����ɫɫ�鵽������
		{
				TurnRightAnyPulse(70);//
				delay(20);
				findRline();
				delay(20);
				do
				{
					Robot_hunting2();
				}while(QTIS > 0);
				Crossblk(5);
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
				while(stdtime2 <= 300)
				{
					Robot_hunting2();
				}                      //�Ӱ׵����ߵ�����������ʱ��2510��ʱ����ߵĳ�
				TIMSK2 &= ~(1<<OCIE2A);
				Crossblk(30);
				back(30);//����
				Turn_180();
				do
				{
					Robot_hunting2();
				}while(QTIS != 15);
				Crossblk(14);
				TurnLeftAnyPulse(30);
				findLline();
				delay(200);
		}
		
		void returnstart(void)
		{
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			Crossblk(50);
			stop();
		}
		void StartCarryToScore(void)
		{
				if(sekuai>=5)//��һ��ɫ�鲻�ò�ֱ࣬�ӵ�ͷ����
				{
					Turn_180();
				}
				else
				{
					do
					{
						Robot_hunting2();
						distance = GetDis();
					}while((distance >=50)&&(distance > 0));//����ת�������ɫ��ʱ��Ҫ���Ի�������ɫ��ľ���
					while(1)
						{
							distance = GetDis();
							if((distance<=45)&&(distance>0))
							{
								delay(2);
								if((distance<=45)&&(distance>0))
								{
									Robot_hunting2();
									distance = GetDis();
									break;
								}
							}
							else
							{
								Robot_hunting2();
							}
						}
					Crossblk(10);//ä�ߣ��û�������ɫ��Ӵ�
					Turn_180();
				}
				do
				{
					Robot_hunting2();
				}while(QTIS != 15);
				Crossblk(8);
				pointsth--;//���һ����ȥ1
				sekuai--;//�Ѿ����ɫ�������һ
				delay(20);
				color = Robot_checkColor();//ʶ����ɫ
				switch(color)
				{
					case Red:RedCarry();break;
					case White:WhiteCarry();break;
					case Yellow:YellowCarry();break;
					case Black:BlackCarry();break;
					case Blue:BlueCarry();break;
					default:break;
				}
				if(sekuai<1)
				{
					process=7;
				}
				else
				{
					process=6;
				}
		}
		
		//��ʼ��
		
		int x[5];
		
		int SpinLexplore(int pulse)
		{
			TurnLeftAnyPulse(pulse);
			findLline();
			delay(100);
		    distance=GetDis();
		    //Serial.println(distance);
		    if(distance<=180&&distance>0)
			{
		    	return 0;
			}
		    else
			{
		    	stop();
		    	stdtime2 =0;
		    	TCNT2 = 0;
		    	TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
		    	while(stdtime2 <= 200)
		    	{
		    	  distance=GetDis(); 
		    	  if(distance<=180&&distance>0)
		    	  break;
		    	}
		    	TIMSK2 &= ~(1<<OCIE2A);	
		    	if(distance<=180&&distance>0)
				{
					return 0;	
				}
		    	else
				{
					return 1;	
				}
			}
		}
		
		int SpinRexplore(int pulse)
		{
			TurnRightAnyPulse(pulse);
			findRline();
			delay(100);
		    distance=GetDis();
		//		    Serial.println(distance);
		    if(distance<=180&&distance>0)
			{
			    return 0;
			} 
		    else
			{
		    	stop();
		    	stdtime2 =0;
		    	TCNT2 = 0;
		    	TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
		    	while(stdtime2 <= 200)
		    	{
		    		distance=GetDis();
		    		if(distance<=180&&distance>0)
		        	break;
		    	}
		    	TIMSK2 &= ~(1<<OCIE2A);	
		    	if(distance<=180&&distance>0)
				{
			    	return 0;
				}
		    	else
				{
		        	return 1;
				}
			}     
		}
		
		int FindEmptyLine()
		{
		  	distance=GetDis();
			if(distance<=180&&distance>0)
		  		x[2]=0;
		  	else
		 		x[2]=1;
			do{
		  		x[0]=SpinLexplore(45);
		    	x[1]=SpinRexplore(ff);
		    	x[3]=SpinRexplore(50);
		    	x[4]=SpinRexplore(ff);
		    	SpinLexplore(nz);
		  	}while(x[0]+x[1]+x[2]+x[3]+x[4]!=1);
		  if(x[0]==1)
		  	return 0;
		  else if(x[1]==1)
		  	return 1;
		  else if(x[2]==1)
		  	return 2;
		  else if(x[3]==1)
		  	return 3;
		  else if(x[4]==1)
		  	return 4;
		}
		
		void setup(void)
		{
			pinMode(trac1, INPUT);
			pinMode(trac2, INPUT);
			pinMode(trac3, INPUT);
			pinMode(trac4, INPUT);
			delay(20);
			pinMode(Led,OUTPUT);
			servo_left.attach(13);
			servo_right.attach(2);
			delay(20);
			InitColorSenor(colorpin);
			InitUltrasonic(trig,echo);
			Init_Timer2();
			Serial.begin(9600);
		}
		void constantTakeL(int pulse)
		{
			Turn_180();
			do {
				Robot_hunting2();
			} while(QTIS!=15);
			Crossblk(13);
			if(pulse!=0)
			{
				TurnLeftAnyPulse(pulse);
				findLline();	
			}
			do{
				Robot_hunting2();
			}while(QTIS!=0);
			takewukuai(40); 
		} 
	void constantTakeR(int pulse)
	{
		Turn_180();
		do {
			Robot_hunting2();
		} while(QTIS!=15);
		Crossblk(13);
		if(pulse!=0){
		TurnRightAnyPulse(pulse);
		findRline();	
		}
		do {
			Robot_hunting2();
		} while(QTIS!=0);
		takewukuai(40);
	} 
		void shibiecolor()
		{	
			do//Ѳ�� 
			{
				Robot_hunting2();//��ת������Ѳ��
			}while(QTIS!=15);
			stop();
			Crossblk(13);//Ѳ�߽��� 
			int ULT=FindEmptyLine();
		//	delay(2000);
			if(ULT==4)
			{ 
				firstfindline(); 
				judgewukuaiE();
			} //EΪ�� 
			else if(ULT==3)
			{ 
				firstfindline();
				judgewukuaiD();
			} //DΪ�� 
			else if(ULT==2)
			{ 
				firstfindline();
				judgewukuaiC();
			}//CΪ�� 
			else if(ULT==1)
			{ 
				firstfindline();
				judgewukuaiB();
			} //BΪ�� 
			else if(ULT==0)//AΪ��
			{
				TurnLeftAnyPulse(40);
				//äתĳ�Ƕ�
			//	delay(2000);
				findRline();//Ѱ����һ������
			//	delay(2000);
				do
				{
					Robot_hunting2();
				}while(QTIS!=0);//�ҵ�ɫ��ǰһֱѲ��ǰ��
				judgewukuaiA();
			} 
			fromlinetocore();
			/*color = Robot_checkColor();//ʶ����ɫ
			switch(color)
			{
				case Red:RedCarry();break;
				case White:WhiteCarry();break;
				case Yellow:YellowCarry();break;
				case Black:BlackCarry();break;
				case Blue:BlueCarry();break;
				default:break;
			}*/
		}//ʶ�������ɫ 
		
		void AIsYellow(void)
		{
			takewukuai(29);
			TurnLeftAnyPulse(100);
			findRline();
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
		}//A����ǻ�ɫ�� 
		
		void ANoYellow(void)
		{
			takewukuaitocore();
			TurnRightAnyPulse(30);
			findRline();
			GotoLine();//��ת�ڷ�ɫ��ڷ�
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			stop();
			Crossblk(14);
			TurnLeftAnyPulse(40);
		//	delay(2000);
			findRline();
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
		}//A��Ĳ��ǻ�ɫ��
		
		void BIsWhite(void)
		{
			takewukuai(29);
			TurnLeftAnyPulse(100);
			findRline();
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
		}//B����ǰ�ɫ�� 
		
		void BNoWhite(void)
		{
			takewukuaitocore();
		//	TurnRightAnyPulse(15);
			findRline();
			GotoLine();//��ת�ڷ�ɫ��ڷ�
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			stop();
			Crossblk(14);
		//	TurnLeftAnyPulse(0);
		//	delay(2000);
		//	findLline();
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
		}//B��Ĳ��ǰ�ɫ�� 
		
		void CIsRed(void)
		{
			takewukuai(29);
			TurnLeftAnyPulse(100);
			findRline();
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
			
		}//C����Ǻ�ɫ�� 
		
		void CNoRed(void)
		{
			takewukuaitocore();
		//	TurnRightAnyPulse(30);
		//	findRline();
			GotoLine();//��ת�ڷ�ɫ��ڷ�
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			stop();
			Crossblk(14);
			TurnRightAnyPulse(50);
		//	delay(2000);
			findLline();
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
		}//C��Ĳ��Ǻ�ɫ��
		
		void DIsBlack(void)
		{
			takewukuai(29);
			TurnLeftAnyPulse(100);
			findRline();
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
		}
		
		void DNoBlack(void)
		{
			takewukuaitocore();
			findLline();
			GotoLine();//��ת�ڷ�ɫ��ڷ�
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			stop();
			Crossblk(14);
			TurnRightAnyPulse(100);
		//	delay(2000);
			findLline();
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
		}
		
		void EIsBlue(void)
		{
			takewukuai(29);
			TurnLeftAnyPulse(40);
			findLline();
			/*stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			while(stdtime2 <= 300)
			{
				Robot_hunting2();
			}
			TIMSK2 &= ~(1<<OCIE2A);
		//	back(40);//����
		//	Turn_180();//
			*/
		}//E�������ɫ�� 
		
		void ENoBlue(void)
		{
			takewukuaitocore();
			TurnLeftAnyPulse(40);
			findLline();
			stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			while(stdtime2 <= 200)
			{
				Robot_hunting2();
			}
			TIMSK2 &= ~(1<<OCIE2A);
			back(40);//����
			pointsth++;
		//	Turn_180();
		}//E.��Ĳ�����ɫ��
		
		void DIsBlackE(void)
		{
			takewukuai(29);
		//	TurnLeftAnyPulse(30);
			findLline();
			stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			while(stdtime2 <= 100)
			{
				Robot_hunting2();
			}
			TIMSK2 &= ~(1<<OCIE2A);
		//	back(40);//����
		//	Turn_180();
		}//D����Ǻ�ɫ��E
		
		void DNoBlackE(void)
		{
			takewukuaitocore();
		//	TurnLeftAnyPulse(30);
			findLline();
			stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			while(stdtime2 <= 300)
			{
				Robot_hunting2();
			}
			TIMSK2 &= ~(1<<OCIE2A);
			back(40);//����
		//	Turn_180();
			pointsth++;
		}//D��Ĳ��Ǻ�ɫ��E
		
		void CIsRedD(void){
			takewukuai(29);
			TurnLeftAnyPulse(40);
			findLline();
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
		}
		
		void CNoRedD(void){
			takewukuaitocore();
		//	TurnRightAnyPulse(30);
		//	findRline();
			GotoLine();//��ת�ڷ�ɫ��ڷ�
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			stop();
			Crossblk(13);
			
			TurnRightAnyPulse(100);
		//	delay(2000);
			findLline();
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
		}
		
		void BIsWhiteC(void){
			takewukuai(29);
			TurnLeftAnyPulse(75);
			findRline();
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
			
		}
		
		void BNoWhiteC(void){
			takewukuaitocore();
		//	TurnRightAnyPulse(15);
			findRline();
			GotoLine();//��ת�ڷ�ɫ��ڷ�
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			
			stop();
			Crossblk(13);
			
			TurnRightAnyPulse(50);
		//	delay(2000);
			findLline();
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
		}
		
		void AIsYellowB(void){
			takewukuai(29);
			TurnLeftAnyPulse(70);
			findRline();
			Crossblk(15);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
		}
		
		void ANoYellowB(void){
			takewukuaitocore();
			TurnRightAnyPulse(30);
			findRline();
			GotoLine();//��ת�ڷ�ɫ��ڷ�
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			
			stop();
			Crossblk(14);
			
		//	TurnRightAnyPulse(50);
		//	delay(2000);
		//	findLline();
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
			Crossblk(15);
		}
		
		void takewukuaitocore(void){//�������������׼��ת�� 
			Turn_180();
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			Crossblk(14);
		//	delay(2000);
		} 
		
		void takewukuai(int pulse){//ͬ��ɫ������������ 
			stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			while(stdtime2 <= 200)
			{
				Robot_hunting2();
			}                      //�Ӱ׵����ߵ�����������ʱ��2510��ʱ����ߵĳ�
			TIMSK2 &= ~(1<<OCIE2A);
			Crossblk(pulse);
			back(30);//����
			Turn_180();
			do
			{
				Robot_hunting2();
			}while(QTIS != 15);
			Crossblk(15);
		}
		
		
		void firstfindline(void){
			TurnLeftAnyPulse(75);
			//findLline();
			//äתĳ�Ƕ�
		//	delay(2000);
			findRline();//Ѱ����һ������
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);//�ҵ�ɫ��ǰһֱѲ��ǰ��
		}
		
	void judgewukuaiE(void)
	{
		//ʶ��A��
		color = Robot_checkColor();
		if(color==Yellow)
		{ 
			AIsYellow();
		}
		else if(color==Blue)
		{
			constantTakeR(0);
			TurnRightAnyPulse(ff);
			findRline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		} 
		else
		{
			ANoYellow();
		}
		
		//ʶ��B��
		color = Robot_checkColor();
		if(color==White) { 
			BIsWhite();
		} 
		else if(color==Yellow)
		{
			constantTakeR(otfR);
			TurnLeftAnyPulse(nz);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else if(color==Blue)
		{
			constantTakeL(ff);
			TurnRightAnyPulse(nz);
			findRline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);
		}
		else 
		{
			BNoWhite();
		}
		
		//ʶ��C��
		color = Robot_checkColor();
		if(color==Red) 
		{ 
			CIsRed();
		} 
		
		else if(color==White) 
		{
			constantTakeR(otfR);
			TurnLeftAnyPulse(nz);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);		
		}
		else if(color==Yellow)
		{
			constantTakeR(nz);
			TurnLeftAnyPulse(ff);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);
		}
		else if(color==Blue)
		{
			constantTakeL(nz);
			TurnRightAnyPulse(otfR);
			findRline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);
		}
		else 
		{
			CNoRed();
		}
		
		//ʶ��D��
		color = Robot_checkColor();
		if(color==Black) { 
			DIsBlackE();
		}
		else if(color==White)
		{
			constantTakeR(nz);
			TurnRightAnyPulse(ff);
			findRline();
			stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			while(stdtime2 <= 250) 
			{
				Robot_hunting2();
			}
			TIMSK2 &= ~(1<<OCIE2A);
		} 
		else if(color==Yellow)
		{
			constantTakeR(ff);
			TurnRightAnyPulse(nz);
			findRline();
			stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			while(stdtime2 <= 250) 
			{
				Robot_hunting2();
			}
			TIMSK2 &= ~(1<<OCIE2A);
		} 
		else if(color==Red)
		{
			constantTakeR(otfR);
			stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			while(stdtime2 <= 250)
			{
				Robot_hunting2();
			}
			TIMSK2 &= ~(1<<OCIE2A);
		}
		else if(color==Blue)
		{
			constantTakeL(otf);
			TurnLeftAnyPulse(nz);
			findLline();
			stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			while(stdtime2 <= 250)
			{
				Robot_hunting2();
			}
			TIMSK2 &= ~(1<<OCIE2A);
		}
		else
		{
			DNoBlackE();
		}
	}//E��Ϊ��
	
	void judgewukuaiD(void)
	{
		//ʶ��A��
		color = Robot_checkColor();
		if(color==Yellow)
		{ 
			AIsYellow();
		} 
		else if(color==Black)
		{
			constantTakeL(ff);
			TurnRightAnyPulse(nz);
			findRline();
			do 
			{
				Robot_hunting2();
			} while(QTIS!=0);	
		} 
		else 
		{
			ANoYellow();
		}
		
		//ʶ��B��
		color = Robot_checkColor();
		if(color==White) 
		{ 
			BIsWhite();
		}
		else if(color==Yellow)
		{
			constantTakeR(otfR);
			TurnLeftAnyPulse(nz);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else if(color==Black)
		{
			constantTakeL(nz);
			TurnRightAnyPulse(otfR);
			findRline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else
		{
			BNoWhite();
		}
		
		//ʶ��C��
		color = Robot_checkColor();
		if(color==Red)
		{ 
			CIsRedD();
		}
		else if(color==Yellow)
		{
			constantTakeR(nz);
			do{
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else if(color==White)
		{
			constantTakeR(otfR);
			TurnLeftAnyPulse(ff);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else if(color==Black)
		{
			constantTakeL(otf);
			TurnLeftAnyPulse(otf);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else
		{
			CNoRedD();
		}
		
		//ʶ��E��
		color = Robot_checkColor();
		if(color==Blue)
		{
			EIsBlue();
		} 
		else if(color==Yellow)
		{
			constantTakeR(0);
			TurnRightAnyPulse(nz);
			findRline();
		}
		else if(color==White)
		{
			constantTakeR(ff);
			TurnRightAnyPulse(ff);
			findRline();
		}
		else if(color==Red)
		{
			constantTakeR(nz);
		}
		else if(color==Black)
		{
			constantTakeR(otfR);
			//TurnLeftAnyPulse(ff);
			findLline();
		}
		else
		{
			ENoBlue();
		}
	}//D��Ϊ��
	
	void judgewukuaiC(void) {
		
		//ʶ��A��
		color = Robot_checkColor();
		if(color==Yellow)
		{ 
			AIsYellow();
		}
		else if(color==Red)
		{
			constantTakeL(nz);
			TurnRightAnyPulse(otfR);
			findRline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		 else 
		{
			ANoYellow();
		}
		
		//ʶ��B��
		color = Robot_checkColor();
		if(color==White)
		{ 
			BIsWhiteC();
		}
		else if(color==Yellow)
		{
			constantTakeR(otfR);
			TurnLeftAnyPulse(ff);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else if(color==Red)
		{
			constantTakeL(otf);
			TurnLeftAnyPulse(otf);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else
		{
			BNoWhiteC();
		}//C��Ϊ��ֱ�ӿ�D
		
		//ʶ��D��
		color = Robot_checkColor();
		if(color==Black)
		{ 
			DIsBlack();
		}
		else if(color==Yellow)
		{
			constantTakeR(ff);
			do {
				Robot_hunting2();
			} while(QTIS!=0);
		} 
		else if(color==White)
		{
			constantTakeR(nz);
			TurnLeftAnyPulse(ff);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);
		} 
		else if(color==Red)
		{
			constantTakeR(otfR);
			TurnLeftAnyPulse(nz);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);
		}
		else
		{
			DNoBlack();
		}
		
		//ʶ��E��
		color = Robot_checkColor();
		if(color==Blue)
		{ 
			EIsBlue();
		}
		else if(color==Yellow)
		{
			constantTakeR(0);
			TurnRightAnyPulse(nz);
			findRline();
		}
		else if(color==White)
		{
			constantTakeR(ff);
			TurnRightAnyPulse(ff);
			findRline();	
		}
		else if(color==Red)
		{
			constantTakeR(nz);		
		}
		else if(color==Black)
		{
			constantTakeR(otfR);
			//TurnLeftAnyPulse(ff);
			findLline();	
		}
		else
		{
			ENoBlue();
		}
	}//C��Ϊ��
	
	void judgewukuaiB(void)
	{
		//ʶ��A��
		color = Robot_checkColor();
		if(color==Yellow)
		{ 
			AIsYellowB();
		}
		else if(color==White)
		{
			constantTakeL(otf);
			TurnLeftAnyPulse(otf);
			findRline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else
		{
			ANoYellowB();
		}//B��Ϊ��ֱ�ӿ�C
		
		//ʶ��C��
		color = Robot_checkColor();
		if(color==Red)
		{ 
			CIsRed();
		}
		else if(color==Yellow)
		{
			constantTakeR(nz);
			TurnLeftAnyPulse(ff);
			findLline();
			do{
				Robot_hunting2();
			} while(QTIS!=0);
		}
		else if(color==White)
		{
			constantTakeR(otfR);
			TurnLeftAnyPulse(nz);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);		
		}
		else
		{
			CNoRed();
		}
		
		//ʶ��D��
		color = Robot_checkColor();
		if(color==Black)
		{ 
			DIsBlack();
		}
		else if(color==Yellow)
		{
			constantTakeR(ff);
			do {
				Robot_hunting2();
			} while(QTIS!=0);
		} 
		else if(color==White)
		{
			constantTakeR(nz);
			TurnLeftAnyPulse(ff);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);
		} 	
		else if(color==Red)
		{
			constantTakeR(otfR);
			TurnLeftAnyPulse(30);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);
		}
		else
		{
			DNoBlack();
		}
		
		//ʶ��E��
		color = Robot_checkColor();
		if(color==Blue)
		{ 
			EIsBlue();
		}
		else if(color==Yellow)
		{
			constantTakeR(0);
			TurnRightAnyPulse(nz);
			findRline();
		}
		else if(color==White)
		{
			constantTakeR(ff);
			TurnRightAnyPulse(ff);
			findRline();
		}
		else if(color==Red)
		{
			constantTakeR(nz);	
		}
		else if(color==Black)
		{
			constantTakeR(otfR);
			//TurnLeftAnyPulse(ff);
			findLline();
		}
		else 
		{
			ENoBlue();
		}
	}//B��Ϊ��
	
	void judgewukuaiA(void)
	{	
		//ʶ��B��
		color = Robot_checkColor();
		if(color==White)
		{ 
			BIsWhite();
		}
		else if(color==Yellow)
		{
			constantTakeR(otfR);
			TurnLeftAnyPulse(nz);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else
		{
			BNoWhite();
		}//A��Ϊ��ֱ�ӿ�B
		
		//ʶ��C��
		color = Robot_checkColor();
		if(color==Red)
		{ 
			CIsRed();
		}
		else if(color==Yellow)
		{
			constantTakeR(nz);
			TurnLeftAnyPulse(ff);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);
		}
		else if(color==White)
		{
			constantTakeR(otfR);
			TurnLeftAnyPulse(nz);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);		
		}
		else 
		{
			CNoRed();
		}
		
		//ʶ��D��
		color = Robot_checkColor();
		if(color==Black) 
		{
			DIsBlack();
		}
		else if(color==Yellow)
		{
			constantTakeR(ff);
			do {
				Robot_hunting2();
			} while(QTIS!=0);
		} 
		else if(color==White)
		{
			constantTakeR(nz);
			TurnLeftAnyPulse(ff);
			findLline();
			do {
			Robot_hunting2();
			} while(QTIS!=0);
		} 	
		else if(color==Red)
		{
			constantTakeR(otfR);
			TurnLeftAnyPulse(nz);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);
		}
		else 
		{
			DNoBlack();
		}
		//ʶ��E��
		color = Robot_checkColor();
		if(color==Blue) 
		{ 
			EIsBlue();
		} 
		else if(color==Yellow)
		{
			constantTakeR(0);
			TurnRightAnyPulse(nz);
			findRline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else if(color==White)
		{
			constantTakeR(ff);
			TurnRightAnyPulse(ff);
			findRline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else if(color==Red)
		{
			constantTakeR(nz);	
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else if(color==Black)
		{
			constantTakeR(otfR);
			//TurnLeftAnyPulse(ff);
			findLline();
			do {
				Robot_hunting2();
			} while(QTIS!=0);	
		}
		else
		{
			ENoBlue();
		}
	}//A��Ϊ��
		
		void fromlinetocore(void)
		{
			while(pointsth>=1)
			{
				do
				{
					Robot_hunting2();
					distance = GetDis();
				}while((distance >=50)&&(distance > 0));//����ת�������ɫ��ʱ��Ҫ���Ի�������ɫ��ľ���
				while(1)
					{
						distance = GetDis();
						if((distance<=45)&&(distance>0))
							{
								delay(2);
								if((distance<=45)&&(distance>0))
								{
									Robot_hunting2();
									distance = GetDis();
									break;
								}
							}
						else
						{
							Robot_hunting2();
						}
					}
				Crossblk(20);//ä�ߣ��û�������ɫ��Ӵ�
				Turn_180();
				do
				{
					Robot_hunting2();
				}while(QTIS != 15);
				Crossblk(7);
				pointsth--;//���һ����ȥ1
				color = Robot_checkColor();//ʶ����ɫ
				switch(color)
				{
					case Red:RedCarry();break;
					case White:WhiteCarry();break;
					case Yellow:YellowCarry();break;
					case Black:BlackCarry();break;
					case Blue:BlueCarry();break;
					default:break;
				}
			}
			stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
			while(stdtime2 <= 600)
			{
				Robot_hunting2();
			}                      //�Ӱ׵����ߵ�����������ʱ��2510��ʱ����ߵĳ�
			TIMSK2 &= ~(1<<OCIE2A);
			Turn_180();
			stop();
		}//���������ᵽ������ 
		
		void BackToLineL()
		{
			do{
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
				while(stdtime2 <= 30)
				{
					Robot_hunting2();
				}                      //�Ӱ׵����ߵ�����������ʱ��2510��ʱ����ߵĳ�
				TIMSK2 &= ~(1<<OCIE2A);
				TurnLeftAnyPulse(3);
				stop();
				}while(QTIS==0);
			Crossblk(15);
		}

void BackToLineR(){
	do{
		stdtime2 =0;
		TCNT2 = 0;
		TIMSK2 |=(1<<OCIE2A);//��ʱ��1����ж�ʹ��,ִ���жϷ�����
		while(stdtime2 <= 30)
		{
			Robot_hunting2();
		}                      //�Ӱ׵����ߵ�����������ʱ��2510��ʱ����ߵĳ�
		TIMSK2 &= ~(1<<OCIE2A);
		TurnRightAnyPulse(3);
		stop();
	}while(QTIS==0);
	Crossblk(15);
}

	void FindgraylineSekuai()
	{
		do{
			Robot_hunting2();
		}while(QTIS!=15);
		Crossblk(10);
		TurnLeftAnyPulse(42);
		findLline();
		stop();
		distance = GetDis();
		delay(100);
		int i=0;
		do
		{
			distance = GetDis();
	        servo_left.writeMicroseconds(1460);
	        servo_right.writeMicroseconds(1460);
			i++;
		}while(!((distance <= 220)&&(distance > 0)));
		TurnLeftAnyPulse(3);
		distance = GetDis();
		TurnRightAnyPulse(6);
		distance1 = GetDis();
		TurnLeftAnyPulse(3);
		if(distance1<distance)
		{
			TurnRightAnyPulse(3);
		}
		else
		{
			TurnLeftAnyPulse(3);
		}
		while(1)
		{
			distance = GetDis();
			if((distance <= 45)&&(distance > 0))
	        {
	        	delay(2);
	        	if((distance <= 45)&&(distance > 0))
	        	{
	        		FastGoForward();
	        		distance = GetDis();
	        		break;
				}
			}
			else
			{
				FastGoForward();
			}
		}
		Crossblk(10);
		if(i<200)
		{
			TurnLeftAnyPulse(70);
    		BackToLineL();
			stop();
    		findLline();
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			Crossblk(15);
			stop();
//			delay(20);
//			color = Robot_checkColor();
//			switch(color)
//			{
//			  case Red:RedCarry();break;
//	          case White:WhiteCarry();break;
//	          case Yellow:YellowCarry();break;
//	          case Black:BlackCarry();break;
//	          case Blue:BlueCarry();break;
//	          default:break;	
//			}
		}
		else
		{
			TurnRightAnyPulse(70);
    		BackToLineR();
			stop();
    		findRline();
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			Crossblk(15);
			stop();
//			delay(20);
//			color = Robot_checkColor();
//			switch(color)
//			{
//			  case Red:RedCarry();break;
//	          case White:WhiteCarry();break;
//	          case Yellow:YellowCarry();break;
//	          case Black:BlackCarry();break;
//	          case Blue:BlueCarry();break;
//	          default:break;	
//			}
		}
	}
	//���˼·����С������ת�����ߣ���QTISֵ��Ϊ15ʱͣ�£�ת��A�㣬��ʼ��ת���ó��������ɫ�飬
	//          ����⵽ɫ��ʱ��ֹͣ��ת����ǰ�ߣ�����⵽ɫ����һ����Χʱ����ʼä�ߣ��е�ɫ�飬����ת180��
	//          ä��һ���������������������ת���ҵ���ת�ߣ�ѭ��һ�ξ������ת180�ȣ�����ʼʶ����ɫ�� 

		int main(void)
		{
			init();
			setup();
			delay(2000);
			digitalWrite(Led,HIGH);
			for(i=0;i<3;i++)
			{
				WhiteBalance(refer_time);//��ƽ��
				delay(200);
			}
			digitalWrite(Led,LOW);
			delay(2000);
			Crossblk(40);
			shibiecolor();
			FindgraylineSekuai(); 
			returnstart();
			stop();
			return 0;
		}
