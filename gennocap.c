#include "shogi.h"

unsigned int * CONV
b_gen_nocaptures(const tree_t * restrict ptree,
unsigned int * restrict pmove)
{
	bitboard_t bb_empty, bb_piece, bb_desti;
	unsigned int utemp;
	int to, from;

	BBOr(bb_empty, BB_BOCCUPY, BB_WOCCUPY);
	BBNot(bb_empty, bb_empty);

	bb_piece.p[1] = BB_BPAWN_ATK.p[1] & bb_empty.p[1];
	bb_piece.p[2] = BB_BPAWN_ATK.p[2] & bb_empty.p[2];
	while (bb_piece.p[1] | bb_piece.p[2])
	{
		to = last_one12(bb_piece.p[1], bb_piece.p[2]);
		bb_piece.p[1] ^= abb_mask[to].p[1];
		bb_piece.p[2] ^= abb_mask[to].p[2];
		from = to + 9;
		*pmove++ = To2Move(to) | From2Move(from) | Piece2Move(pawn);
	}

	//��
	bb_piece = BB_BSILVER;
	
	foreach_bitboard_lastone_no_check(bb_piece, from,
	{
		BBAnd(bb_desti, bb_empty, abb_b_silver_attacks[from]);
		foreach_bitboard_lastone(bb_desti, to,
		{
			utemp = To2Move(to) | From2Move(from) | Piece2Move(silver);
			if (from < A6 || to < A6) { *pmove++ = utemp | FLAG_PROMO; }
			*pmove++ = utemp;
		}
		);
	}
	);
		/*
  while( BBTest( bb_piece ) )
    {
      from   = LastOne( bb_piece );
      Xor( from, bb_piece );

      BBAnd( bb_desti, bb_empty, abb_b_silver_attacks[from] );
      while ( BBTest( bb_desti ) )
	{
	  to = LastOne( bb_desti );
	  Xor( to, bb_desti );

	  utemp = To2Move( to ) | From2Move( from ) | Piece2Move( silver );
	  if ( from < A6 || to < A6 ) { *pmove++ = utemp | FLAG_PROMO; }
	  *pmove++ = utemp;
	}
    }*/
	
	//���A����
  bb_piece = BB_BTGOLD;
  
  foreach_bitboard_lastone_no_check(bb_piece, from,
  {
	  BBAnd(bb_desti, bb_empty, abb_b_gold_attacks[from]);
	  utemp = From2Move(from) | Piece2Move(BOARD[from]);
	  foreach_bitboard_lastone(bb_desti, to,
	  {
		  *pmove++ = To2Move(to) | utemp;
	  }
	  );
  }
  );
	  /*
  while( BBTest( bb_piece ) )
    {
      from = LastOne( bb_piece );
      Xor( from, bb_piece );

      BBAnd( bb_desti, bb_empty, abb_b_gold_attacks[from] );
      utemp = From2Move( from ) | Piece2Move( BOARD[from] ); 
      while ( BBTest( bb_desti ) )
	{
	  to = LastOne( bb_desti );
	  Xor( to, bb_desti );
	  *pmove++ = To2Move( to ) | utemp;
	}
    }*/

  //��
  from = SQ_BKING;
  BBAnd( bb_desti, bb_empty, abb_king_attacks[from] );
  utemp = From2Move( from ) | Piece2Move( king ); 
  
  foreach_bitboard_lastone_no_check(bb_desti, to,
  {
	  *pmove++ = To2Move(to) | utemp;
  }
  ); 
	  /*
  while ( BBTest( bb_desti ) )
    {
      to = LastOne( bb_desti );
      Xor( to, bb_desti );
      *pmove++ = To2Move( to ) | utemp;
    }*/

  //�p
  bb_piece.p[1] = BB_BBISHOP.p[1];
  bb_piece.p[2] = BB_BBISHOP.p[2];
  while( bb_piece.p[1] | bb_piece.p[2] )
    {
      from          = last_one12( bb_piece.p[1], bb_piece.p[2] );
      bb_piece.p[1] ^= abb_mask[from].p[1];
      bb_piece.p[2] ^= abb_mask[from].p[2];

      AttackBishop( bb_desti, from );
      bb_desti.p[1] &= bb_empty.p[1];
      bb_desti.p[2] &= bb_empty.p[2];

      utemp = From2Move( from ) | Piece2Move( bishop );
      while ( bb_desti.p[1] | bb_desti.p[2] )
	{
	  to            = last_one12( bb_desti.p[1], bb_desti.p[2] );
	  bb_desti.p[1] ^= abb_mask[to].p[1];
	  bb_desti.p[2] ^= abb_mask[to].p[2];
	  *pmove++ = To2Move( to ) | utemp;
	}
    }
  //���
  bb_piece.p[1] = BB_BROOK.p[1];
  bb_piece.p[2] = BB_BROOK.p[2];
  while( bb_piece.p[1] | bb_piece.p[2] )
    {
      from          = last_one12( bb_piece.p[1], bb_piece.p[2] );
      bb_piece.p[1] ^= abb_mask[from].p[1];
      bb_piece.p[2] ^= abb_mask[from].p[2];

      AttackRook( bb_desti, from );
      bb_desti.p[1] &= bb_empty.p[1];
      bb_desti.p[2] &= bb_empty.p[2];

      utemp = From2Move( from ) | Piece2Move( rook );
      while ( bb_desti.p[1] | bb_desti.p[2] )
	{
	  to            = last_one12( bb_desti.p[1], bb_desti.p[2] );
	  bb_desti.p[1] ^= abb_mask[to].p[1];
	  bb_desti.p[2] ^= abb_mask[to].p[2];
	  *pmove++ = To2Move( to ) | utemp;
	}
    }
  //�n
  bb_piece = BB_BHORSE;
  foreach_bitboard_lastone(bb_piece, from,
  {
	  AttackHorse(bb_desti, from);
	  BBAnd(bb_desti, bb_desti, bb_empty);
	  utemp = From2Move(from) | Piece2Move(horse);
	  foreach_bitboard_lastone(bb_desti, to,
	  {
		  *pmove++ = To2Move(to) | utemp;
	  }
	  );
  }
  ); 
  /*
  while( BBTest( bb_piece ) )
    {
      from = LastOne( bb_piece );
      Xor( from, bb_piece );
      
      AttackHorse( bb_desti, from );
      BBAnd( bb_desti, bb_desti, bb_empty );
      utemp = From2Move( from ) | Piece2Move( horse ); 
      while ( BBTest( bb_desti ) )
	{
	  to = LastOne( bb_desti );
	  Xor( to, bb_desti );
	  *pmove++ = To2Move( to ) | utemp;
	}
    }*/

  //��
  bb_piece = BB_BDRAGON;
  foreach_bitboard_lastone(bb_piece, from,
  {
	  AttackDragon(bb_desti, from);
	  BBAnd(bb_desti, bb_desti, bb_empty);
	  utemp = From2Move(from) | Piece2Move(dragon);
	  foreach_bitboard_lastone(bb_desti, to,
	  {
		  *pmove++ = To2Move(to) | utemp;
	  }
	  );
  }
  );
  /*
  while( BBTest( bb_piece ) )
    {
      from = LastOne( bb_piece );
      Xor( from, bb_piece );
      
      AttackDragon( bb_desti, from );
      BBAnd( bb_desti, bb_desti, bb_empty );
      utemp = From2Move( from ) | Piece2Move( dragon ); 
      while ( BBTest( bb_desti ) )
	{
	  to = LastOne( bb_desti );
	  Xor( to, bb_desti );
	  *pmove++ = To2Move( to ) | utemp;
	}
    }
	*/
  bb_empty.p[0] &= 0x1ffU;
  //��
  bb_piece = BB_BLANCE;
  foreach_bitboard_lastone(bb_piece, from,
  {
	  bb_desti = AttackFile(from);
	  BBAnd(bb_desti, bb_desti, abb_minus_rays[from]);
	  BBAnd(bb_desti, bb_desti, bb_empty);

	  utemp = From2Move(from) | Piece2Move(lance);

	  foreach_bitboard_lastone(bb_desti, to,
	  {
		  *pmove++ = To2Move(to) | utemp;
	   }
	   );
  }
  );
  /*
  while( BBTest( bb_piece ) )
    {
      from = LastOne( bb_piece );
      Xor( from, bb_piece );

      bb_desti = AttackFile( from );
      BBAnd( bb_desti, bb_desti, abb_minus_rays[from] );
      BBAnd( bb_desti, bb_desti, bb_empty );

      utemp = From2Move( from ) | Piece2Move( lance ); 
      while ( BBTest( bb_desti ) )
	{
	  to = LastOne( bb_desti );
	  Xor( to, bb_desti );
	  *pmove++ = To2Move( to ) | utemp;
	}
    }
  */

  bb_piece = BB_BKNIGHT;
  foreach_bitboard_lastone(bb_piece, from,
  {
	  BBAnd(bb_desti, bb_empty, abb_b_knight_attacks[from]);
	  utemp = From2Move(from) | Piece2Move(knight);

	  foreach_bitboard_lastone(bb_desti, to,
	  {
		  *pmove++ = To2Move(to) | utemp;
	  }
	  );
  }
  );

  /*
  while( BBTest( bb_piece ) )
    {
      from = LastOne( bb_piece );
      Xor( from, bb_piece );

      BBAnd( bb_desti, bb_empty, abb_b_knight_attacks[from] );
      utemp = From2Move( from ) | Piece2Move( knight ); 
      while ( BBTest( bb_desti ) )
	{
	  to = LastOne( bb_desti );
	  Xor( to, bb_desti );
	  *pmove++ = To2Move( to ) | utemp;
	}
    }
	*/

  return pmove;
}


