#ifndef comum_h
#define comum_h

std::string split_string(std::string tokenString,string delim,int saida);
//vector<string> split_string(std::string tokenString,string delim);
vector<string> split_string(std::string tokenString,string delim,bool print=false);
void split_string_print(std::string tokenString,string delim);

int copy_file(std::ofstream &out,std::string in1,std::string in2);
std::string ponto_remove_turn(std::string entrada);

#endif