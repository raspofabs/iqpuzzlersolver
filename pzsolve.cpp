#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char solutionfilename[256];
const int WIDTH = 11;
const int HEIGHT = 5;
struct Piece {
	bool on[4*4];
	char id;
};
Piece Rot( const Piece &p, int r ) {
	Piece a = p;
	r = r & 7;
	const int SX[] = { 0,3,3,0, 3,0,0,3 };
	const int SY[] = { 0,0,3,3, 0,0,3,3 };
	const int XDX[] = { 1,0,-1,0, -1,0,1,0 };
	const int XDY[] = { 0,1,0,-1, 0,1,0,-1 };
	const int YDX[] = { 0,-1,0,1, 0,1,0,-1 };
	const int YDY[] = { 1,0,-1,0, 1,0,-1,0 };
	for( int y = 0; y < 4; ++y ) {
		for( int x = 0; x < 4; ++x ) {
			int ox = SX[r] + XDX[r]*x + YDX[r]*y;
			int oy = SY[r] + XDY[r]*x + YDY[r]*y;
			a.on[ox+oy*4] = p.on[x+y*4];
		}
	}
	return a;
};
struct PuzzleState {
	int filled[ WIDTH * HEIGHT ];
};
void PrintPiece( const Piece &p ) {
	for( int y = 0; y < 4; ++y ) {
		for( int x = 0; x < 4; ++x ) {
			char c = p.on[x+y*4] ? '#' : '.';
			printf( "%c", c );
		}
		printf( "\n" );
	}
}
Piece L = { 0,0,0,0, 1,0,0,0, 1,0,0,0, 1,1,1,0, 'X' };
Piece pieces[] = {
{ 1,1,1,0, 1,0,0,0, 0,0,0,0, 0,0,0,0, 'a' },
{ 1,1,1,0, 1,1,0,0, 0,0,0,0, 0,0,0,0, 'b' },
{ 1,1,1,1, 1,0,0,0, 0,0,0,0, 0,0,0,0, 'c' },
{ 1,1,1,1, 0,1,0,0, 0,0,0,0, 0,0,0,0, 'd' },
{ 1,1,1,0, 0,0,1,1, 0,0,0,0, 0,0,0,0, 'e' },
{ 1,1,0,0, 1,0,0,0, 0,0,0,0, 0,0,0,0, 'f' },
{ 1,1,1,0, 1,0,0,0, 1,0,0,0, 0,0,0,0, 'g' },
{ 1,1,0,0, 0,1,1,0, 0,0,1,0, 0,0,0,0, 'h' },
{ 1,1,1,0, 1,0,1,0, 0,0,0,0, 0,0,0,0, 'i' },
{ 1,1,1,1, 0,0,0,0, 0,0,0,0, 0,0,0,0, 'j' },
{ 1,1,0,0, 1,1,0,0, 0,0,0,0, 0,0,0,0, 'k' },
{ 0,1,0,0, 1,1,1,0, 0,1,0,0, 0,0,0,0, 'l' },
};
bool PieceFits( const PuzzleState &ps, const Piece &piece, int x, int y ) {
	for( int py = 0; py < 4; ++py ) {
		for( int px = 0; px < 4; ++px ) {
			bool on = piece.on[px+py*4];
			if( on ) {
				int ox = px + x;
				int oy = py + y;
				if( ox < 0 || oy < 0 || ox >= WIDTH || oy >= HEIGHT ) {
					//printf( "fail :range\n" );
					return false;
				}
				if( ps.filled[ ox + oy * WIDTH ] ) {
					//printf( "fail :collision\n" );
					return false;
				}
			}
		}
	}
	return true;
}
PuzzleState FillState( const PuzzleState &ps, const Piece &piece, int x, int y ) {
	PuzzleState newState = ps;
	for( int py = 0; py < 4; ++py ) {
		for( int px = 0; px < 4; ++px ) {
			bool on = piece.on[px+py*4];
			if( on ) {
				int ox = px + x;
				int oy = py + y;
				//printf( "fill %i,%i\n", ox, oy );
				newState.filled[ ox + oy * WIDTH ] = piece.id;
			}
		}
	}
	return newState;
}
const int num_pieces = sizeof( pieces ) / sizeof( pieces[0] );
int GetPieceIDFromChar( char c ) {
	for( int i = 0; i < num_pieces; ++i ) {
		if( pieces[i].id == c )
			return i;
	}
	printf( "cannot find piece for char [%c]\n", c );
	return -1;
}
PuzzleState Clear() {
	PuzzleState ps;
	memset( &ps, 0, sizeof( ps ) );
	return ps;
}
void PrintState( const PuzzleState &ps, FILE *fp = stdout ) {
	for( int y = 0; y < HEIGHT; ++y ) {
		for( int x = 0; x < WIDTH; ++x ) {
			bool on = ps.filled[x+y*WIDTH];
			char c = on ? ps.filled[x+y*WIDTH] : '.';
			fprintf( fp, "%c", c );
		}
		fprintf( fp, "\n" );
	}
}

