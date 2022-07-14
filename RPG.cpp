#include <iostream>
#include <ncurses.h>
#include <string>

void initTUI();
void fullClear(WINDOW*& console, WINDOW*& actions, WINDOW*& game);
void welcomeScreen(WINDOW*& console, WINDOW*& actions);
void initMenu(WINDOW*& console, WINDOW*& actions, int initMenu);
void initOverworld(WINDOW*& game);
void quitGame();

class Entity{
protected:
    int yPos;
    int xPos;
public:
    // int nyPos;
    // int nxPos;
    Entity(int YPos, int XPos){
        yPos = YPos;
        xPos = XPos;
    };
    void drawPos(WINDOW*& game){
        mvwprintw(game, yPos, xPos, "@");
        wrefresh(game);
    };
};

class Player:public Entity{
public:
    Player(int yPos, int xPos):Entity(yPos,xPos){};
    void moveUp(WINDOW*& game){
        mvwprintw(game, yPos, xPos, " ");
        yPos--;
        mvwprintw(game, yPos, xPos, "@");
    };
    void moveDown(WINDOW*& game){
        mvwprintw(game, yPos, xPos, " ");
        yPos++;
        mvwprintw(game, yPos, xPos, "@");
    };
    void moveLeft(WINDOW*& game){
        mvwprintw(game, yPos, xPos, " ");
        xPos--;
        mvwprintw(game, yPos, xPos, "@");
    };
    void moveRight(WINDOW*& game){
        mvwprintw(game, yPos, xPos, " ");
        xPos++;
        mvwprintw(game, yPos, xPos, "@");
    };
};

bool checkMove(Player player, WINDOW*& game);
void playerControl(WINDOW*& console, WINDOW*& actions, WINDOW*& game, Player player);
void gameLoop(WINDOW*& console, WINDOW*& actions, WINDOW*& game, Player player);

int main(){
    // Initialize TUI
    initTUI();
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW* console = newwin(yMax/4, xMax, 3*yMax/4, 0);
    WINDOW* game = newwin(3*yMax/4, 3*xMax/4, 0, 0);
    WINDOW* actions = newwin(3*yMax/4, xMax/4, 0, 3*xMax/4);
    refresh();
    keypad(actions, true);

    // Welcome the Player
    // welcomeScreen(console, actions);
    fullClear(console,actions,game);

    // Initialize overworld menu and game
    initOverworld(game);
    initMenu(console,actions,1);
    Player player(5,5);
    player.drawPos(game);

    //Create game loop
    gameLoop(console, actions, game, player);
    // playerControl(console, actions, game);

    endwin();
    return 0;
};



void gameLoop(WINDOW*& console, WINDOW*& actions, WINDOW*& game, Player player){
    while(1){
        playerControl(console, actions, game, player);
    };
};


void playerControl(WINDOW*& console, WINDOW*& actions, WINDOW*& game, Player player){
    
    int movement;
    movement = wgetch(game);
    player.drawPos(game);

    switch(movement){
    case 'w':
        player.moveUp(game);
        break;
    case 's':
        player.moveDown(game);
        break;
    case 'a':
        player.moveLeft(game);
        break;
    case 'd':
        player.moveRight(game);
        break;
    case 'i':
        initMenu(console,actions,0);
        break;
    };

};

// bool checkMove(Player player, WINDOW*& game){
//     switch(mvinch(player.nyPos, player.nxPos)){
//         case ' ':
//             return true;
//         default:
//             return false;
//     };
// };

void initTUI(){
    initscr();
    noecho();
    curs_set(0);
};

void fullClear(WINDOW*& console, WINDOW*& actions, WINDOW*& game){
    wclear(console);
    wclear(actions);
    wclear(game);
    box(console, 0, 0);
    mvwprintw(console, 0, 2, "Console");
    box(game, 0, 0);
    mvwprintw(game, 0, 2, "Game");
    box(actions, 0, 0);
    mvwprintw(actions, 0, 2, "Actions");
    wrefresh(console);
    wrefresh(actions);
    wrefresh(game);
};


void welcomeScreen(WINDOW*& console, WINDOW*& actions){
    // Set up fake menu controls
    wattron(actions, A_REVERSE);
    mvwprintw(actions, 2, 1, "* Continue  ");
    wrefresh(actions);

    // Give welcome text
    mvwprintw(console, 1, 1, "Welcome to the game!");
    wgetch(console);

    mvwprintw(console, 1, 1, "This is a test set of text");
    wgetch(console);

    mvwprintw(console, 1, 1, "Welcome to the world of whatever this is!");
    wgetch(console);

    //Set menu back to normal
    wattroff(actions, A_REVERSE);
};


void initMenu(WINDOW*& console, WINDOW*& actions, int initMenu){

    int amtChoices = 6;
    std::string choices[6] = {"* Interact  ","* Inventory  ","* Map  ","* Stats  ","* Debug  ","* Quit  "};
    for(int i = 0; i<amtChoices; i++){
        mvwprintw(actions, i+2, 1, choices[i].c_str());
    };

    int highlight = 0;
    int choice;

    if(initMenu == 1)
        for(int i = 0; i<amtChoices; i++){
            mvwprintw(actions, i+2, 1, choices[i].c_str());
            wrefresh(actions);
            return;
        };
    
    while(initMenu == 0){
        for(int i = 0; i < amtChoices; i++){
            if(i == highlight)
                wattron(actions, A_REVERSE);
            mvwprintw(actions, i+2, 1, choices[i].c_str());
            wattroff(actions,A_REVERSE);
        };
        choice = wgetch(actions);
        switch(choice){
            case KEY_UP:
                highlight--;
                if(highlight < 0)
                    highlight = 0;
                break;
            case KEY_DOWN:
                highlight++;
                if(highlight > amtChoices-1)
                    highlight = amtChoices-1;
                break;
            case KEY_BACKSPACE:
                mvwprintw(actions, highlight+2, 1, "%s", choices[highlight].c_str());
                wrefresh(actions);
                return;
            default:
                break;
        }
        if(choice == 10)
            break;
    };
    mvwprintw(console, 1, 1, "%s", choices[highlight].c_str());
    if(choices[highlight] == "* Quit  ")
        quitGame();
    wrefresh(console);
};

void initOverworld(WINDOW*& game){
    mvwprintw(game, 2, 2, "|--------|");
    mvwprintw(game, 3, 2, "|        |");
    mvwprintw(game, 4, 2, "|        |");
    mvwprintw(game, 5, 2, "|        |");
    mvwprintw(game, 6, 2, "|--------|");

    mvwprintw(game, 2, 50, "|--------|");
    mvwprintw(game, 3, 50, "|        |");
    mvwprintw(game, 4, 50, "|        |");
    mvwprintw(game, 5, 50, "|        |");
    mvwprintw(game, 6, 50, "|--------|");

    mvwprintw(game, 4, 20, "|-------------|");
    mvwprintw(game, 5, 20, "|             |");
    mvwprintw(game, 6, 20, "|             |");
    mvwprintw(game, 7, 20, "|             |");
    mvwprintw(game, 8, 20, "|             |");
    mvwprintw(game, 9, 20, "|             |");
    mvwprintw(game,10, 20, "|-------------|");

    wrefresh(game);
};

void quitGame(){
    exit(0);
};