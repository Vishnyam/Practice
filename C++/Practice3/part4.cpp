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
    LEX_AND, LEX_REAL, LEX_STRING, LEX_DO, LEX_WHILE, LEX_INT,
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
		float		  r_lex;
	public:
		Lex ( type_of_lex t = LEX_NULL, int v = 0, string str = "", float r = 0): t_lex (t), v_lex (v) , s_lex(str), r_lex(r) { }
		type_of_lex  get_type () const {
			return t_lex;
		}
		int get_value () const {
			return v_lex;
		}
		string get_str () const {
			return s_lex;
		}
		double get_real() const {
			return r_lex;
		}
		friend ostream & operator<< ( ostream &s, Lex l );
};

class Ident {
		string      name;
		bool        declare;
		type_of_lex type;
		bool        assign;
		int         value;
		string      s_value;
		float		r_value;
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
		string get_str () const {
			return s_value;
		}
		float get_real () const {
			return r_value;
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
		void put_str (string s) {
			s_value = s;
		}
		void put_real (float r) {
			r_value = r;
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
Scanner::TW    [] = { "", "and", "real", "string", "do","while", "int",
                      "not", "or","program","read", "continue","write",
                      NULL
                    };

const char *
Scanner::TD    [] = { "", ";", ",", ":", "=", "(", ")", "==", "<",
                      ">", "+", "-","*", "/", "<=", "!=", ">=",
                      "{", "}",NULL
                    };

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
				} else if ( isdigit (c) ) {
					d   = c - '0';
					CS  = NUMB;
				} else if (c == EOF)
					return Lex ( LEX_FIN );
				else if ( c == '=' || c == '<' || c == '>' || c == '!') {
					buf.push_back (c);
					CS  = ALE;
				} else if ( c == '"') {
					CS = STR;
				} else if ( c == '/') {
					gc();
					if ( c == '*')
						CS = COM;
					else {
						ungetc(c, fp);
						buf.push_back('/');
						if ( ( j = look ( buf, TD) ) ) {
							return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
						} else {
							throw c;
						}
					}

				} else {
					buf.push_back (c);

					if ( ( j = look ( buf, TD) ) ) {
						return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
					} else {
						throw c;
					}
				}
				break;
			case IDENT:
				if ( isalpha (c) || isdigit (c) ) {
					buf.push_back (c);
				} else {
					ungetc ( c, fp );
					if ( (j = look ( buf, TW) ) ) {
						return Lex ( (type_of_lex) j, j );
					} else {
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
				} else {
					ungetc(c, fp);
					return Lex(LEX_NUM, d);
				}
				break;
			case REAL:
				if (isdigit(c)) {
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
				} else {
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

template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
	i = st.top();
	st.pop();
}

class Parser {
		Lex          curr_lex;
		type_of_lex  c_type;
		int          c_val;
		string       c_str;
		Scanner      scan;
		stack < int >           st_cont;
		stack < Lex >           st_lex;
		stack < type_of_lex >   st_tlex;
		bool comp_flag;
		type_of_lex dec_type;
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
		void  dec ( type_of_lex type);
		void  check_id ();
		void  check_op ();
		void  check_not ();
		void  eq_type ();
		void  eq_bool ();
		void  check_id_in_read ();

		void  gc() {
			curr_lex  = scan.get_lex ();
			c_type    = curr_lex.get_type ();
			c_val     = curr_lex.get_value ();
			c_str     = curr_lex.get_str();
			cout << curr_lex;
		}
	public:
		vector <Lex> poliz;
		Parser ( const char *program ) : scan (program) { }
		void  analyze();
};

void Parser::analyze () {
	comp_flag = false;
	program ();
	if (c_type != LEX_FIN)
		throw curr_lex;
}

void Parser::program() {
	gc();
	if ( c_type == LEX_PROGRAM ) {
		gc();

	} else
		throw curr_lex;

	if ( c_type == LEX_LFIG ) {
		gc();

	} else
		throw curr_lex;
	descripts();
	operats();

	if ( c_type == LEX_RFIG ) {
// 	cout << "RFIG\n";
		gc();
	}

	else
		throw curr_lex;
}

void Parser::descripts() {
	descript();
	do {
		gc();
		descript();

	} while ( c_type == LEX_SEMICOLON );
}

void Parser::descript() {
	if ( c_type == LEX_REAL or c_type == LEX_INT or c_type == LEX_STRING) {
		if (c_type == LEX_REAL ) {
			dec_type = LEX_REALNUMB;
		}

		else if ( c_type == LEX_INT ) {
			dec_type = LEX_NUM;
		}

		else
			dec_type = LEX_STR;

		do {
			gc();
			variable();
		} while ( c_type == LEX_COMMA );
		/*
		    if ( c_type == LEX_SEMICOLON )
		    {
		    	cout << "SEMILICON\n";
				gc();
			}

		    else
		      throw curr_lex;*/
		dec(dec_type );
	}
}

void Parser::variable() {
//	cout << dec_type << " start program\n"  ;
	if ( c_type == LEX_ID ) {
		st_lex.push ( curr_lex );
		st_tlex.push ( dec_type );
		poliz.push_back (Lex ( POLIZ_ADDRESS, c_val ) );
		gc();
	}

	else
		throw curr_lex;

	if ( c_type == LEX_ASSIGN ) {
		gc();
		if ( c_type == LEX_REALNUMB or c_type == LEX_NUM or c_type == LEX_STR ) {
			st_tlex.push( c_type );
			eq_type();
			poliz.push_back ( Lex ( curr_lex ) );
			poliz.push_back ( Lex ( LEX_ASSIGN ) );
			gc();
		} else
			throw curr_lex;
	} else st_tlex.pop();
}

void Parser::operats() {
	operat();
	while ( c_type == LEX_SEMICOLON or comp_flag) {
		comp_flag = false;
		gc();
		operat();
	}
}

void Parser::operat() {
	int pl0, pl1, pl2, pl3;
	if ( c_type == LEX_DO ) {
		gc();
		if ( c_type == LEX_LFIG )
			gc();
		else
			throw curr_lex;
		pl2 = poliz.size();
		//    st_cont.push(pl2);
		operats();
		if ( c_type == LEX_RFIG )
			gc();
		else
			throw curr_lex;
		if ( c_type == LEX_WHILE )
			gc();
		else
			throw curr_lex;
		if (!st_cont.empty()) {
			from_st(st_cont, pl3);
			poliz[pl3] = Lex(POLIZ_LABEL, poliz.size());
		}

		if ( c_type == LEX_LPAREN )
			gc();
		else
			throw curr_lex;
		cond6();
		poliz.push_back ( Lex(LEX_NOT) );
		eq_bool();
		poliz.push_back(Lex ( POLIZ_LABEL, pl2 ));
		poliz.push_back ( Lex(POLIZ_FGO) );
		if ( c_type == LEX_RPAREN )
			gc();
		else
			throw curr_lex;
		st_cont.pop();
	} else if ( c_type == LEX_CONT ) {
		pl0 = poliz.size();
		st_cont.push(pl0);
		poliz.push_back ( Lex () );
		poliz.push_back ( Lex ( POLIZ_GO) );
		gc();
	}

	else if ( c_type == LEX_READ ) {
		gc();

		if ( c_type == LEX_LPAREN )
			gc();
		else
			throw curr_lex;

		if ( c_type == LEX_ID ) {
			check_id_in_read();
			poliz.push_back (Lex( POLIZ_ADDRESS, c_val));
			gc();
		} else
			throw curr_lex;

		if ( c_type == LEX_RPAREN )
			gc();
		else
			throw curr_lex;

		poliz.push_back (Lex (LEX_READ));
	} else if ( c_type == LEX_WRITE ) {
		gc();
		if ( c_type == LEX_LPAREN)
			gc();
		else
			throw curr_lex;

		do {
			cond6();
		} while (c_type == LEX_COMMA);

		if (c_type == LEX_RPAREN)
			gc();
		else
			throw curr_lex;
		poliz.push_back (Lex(LEX_WRITE));
	} else if (c_type == LEX_LFIG) {
		gc();
		operat();
		while (c_type == LEX_SEMICOLON) {
			gc();
			operat();
		}
		if (c_type == LEX_RFIG)
			comp_flag = true;
		else
			throw curr_lex;
	} else if (c_type == LEX_ID) {
		check_id ();
		poliz.push_back (Lex(POLIZ_ADDRESS, c_val));
		gc();
		if (c_type == LEX_ASSIGN) {
			gc();
			cond6 ();
			eq_type ();
			poliz.push_back (Lex(LEX_ASSIGN));
		} else
			throw curr_lex;
	}
}


void Parser::conditional() {
	cond6();
	poliz.push_back (Lex ( POLIZ_ADDRESS, c_val ) );
	if ( c_type == LEX_ASSIGN ) {
		gc();
		cond6();
		eq_type();
		poliz.push_back (Lex(LEX_ASSIGN));
	}
}

void Parser::cond6() {
	cond5();
	if ( c_type == LEX_OR ) {
		st_tlex.push (c_type);
		gc();
		cond5();
		check_op();
	}
}

void Parser::cond5() {
	cond4();
	if ( c_type == LEX_AND ) {
		st_tlex.push (c_type);
		gc();
		cond4();
		check_op();
	}
}

void Parser::cond4() {
	cond3();
	if ( c_type == LEX_GTR or c_type == LEX_LSS or c_type == LEX_GEQ or c_type == LEX_EQ or c_type ==  LEX_LEQ
	        or c_type == LEX_NEQ) {
		st_tlex.push (c_type);
		gc();
		cond3();
		check_op();
	}
}

void Parser::cond3() {
	cond2();
	if ( c_type == LEX_PLUS or c_type == LEX_MINUS ) {
		st_tlex.push (c_type);
		gc();
		cond2();
		check_op();
	}
}

void Parser::cond2() {
	cond1();
	if ( c_type == LEX_TIMES or c_type == LEX_SLASH ) {
		st_tlex.push (c_type);
		gc();
		cond1();
		check_op();
	}
}

void Parser::cond1() {
	if ( c_type == LEX_ID ) {
		check_id ();
		poliz.push_back ( Lex ( LEX_ID, c_val ) );
		gc();
	} else if ( c_type == LEX_NUM or c_type == LEX_REALNUMB or c_type == LEX_STR) { //вот тут что-то ещё
		st_tlex.push (c_type);
		poliz.push_back(curr_lex);
		gc();
	} else if ( c_type == LEX_NOT ) {
		gc();
		cond1();
		check_not ();
	} else if ( c_type == LEX_LPAREN ) {
		gc();
		cond6();
		if ( c_type == LEX_RPAREN )
			gc();
		else
			throw curr_lex;

	}
}


void Parser::dec (type_of_lex type) {
	Lex lexeme;
	int i;
	while (!st_lex.empty()) {
		from_st ( st_lex, lexeme );
		i = lexeme.get_value();
		if ( TID[i].get_declare () )
			throw "twice";
		else {
			TID[i].put_declare();
			TID[i].put_type(type);
		}
	}
}

void Parser::check_id () {
	if ( TID[c_val].get_declare() )
		st_tlex.push ( TID[c_val].get_type());
	else
		throw "not declared";
}

void Parser::check_op () {
	type_of_lex t1, t2, op, r;
	from_st(st_tlex, t2);
	from_st(st_tlex, op);
	from_st(st_tlex, t1);

	if (op == LEX_PLUS) {
		if (t1 == LEX_STR and t2 == LEX_STR or t1 == LEX_NUM and t2 == LEX_NUM or t1 == LEX_REALNUMB and t2 == LEX_REALNUMB)
			r = t1;
		else if (t1 == LEX_NUM and t2 == LEX_REALNUMB or t1 == LEX_REALNUMB and t2 == LEX_NUM)
			r = LEX_REALNUMB;
		else throw "wrong types are in operation";
	} else if (op == LEX_MINUS or op == LEX_SLASH or op == LEX_TIMES) {
		if (t1 == LEX_NUM and t2 == LEX_NUM or t1 == LEX_REALNUMB and t2 == LEX_REALNUMB)
			r = t1;
		else if (t1 == LEX_NUM and t2 == LEX_REALNUMB or t1 == LEX_REALNUMB and t2 == LEX_NUM)
			r = LEX_REALNUMB;
		else
			throw "wrong types are in operation";
	} else if (op == LEX_GTR or op == LEX_LSS or op == LEX_EQ or op == LEX_NEQ) {
		if (t1 == LEX_STR and t2 == LEX_STR or t1 == LEX_NUM and t2 == LEX_NUM or t1 == LEX_REALNUMB and t2 == LEX_REALNUMB or t1 == LEX_NUM and t2 == LEX_REALNUMB or t1 == LEX_REALNUMB and t2 == LEX_NUM)
			r = LEX_NUM;
		else throw "wrong types are in operation";
	} else if (op == LEX_GEQ or op == LEX_LEQ) {
		if (t1 == LEX_NUM and t2 == LEX_NUM or t1 == LEX_REALNUMB and t2 == LEX_REALNUMB or t1 == LEX_NUM and t2 == LEX_REALNUMB or t1 == LEX_REALNUMB and t2 == LEX_NUM)
			r = LEX_NUM;
		else throw "wrong types are in operation";
	} else if (op == LEX_AND or op == LEX_OR) {
		if (t1 == LEX_NUM and t2 == LEX_NUM)
			r = LEX_NUM;
		else throw "wrong types are in operation";
	} else
		throw "i dont know what went wrong";

	st_tlex.push(r);
	poliz.push_back (Lex(op));
}

void Parser::check_not () {
	if (st_tlex.top() != LEX_NUM)
		throw "wrong type is in not";
	poliz.push_back (Lex (LEX_NOT) );
}

void Parser::eq_type () {
	type_of_lex t1, t2;
	from_st (st_tlex, t1);
	from_st (st_tlex, t2);
	if (t1 == LEX_STR and t2 == LEX_STR or t1 == LEX_NUM and t2 == LEX_NUM or t1 == LEX_REALNUMB and t2 == LEX_REALNUMB or t1 == LEX_NUM and t2 == LEX_REALNUMB or t1 == LEX_REALNUMB and t2 == LEX_NUM)
		return;
	else
		throw "wrong types are in =";
}

void Parser::eq_bool () {
	if ( st_tlex.top () != LEX_NUM )
		throw "expression is not int";
	st_tlex.pop ();
}

void Parser::check_id_in_read () {
	if ( !TID [c_val].get_declare() )
		throw "not declared";
}


class Executer {
	public:
		void execute ( vector<Lex> & poliz );
};

void Executer::execute ( vector<Lex> & poliz ) {
	Lex pc_el;
	stack < Lex > args;
	int val, index = 0, size = poliz.size();
	Lex i, j;
	while ( index < size ) {
		pc_el = poliz [ index ];
        cout << index << " " << pc_el;
		switch ( pc_el.get_type () ) {
			case LEX_REALNUMB:
			case LEX_STR:
			case LEX_NUM:
			case POLIZ_ADDRESS:
			case POLIZ_LABEL:
				args.push (pc_el);
				break;
			case LEX_ID:
				val = pc_el.get_value ();
				if ( TID[val].get_assign () ) {
					args.push ( Lex ( TID[val].get_type(), TID[val].get_value (), TID[val].get_str(),  TID[val].get_real()) );
					break;
				} else
					throw "POLIZ: indefinite identifier";

			case LEX_NOT:
				from_st ( args, i );
				args.push( Lex( i.get_type(), !i.get_value() ) );
				break;

			case LEX_OR:
				from_st ( args, j );
				from_st ( args, i );
				args.push ( Lex( i.get_type(), i.get_value() || j.get_value() ) );
				break;

			case LEX_AND:
				from_st ( args, j );
				from_st ( args, i );
				args.push ( Lex( i.get_type(), i.get_value() && j.get_value() ) );
				break;

			case POLIZ_GO:
				from_st ( args, i );
				index = i.get_value() - 1;
				break;

			case POLIZ_FGO:
				from_st ( args, j );
				from_st ( args, i );
				if ( !i.get_value() ) index = j.get_value() - 1;
				break;

			case LEX_WRITE:
				from_st ( args, i );
				switch ( i.get_type() ) {
					case LEX_NUM :
						cout << i.get_value();
						break;
					case LEX_REALNUMB:
						cout << i.get_real() ;
						break;
					case LEX_STR:
						cout << i.get_str();
						break;
				}
				break;

			case LEX_READ:
				from_st ( args, i );
				val = i.get_value();
				if ( TID[val].get_type () == LEX_NUM ) {
					int k;
					cin >> k;
					TID[val].put_value(k);
				} else if ( TID[val].get_type () == LEX_REALNUMB ) {
					float k;
					cin >> k;
					TID[val].put_real(k);
				} else {
					string s;
					cin >> s;
					TID[val].put_str(s);
				}
				TID[val].put_assign ();
				break;

			case LEX_PLUS:
				from_st ( args, j );
				from_st ( args, i );
				args.push ( Lex( i.get_type(), i.get_value() + j.get_value(), i.get_str() + j.get_str(), i.get_real() + j.get_real() + i.get_value() + j.get_value()) );
				break;

			case LEX_TIMES:
				from_st ( args, j );
				from_st ( args, i );
				if (i.get_type() == j.get_type())
					args.push ( Lex( i.get_type(), i.get_value() * j.get_value(), "", i.get_real() * j.get_real()));
				else if (i.get_type() == LEX_NUM)
					args.push ( Lex( i.get_type(), 0, "", i.get_value() * j.get_real()));
				else
					args.push ( Lex( i.get_type(), 0, "", i.get_real() * j.get_value()));
				break;

			case LEX_MINUS:
				from_st ( args, j );
				from_st ( args, i );
				if (i.get_type() == j.get_type())
					args.push ( Lex( i.get_type(), i.get_value() - j.get_value(), "", i.get_real() - j.get_real()));
				else if (i.get_type() == LEX_NUM)
					args.push ( Lex( i.get_type(), 0, "", i.get_value() - j.get_real()));
				else
					args.push ( Lex( i.get_type(), 0, "", i.get_real() - j.get_value()));
				break;

			case LEX_SLASH:
				from_st ( args, j );
				from_st ( args, i );
				if (j.get_type() == LEX_NUM)
					if (j.get_value()) {
						if (i.get_type() == j.get_type())
							args.push ( Lex( i.get_type(), i.get_value() / j.get_value(), "", 0) );
						else
							args.push ( Lex( i.get_type(), 0, "",  i.get_real() / j.get_value()) );
						break;
					} else
						throw "POLIZ:divide by zero";
				else if (j.get_real()) {
					if (i.get_type() == j.get_type())
						args.push ( Lex( i.get_type(), 0, "",  i.get_real() / j.get_real()) );
					else
						args.push ( Lex( i.get_type(), 0, "",  i.get_value() / j.get_real()) );
					break;
				} else
					throw "POLIZ:divide by zero";


			case LEX_EQ:
				from_st ( args, j );
				from_st ( args, i );
				if (i.get_type() == j.get_type()) {
					if (i.get_type() == LEX_NUM)
						args.push ( Lex( LEX_NUM, i.get_value() == j.get_value()) );
					else if (i.get_type() == LEX_STR)
						args.push ( Lex( LEX_NUM, i.get_str() == j.get_str()) );
					else if (i.get_type() == LEX_REALNUMB)
						args.push ( Lex( LEX_NUM, i.get_real() == j.get_real()) );
				} else if (i.get_type() == LEX_NUM)
					args.push ( Lex( LEX_NUM, i.get_value() == j.get_real() ) );
				else
					args.push ( Lex( LEX_NUM, i.get_real() == j.get_value() ) );
				break;

			case LEX_LSS:
				from_st ( args, j );
				from_st ( args, i );
				if (i.get_type() == j.get_type()) {
					if (i.get_type() == LEX_NUM)
						args.push ( Lex( LEX_NUM, i.get_value() < j.get_value()) );
					else if (i.get_type() == LEX_STR)
						args.push ( Lex( LEX_NUM, i.get_str() < j.get_str()) );
					else if (i.get_type() == LEX_REALNUMB)
						args.push ( Lex( LEX_NUM, i.get_real() < j.get_real()) );
				} else if (i.get_type() == LEX_NUM)
					args.push ( Lex( LEX_NUM, i.get_value() <j.get_real() ) );
				else
					args.push ( Lex( LEX_NUM, i.get_real() < j.get_value() ) );
				break;

			case LEX_LEQ:
				from_st ( args, j );
				from_st ( args, i );
				if (i.get_type() == j.get_type()) {
					if (i.get_type() == LEX_NUM)
						args.push ( Lex( LEX_NUM, i.get_value() <= j.get_value()) );
					else if (i.get_type() == LEX_REALNUMB)
						args.push ( Lex( LEX_NUM, i.get_real() <= j.get_real()) );
				} else if (i.get_type() == LEX_NUM)
					args.push ( Lex( LEX_NUM, i.get_value() <= j.get_real() ) );
				else
					args.push ( Lex( LEX_NUM, i.get_real() <= j.get_value() ) );
				break;

			case LEX_GTR:
				from_st ( args, j );
				from_st ( args, i );
				if (i.get_type() == j.get_type()) {
					if (i.get_type() == LEX_NUM)
						args.push ( Lex( LEX_NUM, i.get_value() > j.get_value()) );
					else if (i.get_type() == LEX_STR)
						args.push ( Lex( LEX_NUM, i.get_str() > j.get_str()) );
					else if (i.get_type() == LEX_REALNUMB)
						args.push ( Lex( LEX_NUM, i.get_real() > j.get_real()) );
				} else if (i.get_type() == LEX_NUM)
					args.push ( Lex( LEX_NUM, i.get_value() > j.get_real() ) );
				else
					args.push ( Lex( LEX_NUM, i.get_real() > j.get_value() ) );
				break;

			case LEX_GEQ:
				from_st ( args, j );
				from_st ( args, i );
				if (i.get_type() == j.get_type()) {
					if (i.get_type() == LEX_NUM)
						args.push ( Lex( LEX_NUM, i.get_value() >= j.get_value()) );
					else if (i.get_type() == LEX_REALNUMB)
						args.push ( Lex( LEX_NUM, i.get_real() >= j.get_real()) );
				} else if (i.get_type() == LEX_NUM)
					args.push ( Lex( LEX_NUM, i.get_value() >= j.get_real() ) );
				else
					args.push ( Lex( LEX_NUM, i.get_real() >= j.get_value() ) );
				break;

			case LEX_NEQ:
				from_st ( args, j );
				from_st ( args, i );
				if (i.get_type() == j.get_type()) {
					if (i.get_type() == LEX_NUM)
						args.push ( Lex( LEX_NUM, i.get_value() != j.get_value()) );
					else if (i.get_type() == LEX_STR)
						args.push ( Lex( LEX_NUM, i.get_str() != j.get_str()) );
					else if (i.get_type() == LEX_REALNUMB)
						args.push ( Lex( LEX_NUM, i.get_real() != j.get_real()) );
				} else if (i.get_type() == LEX_NUM)
					args.push ( Lex( LEX_NUM, i.get_value() != j.get_real() ) );
				else
					args.push ( Lex( LEX_NUM, i.get_real() != j.get_value() ) );
				break;

			case LEX_ASSIGN:
				from_st ( args, j );
				from_st ( args, i );
				val = i.get_value();
				TID[val].put_value (j.get_value());
				TID[val].put_str (j.get_str());
				TID[val].put_real (j.get_real());
				TID[val].put_assign ();
				break;
			default:
				throw "POLIZ: unexpected elem ";
		}
		++index;
	};
	cout << "\nExecuting success" << endl;
}


class Interpretator {
		Parser   pars;
		Executer E;
	public:
		Interpretator ( const char* program ): pars (program) {}
		void     interpretation ();
};

void Interpretator::interpretation () {
	pars.analyze ();
	cout << "Analyze success\n";
	E.execute ( pars.poliz );
}

ostream & operator<< ( ostream &s, Lex l ) {
	string t;
	if ( l.t_lex <= LEX_WRITE )
		t = Scanner::TW[l.t_lex];
	else if ( l.t_lex >= LEX_FIN && l.t_lex <= LEX_RFIG )
		t = Scanner::TD[ l.t_lex - LEX_FIN ];
	else if ( l.t_lex == LEX_NUM )
		t = "NUMB";
	else if (l.t_lex == LEX_STR ) {
		t = "STR";
		s << '(' << t << ',' << l.s_lex << ");" << endl;
		return s;
	} else if(l.t_lex == LEX_REALNUMB) {
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
		Interpretator I ( "prog.txt" );
		I.interpretation ();
	} catch ( char c ) {
		cout << "Unexpected symbol " << int(c) << ' ' << c << endl;
		return 1;
	} catch ( Lex l ) {
		cout << "Unexpected lexeme" << l << endl;
		return 1;
	} catch ( const char *source ) {
		cout << source << endl;
		return 1;
	}

	return 0;
}
