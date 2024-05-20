#include "geesespotter_lib.h"
#include <iostream>

char *create_board(std::size_t x_dim, std::size_t y_dim) {
  char *board = new char[x_dim * y_dim]{};
  int k{0};
  for (k = 0; k < x_dim * y_dim; k++) {
    board[k] = 0;
  }
  return board;
}

void clean_board(char *board) {
  delete[] board;
  board = nullptr;
}

void print_board(char *board, std::size_t x_dim, std::size_t y_dim) {
  // for(all elements)
  int Q{0};
  for (Q = 0; Q < x_dim * y_dim; Q++) {

    //   if marked
    if (marked_mask() & board[Q]) {
      std::cout << "M";

    } //   if hidden ---
    else if (hidden_mask() & board[Q]) {
      std::cout << "*";

    }
    //   if revealed ---
    else {
      std::cout << int(board[Q]);
    }
    if (Q % x_dim == (x_dim - 1)) {
      std::cout << std::endl;
    }
  }
}

void hide_board(char *board, std::size_t x_dim, std::size_t y_dim) {
  // for(all elements)
  // hide (set hidden bit to hidden)
  int L = 0;
  for (L = 0; L < x_dim * y_dim; L++) {
    board[L] |= hidden_mask();
  }
}

int mark(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc,
         std::size_t y_loc) {
  int o = 0;
  o = x_dim * y_loc + x_loc;
  if (board[o] & hidden_mask()) {
    board[o] ^= marked_mask();
  }

  return 0;
}

///////////////////////////NEW NEW NEW NEW NEW NEW NEW NEW NEW/////////////////
unsigned int gridLocationToIndex(unsigned int xloc, unsigned int yloc,
                                 std::size_t x_dim, std::size_t y_dim) {
  int array_index{};
  array_index = x_dim * yloc + xloc;
  return array_index;
}

void compute_neighbours(char *board, std::size_t x_dim, std::size_t y_dim) {
  for (int row = 0; row < x_dim; row++) {
    for (int col = 0; col < y_dim; col++) {
      // if there is a goose on the board at row col
      if ((board[gridLocationToIndex(row, col, x_dim, y_dim)] & value_mask()) ==
          9) {
        {
          if (row > 0 && col > 0 &&
              ((board[gridLocationToIndex(row - 1, col - 1, x_dim, y_dim)] &
                value_mask()) != 9)) {
            board[gridLocationToIndex(row - 1, col - 1, x_dim, y_dim)]++;
          }

          if (row > 0 &&
              ((board[gridLocationToIndex(row - 1, col, x_dim, y_dim)] &
                value_mask()) != 9)) {
            board[gridLocationToIndex(row - 1, col, x_dim, y_dim)]++;
          }

          if (row > 0 && col < y_dim - 1 &&
              ((board[gridLocationToIndex(row - 1, col + 1, x_dim, y_dim)] &
                value_mask()) != 9)) {
            board[gridLocationToIndex(row - 1, col + 1, x_dim, y_dim)]++;
          }

          if (col > 0 &&
              ((board[gridLocationToIndex(row, col - 1, x_dim, y_dim)] &
                value_mask()) != 9)) {
            board[gridLocationToIndex(row, col - 1, x_dim, y_dim)]++;
          }

          if (col < y_dim - 1 &&
              ((board[gridLocationToIndex(row, col + 1, x_dim, y_dim)] &
                value_mask()) != 9)) {
            board[gridLocationToIndex(row, col + 1, x_dim, y_dim)]++;
          }

          if (row < x_dim - 1 && col > 0 &&
              ((board[gridLocationToIndex(row + 1, col - 1, x_dim, y_dim)] &
                value_mask()) != 9)) {
            board[gridLocationToIndex(row + 1, col - 1, x_dim, y_dim)]++;
          }

          if (row < x_dim - 1 &&
              ((board[gridLocationToIndex(row + 1, col, x_dim, y_dim)] &
                value_mask()) != 9)) {
            board[gridLocationToIndex(row + 1, col, x_dim, y_dim)]++;
          }

          if (row < x_dim - 1 && col < y_dim - 1 &&
              ((board[gridLocationToIndex(row + 1, col + 1, x_dim, y_dim)] &
                value_mask()) != 9)) {
            board[gridLocationToIndex(row + 1, col + 1, x_dim, y_dim)]++;
          }
        }
      }
    }
  }
}

bool is_game_won(char *board, std::size_t x_dim, std::size_t y_dim) {
  int counter = 0;
  for (int o = 0; o < x_dim * y_dim; o++) {
    if ((board[o] & value_mask()) != 9 && (board[o] & hidden_mask()) == 0) {
      counter++;
    }
    if ((board[o] & value_mask()) == 9)
      counter++;
  }
  if (counter == x_dim * y_dim) {
    return true;
  } else {
    return false;
  }
}

///////////////////
unsigned int Cortoin(unsigned int xx, unsigned int yy, std::size_t x_dim,
                     std::size_t y_dim);
unsigned int Cortoin(unsigned int xx, unsigned int yy, std::size_t x_dim,
                     std::size_t y_dim) {
  int index{};
  index = x_dim * xx + yy;
  return index;
}
////////////////////

