

#ifndef __MATH_EMU_OP_COMMON_H__
#define __MATH_EMU_OP_COMMON_H__

#define _FP_DECL(wc, X)			\
  _FP_I_TYPE X##_c=0, X##_s=0, X##_e=0;	\
  _FP_FRAC_DECL_##wc(X)



#define _FP_UNPACK_CANONICAL(fs, wc, X)					\
do {									\
  switch (X##_e)							\
  {									\
  default:								\
    _FP_FRAC_HIGH_RAW_##fs(X) |= _FP_IMPLBIT_##fs;			\
    _FP_FRAC_SLL_##wc(X, _FP_WORKBITS);					\
    X##_e -= _FP_EXPBIAS_##fs;						\
    X##_c = FP_CLS_NORMAL;						\
    break;								\
									\
  case 0:								\
    if (_FP_FRAC_ZEROP_##wc(X))						\
      X##_c = FP_CLS_ZERO;						\
    else								\
      {									\
						\
	_FP_I_TYPE _shift;						\
	_FP_FRAC_CLZ_##wc(_shift, X);					\
	_shift -= _FP_FRACXBITS_##fs;					\
	_FP_FRAC_SLL_##wc(X, (_shift+_FP_WORKBITS));			\
	X##_e -= _FP_EXPBIAS_##fs - 1 + _shift;				\
	X##_c = FP_CLS_NORMAL;						\
	FP_SET_EXCEPTION(FP_EX_DENORM);					\
	if (FP_DENORM_ZERO)						\
	  {								\
	    FP_SET_EXCEPTION(FP_EX_INEXACT);				\
	    X##_c = FP_CLS_ZERO;					\
	  }								\
      }									\
    break;								\
									\
  case _FP_EXPMAX_##fs:							\
    if (_FP_FRAC_ZEROP_##wc(X))						\
      X##_c = FP_CLS_INF;						\
    else								\
      {									\
	X##_c = FP_CLS_NAN;						\
						\
	if (!(_FP_FRAC_HIGH_RAW_##fs(X) & _FP_QNANBIT_##fs))		\
	  FP_SET_EXCEPTION(FP_EX_INVALID | FP_EX_INVALID_SNAN);		\
      }									\
    break;								\
  }									\
} while (0)



#define _FP_PACK_CANONICAL(fs, wc, X)				\
do {								\
  switch (X##_c)						\
  {								\
  case FP_CLS_NORMAL:						\
    X##_e += _FP_EXPBIAS_##fs;					\
    if (X##_e > 0)						\
      {								\
	_FP_ROUND(wc, X);					\
	if (_FP_FRAC_OVERP_##wc(fs, X))				\
	  {							\
	    _FP_FRAC_CLEAR_OVERP_##wc(fs, X);			\
	    X##_e++;						\
	  }							\
	_FP_FRAC_SRL_##wc(X, _FP_WORKBITS);			\
	if (X##_e >= _FP_EXPMAX_##fs)				\
	  {							\
	    					\
	    switch (FP_ROUNDMODE)				\
	      {							\
	      case FP_RND_NEAREST:				\
		X##_c = FP_CLS_INF;				\
		break;						\
	      case FP_RND_PINF:					\
		if (!X##_s) X##_c = FP_CLS_INF;			\
		break;						\
	      case FP_RND_MINF:					\
		if (X##_s) X##_c = FP_CLS_INF;			\
		break;						\
	      }							\
	    if (X##_c == FP_CLS_INF)				\
	      {							\
					\
		X##_e = _FP_EXPMAX_##fs;			\
		_FP_FRAC_SET_##wc(X, _FP_ZEROFRAC_##wc);	\
	      }							\
	    else						\
	      {							\
				\
		X##_e = _FP_EXPMAX_##fs - 1;			\
		_FP_FRAC_SET_##wc(X, _FP_MAXFRAC_##wc);		\
	      }							\
	    FP_SET_EXCEPTION(FP_EX_OVERFLOW);			\
            FP_SET_EXCEPTION(FP_EX_INEXACT);			\
	  }							\
      }								\
    else							\
      {								\
				\
	X##_e = -X##_e + 1;					\
	if (X##_e <= _FP_WFRACBITS_##fs)			\
	  {							\
	    _FP_FRAC_SRS_##wc(X, X##_e, _FP_WFRACBITS_##fs);	\
	    if (_FP_FRAC_HIGH_##fs(X)				\
		& (_FP_OVERFLOW_##fs >> 1))			\
	      {							\
	        X##_e = 1;					\
	        _FP_FRAC_SET_##wc(X, _FP_ZEROFRAC_##wc);	\
	      }							\
	    else						\
	      {							\
		_FP_ROUND(wc, X);				\
		if (_FP_FRAC_HIGH_##fs(X)			\
		   & (_FP_OVERFLOW_##fs >> 1))			\
		  {						\
		    X##_e = 1;					\
		    _FP_FRAC_SET_##wc(X, _FP_ZEROFRAC_##wc);	\
		    FP_SET_EXCEPTION(FP_EX_INEXACT);		\
		  }						\
		else						\
		  {						\
		    X##_e = 0;					\
		    _FP_FRAC_SRL_##wc(X, _FP_WORKBITS);		\
		  }						\
	      }							\
	    if ((FP_CUR_EXCEPTIONS & FP_EX_INEXACT) ||		\
		(FP_TRAPPING_EXCEPTIONS & FP_EX_UNDERFLOW))	\
		FP_SET_EXCEPTION(FP_EX_UNDERFLOW);		\
	  }							\
	else							\
	  {							\
	    				\
	    X##_e = 0;						\
	    if (!_FP_FRAC_ZEROP_##wc(X))			\
	      {							\
	        _FP_FRAC_SET_##wc(X, _FP_MINFRAC_##wc);		\
	        _FP_ROUND(wc, X);				\
	        _FP_FRAC_LOW_##wc(X) >>= (_FP_WORKBITS);	\
	      }							\
	    FP_SET_EXCEPTION(FP_EX_UNDERFLOW);			\
	  }							\
      }								\
    break;							\
								\
  case FP_CLS_ZERO:						\
    X##_e = 0;							\
    _FP_FRAC_SET_##wc(X, _FP_ZEROFRAC_##wc);			\
    break;							\
								\
  case FP_CLS_INF:						\
    X##_e = _FP_EXPMAX_##fs;					\
    _FP_FRAC_SET_##wc(X, _FP_ZEROFRAC_##wc);			\
    break;							\
								\
  case FP_CLS_NAN:						\
    X##_e = _FP_EXPMAX_##fs;					\
    if (!_FP_KEEPNANFRACP)					\
      {								\
	_FP_FRAC_SET_##wc(X, _FP_NANFRAC_##fs);			\
	X##_s = _FP_NANSIGN_##fs;				\
      }								\
    else							\
      _FP_FRAC_HIGH_RAW_##fs(X) |= _FP_QNANBIT_##fs;		\
    break;							\
  }								\
} while (0)


#define _FP_ISSIGNAN(fs, wc, X)					\
({								\
  int __ret = 0;						\
  if (X##_e == _FP_EXPMAX_##fs)					\
    {								\
      if (!_FP_FRAC_ZEROP_##wc(X)				\
	  && !(_FP_FRAC_HIGH_RAW_##fs(X) & _FP_QNANBIT_##fs))	\
	__ret = 1;						\
    }								\
  __ret;							\
})







#define _FP_ADD_INTERNAL(fs, wc, R, X, Y, OP)				     \
do {									     \
  switch (_FP_CLS_COMBINE(X##_c, Y##_c))				     \
  {									     \
  case _FP_CLS_COMBINE(FP_CLS_NORMAL,FP_CLS_NORMAL):			     \
    {									     \
       \
      _FP_I_TYPE diff = X##_e - Y##_e;					     \
									     \
      if (diff < 0)							     \
	{								     \
	  diff = -diff;							     \
	  if (diff <= _FP_WFRACBITS_##fs)				     \
	    _FP_FRAC_SRS_##wc(X, diff, _FP_WFRACBITS_##fs);		     \
	  else if (!_FP_FRAC_ZEROP_##wc(X))				     \
	    _FP_FRAC_SET_##wc(X, _FP_MINFRAC_##wc);			     \
	  R##_e = Y##_e;						     \
	}								     \
      else								     \
	{								     \
	  if (diff > 0)							     \
	    {								     \
	      if (diff <= _FP_WFRACBITS_##fs)				     \
	        _FP_FRAC_SRS_##wc(Y, diff, _FP_WFRACBITS_##fs);		     \
	      else if (!_FP_FRAC_ZEROP_##wc(Y))				     \
	        _FP_FRAC_SET_##wc(Y, _FP_MINFRAC_##wc);			     \
	    }								     \
	  R##_e = X##_e;						     \
	}								     \
									     \
      R##_c = FP_CLS_NORMAL;						     \
									     \
      if (X##_s == Y##_s)						     \
	{								     \
	  R##_s = X##_s;						     \
	  _FP_FRAC_ADD_##wc(R, X, Y);					     \
	  if (_FP_FRAC_OVERP_##wc(fs, R))				     \
	    {								     \
	      _FP_FRAC_SRS_##wc(R, 1, _FP_WFRACBITS_##fs);		     \
	      R##_e++;							     \
	    }								     \
	}								     \
      else								     \
	{								     \
	  R##_s = X##_s;						     \
	  _FP_FRAC_SUB_##wc(R, X, Y);					     \
	  if (_FP_FRAC_ZEROP_##wc(R))					     \
	    {								     \
	      				     \
	      if (FP_ROUNDMODE == FP_RND_MINF)				     \
		R##_s |= Y##_s;						     \
	      else							     \
		R##_s &= Y##_s;						     \
	      R##_c = FP_CLS_ZERO;					     \
	    }								     \
	  else								     \
	    {								     \
	      if (_FP_FRAC_NEGP_##wc(R))				     \
		{							     \
		  _FP_FRAC_SUB_##wc(R, Y, X);				     \
		  R##_s = Y##_s;					     \
		}							     \
									     \
	      			     \
	      _FP_FRAC_CLZ_##wc(diff, R);				     \
	      diff -= _FP_WFRACXBITS_##fs;				     \
	      if (diff)							     \
		{							     \
		  R##_e -= diff;					     \
		  _FP_FRAC_SLL_##wc(R, diff);				     \
		}							     \
	    }								     \
	}								     \
      break;								     \
    }									     \
									     \
  case _FP_CLS_COMBINE(FP_CLS_NAN,FP_CLS_NAN):				     \
    _FP_CHOOSENAN(fs, wc, R, X, Y, OP);					     \
    break;								     \
									     \
  case _FP_CLS_COMBINE(FP_CLS_NORMAL,FP_CLS_ZERO):			     \
    R##_e = X##_e;							     \
  case _FP_CLS_COMBINE(FP_CLS_NAN,FP_CLS_NORMAL):			     \
  case _FP_CLS_COMBINE(FP_CLS_NAN,FP_CLS_INF):				     \
  case _FP_CLS_COMBINE(FP_CLS_NAN,FP_CLS_ZERO):				     \
    _FP_FRAC_COPY_##wc(R, X);						     \
    R##_s = X##_s;							     \
    R##_c = X##_c;							     \
    break;								     \
									     \
  case _FP_CLS_COMBINE(FP_CLS_ZERO,FP_CLS_NORMAL):			     \
    R##_e = Y##_e;							     \
  case _FP_CLS_COMBINE(FP_CLS_NORMAL,FP_CLS_NAN):			     \
  case _FP_CLS_COMBINE(FP_CLS_INF,FP_CLS_NAN):				     \
  case _FP_CLS_COMBINE(FP_CLS_ZERO,FP_CLS_NAN):				     \
    _FP_FRAC_COPY_##wc(R, Y);						     \
    R##_s = Y##_s;							     \
    R##_c = Y##_c;							     \
    break;								     \
									     \
  case _FP_CLS_COMBINE(FP_CLS_INF,FP_CLS_INF):				     \
    if (X##_s != Y##_s)							     \
      {									     \
						     \
	_FP_FRAC_SET_##wc(R, _FP_NANFRAC_##fs);				     \
	R##_s = _FP_NANSIGN_##fs;					     \
	R##_c = FP_CLS_NAN;						     \
	FP_SET_EXCEPTION(FP_EX_INVALID | FP_EX_INVALID_ISI);		     \
	break;								     \
      }									     \
    							     \
									     \
  case _FP_CLS_COMBINE(FP_CLS_INF,FP_CLS_NORMAL):			     \
  case _FP_CLS_COMBINE(FP_CLS_INF,FP_CLS_ZERO):				     \
    R##_s = X##_s;							     \
    R##_c = FP_CLS_INF;							     \
    break;								     \
									     \
  case _FP_CLS_COMBINE(FP_CLS_NORMAL,FP_CLS_INF):			     \
  case _FP_CLS_COMBINE(FP_CLS_ZERO,FP_CLS_INF):				     \
    R##_s = Y##_s;							     \
    R##_c = FP_CLS_INF;							     \
    break;								     \
									     \
  case _FP_CLS_COMBINE(FP_CLS_ZERO,FP_CLS_ZERO):			     \
    					     \
    if (FP_ROUNDMODE == FP_RND_MINF)					     \
      R##_s = X##_s | Y##_s;						     \
    else								     \
      R##_s = X##_s & Y##_s;						     \
    R##_c = FP_CLS_ZERO;						     \
    break;								     \
									     \
  default:								     \
    abort();								     \
  }									     \
} while (0)

#define _FP_ADD(fs, wc, R, X, Y) _FP_ADD_INTERNAL(fs, wc, R, X, Y, '+')
#define _FP_SUB(fs, wc, R, X, Y)					     \
  do {									     \
    if (Y##_c != FP_CLS_NAN) Y##_s ^= 1;				     \
    _FP_ADD_INTERNAL(fs, wc, R, X, Y, '-');				     \
  } while (0)




#define _FP_NEG(fs, wc, R, X)		\
  do {					\
    _FP_FRAC_COPY_##wc(R, X);		\
    R##_c = X##_c;			\
    R##_e = X##_e;			\
    R##_s = 1 ^ X##_s;			\
  } while (0)




#define _FP_MUL(fs, wc, R, X, Y)			\
do {							\
  R##_s = X##_s ^ Y##_s;				\
  switch (_FP_CLS_COMBINE(X##_c, Y##_c))		\
  {							\
  case _FP_CLS_COMBINE(FP_CLS_NORMAL,FP_CLS_NORMAL):	\
    R##_c = FP_CLS_NORMAL;				\
    R##_e = X##_e + Y##_e + 1;				\
							\
    _FP_MUL_MEAT_##fs(R,X,Y);				\
							\
    if (_FP_FRAC_OVERP_##wc(fs, R))			\
      _FP_FRAC_SRS_##wc(R, 1, _FP_WFRACBITS_##fs);	\
    else						\
      R##_e--;						\
    break;						\
							\
  case _FP_CLS_COMBINE(FP_CLS_NAN,FP_CLS_NAN):		\
    _FP_CHOOSENAN(fs, wc, R, X, Y, '*');		\
    break;						\
							\
  case _FP_CLS_COMBINE(FP_CLS_NAN,FP_CLS_NORMAL):	\
  case _FP_CLS_COMBINE(FP_CLS_NAN,FP_CLS_INF):		\
  case _FP_CLS_COMBINE(FP_CLS_NAN,FP_CLS_ZERO):		\
    R##_s = X##_s;					\
							\
  case _FP_CLS_COMBINE(FP_CLS_INF,FP_CLS_INF):		\
  case _FP_CLS_COMBINE(FP_CLS_INF,FP_CLS_NORMAL):	\
  case _FP_CLS_COMBINE(FP_CLS_ZERO,FP_CLS_NORMAL):	\
  case _FP_CLS_COMBINE(FP_CLS_ZERO,FP_CLS_ZERO):	\
    _FP_FRAC_COPY_##wc(R, X);				\
    R##_c = X##_c;					\
    break;						\
							\
  case _FP_CLS_COMBINE(FP_CLS_NORMAL,FP_CLS_NAN):	\
  case _FP_CLS_COMBINE(FP_CLS_INF,FP_CLS_NAN):		\
  case _FP_CLS_COMBINE(FP_CLS_ZERO,FP_CLS_NAN):		\
    R##_s = Y##_s;					\
							\
  case _FP_CLS_COMBINE(FP_CLS_NORMAL,FP_CLS_INF):	\
  case _FP_CLS_COMBINE(FP_CLS_NORMAL,FP_CLS_ZERO):	\
    _FP_FRAC_COPY_##wc(R, Y);				\
    R##_c = Y##_c;					\
    break;						\
							\
  case _FP_CLS_COMBINE(FP_CLS_INF,FP_CLS_ZERO):		\
  case _FP_CLS_COMBINE(FP_CLS_ZERO,FP_CLS_INF):		\
    R##_s = _FP_NANSIGN_##fs;				\
    R##_c = FP_CLS_NAN;					\
    _FP_FRAC_SET_##wc(R, _FP_NANFRAC_##fs);		\
    FP_SET_EXCEPTION(FP_EX_INVALID | FP_EX_INVALID_IMZ);\
    break;						\
							\
  default:						\
    abort();						\
  }							\
} while (0)




#define _FP_DIV(fs, wc, R, X, Y)			\
do {							\
  R##_s = X##_s ^ Y##_s;				\
  switch (_FP_CLS_COMBINE(X##_c, Y##_c))		\
  {							\
  case _FP_CLS_COMBINE(FP_CLS_NORMAL,FP_CLS_NORMAL):	\
    R##_c = FP_CLS_NORMAL;				\
    R##_e = X##_e - Y##_e;				\
							\
    _FP_DIV_MEAT_##fs(R,X,Y);				\
    break;						\
							\
  case _FP_CLS_COMBINE(FP_CLS_NAN,FP_CLS_NAN):		\
    _FP_CHOOSENAN(fs, wc, R, X, Y, '/');		\
    break;						\
							\
  case _FP_CLS_COMBINE(FP_CLS_NAN,FP_CLS_NORMAL):	\
  case _FP_CLS_COMBINE(FP_CLS_NAN,FP_CLS_INF):		\
  case _FP_CLS_COMBINE(FP_CLS_NAN,FP_CLS_ZERO):		\
    R##_s = X##_s;					\
    _FP_FRAC_COPY_##wc(R, X);				\
    R##_c = X##_c;					\
    break;						\
							\
  case _FP_CLS_COMBINE(FP_CLS_NORMAL,FP_CLS_NAN):	\
  case _FP_CLS_COMBINE(FP_CLS_INF,FP_CLS_NAN):		\
  case _FP_CLS_COMBINE(FP_CLS_ZERO,FP_CLS_NAN):		\
    R##_s = Y##_s;					\
    _FP_FRAC_COPY_##wc(R, Y);				\
    R##_c = Y##_c;					\
    break;						\
							\
  case _FP_CLS_COMBINE(FP_CLS_NORMAL,FP_CLS_INF):	\
  case _FP_CLS_COMBINE(FP_CLS_ZERO,FP_CLS_INF):		\
  case _FP_CLS_COMBINE(FP_CLS_ZERO,FP_CLS_NORMAL):	\
    R##_c = FP_CLS_ZERO;				\
    break;						\
							\
  case _FP_CLS_COMBINE(FP_CLS_NORMAL,FP_CLS_ZERO):	\
    FP_SET_EXCEPTION(FP_EX_DIVZERO);			\
  case _FP_CLS_COMBINE(FP_CLS_INF,FP_CLS_ZERO):		\
  case _FP_CLS_COMBINE(FP_CLS_INF,FP_CLS_NORMAL):	\
    R##_c = FP_CLS_INF;					\
    break;						\
							\
  case _FP_CLS_COMBINE(FP_CLS_INF,FP_CLS_INF):		\
    R##_s = _FP_NANSIGN_##fs;				\
    R##_c = FP_CLS_NAN;					\
    _FP_FRAC_SET_##wc(R, _FP_NANFRAC_##fs);		\
    FP_SET_EXCEPTION(FP_EX_INVALID | FP_EX_INVALID_IDI);\
    break;						\
							\
  case _FP_CLS_COMBINE(FP_CLS_ZERO,FP_CLS_ZERO):	\
    R##_s = _FP_NANSIGN_##fs;				\
    R##_c = FP_CLS_NAN;					\
    _FP_FRAC_SET_##wc(R, _FP_NANFRAC_##fs);		\
    FP_SET_EXCEPTION(FP_EX_INVALID | FP_EX_INVALID_ZDZ);\
    break;						\
							\
  default:						\
    abort();						\
  }							\
} while (0)




#define _FP_CMP(fs, wc, ret, X, Y, un)					\
  do {									\
    						\
    if ((X##_e == _FP_EXPMAX_##fs && !_FP_FRAC_ZEROP_##wc(X))		\
	|| (Y##_e == _FP_EXPMAX_##fs && !_FP_FRAC_ZEROP_##wc(Y)))	\
      {									\
	ret = un;							\
      }									\
    else								\
      {									\
	int __is_zero_x;						\
	int __is_zero_y;						\
									\
	__is_zero_x = (!X##_e && _FP_FRAC_ZEROP_##wc(X)) ? 1 : 0;	\
	__is_zero_y = (!Y##_e && _FP_FRAC_ZEROP_##wc(Y)) ? 1 : 0;	\
									\
	if (__is_zero_x && __is_zero_y)					\
		ret = 0;						\
	else if (__is_zero_x)						\
		ret = Y##_s ? 1 : -1;					\
	else if (__is_zero_y)						\
		ret = X##_s ? -1 : 1;					\
	else if (X##_s != Y##_s)					\
	  ret = X##_s ? -1 : 1;						\
	else if (X##_e > Y##_e)						\
	  ret = X##_s ? -1 : 1;						\
	else if (X##_e < Y##_e)						\
	  ret = X##_s ? 1 : -1;						\
	else if (_FP_FRAC_GT_##wc(X, Y))				\
	  ret = X##_s ? -1 : 1;						\
	else if (_FP_FRAC_GT_##wc(Y, X))				\
	  ret = X##_s ? 1 : -1;						\
	else								\
	  ret = 0;							\
      }									\
  } while (0)




#define _FP_CMP_EQ(fs, wc, ret, X, Y)					  \
  do {									  \
    						  \
    if ((X##_e == _FP_EXPMAX_##fs && !_FP_FRAC_ZEROP_##wc(X))		  \
	|| (Y##_e == _FP_EXPMAX_##fs && !_FP_FRAC_ZEROP_##wc(Y)))	  \
      {									  \
	ret = 1;							  \
      }									  \
    else								  \
      {									  \
	ret = !(X##_e == Y##_e						  \
		&& _FP_FRAC_EQ_##wc(X, Y)				  \
		&& (X##_s == Y##_s || !X##_e && _FP_FRAC_ZEROP_##wc(X))); \
      }									  \
  } while (0)



#define _FP_SQRT(fs, wc, R, X)						\
do {									\
    _FP_FRAC_DECL_##wc(T); _FP_FRAC_DECL_##wc(S);			\
    _FP_W_TYPE q;							\
    switch (X##_c)							\
    {									\
    case FP_CLS_NAN:							\
	_FP_FRAC_COPY_##wc(R, X);					\
	R##_s = X##_s;							\
    	R##_c = FP_CLS_NAN;						\
    	break;								\
    case FP_CLS_INF:							\
    	if (X##_s)							\
    	  {								\
    	    R##_s = _FP_NANSIGN_##fs;					\
	    R##_c = FP_CLS_NAN; 				\
	    _FP_FRAC_SET_##wc(R, _FP_NANFRAC_##fs);			\
	    FP_SET_EXCEPTION(FP_EX_INVALID);				\
    	  }								\
    	else								\
    	  {								\
    	    R##_s = 0;							\
    	    R##_c = FP_CLS_INF; 			\
    	  }								\
    	break;								\
    case FP_CLS_ZERO:							\
	R##_s = X##_s;							\
	R##_c = FP_CLS_ZERO; 			\
	break;								\
    case FP_CLS_NORMAL:							\
    	R##_s = 0;							\
        if (X##_s)							\
          {								\
	    R##_c = FP_CLS_NAN; 				\
	    R##_s = _FP_NANSIGN_##fs;					\
	    _FP_FRAC_SET_##wc(R, _FP_NANFRAC_##fs);			\
	    FP_SET_EXCEPTION(FP_EX_INVALID);				\
	    break;							\
          }								\
    	R##_c = FP_CLS_NORMAL;						\
        if (X##_e & 1)							\
          _FP_FRAC_SLL_##wc(X, 1);					\
        R##_e = X##_e >> 1;						\
        _FP_FRAC_SET_##wc(S, _FP_ZEROFRAC_##wc);			\
        _FP_FRAC_SET_##wc(R, _FP_ZEROFRAC_##wc);			\
        q = _FP_OVERFLOW_##fs >> 1;					\
        _FP_SQRT_MEAT_##wc(R, S, T, X, q);				\
    }									\
  } while (0)




#define _FP_TO_INT(fs, wc, r, X, rsize, rsigned)				\
  do {										\
    switch (X##_c)								\
      {										\
      case FP_CLS_NORMAL:							\
	if (X##_e < 0)								\
	  {									\
	    FP_SET_EXCEPTION(FP_EX_INEXACT);					\
	  case FP_CLS_ZERO:							\
	    r = 0;								\
	  }									\
	else if (X##_e >= rsize - (rsigned > 0 || X##_s)			\
		 || (!rsigned && X##_s))					\
	  {								\
	  case FP_CLS_NAN:                                                      \
	  case FP_CLS_INF:							\
	    if (rsigned == 2)							\
	      {									\
		if (X##_c != FP_CLS_NORMAL					\
		    || X##_e >= rsize - 1 + _FP_WFRACBITS_##fs)			\
		  r = 0;							\
		else								\
		  {								\
		    _FP_FRAC_SLL_##wc(X, (X##_e - _FP_WFRACBITS_##fs + 1));	\
		    _FP_FRAC_ASSEMBLE_##wc(r, X, rsize);			\
		  }								\
	      }									\
	    else if (rsigned)							\
	      {									\
		r = 1;								\
		r <<= rsize - 1;						\
		r -= 1 - X##_s;							\
	      }									\
	    else								\
	      {									\
		r = 0;								\
		if (!X##_s)							\
		  r = ~r;							\
	      }									\
	    FP_SET_EXCEPTION(FP_EX_INVALID);					\
	  }									\
	else									\
	  {									\
	    if (_FP_W_TYPE_SIZE*wc < rsize)					\
	      {									\
		_FP_FRAC_ASSEMBLE_##wc(r, X, rsize);				\
		r <<= X##_e - _FP_WFRACBITS_##fs;				\
	      }									\
	    else								\
	      {									\
		if (X##_e >= _FP_WFRACBITS_##fs)				\
		  _FP_FRAC_SLL_##wc(X, (X##_e - _FP_WFRACBITS_##fs + 1));	\
		else if (X##_e < _FP_WFRACBITS_##fs - 1)			\
		  {								\
		    _FP_FRAC_SRS_##wc(X, (_FP_WFRACBITS_##fs - X##_e - 2),	\
				      _FP_WFRACBITS_##fs);			\
		    if (_FP_FRAC_LOW_##wc(X) & 1)				\
		      FP_SET_EXCEPTION(FP_EX_INEXACT);				\
		    _FP_FRAC_SRL_##wc(X, 1);					\
		  }								\
		_FP_FRAC_ASSEMBLE_##wc(r, X, rsize);				\
	      }									\
	    if (rsigned && X##_s)						\
	      r = -r;								\
	  }									\
	break;									\
      }										\
  } while (0)

#define _FP_TO_INT_ROUND(fs, wc, r, X, rsize, rsigned)				\
  do {										\
    r = 0;									\
    switch (X##_c)								\
      {										\
      case FP_CLS_NORMAL:							\
	if (X##_e >= _FP_FRACBITS_##fs - 1)					\
	  {									\
	    if (X##_e < rsize - 1 + _FP_WFRACBITS_##fs)				\
	      {									\
		if (X##_e >= _FP_WFRACBITS_##fs - 1)				\
		  {								\
		    _FP_FRAC_ASSEMBLE_##wc(r, X, rsize);			\
		    r <<= X##_e - _FP_WFRACBITS_##fs + 1;			\
		  }								\
		else								\
		  {								\
		    _FP_FRAC_SRL_##wc(X, _FP_WORKBITS - X##_e			\
				      + _FP_FRACBITS_##fs - 1);			\
		    _FP_FRAC_ASSEMBLE_##wc(r, X, rsize);			\
		  }								\
	      }									\
	  }									\
	else									\
	  {									\
	    int _lz0, _lz1;							\
	    if (X##_e <= -_FP_WORKBITS - 1)					\
	      _FP_FRAC_SET_##wc(X, _FP_MINFRAC_##wc);				\
	    else								\
	      _FP_FRAC_SRS_##wc(X, _FP_FRACBITS_##fs - 1 - X##_e,		\
				_FP_WFRACBITS_##fs);				\
	    _FP_FRAC_CLZ_##wc(_lz0, X);						\
	    _FP_ROUND(wc, X);							\
	    _FP_FRAC_CLZ_##wc(_lz1, X);						\
	    if (_lz1 < _lz0)							\
	      X##_e++; 				\
	    _FP_FRAC_SRL_##wc(X, _FP_WORKBITS);					\
	    _FP_FRAC_ASSEMBLE_##wc(r, X, rsize);				\
	  }									\
	if (rsigned && X##_s)							\
	  r = -r;								\
	if (X##_e >= rsize - (rsigned > 0 || X##_s)				\
	    || (!rsigned && X##_s))						\
	  {								\
	  case FP_CLS_NAN:                                                      \
	  case FP_CLS_INF:							\
	    if (!rsigned)							\
	      {									\
		r = 0;								\
		if (!X##_s)							\
		  r = ~r;							\
	      }									\
	    else if (rsigned != 2)						\
	      {									\
		r = 1;								\
		r <<= rsize - 1;						\
		r -= 1 - X##_s;							\
	      }									\
	    FP_SET_EXCEPTION(FP_EX_INVALID);					\
	  }									\
	break;									\
      case FP_CLS_ZERO:								\
        break;									\
      }										\
  } while (0)

#define _FP_FROM_INT(fs, wc, X, r, rsize, rtype)			\
  do {									\
    if (r)								\
      {									\
        unsigned rtype ur_;						\
	X##_c = FP_CLS_NORMAL;						\
									\
	if ((X##_s = (r < 0)))						\
	  ur_ = (unsigned rtype) -r;					\
	else								\
	  ur_ = (unsigned rtype) r;					\
	if (rsize <= _FP_W_TYPE_SIZE)					\
	  __FP_CLZ(X##_e, ur_);						\
	else								\
	  __FP_CLZ_2(X##_e, (_FP_W_TYPE)(ur_ >> _FP_W_TYPE_SIZE), 	\
		     (_FP_W_TYPE)ur_);					\
	if (rsize < _FP_W_TYPE_SIZE)					\
		X##_e -= (_FP_W_TYPE_SIZE - rsize);			\
	X##_e = rsize - X##_e - 1;					\
									\
	if (_FP_FRACBITS_##fs < rsize && _FP_WFRACBITS_##fs <= X##_e)	\
	  __FP_FRAC_SRS_1(ur_, (X##_e - _FP_WFRACBITS_##fs + 1), rsize);\
	_FP_FRAC_DISASSEMBLE_##wc(X, ur_, rsize);			\
	if ((_FP_WFRACBITS_##fs - X##_e - 1) > 0)			\
	  _FP_FRAC_SLL_##wc(X, (_FP_WFRACBITS_##fs - X##_e - 1));	\
      }									\
    else								\
      {									\
	X##_c = FP_CLS_ZERO, X##_s = 0;					\
      }									\
  } while (0)


#define FP_CONV(dfs,sfs,dwc,swc,D,S)			\
  do {							\
    _FP_FRAC_CONV_##dwc##_##swc(dfs, sfs, D, S);	\
    D##_e = S##_e;					\
    D##_c = S##_c;					\
    D##_s = S##_s;					\
  } while (0)





#ifndef __FP_CLZ
#if _FP_W_TYPE_SIZE < 64

#define __FP_CLZ(r, x)				\
  do {						\
    _FP_W_TYPE _t = (x);			\
    r = _FP_W_TYPE_SIZE - 1;			\
    if (_t > 0xffff) r -= 16;			\
    if (_t > 0xffff) _t >>= 16;			\
    if (_t > 0xff) r -= 8;			\
    if (_t > 0xff) _t >>= 8;			\
    if (_t & 0xf0) r -= 4;			\
    if (_t & 0xf0) _t >>= 4;			\
    if (_t & 0xc) r -= 2;			\
    if (_t & 0xc) _t >>= 2;			\
    if (_t & 0x2) r -= 1;			\
  } while (0)
#else 
#define __FP_CLZ(r, x)				\
  do {						\
    _FP_W_TYPE _t = (x);			\
    r = _FP_W_TYPE_SIZE - 1;			\
    if (_t > 0xffffffff) r -= 32;		\
    if (_t > 0xffffffff) _t >>= 32;		\
    if (_t > 0xffff) r -= 16;			\
    if (_t > 0xffff) _t >>= 16;			\
    if (_t > 0xff) r -= 8;			\
    if (_t > 0xff) _t >>= 8;			\
    if (_t & 0xf0) r -= 4;			\
    if (_t & 0xf0) _t >>= 4;			\
    if (_t & 0xc) r -= 2;			\
    if (_t & 0xc) _t >>= 2;			\
    if (_t & 0x2) r -= 1;			\
  } while (0)
#endif 
#endif 

#define _FP_DIV_HELP_imm(q, r, n, d)		\
  do {						\
    q = n / d, r = n % d;			\
  } while (0)

#endif 
