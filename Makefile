#
# Makefile:
#	dispsw - library for the Raspberry Pi to use the expansion board DISP-SW
#
#	Copyright (c) 2015 Dennis Binder
#   copied from wiringPi: Gordon Henderson
#################################################################################
# This file is part of dispsw:
#
#    dispsw is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    wiringPi is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with dispsw.  If not, see <http://www.gnu.org/licenses/>.
#################################################################################

VERSION=$(shell cat ./VERSION)
DESTDIR=/usr
PREFIX=/local

STATIC=libdispsw.a
DYNAMIC=libdispsw.so.$(VERSION)

#DEBUG	= -g -O0
DEBUG	= -O2
CC	= gcc
INCLUDE	= -I.
DEFS	= -D_GNU_SOURCE
CFLAGS	= $(DEBUG) $(DEFS) -Wformat=2 -Wall -Winline $(INCLUDE) -pipe -fPIC

LIBS    =

###############################################################################

SRC	=	dispsw.c	 

HEADERS =	dispsw.h	

OBJ	=	$(SRC:.c=.o)

all:		$(DYNAMIC)

$(DYNAMIC):	$(OBJ)
	@echo "[Link (Dynamic)]"
	@$(CC) -shared -Wl,-soname,libdispsw.so -o libdispsw.so.$(VERSION) -lpthread $(OBJ) 

.c.o:
	@echo [Compile] $<
	@$(CC) -c $(CFLAGS) $< -o $@

.PHONY:	clean
clean:
	@echo "[Clean]"
	@rm -f $(OBJ) $(OBJ_I2C) *~ core tags Makefile.bak libdispsw.*

.PHONY:	tags
tags:	$(SRC)
	@echo [ctags]
	@ctags $(SRC)

.PHONY:	install
install:	$(DYNAMIC)
	@echo "[Install Headers]"
	@install -m 0755 -d						$(DESTDIR)$(PREFIX)/include
	@install -m 0644 $(HEADERS)					$(DESTDIR)$(PREFIX)/include
	@echo "[Install Dynamic Lib]"
	@install -m 0755 -d						$(DESTDIR)$(PREFIX)/lib
	@install -m 0755 libdispsw.so.$(VERSION)			$(DESTDIR)$(PREFIX)/lib/libdispsw.so.$(VERSION)
	@ln -sf $(DESTDIR)$(PREFIX)/lib/libdispsw.so.$(VERSION)	$(DESTDIR)/lib/libdispsw.so
	@ldconfig

.PHONY:	uninstall
uninstall:
	@echo "[UnInstall]"
	@cd $(DESTDIR)$(PREFIX)/include/ && rm -f $(HEADERS)
	@cd $(DESTDIR)$(PREFIX)/lib/     && rm -f libdispsw.*
	@ldconfig

.PHONY:	depend
depend:
	makedepend -Y $(SRC)

# DO NOT DELETE

dispsw.o: dispsw.h
