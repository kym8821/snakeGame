#include <vector>
using namespace std;
	
class point
{
	const static int RIHGT = 261;
	const static int LEFT = 260;
	const static int UP = 259;
	const static int DOWN = 258;
	public:
		int x;
		int y;
		int dir;
		point(int x, int y):x(x), y(y){}
		point(int x, int y, int dir):x(x), y(y), dir(dir){}
		void setpoint(const point p);
		void setpoint(int x, int y, int dir=LEFT);
		bool operator==(const point& p) const;
		bool intersect(vector<point> v);
		bool outOfRange(const int maxX, const int maxY);
		bool isEdge(const int maxX, const int maxY);
};

void point::setpoint(const point p){
	setpoint(p.x, p.y, p.dir);	
}

void point::setpoint(int _x, int _y, int _dir){
	x = _x;
	y = _y;
	dir = _dir;
}

bool point::operator==(const point& p) const{
	return x==p.x&&y==p.y;
}

bool point::intersect(vector<point> v){
	//cout<<"intersect"<<endl;
	for(int i=0; i<v.size(); i++){
		if((*this)==v[i]) return true;
	}
	return false;
}

bool point::outOfRange(const int maxX, const int maxY){
	return (this->x <= 0 || this->y <= 0 || this->x > maxX-2 || this->y > maxY-2);
}

bool point::isEdge(const int maxX, const int maxY){
	if((x==1 && y!=1) && (x!=1 && y==1)) return true;
	if(x==maxX && y!=maxY) return true;
	if(y==maxY && x!=maxY) return true;
	return false;
}
