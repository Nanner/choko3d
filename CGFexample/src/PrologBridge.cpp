#include "PrologBridge.h"
#include <string>
#include <sstream>

#ifdef linux
pid_t PrologBridge::procId = NULL;
#endif
short PrologBridge::port = 60001;

PrologBridge::PrologBridge() {
	initialize(PrologBridge::port);
	++PrologBridge::port;
}

PrologBridge::PrologBridge(int port_n) {
    initialize(port_n);
}

void PrologBridge::initialize(int port_n) {
    string command = CHOKO;
    std::stringstream ss;
    ss << port_n;
    string port_string = ss.str();
    
#ifdef linux
    if (!(procId = fork())) {
        system(command.c_str());
        //kill(getppid(), SIGINT);
        exit(0);
    }
    else {
        usleep(100000);
        con = new Connection(port_n);
}
#else
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    LPSTR command_lpstr = const_cast<char *>(command.c_str());
    if( !CreateProcess( NULL,   // No module name (use command line)
                       command_lpstr,        // Command line
                       NULL,           // Process handle not inheritable
                       NULL,           // Thread handle not inheritable
                       FALSE,          // Set handle inheritance to FALSE
                       0,              // No creation flags
                       NULL,           // Use parent's environment block
                       NULL,           // Use parent's starting directory
                       &si,            // Pointer to STARTUPINFO structure
                       &pi )           // Pointer to PROCESS_INFORMATION structure
       ) {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return;
    }
    
    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    
    Sleep(100);
    con = new Connection(port_n);
    
#endif
}

string PrologBridge::initializeGame() {
    string str("initialize.\n");
    return con->sendMsg(str);
}

GameState PrologBridge::execute(string move, vector<string> board, char player, int playerUnusedPieces, int enemyUnusedPieces, char dropInitiative) {
    stringstream ss;
    ss << "execute(" << move << ',' << GameState::toString(board) << ',' << player << ',' << playerUnusedPieces << ',' << enemyUnusedPieces << ',' << dropInitiative << ").\n";
    
    string reply = con->sendMsg(ss.str());
    reply.pop_back(); reply.pop_back(); // remove "]."
    reply = reply + "," + move + "]";
    GameState gameState(reply);
    
    return gameState;
}

GameState PrologBridge::calculate(vector<string> board, char player, int playerUnusedPieces, int enemyUnusedPieces, char dropInitiative, string playerDifficulty) {
    stringstream ss;
    ss << "calculate(" << GameState::toString(board) << ',' << player << ',' << playerUnusedPieces << ',' << enemyUnusedPieces << ',' << dropInitiative << ',' << playerDifficulty << ").\n";
    
    string reply = con->sendMsg(ss.str());
    GameState gameState(reply);
    return gameState;
}

string PrologBridge::gameOver(vector<string> board, char player, int playerUnusedPieces, int enemyUnusedPieces) {
    stringstream ss;
    ss << "gameOver(" << GameState::toString(board) << ',' << player << ',' << playerUnusedPieces << ',' << enemyUnusedPieces << ").\n";
    
    return con->sendMsg(ss.str());
}
