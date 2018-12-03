#ifndef ESTIMATOR_HPP
#define ESTIMATOR_HPP

#include <vector>
#include "Singleton.hpp"

using namespace std;

typedef pair<short,short> pss;

struct pixel {
	unsigned char r, g, b, a;
};

class Estimator : public Singleton<Estimator>
{
private:
	const int DY[8] = { -1, -1, -1, 0, 1, 1, 1, 0 }, DX[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
	unsigned int DEFAULT_SEA_LEVEL = 64;
	
	vector < vector < unsigned short > > HmapData;
	vector < vector < pixel > > BmapData;
	vector < vector < pss > > descentTable;
	int height;
	int width;
	unsigned int blendMapTexture;
public:
	Estimator() = default;
	Estimator(vector<unsigned short>& data, int _height, int _width);

	// methods
public:
	void blendmapColoring();
	// descentTabling�� �����Ͽ� ���� Ÿ�ϰ��� �����ϴ� �Լ�

	void dumpDescentMapData();

	vector <unsigned short> getHeightMap();
	vector <unsigned char> getBlendMap();
	//HeightMap / BlendMap�� png ����� ���� vector of unsigned short�� ��ȯ�ϴ� �Լ�

	void dumpHeightMapData();
	// HeightMap�� stdout�� ���

	void dumpBlendMapData();
	// BlendMap�� stdout�� ���

	void initHMapData(unsigned int texture, int _width, int _height);
	void generateHeightMap(const char* path, int _width, int _height);
	void generateBlendMap(const char* path, int width, int height);
	unsigned int getBlendMapTexture(void);
	void normalize(int minimumHeight = 0, int maximumHeight = 512);
	// minimumHeight ~ maximumHeight ������ ������
	void smoothness();
private:
	pss descent(short y, short x);
	// ��(y,x)�� ���ؼ� gradient descending�� �����ϴ� �Լ�

	void descentTabling();
	// ��� ���� ���ؼ� gradient descending�� �����ϰ� ������ ������ ��ȯ�ϴ� �Լ�

	pixel randFill(int dryDistance, short y, short x);
	// descentTabling(); �Լ��� ���� Local Minima(���� ���̴� ��)���� �Ÿ� ���� ������ �μ��� �޾Ƽ� ���� Ÿ���� �����ϴ� �Լ�(���� ���� ����)
	void bfsCoastlineOptimization();
	void linearCoastlineOptimization();
};


#endif