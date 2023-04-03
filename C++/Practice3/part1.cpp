#include <iostream>
#include <string>
#include <cstdio>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

enum type_of_lex {
    LEX_NULL,                                                                                   
    LEX_AND, LEX_REAL, LEX_DO, LEX_WHILE, LEX_INT,		                                        
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_CONT, LEX_WRITE,		                        
    LEX_FIN,                                                                                    
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS,   
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ,              
    LEX_LFIG, LEX_RFIG, 		                                                                
    LEX_NUM, 
	LEX_ID,                                                                                   
    LEX_STR,
	LEX_REALNUMB,                                                                                                                                                                    
    POLIZ_LABEL,                                                                                
    POLIZ_ADDRESS,                                                                              
    POLIZ_GO,                                                                                   
    POLIZ_FGO                                                                                   
};

class Lex {
    type_of_lex   t_lex;
    int           v_lex;
    string        s_lex;
    double		  r_lex;
public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0, string str = "", double r = 0): t_lex (t), v_lex (v) , s_lex(str), r_lex(r) { }
    type_of_lex  get_type () const {
      return t_lex;
    }
    int get_value () const {
      return v_lex;
    }
    string get_str () const {
      return s_lex;
    }
    double get_real() const{
        return r_lex;
    }
    friend ostream & operator<< ( ostream &s, Lex l );
};

class Ident {
    string      name;
    bool        declare;
    type_of_lex type;
    bool        assign;
    int          value;
public:
    Ident() {
      declare = false;
      assign  = false;
    }
    bool operator== ( const string& s ) const {
      return name == s;
    }
    Ident ( const string n ) {
      name    = n;
      declare = false;
      assign  = false;
    }
    string get_name () const {
      return name;
    }
    bool get_declare () const {
      return declare;
    }
    void put_declare () {
      declare   = true;
    }
    type_of_lex get_type () const {
      return type;
    }
    void put_type ( type_of_lex t ) {
      type      = t;
    }
    bool get_assign () const {
      return assign;
    }
    void put_assign () {
      assign    = true;
    }
    int  get_value () const {
      return value;
    }
    void put_value ( int v ) {
      value     = v;
    }
};

vector<Ident> TID;

int put ( const string & buf ) {
  vector<Ident>::iterator k;

  if ( ( k = find ( TID.begin (), TID.end (), buf ) ) != TID.end () )
    return k - TID.begin();
  TID.push_back ( Ident(buf) );
  return TID.size () - 1;
}

class Scanner {
    FILE * fp;
    char   c;
    int look ( const string buf, const char ** list ) {
      int i = 0;
      while ( list[i] ) {
        if ( buf == list[i] )
          return i;
        ++i;
      }
      return 0;
    }
    void gc () {
      c = fgetc (fp);
    }
public:
    static const char * TW [], * TD [];
    Scanner ( const char * program ) {
      if ( !(fp = fopen ( program, "r" )) )
        throw  "can't open file" ;
    }
    Lex get_lex ();
};

const char *
    Scanner::TW    [] = { "", "and", "real", "do","while", "int",
                          "not", "or","program","read", "continue","write",
                          NULL };

const char *
    Scanner::TD    [] = { "", ";", ",", ":", "=", "(", ")", "==", "<",
                          ">", "+", "-","*", "/", "<=", "!=", ">=",
                          "{", "}",NULL };

