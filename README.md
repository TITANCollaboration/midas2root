# midas2root
INSTALLATION:

You must have Midas and Rootana compiled already.  

TMDPP16Data.cxx

TMDPP16Data.hxx

TGRIF16Data.cxx

TGRIF16Data.hxx

The above mentioned files must all be copied into $(ROOTANASYS)/libAnalyzer  and then make must be
re-run from the $(ROOTANASYS) directory to recompile the libAnalyzer.a
library.  Once this is done you can run the following from the directory
where this code has been checked out.

cmake ./

make
