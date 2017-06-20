################################################################################
# Automatically-generated file. Do not edit!
################################################################################

PROJ = can_test

-include "..\makefile.init"

all:

clean:
	@rm -f $(GENERATED_FILES)

-include "subdir.mk"
-include "..\makefile.defs"

EXT_LIBRARIES =

all : "can_test.elf"

"can_test.elf" : $(OBJ_FILES) $(EXT_LIBRARIES) ".can_test.elf.opt"
	@echo Linking to ${@F}
	@"C:\Program Files\TASKING\C166-VX v3.0r3\bin\cc166" -f ".can_test.elf.opt"

".can_test.elf.opt" : .refresh
	@argfile ".can_test.elf.opt" -o "can_test.elf" $(OBJ_FILES) $(EXT_LIBRARIES) -C"XC2267-96F" -t -Mn "../${PROJ}.lsl" -Wl-OtZxycL --no-map-file -Wl--map-file="${PROJ}.mapxml":XML -Wl-mcrfiklSmNduQ -Wl--error-limit=42 -g --global-type-checking -Wl-DXC2267_96F -Wl--dep-file=".can_test.elf.d" -Wl--make-target="can_test.elf"
DEPENDENCY_FILES += ".can_test.elf.d"
-include $(DEPENDENCY_FILES)

GENERATED_FILES += "can_test.elf" "can_test.mapxml" "can_test.mdf" ".can_test.elf.opt" "console.log"
-include "..\makefile.targets"
.PHONY : .refresh
