#include<iostream>
using namespace std;
#include<easyx.h>
#include<string>
#include<fstream>
#include <mmsystem.h>
#include<dsound.h>
#pragma comment(lib, "WINMM.LIB")

//函数的声明
void bf_ckdh();
string szzzf(int num);
int zfzsz(string& str, int len);
void gxlj2(string& lj, int& num);
bool pdlj(string& lj);
void csh_ckmb();								//初始化抽卡目标
int sc_sjs(int zsl);							//生成随机数，参数：总数量，返回值：随机结果，注意：数值范围最多3位数（0~999）
void scjg(int jg_s);							//生成结果
void szztys();									//设置字体样式

void bf_ckjmy();								//播放抽卡界面音
void bf_cky();									//播放抽卡音
void bf_csxy();									//播放出三星音
void bf_cwxy();									//播放出五星音

//路径
string ckdh_lj = "gacha-anim//ckdh1.png";
string jmdh_lj = "ui-anim//jmdh1.png";
string ckmb_lj = "data.txt";
string wzbj_lj = "images.png";

string ckdh_temp_lj = "gacha-anim//ckdh1.png";

/*/旧路径
string ckdh_lj = "抽卡动画//ckdh1.png";
string jmdh_lj = "界面动画//jmdh1.png";
string ckmb_lj = "抽卡目标.txt";
string wzbj_lj = "文字背景.png";
//*/

//其他数据
int sl = 1;
int ck_c = 1720;
int ck_k = 930;
int zl = 15;									//帧率（全屏模式时，只需延迟一秒，这里图片张数似乎有问题）
string ckmb[255];
int ckmb_sl = 0;								//抽卡目标数量

int zt_x = 520;									//字体打印位置的坐标
int zt_y = 270;
int zt_gd = 330;								//字体高度
int zt_kd = 0;									//每个字符的宽度

int sl_cqcs = 4;								//十连抽的抽取次数
IMAGE wzbj;										//文字的背景图

int main()
{
	cout << "关闭该窗口以停止程序运行" << endl;
	//cout << "\"祈愿10次\"的抽取次数：" << endl;
	//cin >> sl_cqcs;

	//播放抽卡界面音
	bf_ckjmy();

	initgraph(ck_c, ck_k, 1);						//创建窗口

	IMAGE jm;										//界面
	//loadimage(&jm, "背景2.png", ck_c, ck_k);
	ExMessage xx;									//判断鼠标点击
	int temp_bh = 1;
	int temp_sl = 1;								//临时数量

	csh_ckmb();										//初始化抽卡目标
	szztys();										//设置字体样式
	loadimage(&wzbj, wzbj_lj.c_str(), ck_c, ck_k);	//加载文字背景
	flushmessage();									//清空消息缓冲区

	while (1)
	{
		//putimage(0, 0, &jm);

		loadimage(&jm, jmdh_lj.c_str(), ck_c, ck_k);
		putimage(0, 0, &jm);							//将抽卡动画打印在当前绘图窗口上
		gxlj2(jmdh_lj, temp_bh);						//更新为下一张图片

		if (temp_bh == 201)								//如果已经为最后一张图片的下一张
		{
			temp_bh = 13;								//就将图片更新为第13张
			gxlj2(jmdh_lj, temp_bh);
			sl = 2;
		}

		while (peekmessage(&xx, 1))						//获取一个消息，且仅获取鼠标消息，只要有消息，就会一直进入判断
		{
			if (xx.message == 0x202)					//如果为鼠标左键弹起
			{
				if (xx.x >= 1026 && xx.x <= 1334 && xx.y >= 835 && xx.y <= 902)			//如果为单抽
				{

				}
				else if (xx.x >= 1362 && xx.x <= 1668 && xx.y >= 835 && xx.y <= 902)	//如果为十连
				{
					mciSendString(TEXT("close mysong"), NULL, 0, NULL);					//关闭音频

					temp_sl = sl;						//记录一下当前数量
					sl = 1;
					bf_ckdh();							//播放抽卡动画

					mciSendString(TEXT("close mysong"), NULL, 0, NULL);					//关闭音频

					if (ckmb_sl == 0)
					{
						Sleep(1000);						//由于目前没有抽卡后的结果素材，所以播放完抽卡后延时1秒
					}
					else
					{
						scjg(sl_cqcs);
					}

					csh_ckmb();							//抽取完后更新一次抽卡目标
					sl = temp_sl;
					temp_bh = 13;						//更新图片为第13张
					gxlj2(jmdh_lj, temp_bh);
					sl = 2;
				
					//播放抽卡界面音
					bf_ckjmy();
				}
			}
		}
		Sleep(zl);
	}





	system("pause");
	closegraph();

	return 0;
}