#include <set>
typedef std::set<int> PiecesLeft;
PiecesLeft StartingSet() {
	PiecesLeft p;
	for( int i = 0; i < num_pieces; ++i ) {
		p.insert(i);
	}
	return p;
}

bool Solvable( const PuzzleState &ps ) {
	for( int y = 0; y < HEIGHT; ++y ) {
		for( int x = 0; x < WIDTH; ++x ) {
			int index = x + y * WIDTH;
			bool leftGood = x > 0 ? 0 == ps.filled[ index - 1 ] : false;
			bool rightGood = x < WIDTH-1 ? 0 == ps.filled[ index + 1 ] : false;
			bool topGood = y > 0 ? 0 == ps.filled[ index - WIDTH ] : false;
			bool bottomGood = y < HEIGHT-1 ? 0 == ps.filled[ index + WIDTH ] : false;
			bool a = ps.filled[ index ];
			if( !a ) {
				if( leftGood | rightGood | topGood | bottomGood ) {

				} else {
					return false;
				}
			}
		}
	}
	return true;
}
void Solve( PiecesLeft pl, const PuzzleState &ps ) {
	if(!Solvable(ps)) {
		return;
	}
	if( pl.size() == 0 ) {
		printf( "Solution:\n" );
		PrintState( ps );
		PrintState( ps, fopen( solutionfilename, "w" ) );
		exit( 0 );
	}
	int i = *(pl.begin());
	Piece piece = pieces[i];
	//printf( "Attempting to fit in piece: %c\n", piece.id );

	pl.erase( i );
	for( int r = 0; r < 8; ++r ) {
		Piece newPiece = Rot( piece, r );
		//printf( "Trying this way:\n" );
		//PrintPiece( newPiece );
		for( int y = -2; y < HEIGHT; ++y ) {
			for( int x = -2; x < WIDTH; ++x ) {
				if( PieceFits( ps, newPiece, x, y ) ) {
					PuzzleState newState = FillState( ps, newPiece, x, y );
					Solve( pl, newState );
				}
			}
		}
	}
}
void LoadState( PuzzleState &state, PiecesLeft &remains, const char *filename ) {
	PuzzleState ps = Clear();
	PiecesLeft pl = StartingSet();
	if( FILE *fp = fopen( filename, "r" ) ) {
		char linebuf[256];
		for( int row = 0; row < HEIGHT; ++row ) {
			char *line = fgets( linebuf, sizeof( linebuf ), fp );
			if( !line ) {
				break;
			}
			for( int col = 0; col < WIDTH; ++col ) {
				char c = line[col];
				if( c == 0 || c == '\n' || c == '\r' )
					break;
				if( c != '.' ) {
					int i = GetPieceIDFromChar( c );
					pl.erase(i);
					ps.filled[row*WIDTH + col] = c;
				}
			}
		}
	}
	printf( "loaded %s\n", filename );
	PrintState( ps );
	printf( "\n" );
	state = ps;
	remains = pl;
}
void PrintRemainingPieces( const PiecesLeft &pl ) {
	PiecesLeft::iterator i = pl.begin();
	if( i != pl.end() ) {
		printf( "%c", pieces[*i].id );
		++i;
		for(;i != pl.end(); ++i) {
			printf( ",%c", pieces[*i].id );
		}
	}
	printf( "\n" );
}
int main( int argc, char *argv[] ) {
	printf( "have %i args\n", argc );
	printf( "first arg: %s\n", argv[0] );
	PuzzleState ps;
	PiecesLeft pl;
	if( argc == 2 ) {
		LoadState( ps, pl, argv[1] );
		sprintf( solutionfilename, "SOLVED_%s", argv[1] );
		printf( "Solving\n");
		PrintRemainingPieces( pl );
		Solve( pl, ps );
		printf( "Unsolvable?\n");
	} else {
		Piece e = pieces[4];
		for( int ori = 0; ori < 8; ++ori ) {
			printf( "Piece rotated %i\n", ori );
			PrintPiece( Rot( e, ori ) );
			printf( "\n" );
		}
	}
}
