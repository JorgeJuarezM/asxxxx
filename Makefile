.PHONY: directories all clean clean_object_files format


all: $(OUT_DIR)/as8085 $(OUT_DIR)/aslink

directories: $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)
	$(MAKE) clean_object_files

clean_object_files:
	find . -name "*.o" -exec rm {} \;

format:
	find . -type f -name "*.c" -exec clang-format -i {} \;  
	find . -type f -name "*.h" -exec clang-format -i {} \;  

####################################################################
# Configuration Settings
####################################################################

CC		=	gcc
CCOPT	=	-O3
CCFLAGS	=	-Wall -funsigned-char $(CCOPT)

LD		=	gcc
LDFLAGS	=	-Wall

ASXX 	=	asdata.o asexpr.o aslex.o aslist.o	\
			asmain.o asmcro.o asout.o assubr.o	\
			assym.o	asdbg.o

ASXBAS	:=	./
SRCASX	:=	$(ASXBAS)asxxsrc/
SRCMISC	:=	$(ASXBAS)asxxmisc/
SRCLNK	:=	$(ASXBAS)linksrc/

ASXXSRC =	$(addprefix $(SRCASX),$(ASXX))

MKDIR_P :=	mkdir -p
OUT_DIR :=	dist

####################################################################
# Directories
####################################################################
$(OUT_DIR):
	${MKDIR_P} $(OUT_DIR)


####################################################################
# Object Files
####################################################################
%.o: %.c
	$(CC) $(CCFLAGS) -I$(SRCMISC) -I$(SRCASX) -c $< -o $@

####################################################################
# AS8085
####################################################################
AS8085		=	i85mch.o i85pst.o
SRC8085		=	$(ASXBAS)as8085/
AS8085SRC	=	$(addprefix $(SRC8085),$(AS8085))

$(OUT_DIR)/as8085: directories $(ASXXSRC) $(AS8085SRC)
	$(LD) $(LDFLAGS) $(ASXXSRC) $(AS8085SRC) -o $(OUT_DIR)/as8085
	chmod 755 $(OUT_DIR)/as8085
	$(MAKE) clean_object_files


####################################################################
# ASLINK
####################################################################
ASLINK		=	lkmain.o lkhead.o lkarea.o lkbank.o	\
				lkdata.o lkeval.o lklex.o lklibr.o	\
				lklist.o lkrloc.o lkrloc3.o lkrloc4.o	\
				lksym.o lkout.o lknoice.o lksdcdb.o

ASLINKSRC =	$(addprefix $(SRCLNK),$(ASLINK))
$(ASLINK):	$(SRCMISC)alloc.h $(SRCLNK)aslink.h

$(OUT_DIR)/aslink: directories $(ASLINKSRC)
	$(LD) $(LDFLAGS) $(ASLINKSRC) -o $(OUT_DIR)/aslink
	chmod 755 $(OUT_DIR)/aslink
