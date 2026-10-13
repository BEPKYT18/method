//юабцде╗фгхийклмнопярстужвьызшэщчъ
//йигзэьълщшж╗яефбнтчдвхаоупкюмысрц - ЙКЧВ

#include "funcs.cpp"

int main(){
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::ifstream fin1("Ctext.txt", std::ios::binary);
    std::ifstream fin2("quadgrams.txt", std::ios::binary);

    std::string alphabet="юабцде╗фгхийклмнопярстужвьызшэщчъ", key;
    std::string Otext;
    char table1[256], table2[256];
    
    double S1=0, S2=0;

    key=CreateKey(alphabet);
    std::cout<<key<<'\n';
    MakeTable(key, alphabet, table1);
    Otext=Decrypt(fin1, table1);
    S1=LogRank(Otext, fin2);
    //std::cout<<BestNeighbour(key, alphabet, fin1, fin2, S1);
    std::cout<<S1;

    return 0;
}