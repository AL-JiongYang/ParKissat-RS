SRCS = $(shell find . -name "*.cpp")

OBJS = $(addsuffix .o, $(basename $(SRCS)))

EXEC = painless

LIBS = -lkissat -L../kissat_mab/build \
       -lpthread -lz -lm -static

CXXFLAGS = -I. \
           -I../kissat_mab \
					 -DCOMPACT -DNEMBEDDED -DNMETRICS -DQUIET -DNSTATISTICS \
           -D __STDC_LIMIT_MACROS -D __STDC_FORMAT_MACROS \
           -fopenmp -std=c++17 -O3 -D NDEBUG
					 # -g -DQUIET \

$(EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(OBJS) $(EXEC)
