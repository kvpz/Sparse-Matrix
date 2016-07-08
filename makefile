
home = ../../CodeFiles
cpp  = $(home)/mycpp
tcpp = $(home)/mytcpp

CC = g++ -std=c++11 -I. -I$(cpp) -I$(tcpp) -Wall -Wextra 

all:   mxv.x mxm.x smxsv.x smxsm.x m2sm.x sm2m.x v2sv.x sv2v.x 

mxm.x : matrix.h matrix_util.h mxm.cpp
	$(CC) -omxm.x mxm.cpp

mxv.x : matrix.h matrix_util.h mxv.cpp
	$(CC) -omxv.x mxv.cpp

smxsm.x : hashtbl.h sparse.h sparse_util.h smxsm.cpp
	$(CC) -osmxsm.x smxsm.cpp

smxsv.x : hashtbl.h sparse.h sparse_util.h smxsv.cpp
	$(CC) -osmxsv.x smxsv.cpp

m2sm.x : hashtbl.h matrix.h matrix_util.h sparse.h sparse_util.h m2sm.cpp
	$(CC) -om2sm.x m2sm.cpp

sm2m.x : hashtbl.h matrix.h matrix_util.h sparse.h sparse_util.h sm2m.cpp
	$(CC) -osm2m.x sm2m.cpp

v2sv.x : hashtbl.h sparse.h sparse_util.h v2sv.cpp
	$(CC) -ov2sv.x v2sv.cpp

sv2v.x : hashtbl.h sparse.h sparse_util.h sv2v.cpp
	$(CC) -osv2v.x sv2v.cpp
