#include "lex.h"
//#include "lex.cpp"
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <cctype>
using namespace std;

int main(int argc,char* argv[]){
    ifstream inputFile;
    string fileName;
    bool all=false,integer=false,real=false,str=false,id=false,kw=false;
    map <string,int> identifierMap,stringsMap;
    map <int,int> integerMap;
    map <double,int>realMap;
    map<string,int> keyWordMAP;
    if(argc!=1){
        fileName=argv[1];
    }
    else{
        cerr << "NO SPECIFIED INPUT FILE."<<endl;
        exit(1);
        
    }
    for(int i=2;i<argc;i++){
        if(string(argv[i]).substr(0,1)!="-"){
            cerr<<"ONLY ONE FILE NAME IS ALLOWED."<<endl;
            exit(1);
        }
        if(string(argv[i])=="-all"){
            all=true;
        }
        if(string(argv[i])=="-int"){
            integer=true;
        }
        if(string(argv[i])=="-real"){
            real=true;
        }
        if(string(argv[i])=="-str"){
            str=true;
        }
        if(string(argv[i])=="-id"){
            id=true;
        }
        if(string(argv[i])=="-kw"){
            kw=true;
        }
        if(string(argv[i])=="-hello"){
            cerr<<"UNRECOGNIZED FLAG {"<<argv[i]<<"}"<<endl;
            exit(1);
        }
    }
    inputFile.open(fileName.c_str());
    if(!inputFile.is_open()){
        cerr << "CANNOT OPEN THE FILE "<<argv[1]<<endl;
        exit(1);
    }
    string currentLine,currentWord;
    int totalNumLines=0,totalNumTokens=0;
    LexItem token = getNextToken(inputFile,totalNumLines);

    while(token.GetToken()!=DONE){
        totalNumTokens++;

        if(all){
            if(token.GetToken()== ERR){
            cout << token << endl;
            exit(1);
        }
            cout<< token <<endl;
        }
    
        if(token.GetToken()==IDENT){
            identifierMap[token.GetLexeme()]++;
        }else if(token.GetToken()==RCONST){
            realMap[stod(token.GetLexeme())]++;
        }else if(token.GetToken()==SCONST){
            stringsMap["\"" + token.GetLexeme().substr(1,token.GetLexeme().length()-2)+"\""]++;
        }else if(token.GetToken()==ICONST){
            integerMap[stoi(token.GetLexeme())]++;
        }else if(token.GetToken()==PROGRAM || token.GetToken()==IF || token.GetToken()==ELSE || token.GetToken()==PRINT || token.GetToken()==INTEGER || token.GetToken()==REAL || token.GetToken()== CHARACTER || token.GetToken()==END || token.GetToken()==THEN || token.GetToken()==LEN){
            keyWordMAP[token.GetLexeme()]++;
        }
        
        token=getNextToken(inputFile,totalNumLines);
        //if(token.GetToken()==ERR){
        //cerr<<"Error in line "<<totalNumLines<<": Unrecognized Lexeme {"<<token.GetLexeme()<<"}"<<endl;
        //exit(1);
   // }
    }
    if(totalNumLines==0){
        cout<<"Empty File."<<endl;
        exit(1);
    }
    /*if(token.GetToken()==ERR){
        cerr<<"Error in line "<<totalNumLines<<": Unrecognized Lexeme {"<<token.GetLexeme()<<"}"<<endl;
        exit(1);
    }*/
    cout<<endl;
    cout<<"Lines: "<<totalNumLines<<endl;
    cout<<"Total Tokens: "<<totalNumTokens<<endl;
    cout<<"Identifiers: "<<identifierMap.size()<<endl;
    cout<<"Integers: "<<integerMap.size()<<endl;
    cout<<"Reals: "<<realMap.size()<<endl;
    cout<<"Strings: "<<stringsMap.size()<<endl;
    
    if(id==true){
        cout<<"IDENTIFIERS:"<<endl;
        for(auto i=identifierMap.begin();i!=identifierMap.end();i++){
        cout<<i->first;
        cout<<" ("<<i->second<<")";
        if(i!=--identifierMap.end()){
            cout<<", ";
        }
      }
      cout<<endl;
    }
    if(kw==true){
        cout<<"KEYWORDS:"<<endl;
        for(auto i=keyWordMAP.begin();i!=keyWordMAP.end();i++){
        cout<<i->first;
        cout<<" ("<<i->second<<")";
        if(i!=--keyWordMAP.end()){
            cout<<", ";
        }
      }
      cout<<endl;   
    }
    if(integer==true){
      cout<<"INTEGERS:"<<endl;
      for(auto i=integerMap.begin();i!=integerMap.end();i++){
        cout<<i->first;
        if(i!=--integerMap.end()){
            cout<<", ";
        }
      }
      cout<<endl;
    }
    if(real==true){
        cout<<"REALS:"<<endl;
        for(auto i=realMap.begin();i!=realMap.end();i++){
        cout<<i->first;
        if(i!=--realMap.end()){
            cout<<", ";
        }
      }
      cout<<endl;   
    }
    if(str==true){
        cout<<"STRINGS:"<<endl;
        for(auto i=stringsMap.begin();i!=stringsMap.end();i++){
        cout<<i->first;
        if(i!=--stringsMap.end()){
            cout<<", ";
        }
      }
      cout<<endl;   
    }
    return 0;
}