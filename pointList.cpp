#include "point.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <ncurses.h>
using namespace std;
	
class pointList
{
	public:
		static const int maxX = 41;
		static const int maxY = 21;
		static const int RIGHT = 261;
		static const int LEFT = 260;
		static const int UP = 259;
		static const int DOWN = 258;
		int size;//list �ִ� ũ�� 
		vector<point> list;
		pointList(int size):size(size){};
		//���� �Լ� 
		void pushLast(point p);
		void pushFront(point p);
		//���� �Լ�
		point popLast();
		point popFront();
		void remove(point p);
		//Ž�� �Լ�
		point at(int n); 
		//���� ���� �Լ�
		void createRandom(pointList* p1 = NULL, pointList* p2 = NULL, pointList* p3 = NULL);
		//��ȿ�� �˻� �Լ�
		bool isFull(){return list.size()>=size;}
		bool isEmpty(){return list.size()==0;}
};

void pointList::pushLast(point p){ // list�� �������� ������ ���� 
	list.push_back(p);
}

void pointList::pushFront(point p){ // list�� �������� ������ ���� 
	list.insert(list.begin(), p);
}

point pointList::popLast(){ // list�� ������ ������ ���� / ��ȯ 
	if(list.size()==0){
		printf("segFault : list is empty!");
		exit(0);
	} 
	point ret = list.back();
	list.pop_back();
	return ret;
}

point pointList::popFront(){ // list�� �� �� ������ ���� / ��ȯ 
	if(list.size()==0){
		printf("segFault : list is empty!");
		exit(0);
	} 
	point ret = list.front();
	for(int i=1; i<list.size(); i++) list[i] = list[i-1];
	list.pop_back();
	return ret;
}

point pointList::at(int n){ // list�� n��° ������ ��ȯ 
	return list[n];
}

void pointList::remove(point p){ // list���� p�� ã�Ƽ� ���� 
	for(int i=0; i<list.size(); i++){
		if(list[i]==p){
			for(int j=i+1; j<list.size(); j++){
				list[i] = list[j];
			}
			list.pop_back();
		}
	}
}

void pointList::createRandom(pointList* p1, pointList* p2, pointList* p3){ // p1, p2, p3�� �ߺ��Ǵ� ��ǥ�� ���� �ʴ� ���� ��ǥ ���� + list�� �߰� 
	if(isFull()){
		return;
	}
	//���� ���� + �߰�
	point newPoint(rand()%maxX, rand()%maxY);
	while(newPoint.outOfRange(maxX, maxY) || 
	(p1!=NULL && newPoint.intersect(p1->list)) || 
	(p2!=NULL && newPoint.intersect(p2->list)) || 
	(p3!=NULL && newPoint.intersect(p3->list)) || 
	(newPoint.intersect(this->list))){
		newPoint.setpoint(rand()%maxX, rand()%maxY);
	}
	this->pushLast(newPoint);
}
/***************************************************************************************************************************************************************************************/

class portal:public pointList{
	public:
		portal():pointList(2){}
		void createPortal(pointList* p1 = NULL, pointList* p2 = NULL, pointList* p3 = NULL);
		void clear(){
			while(list.size() > 0){
				list.pop_back();
			}
		}
		point teleport(point p, int dir);
};

void portal::createPortal(pointList* p1, pointList* p2, pointList* p3){
	if(isFull()){
		return;
	}
	//���� ���� + �߰�
	point newPoint(rand()%maxX, rand()%maxY);
	while(newPoint.outOfRange(maxX, maxY) || 
	(p1!=NULL && newPoint.intersect(p1->list)) || 
	(p2!=NULL && newPoint.intersect(p2->list)) || 
	(p3!=NULL && newPoint.intersect(p3->list)) ||
	(newPoint.isEdge(maxX-2, maxY-2) == false ) ||
	(newPoint.intersect(this->list))){
		newPoint.setpoint(rand()%maxX, rand()%maxY);
	}
	//list.push_back(newPoint);
	this->pushLast(newPoint);
}

point portal::teleport(point p, int dir){
	point wayOut = p==list[0] ? list[1]:list[0];
	int d[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
	point res(wayOut.x + d[dir-258][0], wayOut.y + d[dir-258][1], dir);
	if(res.outOfRange(maxX-2, maxY-2)){
		if(wayOut.x == maxX-2){
			res.setpoint(wayOut.x + d[3][0], wayOut.y + d[3][1], LEFT);
		}else if(wayOut.y == maxY-2){
			res.setpoint(wayOut.x + d[1][0], wayOut.y + d[1][1], DOWN);
		}else if(wayOut.x==1){
			res.setpoint(wayOut.x + d[2][0], wayOut.y + d[2][1], RIGHT);
		}else if(wayOut.y==1){
			res.setpoint(wayOut.x + d[0][0], wayOut.y + d[0][1], UP);
		}
	}
	return res;
}

/***************************************************************************************************************************************************************************************/
class item:public pointList{
	public:
		item():pointList(3){}
		void createItem(int times, pointList* p1=NULL, pointList* p2=NULL, pointList* p3=NULL);
};

void item::createItem(int times, pointList* p1, pointList* p2, pointList* p3){
	if(times%10!=0) return;
	if(this->list.size()>=3) return;
	createRandom(p1, p2, p3);
}
