#include <windows.h>
#include<iostream>
using namespace std;
#include<easyx.h>
#include<string>
#include<fstream>
#include <mmsystem.h>
#include<dsound.h>
#pragma comment(lib, "WINMM.LIB")
#include <random>
#include <filesystem>

//旧_方法（可能会有点混乱）
void bf_ckdh();
wstring szzzf(int num);
int zfzsz(wstring& str, int len);
void gxlj2(wstring& lj, int& num);
bool pdlj(wstring& lj);
void csh_ckmb();								//初始化抽卡目标
void scjg(int jg_s);							//生成结果
void szztys();									//设置字体样式

//基础_方法
int GetRandomNumber(int min, int max);				//参数：最小值, 最大值
bool jc_wjsfcz();									//检测_文件是否存在
wstring utf8_to_wstring(const std::string& str);	//将GBK编码字符串，转为UTF-8编码字符串，参数：GBK编码字符串，返回值：UTF-8编码字符串

//音频_方法
void Play_audio(const wstring& file_path, const wstring& alias);
void Turn_off_audio(const wstring& alias);


//图片路径
wstring ckdh_lj = L"gacha-anim/ckdh1.png";
wstring jmdh_lj = L"ui-anim/jmdh1.png";
wstring wzbj_lj = L"assets/images.png";

//音频路径
wstring Audio_ckjmy = L"audio/抽卡界面音.mp3";
wstring Audio_cky = L"audio/抽卡音.mp3";
wstring Audio_c3x = L"audio/出3星音效.mp3";
wstring Audio_c5x = L"audio/出5星音效.mp3.mp3";

//其他路径
wstring ckmb_lj = L"assets/data.txt";			//数据
wstring ckdh_temp_lj = L"gacha-anim/ckdh1.png";	//临时路径
wstring temp_text_path = L"temp/logs/temp.txt";	//临时文本

//音频设备
wstring mysong =  L"mysong";


//其他数据
int sl = 1;
int ck_c = 1720;
int ck_k = 930;
int zl = 15;									//帧率（全屏模式时，只需延迟一秒，这里图片张数似乎有问题）
wstring ckmb[255];
int ckmb_sl = 0;								//抽卡目标数量

int zt_x = 520;									//字体打印位置的坐标
int zt_y = 270;
int zt_gd = 330;								//字体高度
int zt_kd = 0;									//每个字符的宽度

int sl_cqcs = 4;								//十连抽的抽取次数
IMAGE wzbj;										//文字的背景图

namespace fs = std::filesystem;			//将fs设置为命名空间filesystem
string lssj_ml = "temp/logs";			//临时数据目录

