#include "lex.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <map>
using namespace std;

map<string, Token> stringToToken={
		{"PROGRAM", PROGRAM },
		{"PRINT", PRINT},
		{"IF",  IF},
		{"ELSE" ,  ELSE},	
		//{ "IDENT", IDENT },
		{ "END", END },
		{ "INTEGER", INTEGER },
		{ "REAL", REAL },
		{ "CHARACTER", CHARACTER },
		{ "THEN", THEN },
        {"LEN",LEN},
};
map<Token,string> tokenToString= {
    {PROGRAM, "PROGRAM"},
    {PRINT,"PRINT"},
    {IF,"IF"},
    {ELSE,"ELSE"},
    {IDENT,"IDENT"},
    {END,"END"},
    {INTEGER,"INTEGER"},
    {REAL,"REAL"},
    {CHARACTER,"CHARACTER"},
    {THEN,"THEN"},
    {LEN,"LEN"},
};
ostream& operator<<(ostream& out, const LexItem& tok){
    Token token=tok.GetToken();
    if(token==IDENT){
        out<<"IDENT: '"<<tok.GetLexeme()<<"'";
    }
    if(token>=IF && token<=LEN){
        out<<tokenToString[token];
    }
    if(token==ICONST){
        out<<"ICONST: ("<<tok.GetLexeme()<<")";
    }
    if(token==RCONST){
        out<<"RCONST: ("<<tok.GetLexeme()<<")";
    }
    if(token==SCONST){
        out<<"SCONST: \""<<tok.GetLexeme().substr(1,tok.GetLexeme().length()-2)<<"\"";
    }
    if(token==DOT){
        out<<"DOT";
    }
    if(token==DCOLON){
        out<<"DCOLON";
    }
    if(token==COMMA){
        out<<"COMMA";
    }
    if(token==DEF){
        out<<"DEF";
    }
    if(token==RPAREN){
        out<<"RPAREN";
    }
    if(token==LPAREN){
        out<<"LPAREN";
    }
    if(token==EQ){
        out<<"EQ";
    }
    if(token==ASSOP){
        out<<"ASSOP";
    }
    if(token==MULT){
        out<<"MULT";
    }
    if(token==GTHAN){
        out<<"GTHAN";
    }
    if(token==LTHAN){
        out<<"LTHAN";
    }
    if(token==POW){
        out<<"POW";
    }
    if(token==PLUS){
        out<<"PLUS";
    }
    if(token==MINUS){
        out<<"MINUS";
    }
    if(token==DIV){
        out<<"DIV";
    }
    if(token==CAT){
        out<<"CAT";
    }
    if(token==ERR){
        out<<"Error in line "<<tok.GetLinenum()+1<<": Unrecognized Lexeme {"<<tok.GetLexeme()<<"}";
    }    
    return out;

}
LexItem id_or_kw(const string& lexeme, int linenum){
    Token returnToken=IDENT;
    string newLexeme="";
    for(int i=0; i<lexeme.length();i++){
        newLexeme+=toupper(lexeme[i]);
    }
    auto it=stringToToken.find(newLexeme);
    if(it!=stringToToken.end()){
        returnToken=it->second;
    }
    return LexItem(returnToken,lexeme,linenum);
}
string currentWord;
LexItem getNextToken(istream& in, int& linenum){
    enum TokState{START, INID, INSTRING, ININT,INREAL,INCOMMENT}lexstate=START;
    string lexeme="";
    char ch;
    while(in.get(ch)){
        //cout<<ch<<endl;
        if(lexstate==START){
            if(ch=='\n'){
                linenum++;
            }
             if(isspace(ch)){
                 continue;
            }

            lexeme=ch;
            if(isdigit(ch)){
                //lexeme += ch;
                lexstate=ININT;
            }else if(isalpha(ch)){
                //lexeme += ch;
                lexstate=INID;
            }else if(ch=='\"' || ch=='\''){
                //lexeme += ch;
                lexstate=INSTRING;
            }else if(ch=='!'){
                in.putback(ch);
                lexstate=INCOMMENT;
            }else if(ch=='.'&& isdigit(in.peek())){
                //lexeme += ch;
                lexstate=INREAL;
            }
            else{
                ///PUT ALL SINGLES HERE
                Token tokenHolder = ERR;
                if(ch == '+'){
                    lexeme+=ch;
                    tokenHolder=PLUS;
                }else if(ch=='-'){
                    lexeme+=ch;
                    tokenHolder=MINUS;
                }else if(ch=='='){
                    lexeme+=ch;
                    tokenHolder=ASSOP;
                    if(in.peek()=='='){
                        in.get(ch);
                        lexeme+=ch;
                        tokenHolder=EQ;
                    }
                }else if(ch=='*'){
                    lexeme+=ch;
                    tokenHolder=MULT;
                    if(in.peek()=='*'){
                        in.get(ch);
                        lexeme+=ch;
                        tokenHolder=POW;
                    }else if(in.peek()==','){
                        tokenHolder=DEF;
                    }
                }else if(ch==','){
                    lexeme+=ch;
                    tokenHolder=COMMA;
                }else if(ch=='/'){
                    lexeme+=ch;
                    tokenHolder=DIV;
                    if(in.peek()=='/'){
                        in.get(ch);
                        lexeme+=ch;
                        tokenHolder=CAT;
                    }
                }else if(ch=='>'){
                    lexeme+=ch;
                    tokenHolder=GTHAN;
                }else if(ch=='<'){
                    lexeme+=ch;
                    tokenHolder=LTHAN;
                }else if(ch=='('){
                    lexeme+=ch;
                    tokenHolder=LPAREN;
                }else if(ch==')'){
                    lexeme+=ch;
                    tokenHolder=RPAREN;
                }else if(ch==':'&& in.peek()==':'){
                    lexeme+=ch;
                    in.get(ch);
                    lexeme+=ch;
                    tokenHolder=DCOLON;
                }else if(ch=='.'){
                    lexeme+=ch;
                    tokenHolder=DOT;
                }
                return LexItem(tokenHolder,lexeme,linenum);
            }
           // if(lexeme.empty()){
             //   return LexItem(DONE,lexeme,linenum);
            //}else{
              //  return LexItem(ERR,lexeme,linenum);
            //}
            continue;
        }
        if(lexstate==ININT){
            if(isdigit(ch)){
                lexeme+=ch;
            }else if(ch=='.'){
                lexeme+=ch;
                lexstate=INREAL;
                continue;
            }else{
                in.putback(ch);
                return LexItem(ICONST,lexeme,linenum);
            }
            continue;
        
        }
        if(lexstate==INCOMMENT){
           // in.putback(ch);
            while(ch!= '\n'){
                in.get(ch);
            }            
            if(ch=='\n'){
                linenum++;
                
            }
            lexstate = START;
            continue;
        }
        if(lexstate==INSTRING){
            if(ch=='\"'){
                lexeme += ch;
                if(lexeme[0] =='\"' ){
                    return LexItem(SCONST,lexeme,linenum);
                }
                return LexItem(ERR,lexeme,linenum);
            }else if(ch=='\''){
                lexeme+=ch;
                if(lexeme[0]=='\''){
                    return LexItem(SCONST,lexeme,linenum);
                }
                return LexItem(ERR,lexeme,linenum);
            }else if(ch=='\n'){
                return LexItem(ERR,lexeme,linenum);
            }
            else{
                lexeme+=ch;
            }
            continue;
        }
        if(lexstate==INID){
            if(isalnum(ch)|| ch=='_'){
                lexeme+=ch;
            }
            else{
                in.putback(ch);
                return id_or_kw(lexeme,linenum);
            }
            continue;
        }
        if(lexstate==INREAL){
            if(isdigit(ch)){
                lexeme+=ch;
            }else if(ch=='.'){
                lexeme+=ch;
                return LexItem(ERR,lexeme,linenum);
                }
                else{
                    in.putback(ch);
                    return LexItem(RCONST,lexeme,linenum);
                }
                continue;
        }
    }
    if(lexeme.empty()){
        return LexItem(DONE,lexeme,linenum);
        }else{
            return LexItem(ERR,lexeme,linenum);
        }
}