#include "ai.h"

void init_ai(){
	brd = new board();
	//get_komi();
}


int pass_move(int i, int j)
{
  return i == -1 && j == -1;
}

int legal_move(int i, int j, int color)
{
  int other = OTHER_COLOR(color);
  
  /* Pass is always legal. */
  if (pass_move(i, j))
    return 1;
  ++i; ++j;
  /* Already occupied. */
  if (brd->get_cell(i, j) != empty)
    return 0;

  /* Illegal ko recapture. It is not illegal to fill the ko so we must
   * check the color of at least one neighbor.
   */
  if (i == ko_i && j == ko_j
      && ((on_board(i - 1, j) && brd->get_cell(i - 1, j) == other)
	  || (on_board(i + 1, j) && brd->get_cell(i + 1, j) == other)))
    return 0;

  return 1;
}

int valid_fixed_handicap(int handicap)
{
  if (handicap < 2 || handicap > 9)
    return 0;
  if (board_size % 2 == 0 && handicap > 4)
    return 0;
  if (board_size == 7 && handicap > 4)
    return 0;
  if (board_size < 7 && handicap > 0)
    return 0;
  
  return 1;
}

void place_fixed_handicap(int handicap)
{
  int low = board_size >= 13 ? 3 : 2;
  int mid = board_size / 2;
  int high = board_size - 1 - low;

  ++low;++mid;++high;
  
  if (handicap >= 2) {
    play_move(high, low, black);   /* bottom left corner */
    play_move(low, high, black);   /* top right corner */
  }
  
  if (handicap >= 3)
    play_move(low, low, black);    /* top left corner */
  
  if (handicap >= 4)
    play_move(high, high, black);  /* bottom right corner */
  
  if (handicap >= 5 && handicap % 2 == 1)
    play_move(mid, mid, black);    /* tengen */
  
  if (handicap >= 6) {
    play_move(mid, low, black);    /* left edge */
    play_move(mid, high, black);   /* right edge */
  }
  
  if (handicap >= 8) {
    play_move(low, mid, black);    /* top edge */
    play_move(high, mid, black);   /* bottom edge */
  }
}

void place_free_handicap(int handicap)
{
  int k;
  int i, j;
  
  for (k = 0; k < handicap; k++) {
    generate_move(&i, &j, black);
    play_move(i, j, black);
  }
}

void generate_move(int *i, int *j, int color)
{
	int moves[SIZE * SIZE];
	int num_moves = 0;
	int move;
	int ai, aj;
	int k;

	//need interpretation

	i--;j--;
}

void play_move(int i, int j, int color)
{
	int captured_stones = 0;
	int k;

	/* Reset the ko point. */
	ko_i = -1;
	ko_j = -1;
	i++;j++;

	//need interpretation
}

static int get_final_status(int i, int j)
{
	return brd->get_final_status(i+1, j+1);
}

void compute_final_status(void){
	int i, j;
	int pos;
	int k;

	for (i = 0; i < board_size; i++)
		for (j = 0; j < board_size; j++)
			set_final_status(i,j,UNKNOWN);
  
	for (i = 0; i < board_size; i++)
		for (j = 0; j < board_size; j++)
			if (get_board(i, j) == EMPTY)
				for (k = 0; k < 4; k++) {
					int ai = i + offset_x[k];
					int aj = j + offset_y[k];
					if (!on_board(ai, aj))
						continue;
					/* When the game is finished, we know for sure that (ai, aj)
					* contains a stone. The move generation algorithm would
					* never leave two adjacent empty vertices. Check the number
					* of liberties to decide its status, unless it's known
					* already.
					*
					* If we should be called in a non-final position, just make
					* sure we don't call set_final_status_string() on an empty
					* vertex.
					*/
					if (get_final_status(ai, aj) == UNKNOWN) {
						if (get_board(ai, aj) != EMPTY) {
							if (find_liberty(ai, aj) >= 2)
								set_final_status(ai, aj, ALIVE);
							else
								set_final_status(ai, aj, DEAD);
						}
					}
					/* Set the final status of the (i, j) vertex to either black
					* or white territory.
					*/
					if (get_final_status(i, j) == UNKNOWN) {
						if ((get_final_status(ai,aj) == ALIVE) ^ (get_board(ai, aj) == WHITE))
							set_final_status(i, j, BLACK_TERRITORY);
						else
							set_final_status(i, j, WHITE_TERRITORY);
					}
				}
}