// reveal
int reveal(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc,
           std::size_t y_loc) {
  int o = 0;
  o = x_dim * y_loc + x_loc;
  if (((board[o] & marked_mask()) != 0) && ((board[o] & hidden_mask()) != 0)) {
    return 1;
  }
  if (((board[o] & value_mask()) != 9) && ((board[o] & marked_mask()) == 0) &&
      ((board[o] & hidden_mask()) == 0)) {
    return 2;
  }
  if ((board[o] & value_mask()) == 9) {
    board[o] ^= hidden_mask();
    return 9;
  }

  if ((board[o] & value_mask()) != 9 && (board[o] & value_mask()) != 0) {
    board[o] ^= hidden_mask();
    return 0;
  }

  else if ((board[o] & value_mask()) == 0) {
    
    board[o] ^= hidden_mask();
    
    /*if(( board[o-x_dim]!=0)){
        board[o-x_dim]^=hidden_mask();
      }
      if((board[o-x_dim-1]!=0)){
        board[o-x_dim-1]^=hidden_mask();
      }
      if((board[o-x_dim+1]!=0)){
        board[o-x_dim+1]^=hidden_mask();
      }
      if(( board[o+x_dim]!=0)){
        board[o+x_dim]^=hidden_mask();
      }
      if((board[o+x_dim-1]!=0)){
        board[o+x_dim-1]^=hidden_mask();
      }
      if(( board[o+x_dim+1]!=0)){
        board[o+x_dim+1]^=hidden_mask();
      }
      if((board[o-1]!=0)){
        board[o-1]^=hidden_mask();
      }
      if((board[o+1]!=0)){
        board[o+1]^=hidden_mask();
  }*/

        int row = y_loc;
        int col = x_loc;
        // if there is a goose on the board at row col

        if (row-1 >= 0 && col-1 >= 0 && row-1 < x_dim && col-1 < y_dim) {
          if(((board[Cortoin(row - 1, col - 1, x_dim, y_dim)])& hidden_mask())!=0 &&((board[Cortoin(row - 1, col - 1, x_dim, y_dim)])& marked_mask())==0){
          board[Cortoin(row - 1, col - 1, x_dim, y_dim)] ^= hidden_mask();
          }
        }

        if (row-1 >= 0 && col>= 0 && row-1 < x_dim && col< y_dim) {
          if(((board[Cortoin(row - 1, col, x_dim, y_dim)])& hidden_mask())!=0 &&((board[Cortoin(row - 1, col, x_dim, y_dim)])& marked_mask())==0){
          board[Cortoin(row - 1, col, x_dim, y_dim)] ^= hidden_mask();
          }
        }     

        if (row-1 >= 0 && col+1>= 0 && row-1 < x_dim && col+1< y_dim){
          if(((board[Cortoin(row - 1, col + 1, x_dim, y_dim)])&hidden_mask())!=0 &&((board[Cortoin(row - 1, col+1, x_dim, y_dim)])& marked_mask())==0){
          board[Cortoin(row - 1, col + 1, x_dim, y_dim)] ^= hidden_mask();
          }
        }
          

        if (row >= 0 && col-1>= 0 && row < x_dim && col-1< y_dim){
            if(((board[Cortoin(row, col - 1, x_dim, y_dim)])&hidden_mask())!=0 &&((board[Cortoin(row , col-1, x_dim, y_dim)])& marked_mask())==0){            
          board[Cortoin(row, col-1, x_dim, y_dim)] ^= hidden_mask();
        }
      }

        if (row >= 0 && col+1>= 0 && row < x_dim && col+1< y_dim){
          if(((board[Cortoin(row, col + 1, x_dim, y_dim)])&hidden_mask())!=0 && ((board[Cortoin(row, col + 1, x_dim, y_dim)])& marked_mask())==0){
          board[Cortoin(row, col + 1, x_dim, y_dim)] ^= hidden_mask();
        }
      }

        if (row +1>= 0 && col-1>= 0 && row+1 < x_dim && col-1< y_dim) {
          if(((board[Cortoin(row+1, col- 1, x_dim, y_dim)])&hidden_mask())!=0 && ((board[Cortoin(row+1, col- 1, x_dim, y_dim)])& marked_mask())==0){
          board[Cortoin(row + 1, col - 1, x_dim, y_dim)] ^= hidden_mask();
        }
      }
        if (row +1>= 0 && col>= 0 && row+1 < x_dim && col< y_dim) {
          if(((board[Cortoin(row+1, col, x_dim, y_dim)])&hidden_mask())!=0 && ((board[Cortoin(row+1, col, x_dim, y_dim)])& marked_mask())==0){
          board[Cortoin(row + 1, col, x_dim, y_dim)] ^= hidden_mask();
        }
        }
        if (row +1>= 0 && col+1>= 0 && row+1 < x_dim && col+1< y_dim){
          if(((board[Cortoin(row+1, col+1, x_dim, y_dim)])&hidden_mask())!=0 && ((board[Cortoin(row+1, col+1, x_dim, y_dim)])& marked_mask())==0){
          board[Cortoin(row + 1, col + 1, x_dim, y_dim)] ^= hidden_mask();
        }
      }

      
    

    // center point
    // up down left right / topleft topright botleft botright
    // if in bound, we check
    // inbound check -> need to be x>=0 or x<xdim
    // after check -> revealed? marked?
    // if valid and revealed/marked -> do nothing
    // if valid and not reveald&not marked -> reveal
   
    return 0;
  }

return 0;
}