//cmqlib.i
%module cmqlib

%{
#include "euopt.h"
#include "amopt.h"
#include "volmodel.h"
#include "pricer.h"
#include "spotpricer.h"
#include "barriersmile.h"
#include "fxpricer.h"
%}
%include "std_string.i"
%include "std_vector.i"
%include "euopt.h"
%include "amopt.h"
%include "volmodel.h"
%include "spotpricer.h"
%include "pricer.h"
%include "fxpricer.h"
%include "barriersmile.h"
