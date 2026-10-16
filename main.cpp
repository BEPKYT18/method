//ÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß
//ÊÉÇÚÜØßÌİÛÖ¨ÑÅÆÂÎÔŞÄ×ÈÁÏÕĞËÀÍÙÓÒÃ - êëş÷

#include "funcs.cpp"

int main(){
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::ifstream fin1("Ctext.txt", std::ios::binary);
    std::ifstream fin2("quadgrams.txt", std::ios::binary);

    std::string alphabet="ÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß", key;
    std::string Otext;
    char table1[256], table2[256];

    float quadgrams[26*26*26*26];
    MakeQuadgrams(quadgrams, fin2);

    double S1=0, S2=0;

    key=CreateKey(alphabet);
    std::cout<<key<<'\n';
    MakeTable(key, alphabet, table1);
    Otext=Decrypt(fin1, table1);
    S1=LogRank(Otext, quadgrams);
    std::cout<<S1<<'\n';
    std::cout<<BestNeighbour(key, alphabet, fin1, S1, quadgrams)<<'\n';
    return 0;
}