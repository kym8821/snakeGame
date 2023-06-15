#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include "pointList.cpp"
using namespace std;

inline void init(){
	initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    srand(time(NULL));
}

inline void createWindow(WINDOW* win){
	box(win, 0, 0);
	wrefresh(win);	
}

pointList snake(10);
item snake_minus;
item snake_plus;
portal snake_portal;

void printElement(WINDOW* win, pointList* pl, const char* sign){
	for(int i=0; i<pl->list.size(); i++){
		point p = pl->list[i];
		mvwprintw(win, p.y, p.x, sign);
	}
}

void portalMaker(int times){
	if(times == 20){
		/* portal */
		snake_portal.clear();
		snake_portal.createPortal(&snake, &snake_minus, &snake_plus);
		snake_portal.createPortal(&snake, &snake_minus, &snake_plus);
	}	
}

void createElement(WINDOW* win){
	/* snake */
    point start(pointList::maxX/2 + 2,pointList::maxY/2);
	for(int i=0; i<3; i++){
    	snake.pushLast(start);
		start.setpoint(start.x-1, start.y);	
	}
	
	/*print_init*/
	mvprintw(0, 0, "+: %d", snake.list.size());
    printElement(win, &snake, "o");
	printElement(win, &snake_minus, "-");
	printElement(win, &snake_plus, "+");
	printElement(win, &snake_portal, "@");
	mvprintw(0, 0, "+: %d", 3);
	createWindow(win);
}

int main(void){
	init(); 
	WINDOW* win = newwin(pointList::maxY, pointList::maxX, 1, 1);
	bool same_direction = false, gameOver = false;
	int order = KEY_RIGHT, last_order;
	int score = 3, times = 0;
	createElement(win);
	bool onPortal;
	while(!gameOver && snake.list.size() >= 3){
		/*틱 처리*/ 
		if(!same_direction){ // 이전 입력과 다른 방향이면 0.5초 틱처리 
			usleep(300000);
			times+=1;
		}else{ // 이전 입력과 같은 방향이면 틱처리 없애기 
			same_direction = false;	
		}
		
		/*입력값 처리*/ 
		last_order = order;
        order = getch();
        if(order == last_order){
        	same_direction = true;
        	continue;
		}if(order == ERR){
			order = last_order;
		}
		 
		// 헤드(뱀 머리) 값 변경
		point head = snake.at(0); 
        head.dir = order;
		switch(order) {
            case KEY_UP:
                head.y--;
                break;
            case KEY_DOWN:
                head.y++;
                break;
            case KEY_LEFT:
                head.x--;
                break;
            case KEY_RIGHT:
                head.x++;
                break;
            case 'q':
            	gameOver = true;
                break;
        }
        
        //out of range error
        if(head.outOfRange(pointList::maxX, pointList::maxY)){
        	break;
		}
		
        if(head.intersect(snake_portal.list)){
			head = snake_portal.teleport(head, order);
			order=head.dir;
			last_order = order;
			onPortal = true;
		}else{
			onPortal = false;
		}
		mvprintw(0, 10, "%2d %2d %3d", head.x, head.y, head.dir);
		// 뱀 몸통 충돌처리
		if(head.intersect(snake.list)){
			break;
		}
		
		// 뱀 이동과 '+' 아이템 처리 
		snake.pushFront(head); 
		if(head.intersect(snake_plus.list)){
			snake_plus.remove(head);
			score++;
		}else{
        	point del = snake.popLast();
        	mvwprintw(win, del.y, del.x, " ");	
		}
        
        // '-' 아이템 처리 
        if(head.intersect(snake_minus.list)){
        	snake_minus.remove(head);
        	point del = snake.popLast();
        	mvwprintw(win, del.y, del.x, " ");
        	score--;
		}
		
		portalMaker(times);
		snake_plus.createItem(times,&snake_portal,&snake,&snake_minus);
		snake_minus.createItem(times,&snake_portal,&snake,&snake_plus);
		mvprintw(0, 0, "+: %d %d", score, snake_plus.list.size());
        printElement(win, &snake, "o");
		printElement(win, &snake_minus, "-");
		printElement(win, &snake_plus, "+");
		printElement(win, &snake_portal, "@");
		createWindow(win);
    }
	endwin();
	return 0;
}
