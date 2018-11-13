#ifndef ESTIMATOR_HPP
#define ESTIMATOR_HPP

#include <vector>
#include "Singleton.hpp"

using namespace std;

typedef pair<int,int> pii;

struct pixel {
	unsigned char r, g, b, a;
};

class Estimator : public Singleton<Estimator>
{
private:
	const int TERRAIN_SINK_DIVIDER = 4;
	const int DY[8] = { -1, -1, -1, 0, 1, 1, 1, 0 }, DX[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
	
	vector < vector < unsigned char > > HmapData;
	vector < vector < pixel > > BmapData;
	vector < vector < pii > > descentTable;
	int height;
	int width;

public:
	Estimator() = default;
	Estimator(vector<unsigned char>& data, int _height, int _width);

	// methods
public:
	void blendmapColoring();
	// descentTabling�� �����Ͽ� ���� Ÿ�ϰ��� �����ϴ� �Լ�

	void dumpDescentMapData();

	vector <unsigned char> getHeightMap();
	vector <unsigned char> getBlendMap();
	//HeightMap / BlendMap�� png ����� ���� vector of unsigned char�� ��ȯ�ϴ� �Լ�

	void dumpHeightMapData();
	// HeightMap�� stdout�� ���

	void dumpBlendMapData();
	// BlendMap�� stdout�� ���

	void initHMapData(unsigned int texture, int _width, int _height);
	void generateBlendMap(const char* path, int width, int height);
private:
	pii descent(int y, int x);
	// ��(y,x)�� ���ؼ� gradient descending�� �����ϴ� �Լ�

	int descentTabling();
	// ��� ���� ���ؼ� gradient descending�� �����ϰ� ������ ������ ��ȯ�ϴ� �Լ�

	pixel randFill(int areaHeight, int wetDistance, int wetHeightGap, int y, int x);
	// descentTabling(); �Լ��� ���� Local Minima(���� ���̴� ��)���� �Ÿ� ���� ������ �μ��� �޾Ƽ� ���� Ÿ���� �����ϴ� �Լ�(���� ���� ����)
};


#endif