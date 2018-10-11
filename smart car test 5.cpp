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
		
		#define otf 75    //135度
		#define otfR 90    //135度  
		#define nz 40     //90度 
		#define ff 16     //45度
	
		
		#define Yellow  1    //黄
		#define White  	2    //白
		#define Red    	3    //红
		#define Black  	4    //黑
		#define Blue    5    //蓝
		//motor 引脚 
		unsigned long time; 
		int i=0;
		unsigned int i1=0;
		char QTIS=0;
		int trac1 = A0; //从车头方向的最右边开始排序
		int trac2 = A1;
		int trac3 = A2;
		int trac4 = A3;
		//定义两个360舵机的引脚
		Servo servo_left;
		Servo servo_right;
		
		int Led = 9;
		int times= millis();
		
		const int trig = 12;//超声波Trig引脚
		const int echo = 10;//超声波Echo引脚(定时器1) 
		
		unsigned long pulsewidth=0;
		unsigned long millimetre=0;
		unsigned long dis=0;
		uint8_t Trigpin;
		uint8_t Echopin;
		
		//传感器引脚定义
		const int colorpin[6]={7,6,5,4,8,3};//颜色传感器引脚连接
		
		//颜色传感器变量
		int refer_time[3]={0,0,0};//白平衡得到的基准时间
		int clrpulses[3] = {0,0,0};
		int currentcolor=0;
		
		char recog_times=0;
		//char i=0;
		
		volatile int pulses=0;//脉冲数
		volatile int stdtime=0;//时间计数器
		int s0,s1,s2,s3,led,out;//颜色传感器引脚
		volatile bool flag = false;
		
		
		int sekuai=5;
		int color;//实际颜色判断时用到
		int color1;//颜色时测试用
		char Idone=0;
		char process=1;
		char pointsth=0;//在直线上放第几个色块
		unsigned int distance=0;//超声波测距
		unsigned int distance1=0;
		extern volatile int stdtime2;
		
		unsigned char time_flag=0;
		volatile int stdtime2=0;//时间计数器
		
		
		void Init_Timer2(void)
		{
		    TCCR2A |= (1<<WGM21);//CTC功能
		    TCCR2B |= (1<<CS21); //8分频，时钟周期16/8=2MHz，0.5us
		    TCNT2 = 0;
		    OCR2A = 10;
		    sei();//开启总中断
		}
		
		SIGNAL(TIMER2_COMPA_vect)//TCNT2与OCR2A发生匹配时，TCNT2自动清零并发生中断
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
		 * 左旋转
		 */
		void SpinLeft(void)
		{
		  servo_left.writeMicroseconds(1465);
		  servo_right.writeMicroseconds(1465);
		  delay(20);
		 
		}
		/*
		 * 右旋转
		 */
		void SpinRight(void)
		{
		  servo_left.writeMicroseconds(1535);
		  servo_right.writeMicroseconds(1535);
		  delay(20);
		  
		}
		void TurnLeft(void)//向左小转
		{
		  servo_left.writeMicroseconds(1500);
		  servo_right.writeMicroseconds(1470);
		  delay(10);
		  
		}
		void TurnLeft_b(void)//向左大转
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
		 * //检测中间左QTI是否在黑线内
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
		 * //检测中间右QTI是否在黑线内
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
		
		void findLline(void)  //找左边下一根线
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
		
		void findRline(void) //找右边下一根线
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
		      case 4:TurnLeft_b();break;//左转
		      case 8:TurnLeft();break;//左转
		      case 12:TurnLeft();break;//左转
		      case 1:TurnRight();break;//右转
		      case 2:TurnRight_b();break;//右转
		      case 3:TurnRight();break;//右转
		      case 6:FastGoForward();break;//直行
		      default:FastGoForward();break;//这一句不能省，防止前面七种情况没有的时候，执行这个
		    }
		    
		}
		void InitUltrasonic(uint8_t trig,uint8_t echo)
		{
			Trigpin = trig;
			Echopin = echo;
			pinMode(trig,OUTPUT);//PB10置为输出，Trig
			pinMode(echo,INPUT);//PB12置为输入,Echo
			digitalWrite(echo,LOW);
			digitalWrite(trig,LOW);
		}
		unsigned long GetDis(void)
		{
			//首先给Trig引脚一个10us的启动脉冲
			//delayMicroseconds(5);
			digitalWrite(Trigpin,HIGH);
			delayMicroseconds(25);
			digitalWrite(Trigpin,LOW);
			//读取Echo引脚高电平状态持续时间,指定超时时间为18.5ms（18500L）,即18.5*0.34m=6.29m, 3.145mm距离
			//当超出测量范围时，返回值为0
			pulsewidth = pulseIn(Echopin,HIGH,18500L);//pulsewidth单位为us
			millimetre=pulsewidth*0.34;//转换成毫米
			return millimetre/2;//返回实际距离（单位是mm），误差为3mm
			
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
			cli();//屏蔽所有中断
			EICRA |= (1<<ISC10)|(1<<ISC11); //外部中断1上升沿触发中断
		}
		void Init_Timer0(void)
		{
		    TCCR0A |= (1<<WGM01);//CTC功能
		    TCCR0B |= (1<<CS01); //8分频，时钟周期16/8=2MHz，0.5us
		    TCNT0 = 0;
		    OCR0A = 10;
		    sei();//开启总中断
		}
		
		
		void OpenRedFilter(void){//红色滤波器
			digitalWrite(s2,LOW);//S2--PD6
			digitalWrite(s3,LOW);//S3--PD7
		}
		void OpenBlueFilter(void){//蓝色滤波器
			digitalWrite(s2,LOW);
			digitalWrite(s3,HIGH);
		}
		void OpenGreenFilter(void){//绿色滤波器
			digitalWrite(s2,HIGH);
			digitalWrite(s3,HIGH);
		}
		void ClosePower(void){//关闭电源
			digitalWrite(s0,LOW);
			digitalWrite(s1,LOW);
		}
		void Out1than50(void){//输出比例1:50
			digitalWrite(s0,LOW);
			digitalWrite(s1,HIGH);
		}
		void Out1than5(void){//输出比例1:5
			digitalWrite(s0,HIGH);
			digitalWrite(s1,LOW);
		}
		void Out1than1(void){//输出比例1:1
			digitalWrite(s0,HIGH);
			digitalWrite(s1,HIGH);
		}
		bool ColorreCognt(int timestd[3],int value[3])
		{
			digitalWrite(led,HIGH);//开灯
			delay(100);
		
			//红色
			pulses=0;
			stdtime=0;
			TCNT0 = 0;
			TIMSK0 |=(1<<OCIE0A);//定时器1溢出中断使能,执行中断服务函数
			Out1than1();
			OpenRedFilter();
			EIMSK |= (1<<INT1);      /*使能外部中断1请求*/
			while(stdtime != timestd[0]);
			EIMSK &= ~(1<<INT1);
			TIMSK0 &= ~(1<<OCIE0A);
			ClosePower();
			if(pulses > 255){
				pulses = 255;
			}
			value[0] = pulses;
		
		
			//蓝色
			pulses=0;
			stdtime=0;
			TCNT0 = 0;
			TIMSK0 |=(1<<OCIE0A);//定时器1溢出中断使能,执行中断服务函数
			Out1than1();
			OpenBlueFilter();
			EIMSK |= (1<<INT1);      /*使能外部中断1请求*/
			while(stdtime != timestd[1]);
			EIMSK &= ~(1<<INT1);
			TIMSK0 &= ~(1<<OCIE0A);
			ClosePower();
			if(pulses > 255){
				pulses = 255;
			}
			value[1] = pulses;
		
			//绿色
			pulses=0;
			stdtime=0;
			TCNT0 = 0;
			TIMSK0 |=(1<<OCIE0A);//定时器1溢出中断使能,执行中断服务函数
			Out1than1();
			OpenGreenFilter();
			EIMSK |= (1<<INT1);      /*使能外部中断1请求*/
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
		
			//红色滤波器
			stdtime=0;
			pulses=0;	//计数清零
			TCNT0 = 0;
			TIMSK0 |=(1<<OCIE0A);//定时器1溢出中断使能,执行中断服务函数
			Out1than1();
			OpenRedFilter();
			EIMSK |= (1<<INT1);      /*使能外部中断1请求*/
			while(!(flag&0x1));//等待定时到
			TIMSK0 &= ~(1<<OCIE0A);
			ClosePower();
			EIMSK &= ~(1<<INT1);
		
			refertime[0] = stdtime;//保存脉冲数
			flag=false;
		
			//蓝色
			stdtime=0;
			pulses=0;	//计数清零
			TCNT0 = 0;
			TIMSK0 |=(1<<OCIE0A);//定时器1溢出中断使能,执行中断服务函数
			Out1than1();
			OpenBlueFilter();
			EIMSK |= (1<<INT1);      /*使能外部中断1请求*/
			while(!(flag&0x1));
			TIMSK0 &= ~(1<<OCIE0A);
			ClosePower();
			EIMSK &= ~(1<<INT1);
		
			refertime[1] = stdtime;
			flag=false;
		
			//绿色
			stdtime=0;
			pulses=0;	//计数清零
			TCNT0 = 0;
			TIMSK0 |=(1<<OCIE0A);//定时器1溢出中断使能,执行中断服务函数
			Out1than1();
			OpenGreenFilter();
			EIMSK |= (1<<INT1);      /*使能外部中断1请求*/
			while(!(flag&0x1));
			TIMSK0 &= ~(1<<OCIE0A);
			ClosePower();
			EIMSK &= ~(1<<INT1);
		
			refertime[2] = stdtime;
			flag=false;
		
			digitalWrite(s0,LOW);//S0,关闭电源
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
			//OUT = pin[5],必须是PD3
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
		
			Init_INT1();//外部中断1
			Init_Timer0();//定时器0
		}
		
		SIGNAL(INT1_vect)
		{
			pulses++;//脉冲计数器
			if(pulses == 255)//白色光源时，三色滤波的值都默认是255
			{
				flag = true;
			}
		}
		SIGNAL(TIMER0_COMPA_vect)//TCNT0与OCR0A发生匹配时，TCNT0自动清零并发生中断
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
				TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
				while(stdtime2 <= 700)
				{
					Robot_hunting2();
				}
				TIMSK2 &= ~(1<<OCIE2A);
				back(45);//后退
				Turn_180();//掉头
				pointsth++;
		}
		
		void DownSecond()
		{
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
				while(stdtime2 <= 600)
				{
					Robot_hunting2();
				}
				TIMSK2 &= ~(1<<OCIE2A);
				back(45);//后退
				Turn_180();//
				
				pointsth++;
		}
		
		void Downthird()
		{
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
				while(stdtime2 <=500)
				{
					Robot_hunting2();
				}
				TIMSK2 &= ~(1<<OCIE2A);
				back(40);//后退
				Turn_180();//
				
				pointsth++;
		}
		
		void DownFourth()
		{
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
				while(stdtime2 <= 400)
				{
					Robot_hunting2();
				}
				TIMSK2 &= ~(1<<OCIE2A);
				back(40);//后退
				Turn_180();//
				
				pointsth++;
		}
		
		void DownFiveth(void)
		{
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//定时器2溢出中断使能,执行中断服务函数
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
				Robot_hunting2();//中转线上面巡线
			}while(QTIS!=15);
			
			stop();
			Crossblk(13);
		//	delay(2000);
			TurnLeftAnyPulse(40);
			//盲转某角度
		//	delay(2000);
			findLline();//寻找下一根黑线
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);//找到色块前一直巡线前行
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
			GotoLine();//中转摆放色块摆放
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
		
		void RedCarry(void)//搬红色色块到分数区
		{
				do
				{
					Robot_hunting2();
				}while(QTIS > 0);
				Crossblk(5);
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
				while(stdtime2 <= 300)
				{
					Robot_hunting2();
				}  //从白点行走到分数区所用时间550，时间大走的长
				TIMSK2 &= ~(1<<OCIE2A);
				Crossblk(32); 
				back(40);//后退
				Turn_180();
				do
				{
					Robot_hunting2();
				}while(QTIS != 15);
				Crossblk(10);
		}
		void WhiteCarry(void)//搬白色色块到分数区
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
				TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
				while(stdtime2 <= 300)
				{
					Robot_hunting2();
				}  //从白点行走到分数区所用时间2510，时间大走的长
				TIMSK2 &= ~(1<<OCIE2A);
				Crossblk(30);
				back(30);//后退
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
		void YellowCarry(void)//搬黄色色块到分数区
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
				TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
				while(stdtime2 <= 300)
				{
					Robot_hunting2();
				}    //从白点行走到分数区所用时间2510，时间大走的长
				TIMSK2 &= ~(1<<OCIE2A);
				Crossblk(28);
				back(30);//后退
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
		void BlackCarry(void)//搬黑色色块到分数区
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
				TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
				while(stdtime2 <= 300)
				{
					Robot_hunting2();
				}   //从白点行走到分数区所用时间2510，时间大走的长
				TIMSK2 &= ~(1<<OCIE2A);
				Crossblk(50);
				back(30);//后退
				Turn_180();
				do
				{
					Robot_hunting2();
				}while(QTIS != 15);
				Crossblk(14);
				findLline();
				delay(200);
		}
		void BlueCarry(void)//搬蓝色色块到分数区
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
				TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
				while(stdtime2 <= 300)
				{
					Robot_hunting2();
				}                      //从白点行走到分数区所用时间2510，时间大走的长
				TIMSK2 &= ~(1<<OCIE2A);
				Crossblk(30);
				back(30);//后退
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
				if(sekuai>=5)//第一个色块不用测距，直接掉头就行
				{
					Turn_180();
				}
				else
				{
					do
					{
						Robot_hunting2();
						distance = GetDis();
					}while((distance >=50)&&(distance > 0));//从中转线上面搬色块时，要测试机器人与色块的距离
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
					Crossblk(10);//盲走，让机器人与色块接触
					Turn_180();
				}
				do
				{
					Robot_hunting2();
				}while(QTIS != 15);
				Crossblk(8);
				pointsth--;//搬掉一个减去1
				sekuai--;//已经搬的色块个数减一
				delay(20);
				color = Robot_checkColor();//识别颜色
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
		
		//初始化
		
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
		    	TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
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
		    	TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
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
			do//巡线 
			{
				Robot_hunting2();//中转线上面巡线
			}while(QTIS!=15);
			stop();
			Crossblk(13);//巡线结束 
			int ULT=FindEmptyLine();
		//	delay(2000);
			if(ULT==4)
			{ 
				firstfindline(); 
				judgewukuaiE();
			} //E为空 
			else if(ULT==3)
			{ 
				firstfindline();
				judgewukuaiD();
			} //D为空 
			else if(ULT==2)
			{ 
				firstfindline();
				judgewukuaiC();
			}//C为空 
			else if(ULT==1)
			{ 
				firstfindline();
				judgewukuaiB();
			} //B为空 
			else if(ULT==0)//A为空
			{
				TurnLeftAnyPulse(40);
				//盲转某角度
			//	delay(2000);
				findRline();//寻找下一根黑线
			//	delay(2000);
				do
				{
					Robot_hunting2();
				}while(QTIS!=0);//找到色块前一直巡线前行
				judgewukuaiA();
			} 
			fromlinetocore();
			/*color = Robot_checkColor();//识别颜色
			switch(color)
			{
				case Red:RedCarry();break;
				case White:WhiteCarry();break;
				case Yellow:YellowCarry();break;
				case Black:BlackCarry();break;
				case Blue:BlueCarry();break;
				default:break;
			}*/
		}//识别物块颜色 
		
		void AIsYellow(void)
		{
			takewukuai(29);
			TurnLeftAnyPulse(100);
			findRline();
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
		}//A点的是黄色块 
		
		void ANoYellow(void)
		{
			takewukuaitocore();
			TurnRightAnyPulse(30);
			findRline();
			GotoLine();//中转摆放色块摆放
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
		}//A点的不是黄色块
		
		void BIsWhite(void)
		{
			takewukuai(29);
			TurnLeftAnyPulse(100);
			findRline();
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
		}//B点的是白色块 
		
		void BNoWhite(void)
		{
			takewukuaitocore();
		//	TurnRightAnyPulse(15);
			findRline();
			GotoLine();//中转摆放色块摆放
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
		}//B点的不是白色块 
		
		void CIsRed(void)
		{
			takewukuai(29);
			TurnLeftAnyPulse(100);
			findRline();
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);
			
		}//C点的是红色块 
		
		void CNoRed(void)
		{
			takewukuaitocore();
		//	TurnRightAnyPulse(30);
		//	findRline();
			GotoLine();//中转摆放色块摆放
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
		}//C点的不是红色块
		
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
			GotoLine();//中转摆放色块摆放
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
			TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
			while(stdtime2 <= 300)
			{
				Robot_hunting2();
			}
			TIMSK2 &= ~(1<<OCIE2A);
		//	back(40);//后退
		//	Turn_180();//
			*/
		}//E点的是蓝色块 
		
		void ENoBlue(void)
		{
			takewukuaitocore();
			TurnLeftAnyPulse(40);
			findLline();
			stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
			while(stdtime2 <= 200)
			{
				Robot_hunting2();
			}
			TIMSK2 &= ~(1<<OCIE2A);
			back(40);//后退
			pointsth++;
		//	Turn_180();
		}//E.点的不是蓝色块
		
		void DIsBlackE(void)
		{
			takewukuai(29);
		//	TurnLeftAnyPulse(30);
			findLline();
			stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
			while(stdtime2 <= 100)
			{
				Robot_hunting2();
			}
			TIMSK2 &= ~(1<<OCIE2A);
		//	back(40);//后退
		//	Turn_180();
		}//D点的是黑色块E
		
		void DNoBlackE(void)
		{
			takewukuaitocore();
		//	TurnLeftAnyPulse(30);
			findLline();
			stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
			while(stdtime2 <= 300)
			{
				Robot_hunting2();
			}
			TIMSK2 &= ~(1<<OCIE2A);
			back(40);//后退
		//	Turn_180();
			pointsth++;
		}//D点的不是黑色块E
		
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
			GotoLine();//中转摆放色块摆放
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
			GotoLine();//中转摆放色块摆放
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
			GotoLine();//中转摆放色块摆放
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
		
		void takewukuaitocore(void){//将物块推至中心准备转弯 
			Turn_180();
			do
			{
				Robot_hunting2();
			}while(QTIS!=15);
			Crossblk(14);
		//	delay(2000);
		} 
		
		void takewukuai(int pulse){//同颜色的推至分数区 
			stdtime2 =0;
			TCNT2 = 0;
			TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
			while(stdtime2 <= 200)
			{
				Robot_hunting2();
			}                      //从白点行走到分数区所用时间2510，时间大走的长
			TIMSK2 &= ~(1<<OCIE2A);
			Crossblk(pulse);
			back(30);//后退
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
			//盲转某角度
		//	delay(2000);
			findRline();//寻找下一根黑线
		//	delay(2000);
			do
			{
				Robot_hunting2();
			}while(QTIS!=0);//找到色块前一直巡线前行
		}
		
	void judgewukuaiE(void)
	{
		//识别A点
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
		
		//识别B点
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
		
		//识别C点
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
		
		//识别D点
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
			TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
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
			TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
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
			TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
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
			TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
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
	}//E点为空
	
	void judgewukuaiD(void)
	{
		//识别A点
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
		
		//识别B点
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
		
		//识别C点
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
		
		//识别E点
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
	}//D点为空
	
	void judgewukuaiC(void) {
		
		//识别A点
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
		
		//识别B点
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
		}//C点为空直接跨D
		
		//识别D点
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
		
		//识别E点
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
	}//C点为空
	
	void judgewukuaiB(void)
	{
		//识别A点
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
		}//B点为空直接跨C
		
		//识别C点
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
		
		//识别D点
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
		
		//识别E点
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
	}//B点为空
	
	void judgewukuaiA(void)
	{	
		//识别B点
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
		}//A点为空直接跨B
		
		//识别C点
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
		
		//识别D点
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
		//识别E点
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
	}//A点为空
		
		void fromlinetocore(void)
		{
			while(pointsth>=1)
			{
				do
				{
					Robot_hunting2();
					distance = GetDis();
				}while((distance >=50)&&(distance > 0));//从中转线上面搬色块时，要测试机器人与色块的距离
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
				Crossblk(20);//盲走，让机器人与色块接触
				Turn_180();
				do
				{
					Robot_hunting2();
				}while(QTIS != 15);
				Crossblk(7);
				pointsth--;//搬掉一个减去1
				color = Robot_checkColor();//识别颜色
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
			TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
			while(stdtime2 <= 600)
			{
				Robot_hunting2();
			}                      //从白点行走到分数区所用时间2510，时间大走的长
			TIMSK2 &= ~(1<<OCIE2A);
			Turn_180();
			stop();
		}//将线上物块搬到分数区 
		
		void BackToLineL()
		{
			do{
				stdtime2 =0;
				TCNT2 = 0;
				TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
				while(stdtime2 <= 30)
				{
					Robot_hunting2();
				}                      //从白点行走到分数区所用时间2510，时间大走的长
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
		TIMSK2 |=(1<<OCIE2A);//定时器1溢出中断使能,执行中断服务函数
		while(stdtime2 <= 30)
		{
			Robot_hunting2();
		}                      //从白点行走到分数区所用时间2510，时间大走的长
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
	//设计思路：让小车从中转线上走，当QTIS值都为15时停下，转至A点，开始旋转并用超声波检测色块，
	//          当检测到色块时，停止旋转并向前走，当检测到色块在一定范围时，开始盲走，夹到色块，并旋转180度
	//          盲走一定距离后，视情况向左或向右转，找到中转线，循迹一段距离后，旋转180度，并开始识别颜色。 

		int main(void)
		{
			init();
			setup();
			delay(2000);
			digitalWrite(Led,HIGH);
			for(i=0;i<3;i++)
			{
				WhiteBalance(refer_time);//白平衡
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