Lex Scanner::get_lex () {
  enum    state { H, IDENT, NUMB, ALE, STR, COM, REAL};
  int     d = 0, j, t = 10;
  double dd;
  string  buf;
  state   CS = H;
  do {
    gc ();
    switch ( CS ) {
      case H:
        if ( c==' ' || c == '\n' || c== '\r' || c == '\t' );
        else if ( isalpha (c) ) {                            
          buf.push_back (c);
          CS  = IDENT;
        }
        else if ( isdigit (c) ) {
          d   = c - '0';
          CS  = NUMB;
        }
        else if (c == EOF)
          return Lex ( LEX_FIN );
        else if ( c == '=' || c == '<' || c == '>' || c == '!') {
          buf.push_back (c);
          CS  = ALE;
        }
        else if ( c == '"') {
          CS = STR;
        }
        else if ( c == '/') {
          gc();
          if ( c == '*')
            CS = COM;
          else {
            ungetc(c, fp);
            buf.push_back('/');
            if ( ( j = look ( buf, TD) ) ){
              return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
            }
            else {
              throw c;
            }
          }

        }
        else {           
          buf.push_back (c);

          if ( ( j = look ( buf, TD) ) ){
            return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
          }
          else {
            throw c;
          }
        }
        break;
      case IDENT:
        if ( isalpha (c) || isdigit (c) ) {
          buf.push_back (c);
        }
        else {
          ungetc ( c, fp );
          if ( (j = look ( buf, TW) ) ) {
            return Lex ( (type_of_lex) j, j );
          }
          else {
            j   = put ( buf );
            return Lex ( LEX_ID, j );
          }
        }
        break;
      case NUMB:
        if (isdigit(c)) {
        	d = d * 10 + (c - '0');
        } else if (c == '.') {
            dd = d;
            CS = REAL;
        }
        else {
            ungetc(c, fp);
            return Lex(LEX_NUM, d);
        }
        break;
      case REAL:
        if (isdigit(c)){
            dd = dd + (1.0 * (c - '0'))/t;
            t *= 10;
        } else if (int(c) == EOF) {
            throw c;
        } else
            return Lex(LEX_REALNUMB, 0, "",dd);
        break;
      case ALE:
        if ( c == '=' ) {
          buf.push_back ( c );
          j   = look ( buf, TD );
          return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
        }
        else {
          ungetc ( c, fp );
          j   = look ( buf, TD );
          return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
        }
        break;
      case STR:
        if ( c != '"' )
          buf.push_back(c);
        else {
          return Lex (LEX_STR, 0, buf);
        }
      case COM:
        if ( c == EOF )
          throw c;
        else if ( c == '*' ) {
          gc();
          if ( c == '/' )
            CS = H;
          else if ( c == EOF )
            throw c;
        }
    } 
  } while (true);
}

ostream & operator<< ( ostream &s, Lex l ) {
  string t;
  if ( l.t_lex <= LEX_WRITE )
    t = Scanner::TW[l.t_lex];
  else if ( l.t_lex >= LEX_FIN && l.t_lex <= LEX_RFIG )
    t = Scanner::TD[ l.t_lex - LEX_FIN ];
  else if ( l.t_lex == LEX_NUM )
    t = "NUMB";
  else if (l.t_lex == LEX_STR ){
  	t = "STR";
  	s << '(' << t << ',' << l.s_lex << ");" << endl;
    return s;
  }
  else if(l.t_lex == LEX_REALNUMB){
        t = "REAL NUMB";
        s << '(' << t << ',' << l.r_lex << ");" << endl;
        return s;
	}
  else if ( l.t_lex == LEX_ID )
    t = TID[l.v_lex].get_name ();
  else if ( l.t_lex == POLIZ_LABEL )
    t = "Label";
  else if ( l.t_lex == POLIZ_ADDRESS )
    t = "Addr";
  else if ( l.t_lex == POLIZ_GO )
    t = "!";
  else if ( l.t_lex == POLIZ_FGO )
    t = "!F";
  else
    throw l;
  s << '(' << t << ',' << l.v_lex << ");" << endl;
  return s;
}



int main () {
  try {
    Scanner scan("prog.txt");
    while(1){
      Lex l = scan.get_lex();
      if (l.get_type() == LEX_FIN )
        break;
      cout << l;
    }

  }
  catch ( char c ) {
    cout << "unexpected symbol " << int(c) << ' ' << c << endl;
    return 1;
  }
  catch ( Lex l ) {
    cout << "unexpected lexeme" << l << endl;
    return 1;
  }
  catch ( const char *source ) {
    cout << source << endl;
    return 1;
  }
  return 0;
}
