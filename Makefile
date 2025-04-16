.PHONY: directories all clean





CC=		gcc
CCOPT=		-O3
CCFLAGS=	-Wall -funsigned-char $(CCOPT)

LD=		gcc
LDFLAGS=	-Wall





ASXX =	asdata.o asexpr.o aslex.o aslist.o	\
	asmain.o asmcro.o asout.o assubr.o	\
	assym.o	asdbg.o

ASXBAS:=	./
SRCASX:=	$(ASXBAS)asxxsrc/
ASXXSRC =	$(addprefix $(SRCASX),$(ASXX))

AS8085 =	i85mch.o i85pst.o
SRC8085=	$(ASXBAS)as8085/
AS8085SRC =	$(addprefix $(SRC8085),$(AS8085))
SRCMISC:=	$(ASXBAS)asxxmisc/


%.o: %.c
	$(CC) $(CCFLAGS) -I$(SRCMISC) -I$(SRCASX) -c $< -o $@


MKDIR_P := mkdir -p
OUT_DIR := build

all: $(OUT_DIR)/as8085
directories: $(OUT_DIR)

$(OUT_DIR):
	${MKDIR_P} $(OUT_DIR)

$(OUT_DIR)/as8085: directories $(ASXXSRC) $(AS8085SRC)
	$(LD) $(LDFLAGS) $(ASXXSRC) $(AS8085SRC) -o $(OUT_DIR)/as8085
	chmod 755 $(OUT_DIR)/as8085

clean:
	rm -rf $(OUT_DIR)
	find . -name "*.o" -exec rm {} \;


format:
	find . -type f -name "*.c" -exec clang-format -i {} \;  
	find . -type f -name "*.h" -exec clang-format -i {} \;  
