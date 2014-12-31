#include "shogi.h"

unsigned int * CONV
b_gen_captures( const tree_t * restrict ptree, unsigned int * restrict pmove )
{
  bitboard_t bb_movable, bb_capture, bb_piece, bb_desti;
  unsigned int utemp;
  int ito, ifrom;

  bb_capture = BB_WOCCUPY;
  BBNot( bb_movable, BB_BOCCUPY );

  bb_desti.p[0] = BB_BPAWN_ATK.p[0] & bb_movable.p[0];
  bb_desti.p[1] = BB_BPAWN_ATK.p[1] & bb_capture.p[1];
  bb_desti.p[2] = BB_BPAWN_ATK.p[2] & bb_capture.p[2];

  foreach_bitboard_lastone(bb_desti, ito,
  {
	  ifrom = ito + 9;
	  utemp = (To2Move(ito) | From2Move(ifrom) | Cap2Move(-BOARD[ito])
	  | Piece2Move(pawn));
	  if (ito < A6) { utemp |= FLAG_PROMO; }
	  *pmove++ = utemp;
  }
  );
  /*
  while ( BBToU( bb_desti ) )
    {
      ito = LastOne( bb_desti );
      Xor( ito, bb_desti );

      ifrom = ito + 9;
      utemp = ( To2Move(ito) | From2Move(ifrom) | Cap2Move(-BOARD[ito])
		| Piece2Move(pawn) );
      if ( ito < A6 ) { utemp |= FLAG_PROMO; }
      *pmove++ = utemp;
    }*/

  bb_piece = BB_BSILVER;
  foreach_bitboard_lastone(bb_piece, ifrom,
  {
	  BBAnd(bb_desti, bb_capture, abb_b_silver_attacks[ifrom]);

	  foreach_bitboard_lastone(bb_desti, ito,
	  {
		  utemp = (To2Move(ito) | From2Move(ifrom) | Cap2Move(-BOARD[ito])
		  | Piece2Move(silver));
		  if (ito < A6 || ifrom < A6) { *pmove++ = utemp | FLAG_PROMO; }
		  *pmove++ = utemp;
	  }
	  );
  }
  );
  /*
  while ( BBTest( bb_piece ) )
    {
      ifrom = LastOne( bb_piece );
      Xor( ifrom, bb_piece );

      BBAnd( bb_desti, bb_capture, abb_b_silver_attacks[ifrom] );
      while ( BBTest( bb_desti ) )
	{
	  ito = LastOne( bb_desti );
	  Xor( ito, bb_desti );

	  utemp = ( To2Move(ito) | From2Move(ifrom) | Cap2Move(-BOARD[ito])
		    | Piece2Move(silver) );
	  if ( ito < A6 || ifrom < A6 ) { *pmove++ = utemp | FLAG_PROMO; }
	  *pmove++ = utemp;
	}
    }*/

  bb_piece = BB_BTGOLD;
  foreach_bitboard_lastone(bb_piece, ifrom,
  {
	  BBAnd(bb_desti, bb_capture, abb_b_gold_attacks[ifrom]);

	  foreach_bitboard_lastone(bb_desti, ito,
	  {
		  *pmove++ = (To2Move(ito) | From2Move(ifrom)
		  | Cap2Move(-BOARD[ito])
		  | Piece2Move(BOARD[ifrom]));
	  }
	  );
  }
  );
  /*
  while( BBTest( bb_piece ) )
    {
      ifrom = LastOne( bb_piece );
      Xor( ifrom, bb_piece );

      BBAnd( bb_desti, bb_capture, abb_b_gold_attacks[ifrom] );
      while ( BBTest( bb_desti ) )
	{
	  ito = LastOne( bb_desti );
	  Xor( ito, bb_desti );

	  *pmove++ = ( To2Move(ito) | From2Move(ifrom)
		       | Cap2Move(-BOARD[ito])
		       | Piece2Move(BOARD[ifrom]) );
	}
    }*/

  ifrom = SQ_BKING;
  BBAnd( bb_desti, bb_capture, abb_king_attacks[ifrom] );

  bb_piece = BB_BTGOLD;
  foreach_bitboard_lastone(bb_desti, ito,
  {
	  *pmove++ = (To2Move(ito) | From2Move(ifrom)
			| Cap2Move(-BOARD[ito]) | Piece2Move(king));
  }
  );
  /*
  while ( BBTest( bb_desti ) )
    {
      ito = LastOne( bb_desti );
      Xor( ito, bb_desti );

      *pmove++ = ( To2Move(ito) | From2Move(ifrom)
		   | Cap2Move(-BOARD[ito]) | Piece2Move(king) );
    }*/

  bb_piece = BB_BBISHOP;
  foreach_bitboard_lastone(bb_piece, ifrom,
  {
	  AttackBishop(bb_desti, ifrom);
	  bb_desti.p[0] &= bb_movable.p[0];
	  if (ifrom < A6)
	  {
		  bb_desti.p[1] &= bb_movable.p[1];
		  bb_desti.p[2] &= bb_movable.p[2];
	  }
	  else {
		  bb_desti.p[1] &= bb_capture.p[1];
		  bb_desti.p[2] &= bb_capture.p[2];
	  }

	  foreach_bitboard_lastone(bb_desti, ito,
	  {
		  utemp = (To2Move(ito) | From2Move(ifrom)
				| Cap2Move(-BOARD[ito]) | Piece2Move(bishop));
		  if (ito < A6 || ifrom < A6) { utemp |= FLAG_PROMO; }
		  *pmove++ = utemp;
	  }
	  );
  }
  );
  /*
  while ( BBTest( bb_piece ) )
    {
      ifrom = LastOne( bb_piece );
      Xor( ifrom, bb_piece );

      AttackBishop( bb_desti, ifrom );
      bb_desti.p[0] &= bb_movable.p[0];
      if ( ifrom < A6 )
	{
	  bb_desti.p[1] &= bb_movable.p[1];
	  bb_desti.p[2] &= bb_movable.p[2];
	}
      else {
	bb_desti.p[1] &= bb_capture.p[1];
	bb_desti.p[2] &= bb_capture.p[2];
      }

      while ( BBTest( bb_desti ) )
	{
	  ito = LastOne( bb_desti );
	  Xor( ito, bb_desti );

	  utemp = ( To2Move(ito) | From2Move(ifrom)
		    | Cap2Move(-BOARD[ito]) | Piece2Move(bishop) );
	  if ( ito < A6 || ifrom < A6 ) { utemp |= FLAG_PROMO; }
	  *pmove++ = utemp;
	}
    }*/

  bb_piece = BB_BROOK;
  foreach_bitboard_lastone(bb_piece, ifrom,
  {
	  AttackRook(bb_desti, ifrom);
	  bb_desti.p[0] &= bb_movable.p[0];
	  if (ifrom < A6)
	  {
		  bb_desti.p[1] &= bb_movable.p[1];
		  bb_desti.p[2] &= bb_movable.p[2];
	  }
	  else {
		  bb_desti.p[1] &= bb_capture.p[1];
		  bb_desti.p[2] &= bb_capture.p[2];
	  }

	  foreach_bitboard_lastone(bb_desti, ito,
	  {
		  utemp = (To2Move(ito) | From2Move(ifrom)
		  | Cap2Move(-BOARD[ito]) | Piece2Move(rook));
		  if (ito < A6 || ifrom < A6) { utemp |= FLAG_PROMO; }
		  *pmove++ = utemp;
	  }
	  );
  }
  );
  /*
  while ( BBTest( bb_piece ) )
    {
      ifrom = LastOne( bb_piece );
      Xor( ifrom, bb_piece );

      AttackRook( bb_desti, ifrom );
      bb_desti.p[0] &= bb_movable.p[0];
      if ( ifrom < A6 )
	{
	  bb_desti.p[1] &= bb_movable.p[1];
	  bb_desti.p[2] &= bb_movable.p[2];
	}
      else {
	bb_desti.p[1] &= bb_capture.p[1];
	bb_desti.p[2] &= bb_capture.p[2];
      }

      while ( BBTest( bb_desti ) )
	{
	  ito = LastOne( bb_desti );
	  Xor( ito, bb_desti );

	  utemp = ( To2Move(ito) | From2Move(ifrom)
		    | Cap2Move(-BOARD[ito]) | Piece2Move(rook) );
	  if ( ito < A6 || ifrom < A6 ) { utemp |= FLAG_PROMO; }
	  *pmove++ = utemp;
	}
    }*/

  bb_piece = BB_BHORSE;
  foreach_bitboard_lastone(bb_piece, ifrom,
  {
	  AttackHorse(bb_desti, ifrom);
	  BBAnd(bb_desti, bb_desti, bb_capture);

	  foreach_bitboard_lastone(bb_desti, ito,
	  {
		  *pmove++ = (To2Move(ito) | From2Move(ifrom)
					| Cap2Move(-BOARD[ito]) | Piece2Move(horse));
	  }
	  );
  }
  );
  /*
  while ( BBTest( bb_piece ) )
    {
      ifrom = LastOne( bb_piece );
      Xor( ifrom, bb_piece );

      AttackHorse( bb_desti, ifrom );
      BBAnd( bb_desti, bb_desti, bb_capture );
      while ( BBTest( bb_desti ) )
	{
	  ito = LastOne( bb_desti );
	  Xor( ito, bb_desti );

	  *pmove++ = ( To2Move(ito) | From2Move(ifrom)
		       | Cap2Move(-BOARD[ito]) | Piece2Move(horse) );
	}
    }*/

  bb_piece = BB_BDRAGON;
  foreach_bitboard_lastone(bb_piece, ifrom,
  {
	  AttackDragon(bb_desti, ifrom);
	  BBAnd(bb_desti, bb_desti, bb_capture);

	  foreach_bitboard_lastone(bb_desti, ito,
	  {
		  *pmove++ = (To2Move(ito) | From2Move(ifrom)
					| Cap2Move(-BOARD[ito]) | Piece2Move(dragon));
	  }
	  );
  }
  );
  /*
  while ( BBTest( bb_piece ) )
    {
      ifrom = LastOne( bb_piece );
      Xor( ifrom, bb_piece );

      AttackDragon( bb_desti, ifrom );
      BBAnd( bb_desti, bb_desti, bb_capture );
      while ( BBTest( bb_desti ) )
	{
	  ito = LastOne( bb_desti );
	  Xor( ito, bb_desti );

	  *pmove++ = ( To2Move(ito) | From2Move(ifrom)
		       | Cap2Move(-BOARD[ito]) | Piece2Move(dragon) );
	}
    }*/

  bb_piece = BB_BLANCE;

  foreach_bitboard_lastone(bb_piece, ifrom,
  {
	  bb_desti = AttackFile(ifrom);
	  BBAnd(bb_desti, bb_desti, abb_minus_rays[ifrom]);
	  bb_desti.p[0] &= bb_movable.p[0];
	  bb_desti.p[1] &= bb_capture.p[1];
	  bb_desti.p[2] &= bb_capture.p[2];

	  foreach_bitboard_lastone(bb_desti, ito,
	  {
		  utemp = (To2Move(ito) | From2Move(ifrom)
				| Cap2Move(-BOARD[ito]) | Piece2Move(lance));
		  if (ito < A7) { *pmove++ = utemp | FLAG_PROMO; }
		  else if (ito < A6)
		  {
			  *pmove++ = utemp | FLAG_PROMO;
			  if (UToCap(utemp)) { *pmove++ = utemp; }
		  }
		  else { *pmove++ = utemp; }
	  }
	  );
  }
  );
  /*
  while( BBTest( bb_piece ) )
    {
      ifrom = LastOne( bb_piece );
      Xor( ifrom, bb_piece );

      bb_desti = AttackFile( ifrom );
      BBAnd( bb_desti, bb_desti, abb_minus_rays[ifrom] );
      bb_desti.p[0] &= bb_movable.p[0];
      bb_desti.p[1] &= bb_capture.p[1];
      bb_desti.p[2] &= bb_capture.p[2];

      while ( BBTest( bb_desti ) )
	{
	  ito = LastOne( bb_desti );
	  Xor( ito, bb_desti );

	  utemp = ( To2Move(ito) | From2Move(ifrom)
		    | Cap2Move(-BOARD[ito]) | Piece2Move(lance) );
	  if      ( ito < A7 ) { *pmove++ = utemp | FLAG_PROMO; }
	  else if ( ito < A6 )
	    {
	      *pmove++ = utemp | FLAG_PROMO;
	      if ( UToCap(utemp) ) { *pmove++ = utemp; }
	    }
	  else { *pmove++ = utemp; }
	}
    }*/

  bb_piece = BB_BKNIGHT;
  foreach_bitboard_lastone(bb_piece, ifrom,
  {
	  bb_desti = abb_b_knight_attacks[ifrom];
	  bb_desti.p[0] &= bb_movable.p[0];
	  bb_desti.p[1] &= bb_capture.p[1];
	  bb_desti.p[2] &= bb_capture.p[2];

	  foreach_bitboard_lastone(bb_desti, ito,
	  {
		  utemp = (To2Move(ito) | From2Move(ifrom)
				| Cap2Move(-BOARD[ito]) | Piece2Move(knight));
		  if (ito < A7) { *pmove++ = utemp | FLAG_PROMO; }
		  else if (ito < A6)
		  {
			  *pmove++ = utemp | FLAG_PROMO;
			  if (UToCap(utemp)) { *pmove++ = utemp; }
		  }
		  else { *pmove++ = utemp; }
	  }
	  );
  }
  );
  /*
  while( BBTest( bb_piece ) )
    {
      ifrom = LastOne( bb_piece );
      Xor( ifrom, bb_piece );

      bb_desti = abb_b_knight_attacks[ifrom];
      bb_desti.p[0] &= bb_movable.p[0];
      bb_desti.p[1] &= bb_capture.p[1];
      bb_desti.p[2] &= bb_capture.p[2];

      while ( BBTest( bb_desti ) )
	{
	  ito = LastOne( bb_desti );
	  Xor( ito, bb_desti );

	  utemp = ( To2Move(ito) | From2Move(ifrom)
		    | Cap2Move(-BOARD[ito]) | Piece2Move(knight) );
	  if      ( ito < A7 ) { *pmove++ = utemp | FLAG_PROMO; }
	  else if ( ito < A6 )
	    {
	      *pmove++ = utemp | FLAG_PROMO;
	      if ( UToCap(utemp) ) { *pmove++ = utemp; }
	    }
	  else { *pmove++ = utemp; }
	}
    }*/

  return pmove;
}


