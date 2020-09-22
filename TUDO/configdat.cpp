#include <iostream>
#include <fstream>
#include <iomanip>      // std::setprecision & std::setw
#include <algorithm>    // std::count
#include <math.h>       // M_PI
#include <sstream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

#include "comum.h"
#include "configdat.h"

void Config::imprime(std::fstream &config_dat){
  config_dat<<";FOLD ENUM Pallet"<<endl; 
  for (int i=0;i<MaxPallets;i++)config_dat<< "CONST INT Pallet_"<<i+1<<" = "<<i+1<<endl;
  config_dat<<";ENDFOLD\n"<<endl; 


  int contador = 1;
  config_dat<<";FOLD ENUM RECEITA"<<endl; 
  for (auto &outt : ENUM_RECEITA)config_dat<< "CONST INT Prdt_"<<outt<<" = "<<(contador++)<<endl;
  config_dat<<";ENDFOLD\n"<<endl; 


  config_dat<<";FOLD STR"<<endl; 
  config_dat<<";---------- StrPallet[MaxPallets] ----------"<<endl;
  config_dat<<"CONST INT MaxPallets="<<MaxPallets<<endl;
  config_dat<<"DECL GLOBAL DefPallet StrPallet["<<MaxPallets<<"];[MaxPallets]"<<endl;
  config_dat<<";---------- StrReceita[MaxReceitas] ----------"<<endl;
  config_dat<<"CONST INT MaxReceitas="<<MaxReceitas<<endl;
  config_dat<<"CONST INT MaxCamadas="<<MaxCamadas<<endl;
  config_dat<<"DECL GLOBAL DefStrReceita StrReceita["<<MaxReceitas<<"];[MaxReceitas]"<<endl;
  config_dat<<"INT  StrReceitaLayer["<<MaxReceitas<<","<<MaxCamadas<<"];[MaxReceitas,MaxCamadas]"<<endl;
  config_dat<<";---------- MatrizPontos[MaxMatrizI,MaxMatrizJ,MaxMatrizK] ----------"<<endl;
  config_dat<<"CONST INT MaxMatrizI="<<MaxMatrizI<<";[numero de pallets por receita]"<<endl;
  config_dat<<"CONST INT MaxMatrizJ="<<MaxMatrizJ<<";[acompanha o MaxReceitas]"<<endl;
  config_dat<<"CONST INT MaxMatrizK="<<MaxMatrizK<<";[numero Maximo de Pontos]"<<endl;
  config_dat<<"DECL GLOBAL DefMatriz MatrizPontos["<<MaxMatrizI<<","<<MaxMatrizJ<<","<<MaxMatrizK<<"];[MaxMatrizI,MaxMatrizJ,MaxMatrizK]"<<endl;
  config_dat<<";ENDFOLD\n"<<endl;
}
