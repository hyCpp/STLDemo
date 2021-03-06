#####################################################################
# Automatically generated by qtpromaker
#####################################################################

TEMPLATE = app
TARGET = STL
QMAKE_CXXFLAGS += -std=c++0x

SOURCES *= \
    ./main.cpp \
    test.cpp

HEADERS *= \
    ./list/list.h \
    ./string/string.h \
    ./t_allocator.h \
    ./t_construct.h \
    ./vector/vector.h \
    ./t_uninitialized.h \
    ./t_traits.h \
    ./t_iterator.h \
    ./map/map.h \
    ./set/set.h \
    ./pair/pair.h \
    ./hashtable/hashtable.h \
    ./hashtable/hash_func.h \
    ./t_stl_func.h \
    ./t_algorithm.h \
    ./Graph/Graph.h \
    ./Graph/GraphInk.h \
    ./Graph/GraphMtx.h \
    ./tree/b_tree.h \
    ./tree/t_br_tree.h \
    ./deque/deque.h \
    ./queue/queue.h \
    ./heap/heap.h \
    stack/stack.h

PATHS *= \
    . \
    ./list \
    ./map \
    ./vector \
    ./set \
    ./pair \
    ./tree \
    ./Graph \
    ./hashtable \
    ./deque \
    ./heap \

DEPENDPATH *= $$PATHS

INCLUDEPATH *= $$PATHS
