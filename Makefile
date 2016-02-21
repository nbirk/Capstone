##---------------------------------------------------------------------------
##
## Copyright:   COPYRIGHT 2007,2014 Rockwell Collins. All rights reserved.
## 
## Source File: Makefile
##---------------------------------------------------------------------------
GPP=g++

INCLUDES := -I. -I./usr/include/
LDFLAGS := -L./usr/lib/ -lprotobuf -Wl,-rpath,./usr/lib/
CFLAGS += -Wall -Wl,-rpath,./usr/lib/ $(INCLUDES)

TARGET := protoExample

FILES := main.x \
         PBNetStat.pb.x

all : $(TARGET)

# ------------------------------------------------------------------------------

%.o : %.cpp
	@echo "C++ $< -> $@"
	@$(GPP) $(CFLAGS) -c -o $@ $<

# ------------------------------------------------------------------------------

%.o : %.cc
	@echo "C++ $< -> $@"
	@$(GPP) $(CFLAGS) -c -o $@ $<

# ------------------------------------------------------------------------------

$(TARGET): $(FILES:.x=.o)
	@echo "LINK $@"
	@$(GPP) -o $@ $^ $(LDFLAGS)

# ------------------------------------------------------------------------------

clean :
	$(RM) *.o
	$(RM) $(TARGET)

mrproper: clean
	$(RM) -rf ./usr/lib ./usr/include ./usr/bin ./usr/protobuf-2.6.1
	$(RM) PBNetStat.pb.*
