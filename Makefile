# The MIT License (MIT)
#
# Copyright (c) 2014 Antonio SJ Musumeci <trapexit@spawn.link>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

SRC	=	$(wildcard src/*.cpp)
OBJ	=	$(SRC:src/%.cpp=obj/%.o)
DEPS	=	$(OBJ:obj/%.o=obj/%.d)
TARGET	=	mergerfs
CFLAGS	=	-g -Wall \
		$(shell pkg-config fuse --cflags) \
		-DFUSE_USE_VERSION=26 \
		-MMD

ifdef WITHOUT_XATTR
CFLAGS	+=  	-DWITHOUT_XATTR
endif

LDFLAGS	=	$(shell pkg-config fuse --libs)

all: $(TARGET)

help:
	@echo usage: make
	@echo make WITHOUT_XATTR=1 - to build program without xattrs functionality

$(TARGET): obj/obj-stamp $(OBJ)
	g++ $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

obj/obj-stamp:
	mkdir -p obj
	touch $@

obj/%.o: src/%.cpp
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj "$(TARGET)"
	find -name "*~" -delete

.PHONY: all clean help

include $(wildcard obj/*.d)