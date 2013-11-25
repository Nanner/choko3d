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

    string str("initialize.\n");
    con->sendMsg(str);
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
  
  string str("initialize.");
  con->sendMsg(str);
#endif

}

bool PrologBridge::createWindow(int width, int height) {
  char buff[200];
  sprintf(buff, "createWindow %d %d\n", width, height);
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::closeWindow() {
  char buff[200];
  sprintf(buff, "closeWindow\n");
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::addNode(int id) {
  char buff[200];
  sprintf(buff, "addNode1 %d\n", id);
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::addNode(int id, int x, int y) {
  char buff[200];
  sprintf(buff, "addNode3 %d %d %d\n", id, x, y);
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::addEdge(int id, int v1, int v2, int edgeType) {
  char buff[200];
  sprintf(buff, "addEdge %d %d %d %d\n", id, v1, v2, edgeType);
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::setEdgeLabel(int k, string label) {
  char buff[200];
  sprintf(buff, "setEdgeLabel %d %s\n", k, label.c_str());
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::setVertexLabel(int k, string label) {
  char buff[200];
  sprintf(buff, "setVertexLabel %d %s\n", k, label.c_str());
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::defineEdgeColor(string color) {
  char buff[200];
  sprintf(buff, "defineEdgeColor %s\n", color.c_str());
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::removeNode(int id) {
  char buff[200];
  sprintf(buff, "removeNode %d\n", id);
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::removeEdge(int id) {
  char buff[200];
  sprintf(buff, "removeEdge %d\n", id);
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::setEdgeColor(int k, string color) {
  char buff[200];
  sprintf(buff, "setEdgeColor %d %s\n", k, color.c_str());
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::setEdgeThickness(int k, int thickness) {
  char buff[200];
  sprintf(buff, "setEdgeThickness %d %d\n", k, thickness);
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::defineVertexColor(string color) {
  char buff[200];
  sprintf(buff, "defineVertexColor %s\n", color.c_str());
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::setVertexColor(int k, string color) {
  char buff[200];
  sprintf(buff, "setVertexColor %d %s\n", k, color.c_str());
  string str(buff);
  string answer = con->sendMsg(str); return true;
}

bool PrologBridge::setBackground(string path) {
  char buff[200];
  sprintf(buff, "setBackground %s\n", path.c_str());
  string str(buff);
  string answer = con->sendMsg(str); return true;
}
