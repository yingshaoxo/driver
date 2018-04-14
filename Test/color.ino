/*首先进行白平衡，把一个白色物体放置在TCS3200颜色传感器之下，两者相距10mm左右，
点亮传感器上的4个白光LED灯，用Arduino控制器的定时器设置一固定时间1s，
然后选通三种颜色的滤波器，让被测物体反射光中红、绿、蓝三色光分别通过滤波器，
计算1s时间内三色光分别对应的TCS3200的输出脉冲数，再通过算式得到
白色物体RGB值255与三色光脉冲数的比例因子。
有了白平衡后，得到的RGB比例因子，则其他颜色物体反射光中红、绿、蓝三色光对应
的1s内TCS3200输出信号脉冲数乘以R、G、B比例因子，就可换算出被测物体的RGB标准值。*/
#include "TimerOne.h"

//把TCS3200颜色传感器各控制引脚连到Arduino数字端口 
#define S0     A0   //物体表面的反射光越强，TCS3002D的内置振荡器产生的方波频率越高，
#define S1     A1   //S0和S1的组合决定输出信号频率比率因子，比例因子为2%
                    //比率因子为TCS3200传感器OUT引脚输出信号频率与其内置振荡器频率之比
#define S2     A4   //S2和S3的组合决定让红、绿、蓝，哪种光线通过滤波器
#define S3     A5
#define OUT    2  	//TCS3200颜色传感器输出信号输入到Arduino中断0引脚，并引发脉冲信号中断
                  	//在中断函数中记录TCS3200输出信号的脉冲个数
#define LED    A3   //控制TCS3200颜色传感器是否点亮
int   g_count = 0;    // 计算与反射光强相对应TCS3200颜色传感器输出信号的脉冲数
// 数组存储在1s内TCS3200输出信号的脉冲数，它乘以RGB比例因子就是RGB标准值
int   g_array[3];   
int   g_flag = 0;     //滤波器模式选择顺序标志
float g_SF[3];       // 存储从TCS3200输出信号的脉冲数转换为RGB标准值的RGB比例因子
  
// 初始化TSC3200各控制引脚的输入输出模式
//设置TCS3002D的内置振荡器方波频率与其输出信号频率的比例因子为2%
void TSC_Init()
{
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  pinMode(LED, OUTPUT); 
  digitalWrite(S0, LOW);  
  digitalWrite(S1, HIGH); 
}
 
//选择滤波器模式，决定让红、绿、蓝，哪种光线通过滤波器 
void TSC_FilterColor(int Level01, int Level02)
{
  if(Level01 != LOW)
    Level01 = HIGH; 
  if(Level02 != LOW)
    Level02 = HIGH; 
  digitalWrite(S2, Level01); 
  digitalWrite(S3, Level02); 
}
//中断函数，计算TCS3200输出信号的脉冲数 
void TSC_Count()
{
  g_count ++ ;
}
//定时器中断函数，每1s中断后，把该时间内的红、绿、蓝三种光线通过滤波器时，
//TCS3200输出信号脉冲个数分别存储到数组g_array[3]的相应元素变量中
void TSC_Callback()
{
  switch(g_flag)
  {
    case 0: 
         TSC_WB(LOW, LOW);              //选择让红色光线通过滤波器的模式
         break;
    case 1:
         g_array[0] = g_count;       //存储1s内的红光通过滤波器时，TCS3200输出的脉冲个数
         TSC_WB(HIGH, HIGH);         //选择让绿色光线通过滤波器的模式
         break;
    case 2:
         g_array[1] = g_count;       //存储1s内的绿光通过滤波器时，TCS3200输出的脉冲个数
         TSC_WB(LOW, HIGH);          //选择让蓝色光线通过滤波器的模式
         break;
 
    case 3:
         g_array[2] = g_count;       //存储1s内的蓝光通过滤波器时，TCS3200输出的脉冲个数
         TSC_WB(HIGH, LOW);             //选择无滤波器的模式   
         break;
   default:
         g_count = 0;    //计数值清零
         break;
  }
}
//设置反射光中红、绿、蓝三色光分别通过滤波器时如何处理数据的标志
//该函数被TSC_Callback( )调用
void TSC_WB(int Level0, int Level1)     
{
  g_count = 0;   //计数值清零
  g_flag ++;     //输出信号计数标志
  TSC_FilterColor(Level0, Level1); //滤波器模式
  Timer1.setPeriod(100000);      //设置输出信号脉冲计数时长1s
}
//初始化
void setup()
{
  TSC_Init();
  Serial.begin(9600); //启动串行通信
  Timer1.initialize();   // defaulte is 1s
  Timer1.attachInterrupt(TSC_Callback); //设置定时器1的中断，中断调用函数为TSC_Callback() 
  //设置TCS3200输出信号的上跳沿触发中断，中断调用函数为TSC_Count()
  attachInterrupt(0, TSC_Count, RISING);  
  digitalWrite(LED, HIGH);//点亮LED灯
  delay(1500); //延时1.5s，以等待被测物体红、绿、蓝三色在1s内的TCS3200输出信号脉冲计数
  //通过白平衡测试，计算得到白色物体RGB值255与1s内三色光脉冲数的RGB比例因子
  g_SF[0] = 255.0/g_array[0];     //红色光比例因子
  g_SF[1] = 255.0/g_array[1];    //绿色光比例因子
  g_SF[2] = 255.0/g_array[2];    //蓝色光比例因子
  //红、绿、蓝三色光对应的1s内TCS3200输出脉冲数乘以相应的比例因子就是RGB标准值

}
//主程序 
void loop()
{
	int color[3];
    g_flag = 0;
    //每获得一次被测物体RGB颜色值需时4s
    delay(500);
    //打印出被测物体RGB颜色值 
    for(int i=0; i<3; i++)      
    	color[i] = g_array[i] * g_SF[i];
    Serial.println((String)(color[0]) + '+' + (String)(color[1]) + '+' + (String)(color[2]) + '+');
}