void bf_ckdh()								//抽卡动画
{
	IMAGE ckdh;
	ExMessage xx;
	int temp_bh = 1;
	bool xstg = false;							//显示跳过
	int xstg_sj = 2000;							//显示跳过持续时间
	flushmessage();								//清空消息缓冲区

	//bf_cky();

	for (int i = 0;i < 177;i++)
	{
		if (!pdlj(ckdh_lj))						//如果打开失败
		{
			cout << "打开失败" << endl;
			gxlj2(ckdh_lj, temp_bh);
			continue;
		}

		//*/
		if (i == 2)
		{
			//播放抽卡音
			bf_cky();
		}
		//*/

		loadimage(&ckdh, ckdh_lj.c_str(), ck_c, ck_k);
		putimage(0, 0, &ckdh);							//将抽卡动画打印在当前绘图窗口上		

		gxlj2(ckdh_lj, temp_bh);

		if (i < 174)
		{
			while (peekmessage(&xx, 1))					//获取一个消息，且仅获取鼠标消息，只要有消息，就会一直进入判断
			{
				if (xstg == false && xx.message == 0x201)
				{
					xstg = true;
				}
				else if (xstg == true && xx.message == 0x202 && xx.x >= 1000 && xx.x <= 1050 && xx.y >= 20 && xx.y <= 36)//如果为鼠标左键弹起
				{
					i = 174;							//目前这个不准确
				}
				else
				{
					xstg_sj -= 33;
				}
			}
		}

		Sleep(zl);											//每秒50帧
	}
	ckdh_lj = ckdh_temp_lj;
	sl = 1;
}
void gxlj2(string& lj, int& num)						//参数：路径，当前编号
{
	string temp;
	int wz = lj.rfind(".");							//获取路径中，最后一个点的位置
	num++;

	/*/
	temp = lj.substr(wz - sl, sl);					//获取字符串类型的数字
	int s = zfzsz(temp, temp.size());				//将该数字转为整数类型
	s++;											//让该数值++
	temp = szzzf(s);
	lj.replace(wz - sl, sl, temp);
	//*/
	temp = szzzf(num);								//将传入的数字，转为对应整数类型

	//cout << "lj：" << lj << endl;
	//cout << "num：" << num << endl;
	//system("pause");

	lj.replace(wz - sl, sl, temp);					//将路径中对应的数值，转为传入的数


	if (num == 10)
	{
		sl = 2;
	}
	else if (num == 100)
	{
		sl = 3;
	}
}
int zfzsz(string& str, int len)			//字符转数字（由于数组传进函数后，长度固定为8，无法靠str.size()在函数里获取长度，所以长度需要传进来）
{
	int s = 0;
	int sz[255];
	int wz = 0;

	for (int i = 0;i < len;i++)
	{
		sz[wz] = str[i];
		wz++;
	}

	for (int i = 0;i < wz;i++)
	{
		s = s * 10 + (sz[i] - 48);
	}

	return s;
}
string szzzf(int num)
{
	string sz;
	fstream fs;

	fs.open("text.txt", ios::out);
	fs << num << endl;					//将数字传到文件中
	fs.close();

	fs.open("text.txt", ios::in);
	getline(fs, sz);					//再将文件中的内容读回字符串中
	fs.close();

	return sz;
}
bool pdlj(string& lj)							//判断文件（路径）是否可以打开
{
	ifstream ifs;
	ifs.open(lj, ios::in | ios::binary);
	if (ifs.is_open())							//如果打开成功
	{
		ifs.close();
		return true;							//返回true
	}
	else										//否则（打开失败）
	{
		ifs.close();
		return false;							//返回false
	}
}
void csh_ckmb()								//初始化抽卡目标
{
	ckmb_sl = 0;

	ifstream ifs;
	ifs.open(ckmb_lj, ios::in);				//读取该文件
	if (!ifs.is_open())						//如果没有该文件
	{
		ifs.close();
		ofstream ofs;
		ofs.open(ckmb_lj, ios::out);		//则创建一个该文件
		ofs.close();
		ckmb_sl = 0;						//并记录抽卡目标数量为0，然后返回
		return;
	}
	for (int i = 0;i < 255;i++)				//将文件中的信息，传入抽卡目标数组中
	{
		if (!getline(ifs, ckmb[i]))			//如果没有内容了
		{
			break;							//退出循环
		}
		ckmb_sl++;							//抽卡目标数量++
	}
	ifs.close();
}
int sc_sjs(int zsl)						//参数：总数量，返回值：随机结果（数值从0开始，如果需要数值从某值开始，可以在返回结果上加上该数值）
{
										//使用条件说明：除了最高位，其他位数数值范围都必须是9
										//如：10，当十位数为0时（50%概率），有10种可能（0~9），每一种可能的概率都为50%÷10=5%
										//而十位数为1时（50%概率），只有1种可能（10），10的概率是50÷1=50%，概率不平均
										//结论：当十位数为1，只有在19的时候，概率平均
										//如：109，当百位数为0（50%概率），有100种可能（0~99），每一种可能的概率都为50%÷100=0.5%
										//而百位数为1时（50%概率），只有10种可能（100~109），每一种可能的概率是50÷10=5%，概率不平均
										//结论：当百位数为1，只有在199的时，概率平均
										//总结论：除了最高位，其他位数都必须是9，如：19、29、99、199、299、999、1999、2999（109和1099都不行）

	int s = 0;							//数

	int ws1 = 0;						//第一位数
	int ws2 = 0;						//第一位数的范围
	int ws3 = 0;
	int ws4 = 0;
	int ws_fw1 = 0;						//第一位数的范围
	int ws_fw2 = 0;
	int ws_fw3 = 0;
	int ws_fw4 = 0;

	//srand((unsigned int)time(NULL));	//这条随机数种子不能放在函数内，必须设置在main函数中
	if (zsl < 10)						//如果总数量为一位数
	{
		if (zsl <= 0)					//如果传入的总数量小于或等于0
		{
			return 0;					//返回0
		}
		return rand() % zsl;
	}
	else if (zsl < 100)					//如果总数量不小于10，且小于100（两位数）
	{
		ws_fw1 = zsl / 10;				//获取十位数的最大数（范围）
		ws1 = rand() % ws_fw1 + 1;
		if (ws1 == ws_fw1)				//如果位数的数值，等于最大范围数
		{
			ws_fw2 = zsl % 10;
			ws2 = rand() % (ws_fw2 + 1);
		}
		else
		{
			ws2 = rand() % 10;
		}
		s = ws1 * 10 + ws2;
	}
	else if (zsl < 1000)
	{
		ws_fw1 = zsl / 100;				//获取百位数的最大数（范围）
		ws1 = rand() % ws_fw1 + 1;

		if (ws1 == ws_fw1)				//如果百位数的数值，等于最大范围数
		{
			ws_fw2 = zsl / 10 % 10;
			ws2 = rand() % (ws_fw2 + 1);
		}
		else
		{
			ws2 = rand() % 10;
		}

		if (ws1 == ws_fw1 && ws2 == ws_fw2)				//如果百位数的数值，等于最大范围数
		{
			ws_fw3 = zsl % 10;
			ws3 = rand() % (ws_fw3 + 1);
		}
		else
		{
			ws3 = rand() % 10;
		}
		s = ((ws1 * 10) + ws2) * 10 + ws3;
	}
	else if (zsl < 10000)
	{
		ws_fw1 = zsl / 1000;				//获取千位数的最大数（范围）
		ws1 = rand() % ws_fw1 + 1;

		if (ws1 == ws_fw1)					//如果千位数的数值，等于最大范围数
		{
			ws_fw2 = zsl / 100 % 10;
			ws2 = rand() % (ws_fw2 + 1);
		}
		else
		{
			ws2 = rand() % 10;
		}

		if (ws1 == ws_fw1 && ws2 == ws_fw2)			//如果千位数的数值，等于最大范围数，且百位数的数值，等于最大范围
		{
			ws_fw3 = zsl / 10 % 10;
			ws3 = rand() % (ws_fw3 + 1);
		}
		else
		{
			ws3 = rand() % 10;
		}

		if (ws1 == ws_fw1 && ws2 == ws_fw2 && ws3 == ws_fw3)//如果千位数的数值，等于最大范围数，且百位数的数值，等于最大范围，且十位数的数值，等于最大范围
		{
			ws_fw4 = zsl % 10;
			ws4 = rand() % (ws_fw4 + 1);
		}
		else
		{
			ws4 = rand() % 10;
		}

		s = (((ws1 * 10) + ws2) * 10 + ws3) * 10 + ws4;
	}

	if (s == zsl)			//如果最后随机的结果为数的最大值，必须-1（因为，如果最大值为10，则应该只有0~9的范围，不能是10）
	{
		s - 1;
	}

	return s;
}
void scjg(int jg_s)							//生成结果，参数：结果的数量
{
	int sjs = 0;							//随机数
	int zfs = 0;							//字符数
	int djs = 0;							//倒计时（达到2秒时停止播放音频）
	int dc_pdsj = 80;						//单次判断时间
	int ys_sj = 800;						//延时时间（抽完一次后的延时时间）
	int pd = false;

	setbkcolor(WHITE);						//设置屏幕背景色为白色
	ExMessage xx;

	for (int i = 0;i < jg_s;i++)
	{
		djs = 0;							//刷新倒计时
		pd = false;
		sjs = sc_sjs(ckmb_sl);				//生成一个随机数

		zfs = ckmb[sjs].size();
		if (zfs >= 3 && zfs <= 4)			//如果字符数为3~4个字符（2个中文汉字）
		{
			zt_x = 520;
		}
		else if (zfs >= 5 && zfs <= 6)		//如果字符数为5~6个字符（3个中文汉字）
		{
			zt_x = 350;
		}
		else if (zfs >= 7 && zfs <= 8)		//如果字符数为7~8个字符（4个中文汉字）
		{
			zt_x = 200;
		}

		//播放出3星音
		bf_csxy();		

		cleardevice();						//清屏
		putimage(0, 0, &wzbj);				//打印文字背景
		
		outtextxy(zt_x, zt_y, ckmb[sjs].c_str());		//在对应位置打印对应字符串
		Sleep(ys_sj);
		
		flushmessage();						//清空消息缓冲区

		//新逻辑
		while (1)							//获取一个鼠标消息
		{
			if (peekmessage(&xx, 1, true))		//如果有接收到新鼠标消息
			{
				if (xx.message == 0x202)		//如果为左键弹起消息
				{
					mciSendString(TEXT("close mysong"), NULL, 0, NULL);				//关闭音频

					break;						//则退出，打印下一个字符串
				}
			}
			if (pd == false)					//如果音频没有播放完，则延时（如果已经播放完音频则不要延时）
			{
				Sleep(dc_pdsj);
				djs += dc_pdsj;
				if (djs >= 2000 - ys_sj && pd == false)
				{
					//mciSendString(TEXT("close mysong"), NULL, 0, NULL);			//关闭音频（已取消音频循环，不需要再用计时方式关闭）
					pd = true;
				}
			}
		}
		/*/旧逻辑
		while (1)							//获取一个鼠标消息
		{
			if (peekmessage(&xx, 1, true))		//如果有接收到新鼠标消息
			{
				if (xx.message == 0x202)		//如果为左键弹起消息
				{
					if (pd == false)
					{
						mciSendString(TEXT("close mysong"), NULL, 0, NULL);					//关闭音频
					}
					break;						//则退出，打印下一个字符串
				}
			}
			if (pd == false)					//如果音频没有播放完，则延时（如果已经播放完音频则不要延时）
			{
				Sleep(dc_pdsj);
				djs += dc_pdsj;
				if (djs >= 2000 - ys_sj && pd == false)
				{
					mciSendString(TEXT("close mysong"), NULL, 0, NULL);					//关闭音频
					pd = true;
				}
			}
		}
		//*/
	}
}
void szztys()						//设置字体样式
{
	//settextcolor(BLACK);			//设置字体颜色为黑色
	setbkmode(1);

	LOGFONT zt;
	zt.lfHeight = zt_gd;
	zt.lfWeight = zt_kd;
	zt.lfFaceName[0] = '1';
	//settextstyle(&zt);

	settextstyle(zt_gd, zt_kd, "1", 0, 0, 0, false, false, false);
}

void bf_ckjmy()								//播放抽卡界面音
{
	//播放音频
	mciSendString(TEXT("open audio//抽卡界面音.mp3 alias mysong"), NULL, 0, NULL);
	mciSendString(TEXT("play mysong"), NULL, 0, NULL);
}
void bf_cky()								//播放抽卡音
{
	//播放音频
	mciSendString(TEXT("open audio//抽卡音.mp3 alias mysong"), NULL, 0, NULL);
	mciSendString(TEXT("play mysong"), NULL, 0, NULL);
}
void bf_csxy()								//播放出三星音
{
	//播放音频
	mciSendString(TEXT("open audio//出3星音效.mp3 alias mysong"), NULL, 0, NULL);
	mciSendString(TEXT("play mysong"), NULL, 0, NULL);
}
void bf_cwxy()								//播放出五星音
{
	//播放音频
	mciSendString(TEXT("open audio//出5星音效.mp3 alias mysong"), NULL, 0, NULL);
	mciSendString(TEXT("play mysong"), NULL, 0, NULL);
}