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

string PrologBridge::toString(vector<string> board) {
    string boardString = "[";
    for (int i = 0; i < board.size(); i++) {
        boardString += board.at(i);
        if (i < board.size()-1)
            boardString += ",";
    }
    boardString += "]";
    return boardString;
}

vector<string> PrologBridge::toVector(string board) {
    vector<string> boardVector;
    stringstream ss(board);
    ss.ignore(1, '[');
    string boardCell;
    while (getline(ss, boardCell, ',')) {
        if (boardCell.at(boardCell.size()-1) == ']')
            boardCell.pop_back();
        boardVector.push_back(boardCell);
    }
    return boardVector;
}

string PrologBridge::execute(string move, vector<string> board, char player, int playerUnusedPieces, int enemyUnusedPieces, char dropInitiative) {
    stringstream ss;
    ss << "execute(" << move << ',' << toString(board) << ',' << player << ',' << playerUnusedPieces << ',' << enemyUnusedPieces << ',' << dropInitiative << ").\n";
    
    return con->sendMsg(ss.str());
}

string PrologBridge::calculate(vector<string> board, char player, int playerUnusedPieces, int enemyUnusedPieces, char dropInitiative, string playerDifficulty) {
    stringstream ss;
    ss << "calculate(" << toString(board) << ',' << player << ',' << playerUnusedPieces << ',' << enemyUnusedPieces << ',' << dropInitiative << ',' << playerDifficulty << ").\n";
    
    return con->sendMsg(ss.str());
}

string PrologBridge::gameOver(vector<string> board, char player, int playerUnusedPieces, int enemyUnusedPieces) {
    stringstream ss;
    ss << "gameOver(" << toString(board) << ',' << player << ',' << playerUnusedPieces << ',' << enemyUnusedPieces << ").\n";
    
    return con->sendMsg(ss.str());
}