unsigned int * CONV
w_gen_captures( const tree_t * restrict ptree, unsigned int * restrict pmove )
{
  bitboard_t bb_movable, bb_capture, bb_piece, bb_desti;
  unsigned int utemp;
  int ito, ifrom;

  bb_capture = BB_BOCCUPY;
  BBNot( bb_movable, BB_WOCCUPY );

  bb_desti.p[2] = BB_WPAWN_ATK.p[2] & bb_movable.p[2];
  bb_desti.p[1] = BB_WPAWN_ATK.p[1] & bb_capture.p[1];
  bb_desti.p[0] = BB_WPAWN_ATK.p[0] & bb_capture.p[0];

  foreach_bitboard_firstone(bb_desti, ito,
  {
	  ifrom = ito - 9;
	  utemp = (To2Move(ito) | From2Move(ifrom) | Cap2Move(BOARD[ito])
		  | Piece2Move(pawn));
	  if (ito > I4) { utemp |= FLAG_PROMO; }
	  *pmove++ = utemp;
  }
  );
  /*
  while ( BBToU( bb_desti ) )
    {
      ito = FirstOne( bb_desti );
      Xor( ito, bb_desti );

      ifrom = ito - 9;
      utemp = ( To2Move(ito) | From2Move(ifrom) | Cap2Move(BOARD[ito])
		| Piece2Move(pawn) );
      if ( ito > I4 ) { utemp |= FLAG_PROMO; }
      *pmove++ = utemp;
    }*/

  bb_piece = BB_WSILVER;
  while ( BBTest( bb_piece ) )
    {
      ifrom = FirstOne( bb_piece );
      Xor( ifrom, bb_piece );

      BBAnd( bb_desti, bb_capture, abb_w_silver_attacks[ifrom] );
      while ( BBTest( bb_desti ) )
	{
	  ito = FirstOne( bb_desti );
	  Xor( ito, bb_desti );

	  utemp = ( To2Move(ito) | From2Move(ifrom) | Cap2Move(BOARD[ito])
		    | Piece2Move(silver) );
	  if ( ito > I4 || ifrom > I4 ) { *pmove++ = utemp | FLAG_PROMO; }
	  *pmove++ = utemp;
	}
    }

  bb_piece = BB_WTGOLD;
  while( BBTest( bb_piece ) )
    {
      ifrom = FirstOne( bb_piece );
      Xor( ifrom, bb_piece );

      BBAnd( bb_desti, bb_capture, abb_w_gold_attacks[ifrom] );
      while ( BBTest( bb_desti ) )
	{
	  ito = FirstOne( bb_desti );
	  Xor( ito, bb_desti );

	  *pmove++ = ( To2Move(ito) | From2Move(ifrom)
		       | Cap2Move(BOARD[ito])
		       | Piece2Move(-BOARD[ifrom]) );
	}
    }

  ifrom = SQ_WKING;
  BBAnd( bb_desti, bb_capture, abb_king_attacks[ifrom] );
  while ( BBTest( bb_desti ) )
    {
      ito = FirstOne( bb_desti );
      Xor( ito, bb_desti );

      *pmove++ = ( To2Move(ito) | From2Move(ifrom)
		   | Cap2Move(BOARD[ito]) | Piece2Move(king) );
    }

  bb_piece = BB_WBISHOP;
  while ( BBTest( bb_piece ) )
    {
      ifrom = FirstOne( bb_piece );
      Xor( ifrom, bb_piece );

      AttackBishop( bb_desti, ifrom );
      bb_desti.p[2] &= bb_movable.p[2];
      if ( ifrom > I4 )
	{
	  bb_desti.p[1] &= bb_movable.p[1];
	  bb_desti.p[0] &= bb_movable.p[0];
	}
      else {
	bb_desti.p[1] &= bb_capture.p[1];
	bb_desti.p[0] &= bb_capture.p[0];
      }

      while ( BBTest( bb_desti ) )
	{
	  ito = FirstOne( bb_desti );
	  Xor( ito, bb_desti );

	  utemp = ( To2Move(ito) | From2Move(ifrom)
		    | Cap2Move(BOARD[ito]) | Piece2Move(bishop) );
	  if ( ito > I4 || ifrom > I4 ) { utemp |= FLAG_PROMO; }
	  *pmove++ = utemp;
	}
    }

  bb_piece = BB_WROOK;
  while ( BBTest( bb_piece ) )
    {
      ifrom = FirstOne( bb_piece );
      Xor( ifrom, bb_piece );

      AttackRook( bb_desti, ifrom );
      bb_desti.p[2] &= bb_movable.p[2];
      if ( ifrom > I4 )
	{
	  bb_desti.p[1] &= bb_movable.p[1];
	  bb_desti.p[0] &= bb_movable.p[0];
	}
      else {
	bb_desti.p[1] &= bb_capture.p[1];
	bb_desti.p[0] &= bb_capture.p[0];
      }

      while ( BBTest( bb_desti ) )
	{
	  ito = FirstOne( bb_desti );
	  Xor( ito, bb_desti );

	  utemp = ( To2Move(ito) | From2Move(ifrom)
		    | Cap2Move(BOARD[ito]) | Piece2Move(rook) );
	  if ( ito > I4 || ifrom > I4 ) { utemp |= FLAG_PROMO; }
	  *pmove++ = utemp;
	}
    }

  bb_piece = BB_WHORSE;
  while ( BBTest( bb_piece ) )
    {
      ifrom = FirstOne( bb_piece );
      Xor( ifrom, bb_piece );

      AttackHorse( bb_desti, ifrom );
      BBAnd( bb_desti, bb_desti, bb_capture );
      while ( BBTest( bb_desti ) )
	{
	  ito = FirstOne( bb_desti );
	  Xor( ito, bb_desti );

	  *pmove++ = ( To2Move(ito) | From2Move(ifrom)
		       | Cap2Move(BOARD[ito]) | Piece2Move(horse) );
	}
    }

  bb_piece = BB_WDRAGON;
  while ( BBTest( bb_piece ) )
    {
      ifrom = FirstOne( bb_piece );
      Xor( ifrom, bb_piece );

      AttackDragon( bb_desti, ifrom );
      BBAnd( bb_desti, bb_desti, bb_capture );
      while ( BBTest( bb_desti ) )
	{
	  ito = FirstOne( bb_desti );
	  Xor( ito, bb_desti );

	  *pmove++ = ( To2Move(ito) | From2Move(ifrom)
		       | Cap2Move(BOARD[ito]) | Piece2Move(dragon) );
	}
    }

  bb_piece = BB_WLANCE;
  while( BBTest( bb_piece ) )
    {
      ifrom = FirstOne( bb_piece );
      Xor( ifrom, bb_piece );

      bb_desti = AttackFile( ifrom );
      BBAnd( bb_desti, bb_desti, abb_plus_rays[ifrom] );
      bb_desti.p[2] &= bb_movable.p[2];
      bb_desti.p[1] &= bb_capture.p[1];
      bb_desti.p[0] &= bb_capture.p[0];

      while ( BBTest( bb_desti ) )
	{
	  ito = FirstOne( bb_desti );
	  Xor( ito, bb_desti );

	  utemp = ( To2Move(ito) | From2Move(ifrom)
		    | Cap2Move(BOARD[ito]) | Piece2Move(lance) );
	  if      ( ito > I3 ) { *pmove++ = utemp | FLAG_PROMO; }
	  else if ( ito > I4 )
	    {
	      *pmove++ = utemp | FLAG_PROMO;
	      if ( UToCap(utemp) ) { *pmove++ = utemp; }
	    }
	  else { *pmove++ = utemp; }
	}
    }

  bb_piece = BB_WKNIGHT;
  while( BBTest( bb_piece ) )
    {
      ifrom = FirstOne( bb_piece );
      Xor( ifrom, bb_piece );

      bb_desti = abb_w_knight_attacks[ifrom];
      bb_desti.p[2] &= bb_movable.p[2];
      bb_desti.p[1] &= bb_capture.p[1];
      bb_desti.p[0] &= bb_capture.p[0];

      while ( BBTest( bb_desti ) )
	{
	  ito = FirstOne( bb_desti );
	  Xor( ito, bb_desti );

	  utemp = ( To2Move(ito) | From2Move(ifrom)
		    | Cap2Move(BOARD[ito]) | Piece2Move(knight) );
	  if      ( ito > I3 ) { *pmove++ = utemp | FLAG_PROMO; }
	  else if ( ito > I4 )
	    {
	      *pmove++ = utemp | FLAG_PROMO;
	      if ( UToCap(utemp) ) { *pmove++ = utemp; }
	    }
	  else { *pmove++ = utemp; }
	}
    }

  return pmove;
}