unsigned int * CONV
w_gen_nocaptures( const tree_t * restrict ptree,
		  unsigned int * restrict pmove )
{
  bitboard_t bb_empty, bb_piece, bb_desti;
  unsigned int utemp;
  int to, from;

  BBOr( bb_empty, BB_BOCCUPY, BB_WOCCUPY );
  BBNot( bb_empty, bb_empty );

  bb_piece.p[0] = BB_WPAWN_ATK.p[0] & bb_empty.p[0];
  bb_piece.p[1] = BB_WPAWN_ATK.p[1] & bb_empty.p[1];
  while( bb_piece.p[0] | bb_piece.p[1] )
    {
      to            = first_one01( bb_piece.p[0], bb_piece.p[1] );
      bb_piece.p[0] ^= abb_mask[to].p[0];
      bb_piece.p[1] ^= abb_mask[to].p[1];
      from          = to - 9;
      *pmove++ = To2Move( to ) | From2Move( from ) | Piece2Move( pawn );
    }

  bb_piece = BB_WSILVER;
  foreach_bitboard_firstone_no_check(bb_piece, from,
  {
	  BBAnd(bb_desti, bb_empty, abb_w_silver_attacks[from]);

	  foreach_bitboard_firstone(bb_desti, to,
	  {
		  utemp = To2Move(to) | From2Move(from) | Piece2Move(silver);
		  if (from > I4 || to > I4) { *pmove++ = utemp | FLAG_PROMO; }
		  *pmove++ = utemp;
	  }
	  );
  }
  );
  /*
  while( BBTest( bb_piece ) )
    {
      from = FirstOne( bb_piece );
      Xor( from, bb_piece );

      BBAnd( bb_desti, bb_empty, abb_w_silver_attacks[from] );
      while ( BBTest( bb_desti ) )
	{
	  to = FirstOne( bb_desti );
	  Xor( to, bb_desti );

	  utemp = To2Move( to ) | From2Move( from ) | Piece2Move( silver );
	  if ( from > I4 || to > I4 ) { *pmove++ = utemp | FLAG_PROMO; }
	  *pmove++ = utemp;
	}
    }*/

  bb_piece = BB_WTGOLD;

  foreach_bitboard_firstone_no_check(bb_piece, from,
  {
	  BBAnd(bb_desti, bb_empty, abb_w_gold_attacks[from]);
	  utemp = From2Move(from) | Piece2Move(-BOARD[from]);

	  foreach_bitboard_firstone(bb_desti, to,
	  {
		  *pmove++ = To2Move(to) | utemp;
	  }
	  );
  }
  );
  /*
  while( BBTest( bb_piece ) )
    {
      from = FirstOne( bb_piece );
      Xor( from, bb_piece );

      BBAnd( bb_desti, bb_empty, abb_w_gold_attacks[from] );
      utemp = From2Move( from ) | Piece2Move( -BOARD[from] ); 
      while ( BBTest( bb_desti ) )
	{
	  to = FirstOne( bb_desti );
	  Xor( to, bb_desti );
	  *pmove++ = To2Move( to ) | utemp;
	}
    }*/

  from = SQ_WKING;
  BBAnd( bb_desti, bb_empty, abb_king_attacks[from] );
  utemp = From2Move( from ) | Piece2Move( king ); 

  foreach_bitboard_firstone_no_check(bb_desti, to,
  {
	  *pmove++ = To2Move(to) | utemp;
  }
  );
  /*
  while ( BBTest( bb_desti ) )
    {
      to = FirstOne( bb_desti );
      Xor( to, bb_desti );
      *pmove++ = To2Move( to ) | utemp;
    }*/

  bb_piece.p[0] = BB_WBISHOP.p[0];
  bb_piece.p[1] = BB_WBISHOP.p[1];
  while( bb_piece.p[0] | bb_piece.p[1] )
    {
      from          = first_one01( bb_piece.p[0], bb_piece.p[1] );
      bb_piece.p[0] ^= abb_mask[from].p[0];
      bb_piece.p[1] ^= abb_mask[from].p[1];

      AttackBishop( bb_desti, from );
      bb_desti.p[0] &= bb_empty.p[0];
      bb_desti.p[1] &= bb_empty.p[1];

      utemp = From2Move( from ) | Piece2Move( bishop );
      while ( bb_desti.p[0] | bb_desti.p[1] )
	{
	  to            = first_one01( bb_desti.p[0], bb_desti.p[1] );
	  bb_desti.p[0] ^= abb_mask[to].p[0];
	  bb_desti.p[1] ^= abb_mask[to].p[1];
	  *pmove++ = To2Move( to ) | utemp;
	}
    }

  bb_piece.p[0] = BB_WROOK.p[0];
  bb_piece.p[1] = BB_WROOK.p[1];
  while( bb_piece.p[0] | bb_piece.p[1] )
    {
      from          = first_one01( bb_piece.p[0], bb_piece.p[1] );
      bb_piece.p[0] ^= abb_mask[from].p[0];
      bb_piece.p[1] ^= abb_mask[from].p[1];

      AttackRook( bb_desti, from );
      bb_desti.p[0] &= bb_empty.p[0];
      bb_desti.p[1] &= bb_empty.p[1];

      utemp = From2Move( from ) | Piece2Move( rook );
      while ( bb_desti.p[0] | bb_desti.p[1] )
	{
	  to            = first_one01( bb_desti.p[0], bb_desti.p[1] );
	  bb_desti.p[0] ^= abb_mask[to].p[0];
	  bb_desti.p[1] ^= abb_mask[to].p[1];
	  *pmove++ = To2Move( to ) | utemp;
	}
    }

  bb_piece = BB_WHORSE;
  foreach_bitboard_firstone(bb_piece, from,
  {
	  AttackHorse(bb_desti, from);
	  BBAnd(bb_desti, bb_desti, bb_empty);
	  utemp = From2Move(from) | Piece2Move(horse);

	  foreach_bitboard_firstone(bb_desti, to,
	  {
		  *pmove++ = To2Move(to) | utemp;
	  }
	  );
  }
  );
  /*
  while( BBTest( bb_piece ) )
    {
      from = FirstOne( bb_piece );
      Xor( from, bb_piece );
      
      AttackHorse( bb_desti, from );
      BBAnd( bb_desti, bb_desti, bb_empty );
      utemp = From2Move( from ) | Piece2Move( horse ); 
      while ( BBTest( bb_desti ) )
	{
	  to = FirstOne( bb_desti );
	  Xor( to, bb_desti );
	  *pmove++ = To2Move( to ) | utemp;
	}
    }*/

  bb_piece = BB_WDRAGON;
  foreach_bitboard_firstone(bb_piece, from,
  {
	  AttackDragon(bb_desti, from);
	  BBAnd(bb_desti, bb_desti, bb_empty);
	  utemp = From2Move(from) | Piece2Move(dragon);

	  foreach_bitboard_firstone(bb_desti, to,
	  {
		  *pmove++ = To2Move(to) | utemp;
	  }
	  );
  }
  );
  /*
  while( BBTest( bb_piece ) )
    {
      from = FirstOne( bb_piece );
      Xor( from, bb_piece );
      
      AttackDragon( bb_desti, from );
      BBAnd( bb_desti, bb_desti, bb_empty );
      utemp = From2Move( from ) | Piece2Move( dragon ); 
      while ( BBTest( bb_desti ) )
	{
	  to = FirstOne( bb_desti );
	  Xor( to, bb_desti );
	  *pmove++ = To2Move( to ) | utemp;
	}
    }*/

  bb_empty.p[2] &= 0x7fc0000U;

  bb_piece = BB_WLANCE;
  foreach_bitboard_firstone(bb_piece, from,
  {
	  bb_desti = AttackFile(from);
	  BBAnd(bb_desti, bb_desti, abb_plus_rays[from]);
	  BBAnd(bb_desti, bb_desti, bb_empty);

	  utemp = From2Move(from) | Piece2Move(lance);

	  foreach_bitboard_firstone(bb_desti, to,
	  {
		  *pmove++ = To2Move(to) | utemp;
	  }
	  );
  }
  );
  /*
  while( BBTest( bb_piece ) )
    {
      from = FirstOne( bb_piece );
      Xor( from, bb_piece );

      bb_desti = AttackFile( from );
      BBAnd( bb_desti, bb_desti, abb_plus_rays[from] );
      BBAnd( bb_desti, bb_desti, bb_empty );

      utemp = From2Move( from ) | Piece2Move( lance ); 
      while ( BBTest( bb_desti ) )
	{
	  to = FirstOne( bb_desti );
	  Xor( to, bb_desti );
	  *pmove++ = To2Move( to ) | utemp;
	}
    }*/

  bb_piece = BB_WKNIGHT;
  foreach_bitboard_firstone(bb_piece, from,
  {
	  BBAnd(bb_desti, bb_empty, abb_w_knight_attacks[from]);
	  utemp = From2Move(from) | Piece2Move(knight);

	  foreach_bitboard_firstone(bb_desti, to,
	  {
		  *pmove++ = To2Move(to) | utemp;
	  }
	  );
  }
  );
  /*
  while( BBTest( bb_piece ) )
    {
      from = FirstOne( bb_piece );
      Xor( from, bb_piece );

      BBAnd( bb_desti, bb_empty, abb_w_knight_attacks[from] );
      utemp = From2Move( from ) | Piece2Move( knight ); 
      while ( BBTest( bb_desti ) )
	{
	  to = FirstOne( bb_desti );
	  Xor( to, bb_desti );
	  *pmove++ = To2Move( to ) | utemp;
	}
    }*/

  return pmove;
}
