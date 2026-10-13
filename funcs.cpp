#include <fstream>
#include <iostream>
#include <windows.h>
#include <math.h>
#include <string>
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>

void MakeTable(const std::string& key, const std::string& alphabet, char* table){
    for(int i=0;i<alphabet.size();++i){
        table[static_cast<unsigned char>(key[i])]=alphabet[i];
    }
}

std::string Decrypt(std::ifstream& fin, const char* table){
    fin.clear();
    fin.seekg(0, std::ios::beg);
    char c;
    std::string Otext;
    while(fin.get(c)){
       Otext+=table[static_cast<unsigned char>(c)];
    }
    return Otext;
}

double LogRank(const std::string& Otext, float table[33][33][33][33]){  
    std::string word="";
    std::string buff;
    int num;

    double S=0;

    for(int i=0;i<Otext.size()-3;++i){
        for(int j=0;j<4;++j){
            word+=Otext[i+j];
        }
        try{
            num=table[ static_cast<unsigned char> (word[0]) ][ static_cast<unsigned char> (word[1]) ][ static_cast<unsigned char> (word[2]) ][ static_cast<unsigned char> (word[3]) ];
        }catch(...){continue;}
        word="";
    }
    return S;
}

int randint(const int& min,const int& max){
    std::random_device rd;
    std::seed_seq seed{
        rd(), rd(), rd(), rd(),
        static_cast<unsigned>(
            std::chrono::high_resolution_clock::now()
                .time_since_epoch().count())
    };
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(min, max);

    return dist(gen);
}

std::string CreateKey(const std::string& alphabet){
    std::random_device rd;
    std::seed_seq seed{
        rd(), rd(), rd(), rd(),
        static_cast<unsigned>(
            std::chrono::high_resolution_clock::now()
                .time_since_epoch().count())
    };
    std::mt19937 gen(seed);

    std::string key=alphabet;

    std::shuffle(key.begin(), key.end(), gen);
    return key;
}
 
std::string BestNeighbour(const std::string& key, const std::string& alphabet, std::ifstream& finC, std::ifstream& finQ, const double& rank){
    char table[256];
    std::string best_neighbour, Otext, neighbour;
    double Best_Rank=rank, S;
    for(int i=0;i<key.size();++i){
        for(int j=1;j<key.size()-i;++j){
            neighbour=key;
            std::swap(neighbour[i], neighbour[i+j]);
            MakeTable(neighbour, alphabet, table);
            Otext=Decrypt(finC, table);
            //S=LogRank(Otext, finQ);
            if(S>Best_Rank){
                Best_Rank=S;
                best_neighbour=neighbour;
            }
        }
    }
    return best_neighbour;
}

void MakeRankedTable(std::ifstream& fin, float table[33][33][33][33]){
    fin.clear();
    fin.seekg(0, std::ios::beg);
    std::string buff;

    while(std::getline(fin,buff)){
           table[ static_cast<unsigned char> (buff[0]) ][ static_cast<unsigned char> (buff[1]) ][ static_cast<unsigned char> (buff[2]) ][ static_cast<unsigned char> (buff[3]) ]=std::stoi(buff.substr(5));
    }
       
}
