//# +---------------------------------------------------------------------------------------------------------------------------------------------------<< 
//             ╔══════════════════════════════════════════════════╗
//             ║  Copyright (C) 2020 Paulo Victor Duarte          ║
//             ╚══════════════════════════════════════════════════╝
//# +---------------------------------------------------------------------------------------------------------------------------------------------------<< 
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

#include "comum.h"
#include "simulacao.h"
#include "matriz_pontos.h"

int main(int argc, char **argv)
{
//+------------------------------------------------------------<< 
  //system("clear");
  // system("cls");
//+------------------------------------------------------------<< 
  // cout << "# PROGRAMA INICIADO #"<<endl;
  // std::ofstream Relatorio("Relatorio/Relatorio.md");
  // std::streambuf *coutbuf=std::cout.rdbuf(); //save old buf
  // std::cout.rdbuf(Relatorio.rdbuf()); //redirect std::cout to out.txt!
  // cout << "Relatorio gerado em [ Relatorio/Relatorio.md ]"<<endl;
//+------------------------------------------------------------<< 
  cout << "# $ >> PROGRAMA INICIADO << $"<<endl;
  cout << "Valores de inicializacao" << endl; 
//+------------------------------------------------------------<< 
  //configuracoes de inicializacao
  int Pallet=1;
  int quadrante=4;
  class Pose app;
  //quadrante
  if(argc==2){
    quadrante = stoi(argv[1]);
    Pallet = quadrante;
  }
  //quadrante e force_pallet
  if(argc==3){
    quadrante = stoi(argv[1]);
    Pallet = stoi(argv[2]);
  }
  switch (quadrante){
    case 1:
      app.X=(+60);
      app.Y=(-60);
      break;
    case 2:
      app.X=(-60);
      app.Y=(-60);
      break;
    case 3:
      app.X=(+60);
      app.Y=(+60);
      break;
    case 4:
      app.X=(-60);
      app.Y=(+60);
      break;
    default:
      app.X=(-60);
      app.Y=(+60);
      break;
  }
  app.Z=100;
  //print
  cout << "Pallet: " << Pallet << endl;
  cout << "Valor de aproximacao: " << app << endl;
  cout << "\n-----------------------------\n" << endl;
//+------------------------------------------------------------<< 
  //variaveis
  std::string entrada;
  int NumPontos=0;
  int NumLayers=0;
  bool aux_name=false;

  int coluna_num=0;
  double coluna_valor_X=0;
  bool coluna_aux1=true;
  bool coluna_aux2=true;

  class Pose pose_aux;
  class Receita receita;
//+------------------------------------------------------------<<
  //lê o arquivo
  std::ifstream file_in("in/My_Job.json");
  if( !file_in ){cout << "Erro ao abrir os arquivos.\n";return 0;}
  //faz a coleta dos dados
  while (!file_in.eof())
  {
    if (!file_in.good())break;
    getline(file_in,entrada);
    //coleta o nome da receita
    if(buscar_chave(entrada,"name") && aux_name==false)
    {
      aux_name=true;
      //imprime_vetor(split_string(entrada,"[\"]+"));
      receita.nome = split_string(entrada,"[\"]+",3);
    }
    //pegar valor de paller
    if(buscar_chave(entrada,"dimensions"))
    {
      getline(file_in,entrada);
      receita.Pallet.height = valor(entrada);
      getline(file_in,entrada);
      receita.Pallet.length = valor(entrada);
      getline(file_in,entrada);
      receita.Pallet.width = valor(entrada);
      cout << "receita.Pallet\n" << receita.Pallet << endl;
    }
    //pega valor de receita.Caixa
    if(buscar_chave(entrada,"productDimensions"))
    {
      getline(file_in,entrada);
      receita.Caixa.width = valor(entrada);
      getline(file_in,entrada);
      receita.Caixa.height = valor(entrada);
      getline(file_in,entrada);
      receita.Caixa.length = valor(entrada);
      getline(file_in,entrada);
      receita.Caixa.weight = valor(entrada);
      cout << "receita.Caixa\n" << receita.Caixa << endl;
    }
    //coleta os layers
    if(buscar_chave(entrada,"layers"))
    {
      while (entrada.find("]")!=0)
      {
        getline(file_in,entrada);
        if (!file_in.good()||buscar_chave(entrada,"]"))break;
        receita.LayersVector.push_back(valor(entrada));
      }
    }
    //captura os pontos
    if(buscar_chave(entrada,"pattern"))
    {
      NumLayers++;
      while (entrada.find("        }")!=0)
      {
        getline(file_in,entrada);
        if (!file_in.good())break;

        if(buscar_chave(entrada,"x"))
        {
          NumPontos ++;
          pose_aux.X=valor(entrada);
          getline(file_in,entrada);
          pose_aux.Y=valor(entrada);
          getline(file_in,entrada);
          getline(file_in,entrada);
          pose_aux.A=valor(entrada);
          getline(file_in,entrada);
          //pose_aux.B=valor(entrada);
          pose_aux.Z=receita.Caixa.height;
          // pose_aux.Z=receita.Caixa.height*NumLayers;
          //cout << pose_aux << endl;
          receita.all_poses.push_back(pose_aux);
        }
      }
    }
  }
//+------------------------------------------------------------<< 
  //abre os arquivos
  std::ofstream simulacao_src("simulacao/simulacao.src",std::ofstream::out);
  std::ofstream simulacao_dat("simulacao/simulacao.dat",std::ofstream::out);
  if( !simulacao_src ){cout << "Erro ao abrir os arquivos.\n";return 0;}
  if( !simulacao_dat ){cout << "Erro ao abrir os arquivos.\n";return 0;}

  std::ofstream TReceita_src("out/INIT/TReceita.src",std::ofstream::out);
  std::ofstream TReceita_dat("out/INIT/TReceita.dat",std::ofstream::out);
  if( !TReceita_src ){cout << "Erro ao abrir os arquivos.\n";return 0;}
  if( !TReceita_dat ){cout << "Erro ao abrir os arquivos.\n";return 0;}

  std::ofstream TMatriz_src ("out/INIT/TMatriz.src", std::ofstream::out);
  std::ofstream TMatriz_dat ("out/INIT/TMatriz.dat", std::ofstream::out);
  if( !TMatriz_src ){cout << "Erro ao abrir os arquivos.\n";return 0;}
  if( !TMatriz_dat ){cout << "Erro ao abrir os arquivos.\n";return 0;}

  init_files(simulacao_src,simulacao_dat,"simulacao");
  init_files(TReceita_src,TReceita_dat,"TReceita");
  init_files(TMatriz_src,TMatriz_dat,"TMatriz");
  //padrao_move(simulacao_src);

//+------------------------------------------------------------<< 
  //preenche a receita
  receita.FinalContador=NumPontos;
  receita.PlacesCamada=NumPontos/NumLayers;
  receita.AlturaCaixa=receita.Caixa.height;
  receita.Camadas=receita.LayersVector.size();
  receita.Layers=NumLayers;
  receita.imprime(TReceita_src);
  cout << "coluna_num: " << coluna_num << endl;
  receita.quadrante_vector(quadrante);
//+------------------------------------------------------------<< 
  // distribui as informações aos arquivos 
  matriz_maker(TMatriz_src,TMatriz_dat,Pallet,receita,app);
  simulacao_maker(simulacao_src,simulacao_dat,Pallet,receita,app);
//+------------------------------------------------------------<< 
  end_files(simulacao_src,simulacao_dat);
  end_files(TReceita_src,TReceita_dat);
  end_files(TMatriz_src,TMatriz_dat);
  simulacao_src.close();
  simulacao_dat.close();
  TReceita_src.close();
  TReceita_dat.close();
  TMatriz_src.close();
  TMatriz_dat.close();
  file_in.close();
  //Relatorio.close();
  cout << "# $ >> FIM << $"<<endl;
  return 0;
}