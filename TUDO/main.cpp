//+-------------------------------------------------------------------------------+
//             ╔══════════════════════════════════════════════════╗
//             ║  Copyright (C) 2020 Paulo Victor Duarte          ║
//             ╚══════════════════════════════════════════════════╝
//+-------------------------------------------------------------------------------+
// BIBLIOTECAS C++

// COMPILAR
// g++ *.cpp -o executavel -std=c++11 && ./executavel 

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

#include "matriz_pontos.h"
#include "pallet.h"
#include "receita.h"
#include "configdat.h"
#include "receita.h"
#include "comum.h"
#include "garra.h"

std::fstream TReceita_src("out/TReceita.src",std::ofstream::out | std::ofstream::trunc);
std::fstream TReceita_dat("out/TReceita.dat",std::ofstream::out | std::ofstream::trunc);
std::fstream TMatriz_src ("out/TMatriz.src",std::ofstream::out | std::ofstream::trunc);
std::fstream TMatriz_dat ("out/TMatriz.dat",std::ofstream::out | std::ofstream::trunc);
std::fstream TPallet_src ("out/TPallet.src",std::ofstream::out | std::ofstream::trunc);
std::fstream TPallet_dat ("out/TPallet.dat",std::ofstream::out | std::ofstream::trunc);

std::fstream config_dat("out/config.dat",std::ofstream::out | std::ofstream::trunc);

void init_all(void){
  prefixo(TReceita_src,"TReceita",false);
  prefixo(TReceita_dat,"TReceita",true);
  prefixo(TMatriz_src,"TMatriz",false);
  prefixo(TMatriz_dat,"TMatriz",true);
  prefixo(TPallet_src,"TPallet",false);
  prefixo(TPallet_dat,"TPallet",true);
}

void end_all(void){
  sufixo(TReceita_src,false);
  sufixo(TReceita_dat,true);
  sufixo(TMatriz_src,false);
  sufixo(TMatriz_dat,true);
  sufixo(TPallet_src,false);
  sufixo(TPallet_dat,true);
}

int MaxCamadas=8;
int MaxPallets=2;

int main(int argc, char **argv)
{
//+-------------------------------------------------------------------------------+
  // variaveis
  int MaxMatrizK;
  int PlacesCamada;
  std::vector<class Receita> receitas;
  class Receita aux_receita("A");
  class Config config;
  std::string pallet;
  std::string produto;
  std::string entrada;
   
  // Informar o numero de linhas do arquivo 
  // std::ifstream My_Job_src(argv[1]); 
  std::ifstream My_Job_src("My_Job/My_Job.src"); 
  std::ifstream My_Job_dat("My_Job/My_Job.dat"); 
  if(!My_Job_src) {
    cout << "Arquivo não foi encontrado.\n";
    return 0;
  }
  init_all();
  // ## Garra
  // cria todos os out de garra
  garra_exe(My_Job_src);
  My_Job_src.clear();
  My_Job_src.seekg(0);
  // ## TMatriz -> cria a matriz dat e src
  // começa procurando os MatrizPontos
  // !encontra o Produto
  // PlacesCamada++;
  // layers++;

  // ## TReceita -> cria o Receita src e dat
  // antes de imprimir (PlacesCamada/layers)
  
  while (!My_Job_src.eof())
  {
    getline(My_Job_src,entrada);
    if (!My_Job_src.good()) {
      // cout << "ponteiro nulo" << endl;
      break;
    }
    if(entrada.find("DEF Mtrz") !=std::string::npos)
    {
      pallet=split_string(entrada,"[_()]+",1);//pallet
      produto=split_string(entrada,"[_()]+",2);//produto
      entrada=split_string(entrada,"[ ()]+",1);//produto
      cout << entrada << endl;
      PlacesCamada=matriz_pontos_str(My_Job_src,TMatriz_src,pallet,produto);
      MaxMatrizK=matriz_pontos_dat(My_Job_dat,TMatriz_dat,pallet,produto,entrada);
      config.MaxMatrizK=MaxMatrizK>config.MaxMatrizK?MaxMatrizK:config.MaxMatrizK;
      if(std::find(config.ENUM_RECEITA.begin(), config.ENUM_RECEITA.end(),produto) == config.ENUM_RECEITA.end())
      {
        // ## TPallet -> cria o receita src e dat
        // coloca as testeiras e os pallets
        config.ENUM_RECEITA.push_back(produto);
        aux_receita.nome = produto;
        aux_receita.PlacesCamada=PlacesCamada;
        receitas.push_back(aux_receita);
        config.MaxReceitas++;
        config.MaxMatrizJ++;
      } 
      

    }
  }
  config.MaxCamadas=MaxCamadas;
  config.MaxPallets=MaxPallets;
  config.MaxMatrizI=MaxPallets;
  config.imprime(config_dat);
  for (auto &outt : receitas)outt.imprime(TReceita_src,MaxCamadas);
  pick(My_Job_dat,TReceita_dat);
  pallet_src(My_Job_src,TPallet_src,MaxPallets);


  end_all();
  config_dat.close();
  TReceita_dat.close();
  TReceita_src.close();
  TMatriz_dat.close();
  TMatriz_src.close();
  TPallet_dat.close();
  TPallet_src.close();
  My_Job_src.close();
  My_Job_dat.close();
  return 0;
}