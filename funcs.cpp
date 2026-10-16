#include <fstream>
#include <iostream>
#include <windows.h>
#include <math.h>
#include <string>
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>

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

void MakeTable(const std::string& key, const std::string& alphabet, char* table){
    for(int i=0;i<alphabet.size();++i){
        table[static_cast<unsigned char>(key[i])]=alphabet[i];
    }
}//создание таблицы для расшифрования текста по ключу

std::string Decrypt(std::ifstream& fin, const char* table){
    fin.clear();
    fin.seekg(0, std::ios::beg);
    char c;
    std::string Otext;
    while(fin.get(c)){
       Otext+=table[static_cast<unsigned char>(c)];
    }
    return Otext;
}/*расшифрование текста по ключу(о.т. идёт в строку, до этого вообще делал в файл, но в планах 
передавать не весь текст, а отдельную четырёхграмму. Но тогда у меня будет больше вызовов функции
на каждой четырёхграмме*/

size_t CodingQuadgram(const std::string buff){
    size_t index=static_cast<unsigned char>(buff[0])*26*26*26+
        static_cast<unsigned char>(buff[1])*26*26+
        static_cast<unsigned char>(buff[2])*26+static_cast<unsigned char>(buff[3]);
        return index;
}/*перекодировка четырёхграммы в индекс массива*/

void MakeQuadgrams(float* arr, std::ifstream& fin){
    fin.clear();
    fin.seekg(0, std::ios::beg);
    std::string buff;
    size_t index;
    while(std::getline(fin,buff)){
        index=CodingQuadgram(buff.substr(0,4));
        arr[index]=std::stoi(buff.substr(5));
    }
    //создание массива с рейтингами четырёхграмм
}

double LogRank(const std::string& Otext, const float* CodedQuadgrams){
    std::string word="";

    double S=0;

    for(int i=0;i<Otext.size()-3;++i){
        for(int j=0;j<4;++j){
            word+=Otext[i+j];   /*идём по о.т. и берём четырёхгаммы (в планах идти по ш.т.
                                и расшифровывать каждую четырёхграмму или даже символ,
                                но тогда будет много вызовов функций, не знаю, как будет 
                                по памяти и времени)*/
        }
        
        S+=CodedQuadgrams[CodingQuadgram(word)];

        word="";
    }
    return S;
}/*подсчёт рейтинга ключа. До этого каждый раз шёл по файлу с четырёхграммами
- очень долго даже для одного ключа, поэтому сначала делаю перекодировку всего 
файла в в массив*/

std::string BestNeighbour(const std::string& key, const std::string& alphabet, 
    std::ifstream& fin, const double& rank, const float* CodedQuadgrams){
    char table[256];
    std::string best_neighbour, Otext, neighbour;
    double Best_Rank=rank, S;

    for(int i=0;i<key.size();++i){//алгоритм поиска лучшего соседа.
        for(int j=1;j<key.size()-i;++j){/*перебираем всех соседей и берём лучшего
                                        (меньше шанс попасть в локальный максимум).*/
            neighbour=key;

            std::swap(neighbour[i], neighbour[i+j]);
            MakeTable(neighbour, alphabet, table);
            Otext=Decrypt(fin, table);
            S=LogRank(Otext, CodedQuadgrams);

            if(S>Best_Rank){//если рейтинг соседа лучший, то он становиться лучшим соседом
                Best_Rank=S;
                best_neighbour=neighbour;
            }
        }
    }
    return best_neighbour;
}


/*основная проблема - это очень долгий подсчёт рейтинга. Уже принял какие-то изменения.
Также есть проблема попадания в локальный максимум, потому нужно находить лучших по разным 
ключам и брать лучший из них(сколько раз так делать, тоже не понятно)
*/