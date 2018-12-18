/******************************************************************
*文件名:CUIGUI配置文件
*创建者:博硕电子科技，www.explorersoftware.taobao.com
*创建时间:2017年6月28日09:57:36
*文件说明:基于CUIGUI，适应各种屏需要配置更改的函数
*/

#ifndef  __CUIGUI_CONFIG_H  
#define  __CUIGUI_CONFIG_H

/*支持BMP格式*/
#define     USE_BMP           1           //使用位图

/*支持字体*/
#define     USE_FONT          1           //使用字体

//#if         USE_FONT > 0 
///*支持汉字*/
//#define     CUIGUI_FONT936    1           //支持汉字开关
//#endif

/*设置保存句柄的最大数组*/
#define     HWIN_MAX          50   //句柄的最大数组

/*颜色配置*/

#define     COLOR_DATTYPE     u16         //定义颜色的数据类型
#define     LCD_NUM_COLORS    65536       //颜色数


#endif
