#include <gl/glut.h>
#include <windows.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

const int N = 1e3 + 4;
const float esp = 1e-5;


typedef struct ActiveEdge {
	float x, ymx;
	float k;

	bool operator<(const ActiveEdge t)
	{
		if (fabs(x - t.x) > esp) return x < t.x;
		return k < t.k;
	}
}AE;//有效边
struct Point {
	float x, y;
};
Point point[N]; // 存放点的信息按顺时针输入
vector<AE> v[N];
bool check(int a, int b, int c) // 判断顶点交点记为1
{
	float xa = point[a].x, ya = point[a].y;
	float xb = point[b].x, yb = point[b].y;
	float xc = point[c].x, yc = point[c].y;
	if (yc > yb && yb > ya) return true;
	return false;
}
int cnt;
void disPlay()
{
	cnt++;
	int n;
	cin >> n;
	float mn = 1e9, mx = 0; // 找到上下边界
	float mnn = 1e9, mnn1 = 1e9;
	for (int i = 0; i < n; i++)
	{
		cin >> point[i].x >> point[i].y;
		mnn = min(mnn, point[i].x);
		mnn1 = min(mnn1, point[i].y);
		mn = min(mn, point[i].y);
		mx = max(mx, point[i].y);
	}
	if (mnn < 0)
	{
		for (int i = 0; i < n; i++) point[i].x -= mnn, point[i].x += 100;
		
	}
	if (mnn1 < 0)
	{
		for (int i = 0; i < n; i++) point[i].y -= mnn1, point[i].y += 100;
		mn -= mnn1, mn += 100;
		mx -= mnn1, mx += 100;
	}
	glClear(GL_COLOR_BUFFER_BIT); //完成了把整个窗口清除为黑色的任务
	glColor3f(1.0, 0.0, 0.0); // 设置颜色
	glBegin(GL_POINTS); //把每个顶点作为一个点进行处理
	//for (int i = 0; i < n; i++) cout << point[i].x << " " << point[i].y << endl;
	
	for (int i = mn; i <= mx; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (point[j].y == i)
			{
				int cnt = -1;
				float xa = point[j].x, ya = point[j].y;
				AE ae[2];
				if (point[(j - 1 + n) % n].y > point[j].y)
				{
					cnt = 0; // 说明第一个点在该点的上方
					float xb = point[(j - 1 + n) % n].x, yb = point[(j - 1 + n) % n].y;
					//cout << i << " " << xa << " " << ya << endl;
					//if(fabs(yb - ya) == 0)
					ae[0] = { xa,yb,(xb - xa)/(yb - ya)   };
				}
				if (point[(j + 1 + n) % n].y > point[j].y)
				{
					if (cnt == 0) cnt = 2; // 说明都在该点的上方
					else cnt = 1; // 说明第二个点在该点的上方
					float xb = point[(j + 1 + n) % n].x, yb = point[(j + 1 + n) % n].y;
					ae[1] = { xa,yb,(xb - xa) / (yb - ya) };
				}
				if (cnt == 0) {
					if (check(j, (j - 1 + n) % n, (j - 2 + n) % n)) {// 如果顶点交点记为1
						ae[0].ymx -= 1;
					}
					v[i].push_back(ae[0]);
				}
				else if (cnt == 1) {
					if (check(j, (j + 1 + n) % n, (j + 2 + n) % n)) {// 如果顶点交点记为1
						ae[1].ymx -= 1;
					}
					v[i].push_back(ae[1]);
				}else if (cnt == 2) {
					if (check(j, (j + 1 + n) % n, (j + 2 + n) % n)) {// 如果顶点交点记为1
						ae[1].ymx -= 1;
					}
					if (check(j, (j - 1 + n) % n, (j - 2 + n) % n)) {// 如果顶点交点记为1
						ae[0].ymx -= 1;
					}
					v[i].push_back(ae[0]);
					v[i].push_back(ae[1]);
				}
			}
		}
	}
	for (int i = mn; i <= mx; i++)
	{
		if ((int)v[i].size() > 0) sort(v[i].begin(), v[i].end()); // 对边表进行排序
	}


	vector<AE> Aet;
	for (int i = mn; i <= mx; i++)
	{
		if (v[i].size() > 0)
		{
			for (auto &t : v[i]) Aet.push_back(t);
		}
		sort(Aet.begin(), Aet.end());
		//cout << "有效边表中点的数量：" << Aet.size() << endl;
		vector<AE> last;
		if(i & 1)glColor3f(1.0, 0.0, 0.0); // 设置颜色
		else glColor3f(0.0, 1.0, 0.0);
		for (int j = 0; j < Aet.size(); j += 2) // 从头循环有效边表
		{
			auto t = Aet[j], t1 = Aet[j + 1];
			//cout << "划线范围：" << t.x << " " << t1.x << endl;
			for (float k = t.x; k <= t1.x; k += 1)
				glVertex2i(static_cast<int>(k), i);
			//cout << "y扫描线：" << i << endl;
			//cout << "ymax1 " << t.ymx << " ymx2 " << t1.ymx << endl;
			//cout << endl;
			if (t.ymx != i) {
				t.x += t.k;
				last.push_back(t);
			}
			if (t1.ymx != i) {
				t1.x += t1.k;
				last.push_back(t1);
			}
		}
		Aet.clear();
		for (auto &t : last) {
			Aet.push_back(t);
		}
	}
	glEnd();
	glFlush();//用于强制刷新缓冲，保证绘图命令将被执行
}
void main(int argc, char* argv) {
	glutInit(&argc, &argv);//窗口的初始化
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//窗口模式的设定
	glutInitWindowPosition(200, 400);//窗口位置的设定
	glutInitWindowSize(500, 400);//窗口大小的设定
	glutCreateWindow("Y连贯性算法");// 创建窗口
	glClearColor(1.0, 1.0, 1.0, 0.0);//函数指定颜色缓冲区的清除值，颜色缓冲区的清除值就是窗口的背景颜色，已经归一化

	gluOrtho2D(0.0, 600.0, 0.0, 450.0);//窗口的左下角是(0,0);坐标范围是 X:0~WIDTH,Y:0~HEIGHT
	glutDisplayFunc(disPlay); 
	glutMainLoop();
	//调用函数
	//使程序进入事件处理循环
}

/*
5
100 200
200 140
300 160
220 100
120 150

漏斗
4
30 30
90 90
90 30
30 90
正方形
4
-100 -100
-100 -200
-200 -200
-200 -100



13
50 50
70 50
70 70
50 70
30 30
90 30
90 90
30 90
50 70
30 30
30 90
50 70
50 50
*/