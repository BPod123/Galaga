################################################################################
# These are variables for the GBA toolchain build
# You can add others if you wish to
# ***** Ben *****
################################################################################

# TA-TODO: Put your game name here.
# The name of your desired application
# This should be a just a name i.e MyFirstGBAGame
# No SPACES AFTER THE NAME.
PROGNAME = Galaga

# TA-TODO: Add the C files you want compiled here (replace extension with .o)
# Here you must put a list of all of the object files
# that will be compiled into your program. For example
# if you have main.c and myLib.c then in the following
# line you would put main.o and myLib.o
OFILES = playState.o maneuvers.o  images/loseImage.o gba.o testing.o font.o game.o level.o ship.o images/levelBackground.o images/winImage.o images/playerShip_Up.o images/blueAnt_Up.o images/blueAnt_Down.o images/blueAnt_Left.o images/blueAnt_Right.o images/blueAnt_UL.o images/blueAnt_UR.o images/blueAnt_DL.o images/blueAnt_DR.o images/redAnt_Up.o images/redAnt_Down.o images/redAnt_Left.o images/redAnt_Right.o images/redAnt_UL.o images/redAnt_UR.o images/redAnt_DL.o images/redAnt_DR.o images/redFly_Up.o images/redFly_Down.o images/redFly_Left.o images/redFly_Right.o images/redFly_UL.o images/redFly_UR.o images/redFly_DL.o images/redFly_DR.o images/blueFly_Up.o images/blueFly_Down.o images/blueFly_Left.o images/blueFly_Right.o images/blueFly_UL.o images/blueFly_UR.o images/blueFly_DL.o images/blueFly_DR.o images/missile_Up.o images/explosion.o images/explosionBig.o images/explosionPlayer.o images/titleScreen.o
# testing.o util.o
################################################################################
# These are various settings used to make the GBA toolchain work
# DO NOT EDIT BELOW.
################################################################################

.PHONY: all
all : CFLAGS += $(CRELEASE) -I../shared
all : LDFLAGS += $(LDRELEASE)
all: $(PROGNAME).gba
	@echo "[FINISH] Created $(PROGNAME).gba"

include /opt/cs2110-tools/GBAVariables.mak

# Son, it's time to be an adult and use adult prototypes
CFLAGS += -Wstrict-prototypes -Wold-style-definition

debug : CFLAGS += $(CDEBUG) -I../shared
debug : LDFLAGS += $(LDDEBUG)
debug : $(PROGNAME).gba
	@echo "[FINISH] Created $(PROGNAME).gba"

$(PROGNAME).gba : $(PROGNAME).elf
	@echo "[LINK] Linking objects together to create $(PROGNAME).gba"
	@$(OBJCOPY) -O binary $(PROGNAME).elf $(PROGNAME).gba

$(PROGNAME).elf : crt0.o $(GCCLIB)/crtbegin.o $(GCCLIB)/crtend.o $(GCCLIB)/crti.o $(GCCLIB)/crtn.o $(OFILES) libc_sbrk.o
	$(CC) -o $(PROGNAME).elf $^ $(LDFLAGS)

.PHONY : emu
emu : med

.PHONY : vba
vba : CFLAGS += $(CRELEASE) -I../shared
vba : LDFLAGS += $(LDRELEASE)
vba : $(PROGNAME).gba
	@echo "[EXECUTE] Running Emulator VBA-M"
	@echo "          Please see emulator.log if this fails"
	@vbam $(VBAOPT) $(PROGNAME).gba >emulator.log 2>&1

.PHONY : med
med : CFLAGS += $(CRELEASE) -I../shared
med : LDFLAGS += $(LDRELEASE)
med : $(PROGNAME).gba
	@echo "[EXECUTE] Running emulator Mednafen"
	@echo "          Please see emulator.log if this fails"
# Default mednafen keybinds are "Gaaarbage", in the words of Farzam
	@mkdir -p ~/.mednafen/
	@cp mednafen-09x.cfg ~/.mednafen/
	@mednafen $(MEDOPT) $(PROGNAME).gba >emulator.log 2>&1

.PHONY : submit
submit: clean
	@rm -f submission.tar.gz
	@tar czvf submission.tar.gz *

.PHONY : clean
clean :
	@echo "[CLEAN] Removing all compiled files"
	rm -f *.o *.elf *.gba *.log */*.o