int main()
{
	//在当前目录下，创建一个临时数据目录，如果目录存在会自动跳过
	fs::create_directories(lssj_ml);

	cout << "关闭该窗口以停止程序运行" << endl;
	//cout << "\"祈愿10次\"的抽取次数：" << endl;
	//cin >> sl_cqcs;

	//播放抽卡界面音
	Play_audio(Audio_ckjmy, mysong);

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
					Turn_off_audio(mysong);				//关闭音频

					temp_sl = sl;						//记录一下当前数量
					sl = 1;
					bf_ckdh();							//播放抽卡动画

					Turn_off_audio(mysong);				//关闭音频

					if (ckmb_sl == 0)
					{
						Sleep(1000);					//由于目前没有抽卡后的结果素材，所以播放完抽卡后延时1秒
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
					Play_audio(Audio_ckjmy, mysong);
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
			Play_audio(Audio_cky, mysong);
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
void gxlj2(wstring& lj, int& num)						//参数：路径，当前编号
{
	wstring temp;
	int wz = lj.rfind(L".");							//获取路径中，最后一个点的位置
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
int zfzsz(wstring& str, int len)			//字符转数字（由于数组传进函数后，长度固定为8，无法靠str.size()在函数里获取长度，所以长度需要传进来）
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
wstring szzzf(int num)
{
	wstring sz;
	wfstream fs;

	fs.open(temp_text_path, ios::out);
	fs << num << endl;					//将数字传到文件中
	fs.close();

	fs.open(temp_text_path, ios::in);
	getline(fs, sz);					//再将文件中的内容读回字符串中
	fs.close();

	return sz;
}
bool pdlj(wstring& lj)							//判断文件（路径）是否可以打开
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
	string line_utf8;
		
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

	//读取抽卡目标
	for (int i = 0;i < 255;i++)				//将文件中的信息，传入抽卡目标数组中
	{
		if (getline(ifs, line_utf8))
		{
			// 将 UTF-8 的 string 转换为 wstring
			wstring line_wide = utf8_to_wstring(line_utf8);
			ckmb[i] = line_wide;
			ckmb_sl++;						//抽卡目标数量++			
		}
		else								//如果没有内容了
		{
			break;							//退出循环
		}
		
	}
	ifs.close();
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
		sjs = GetRandomNumber(0, ckmb_sl - 1);	//生成一个随机数，生成范围：0~抽卡目标数量-1（0~3）

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
		Play_audio(Audio_c3x, mysong);

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
					Turn_off_audio(mysong);				//关闭音频

					break;						//则退出，打印下一个字符串
				}
			}
			if (pd == false)					//如果音频没有播放完，则延时（如果已经播放完音频则不要延时）
			{
				Sleep(dc_pdsj);
				djs += dc_pdsj;
				if (djs >= 2000 - ys_sj && pd == false)
				{
					//Turn_off_audio(mysong);				//关闭音频已取消音频循环，不需要再用计时方式关闭）
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
						Turn_off_audio(mysong);				//关闭音频
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
					Turn_off_audio(mysong);				//关闭音频
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

	settextstyle(zt_gd, zt_kd, L"1", 0, 0, 0, false, false, false);
}

int GetRandomNumber(int min, int max)		//参数：最小值, 最大值
{
	/*/
	// 1. 静态设备：只构造一次，用于获取系统真随机种子
	static std::random_device rd;
	// 2. 静态引擎：只初始化一次，之后状态持续向前推进
	static std::mt19937 gen(rd());
	// 3. 静态分布：只定义一次，范围 1~10000
	static std::uniform_int_distribution<int> dist(1, 10000);
	//*/

	// 1. 引擎（含种子）：整个程序只播种一次，并且不长期占用 random_device 资源
	static std::mt19937 gen(std::random_device{}());

	// 2. 分布规则：不用 static，每次根据传入的参数动态创建（极轻量，无性能负担）
	std::uniform_int_distribution<int> dist(min, max);

	// 每次调用只执行这一句，返回不同的数
	return dist(gen);
}
bool jc_wjsfcz()
{
	ifstream ifs;
	bool sf_czwj = true;		//是否_存在文件

	//1.抽卡动画_图片
	ifs.open(ckdh_lj, ios::in);
	if (!ifs.is_open())
	{
		wcout << "缺少文件：" << ckdh_lj << endl;
		sf_czwj = false;
	}

	//2.界面动画_图片
	ifs.close();
	ifs.open(jmdh_lj, ios::in);
	if (!ifs.is_open())
	{
		wcout << "缺少文件：" << jmdh_lj << endl;
		sf_czwj = false;
	}

	//3.抽卡目标_数据
	ifs.close();
	ifs.open(ckmb_lj, ios::in);
	if (!ifs.is_open())
	{
		wcout << "缺少文件：" << ckmb_lj << endl;
		sf_czwj = false;
	}

	//4.文字背景_图片
	ifs.close();
	ifs.open(wzbj_lj, ios::in);
	if (!ifs.is_open())
	{
		wcout << "缺少文件：" << wzbj_lj << endl;
		sf_czwj = false;
	}

	//5.音频资源_音频
	ifs.close();
	ifs.open(Audio_ckjmy, ios::in);
	if (!ifs.is_open())
	{
		wcout << "缺少文件：" << Audio_ckjmy << endl;
		sf_czwj = false;
	}

	//如果不存在文件，则返回false
	if (sf_czwj == false)
	{
		return false;
	}

	ifs.close();
	return true;					//存在文件，返回true
}
wstring utf8_to_wstring(const std::string& str)
{
	if (str.empty()) return std::wstring();
	// 1. 计算转换后需要的宽字符缓冲区大小
	int wsize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	if (wsize <= 0) return std::wstring();
	// 2. 进行转换
	std::wstring wstr(wsize - 1, 0); // -1 是因为 MultiByteToWideChar 返回的大小包含 L'\0'
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], wsize);
	return wstr;
}

void Play_audio(const wstring& file_path, const wstring& alias)
{
    wstring close_cmd = L"close " + alias;
	wstring open_cmd = L"open " + file_path + L" alias " + alias;
	wstring play_cmd = L"play " + alias;

	// 1.关闭旧音频
    mciSendString(close_cmd.c_str(), NULL, 0, NULL);

    // 2.打开音频
    mciSendString(open_cmd.c_str(), NULL, 0, NULL);

    // 3.播放音频
    mciSendString(play_cmd.c_str(), NULL, 0, NULL);
}
void Turn_off_audio(const wstring& alias)
{
	std::wstring close_cmd = L"close " + alias;
	mciSendString(close_cmd.c_str(), NULL, 0, NULL);
}
