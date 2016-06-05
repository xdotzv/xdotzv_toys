#include <ncurses.h>
#include <array>
#include <cassert>
#include <random>
#include <vector>
#include <iterator>

using std::string;

template<unsigned N = 4>
class Game{

public:
  typedef std::array<std::array<int, N>, N> Board;
  enum Status {RUNNING, EXITED, FAILED};
  Board board;
  unsigned cw; // cell width
  int board_width;
  int board_height;

  string hline;
  string cell_format;
  string empty_cell;
  Status status;

  Board invert(const Board &b){
    Board ret;
    for(int i=0; i<N; ++i)
      for(int j=0; j<N; ++j)
        ret[i][j] = b[i][N-1-j];
    return ret;
  }

  Board transpose(const Board &b){
    Board ret;
    for(int i=0; i<N; ++i)
      for(int j=0; j<N; ++j)
        ret[i][j] = b[j][i];
    return ret;
  }

  auto get_color(int value) -> decltype(COLOR_PAIR(1)){
    int count = 0;
    while(value>0){
      value >>= 1;
      ++count;
    }
    if(count < 4) count = 4;
    count -= 3;
    return COLOR_PAIR(count);
  }

  void draw_horizontal(){
    printw(hline.c_str());
  }
  void draw_cell(int i, int j){
    assert(i<N && j <N);

    auto x = i*2+1;
    auto y = j*(cw+1)+1;

    if(board[i][j]==0) {
      mvprintw(x, y, empty_cell.c_str());
      return;
    }

    auto cp = get_color(board[i][j]);
    attron(cp);
    mvprintw(x, y, cell_format.c_str(), board[i][j]);
    attroff(cp);
  }
  void draw_board(){
    draw_horizontal();
    for(int i=1; i<=N; ++i){
      printw("|");
      for(int j=1; j<=N; ++j){
        for(int k=1; k<=cw; ++k) printw(" ");
        printw("|");
      }
      printw("\n");
      draw_horizontal();
    }
  }
  void draw_cell(){
    for(int i=0; i<N; ++i)
      for(int j=0; j<N; ++j)
        draw_cell(i, j);
  }
  void print_middle(int x, const string &s, char c=' '){
    assert(s.size()<=board_width);
    int m = s.size();
    move(x, 0);
    m = (board_width - m)/2;
    for(int i=1; i<=m; ++i)
      addch(c);
    printw(s.c_str());
    for(int i=1; i<=board_width-s.size()-m; ++i)
      addch(c);
  }
  void draw_instructions(){
    print_middle(board_height, "<q> quit; <r> restart");
  }
  void draw_info(const string &s){
    print_middle(board_height+1, s, '*');
  }
  bool init_rand_cell(){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> cell_number_dis(1,2);

    std::vector<decltype(std::begin(board[0]))> candidates;
    for(auto &row : board)
      for(auto it = std::begin(row); it!=std::end(row); ++it)
        if(*it==0) candidates.push_back(it);

    if(candidates.empty()) {
      status = FAILED;
      return false;
    }

    std::uniform_int_distribution<> dis(0, candidates.size()-1);
    auto pos = candidates[dis(gen)];
    *pos = cell_number_dis(gen)*2;

    return true;
  }

  Board do_move_left(const Board &b){
    Board ret = {};
    for(int i=0; i<N; ++i){
      int index = 0;
      for(int j=0; j<N; ++j)
        if(b[i][j]!=0) ret[i][index++] = b[i][j];
      for(int j=0; j<index-1; ++j){
        if(ret[i][j]==ret[i][j+1]) {
          ret[i][j]*=2;
          ++j;
          ret[i][j] = 0;
        }
       }
      index = 0;
      for(int j=0; j<N; ++j)
        if(ret[i][j]!=0){
          ret[i][index] = ret[i][j];
          index++;
        }
      for(int j=index; j<N; ++j)
        ret[i][j] = 0;
    }
    return ret;
  }
  void move_left(){
    board = do_move_left(board);
    init_rand_cell();
  }
  void move_right(){
    board = invert(do_move_left(invert(board)));
    init_rand_cell();
  }
  void move_up(){
    board = transpose(do_move_left(transpose(board)));
    init_rand_cell();
  }
  void move_down(){
    board = transpose(invert(do_move_left(invert(transpose(board)))));
    init_rand_cell();
  }
  void init(){
    status = RUNNING;
    for(auto &row: board)
      for(auto &x: row)
        x = 0;
    init_rand_cell();
  }
  void restart(){
    start();
  }
  void quit(){
    status = EXITED;
  }

public:
  Game(int __cw = 5):board(), cw(__cw), board_width((__cw+1)*N+1), board_height(N*2+1) {
    for(int i=1; i<=N; ++i){
      hline = hline + "+" + string(cw, '-');
    }
    hline = hline + "+\n";
    cell_format = "%"+ std::to_string(cw)+"d";
    empty_cell = string(cw, ' ');

    initscr();			/* Start curses mode 		*/
    raw();				/* Line buffering disabled	*/
    noecho();			/* Don't echo() while we do getch */
    keypad(stdscr, TRUE); /* F1 F2 and more */
    curs_set(0);  // invisable cursor

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_RED, COLOR_BLACK);
  }
  void start(){
    clear();
    init();
    draw_board();
    draw_cell();
    draw_instructions();
  }
  void loop(){
    while(status!=EXITED){
      if(status==FAILED){
        draw_info("Game Over");
        while(true){
          int ch = getch();
          if(ch=='q'){
            quit();
            break;
          }
          else if(ch=='r'){
            restart();
            break;
          }
        }
      }

      while(status==RUNNING){
        int ch = getch();
        switch (ch){
        case 'q':{
          quit();
          break;
        }
        case 'r':{
          restart();
          break;
        }
        case KEY_LEFT:{
          move_left();
          break;
        }
        case KEY_RIGHT:{
          move_right();
          break;
        }
        case KEY_UP:{
          move_up();
          break;
        }
        case KEY_DOWN:{
          move_down();
          break;
        }
        }
        draw_cell();
        refresh();
      }
    }
    endwin();
  }
};
int main()
{

  Game<> game;

  game.start();
  game.loop();
	return 0;
}
