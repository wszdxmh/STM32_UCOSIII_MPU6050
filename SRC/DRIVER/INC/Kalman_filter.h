
#ifndef __KALMAN_FILTER_H_
#define __KALMAN_FILTER_H_

#define Pi 	3.1415926f
//卡尔曼噪声参数
#define dt           0.012f
#define R_angle      0.5f
#define Q_angle      0.001f
#define Q_gyro       0.003f//越大越滞后

//引用外部变量
extern short aacx, aacy, aacz;		//加速度传感器原始数据
extern short gyrox, gyroy, gyroz;	//陀螺仪原始数据

extern float  Acc_angle,Gry_vivi;
extern float Angle,Gyro_x;  //小车滤波后倾斜角度/角速度
extern float Angle_x_temp;  //由加速度计算的x倾斜角度
extern float Angle_y_temp;  //由加速度计算的y倾斜角度
extern float Angle_z_temp;
extern float Angle_X_Final; //X最终倾斜角度
extern float Angle_Y_Final; //Y最终倾斜角度
extern float Angle_Z_Final; //Z最终倾斜角度
extern float Gyro_x;		 //X轴陀螺仪数据暂存
extern float Gyro_y;     //Y轴陀螺仪数据暂存
extern float Gyro_z;		 //Z轴陀螺仪数据暂存
extern float A_P,A_R,A2_P;

double KalmanFilter(const double ResrcData, double ProcessNiose_Q, double MeasureNoise_R);
void Angle_Calcu(void);
void Kalman_Filter_X(float Accel,float Gyro);
void Kalman_Filter_Y(float Accel,float Gyro);
void Kalman_Filter_Z(float Accel, float Gyro);
void yijiehubu_P(float angle_m, float gyro_m);
void erjiehubu_P(float angle_m, float gyro_m);
void Erjielvbo(float angle_m,float gyro_m);

#endif
