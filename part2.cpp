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
        	ungetc(c, fp);
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



class Parser {
    Lex          curr_lex;
    type_of_lex  c_type;
    int          c_val;
    string       c_str;
    Scanner      scan;
    void  program();
    void  descripts();
    void  descript();
    void  variable();
    void  operats();
    void  operat();
    void  conditional();
    void cond1();
    void cond2();
    void cond3();
    void cond4();
    void cond5();
    void cond6();

    void  gc() {
      curr_lex  = scan.get_lex ();
      c_type    = curr_lex.get_type ();
      c_val     = curr_lex.get_value ();
      c_str     = curr_lex.get_str();
    }
public:
    Parser ( const char *program ) : scan (program) { }
    void  analyze();
};

void Parser::analyze () {
  program ();
  if (c_type != LEX_FIN)
    throw curr_lex;
}

void Parser::program() {
  gc();
  if ( c_type == LEX_PROGRAM ) {
    gc();
  }
  else
    throw curr_lex;

  if ( c_type == LEX_LFIG ) {}
  else
    throw curr_lex;

  descripts();
  operats();

  if ( c_type == LEX_RFIG )
    gc();
  else
    throw curr_lex;
}

void Parser::descripts() {

  do {
    gc();
    descript();
  } while ( c_type == LEX_SEMICOLON );
}

void Parser::descript() {
  if ( c_type == LEX_REAL or c_type == LEX_INT or c_type == LEX_STR )
    do {
      gc();
      variable();
    } while ( c_type == LEX_COMMA );
}

void Parser::variable() {
  if ( c_type == LEX_ID )
    gc();
  else
    throw curr_lex;

  if ( c_type == LEX_ASSIGN ) {
    gc();
    if ( c_type == LEX_REALNUMB or c_type == LEX_NUM or c_type == LEX_STR )
      gc();
    else
      throw curr_lex;
  }
}

void Parser::operats() {
  operat();
  while ( c_type == LEX_SEMICOLON ) {
    gc();
    operat();
  }
}

void Parser::operat() {
  if ( c_type == LEX_DO ) {
    gc();
    if ( c_type == LEX_LFIG )
      gc();
    else
      throw curr_lex;
    operat();
    if ( c_type == LEX_SEMICOLON )
    	gc();
    else
      throw curr_lex;	
    if ( c_type == LEX_RFIG )
      gc();
    else
      throw curr_lex;
    if ( c_type == LEX_WHILE )
      gc();
    else
      throw curr_lex;
    if ( c_type == LEX_LPAREN )
      gc();
    else
      throw curr_lex;
    cond6();
    if ( c_type == LEX_RPAREN )
      gc();
    else
      throw curr_lex;
  }
  else if ( c_type == LEX_CONT ) {
    gc();
  }

  else if ( c_type == LEX_READ ) {
    gc();

    if ( c_type == LEX_LPAREN )
      gc();
    else
      throw curr_lex;

    if ( c_type == LEX_ID )
      gc();
    else
      throw curr_lex;

    if ( c_type == LEX_RPAREN )
      gc();
    else
      throw curr_lex;
  }
  else if ( c_type == LEX_WRITE ) {
    gc();
    if ( c_type == LEX_LPAREN)
      gc();
    else
      throw curr_lex;

	 do {
      cond6();
    } while ( c_type == LEX_COMMA );

    if ( c_type == LEX_RPAREN )
      gc();
    else
      throw curr_lex;
	}
	else{
		conditional();
	}
}






void Parser::conditional() {
  cond6();
  if ( c_type == LEX_ASSIGN ) {
    gc();
    cond6();
  }
}

void Parser::cond6() {
  cond5();
  if ( c_type == LEX_OR ) {
    gc();
    cond5();
  }
}

void Parser::cond5() {
  cond4();
  if ( c_type == LEX_AND ) {
    gc();
    cond4();
  }
}

void Parser::cond4() {
  cond3();
  if ( c_type == LEX_GTR or c_type == LEX_LSS or c_type == LEX_GEQ or c_type == LEX_EQ or c_type ==  LEX_LEQ
        or c_type == LEX_NEQ) {
    gc();
    cond3();
  }
}

void Parser::cond3() {
  cond2();
  if ( c_type == LEX_PLUS or c_type == LEX_MINUS ) {
    gc();
    cond2();
  }
}

void Parser::cond2() {
  cond1();
  if ( c_type == LEX_TIMES or c_type == LEX_SLASH ) {
    gc();
    cond1();
  }
}

void Parser::cond1() {
  if ( c_type == LEX_ID or c_type == LEX_NUM or c_type == LEX_REALNUMB or c_type == LEX_STR)
    gc();
  else if ( c_type == LEX_NOT ) {
    gc();
    cond1();
  }
  else if ( c_type == LEX_LPAREN ) {
    gc();
    conditional();
    if ( c_type == LEX_RPAREN )
      gc();
    else
      throw curr_lex;

  }
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
    Parser parse("prog.txt");
    parse.analyze();
    cout << "Analyze end. Success" << endl;
  }
  catch ( char c ) {
    cout << "Unexpected symbol " << int(c) << ' ' << c << endl;
    return 1;
  }
  catch ( Lex l ) {
    cout << "Unexpected lexeme" << l << endl;
    return 1;
  }
  catch ( const char *source ) {
    cout << source << endl;
    return 1;
  }
  return 0;
}
