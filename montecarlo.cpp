#include "montecarlo.h"
#include "board.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

using namespace std;

montecarlo::montecarlo(board &inBoard,bool bplayer) {
	getInitBoard(inBoard);
	player = !bplayer;
	run();
}

montecarlo::~montecarlo() {}

void montecarlo::getInitBoard(board &inBoard) {
	currentBoard = inBoard;
}

void montecarlo::run() {
	int fault = 0;
	bool flag=true;
	//printf("random: ");
	int triedtimes=0;
	while (flag) {
		bool walked = false;
		bool flag1=false;
		bool flag2=false;
		bool flag3=false;

		set<short>* ataripositionalley = player?(&currentBoard.ataripositionforblack):(&currentBoard.ataripositionforwhite);
		set<short>* validsetalley = player?(&currentBoard.validsetforblack):(&currentBoard.validsetforwhite);
		set<short>* ataripositionenemy = (!player)?(&currentBoard.ataripositionforblack):(&currentBoard.ataripositionforwhite);
		set<short>* validsetenemy = !player?(&currentBoard.validsetforblack):(&currentBoard.validsetforwhite);
		set<short>::iterator iter;
		while (triedtimes<=300&&!ataripositionalley->empty()){
			triedtimes++;
			iter = ataripositionalley->begin();
			int randomNumber = rand() * ataripositionalley->size() / (RAND_MAX + 1);
			for (int i=0;i<randomNumber;i++)
				iter++;
			int coordX = (*iter-1) / BOARDSIZE + 1;
			int coordY = (*iter-1) % BOARDSIZE + 1;
			if (currentBoard.play(player, coordX, coordY)) {
				player = !player;
				walked = true;
				flag1=true;
				break;
			}
			ataripositionalley->erase(*iter);//这里没有考虑周全
		}
		while (triedtimes<=300&&!walked&&!ataripositionenemy->empty()){
			triedtimes++;
			iter = ataripositionenemy->begin();
			int randomNumber = rand() * ataripositionenemy->size() / (RAND_MAX + 1);
			for (int i=0;i<randomNumber;i++)
				iter++;
			int coordX = (*iter-1) / BOARDSIZE + 1;
			int coordY = (*iter-1) % BOARDSIZE + 1;
			if (currentBoard.play(player, coordX, coordY)) {
				player = !player;
				walked = true;
				flag2=true;
				break;
			}
			ataripositionenemy->erase(*iter);
		}
		while (triedtimes<=300&&!walked&&!validsetalley->empty()){
			triedtimes++;
			iter = validsetalley->begin();
			int randomNumber = rand() * validsetalley->size() / (RAND_MAX + 1);
			for (int i=0;i<randomNumber;i++)
				iter++;
			int coordX = (*iter-1) / BOARDSIZE + 1;
			int coordY = (*iter-1) % BOARDSIZE + 1;
			if (currentBoard.play(player, coordX, coordY)) {
				player = !player;
				walked = true;
				flag3=true;
				break;
			}
			validsetalley->erase(*iter);
		}
		if (!flag1&&!flag2&&!flag3){
			player = !player;
			walked = true;
		}
		if (triedtimes>300)
			flag=false;
		if (validsetalley->empty()&&validsetenemy->empty()&&ataripositionalley->empty()&&ataripositionenemy->empty()){
			flag=false;
//  			currentBoard.showboard();
// 			printf("%d\n",triedtimes);
//  			system("pause");
		}
// 		currentBoard.showboard();
// 		for (iter=ataripositionalley->begin();iter!=ataripositionalley->end();iter++)
// 			printf("%d ",*iter);
// 		printf("\n");
// 		for (iter=ataripositionenemy->begin();iter!=ataripositionenemy->end();iter++)
// 			printf("%d ",*iter);
// 		printf("\n");
// 		for (iter=validsetalley->begin();iter!=validsetalley->end();iter++)
// 			printf("%d ",*iter);
// 		printf("\n");
// 		for (iter=validsetenemy->begin();iter!=validsetenemy->end();iter++)
// 			printf("%d ",*iter);
// 		printf("\n");
// 
// 		system("pause");
	}
	//printf("\n");
	winner = currentBoard.judge()>0?1:0;
}

bool montecarlo::getWinner() {
	return winner;
}