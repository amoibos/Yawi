TARGET_PLATFORM ?= sc
# Hole nur den letzten Teil des Pfades (funktioniert mit / und \)
OUTPUT := $(lastword $(subst /, ,$(subst \, ,$(CURDIR))))
MAINENTRY := main

COMPILER := sdcc
FLAGS := -DPLATFORM_SG -DGAME_NAME=\"$(OUTPUT)\" --debug
LINK := ihx2sms
ROMEXT := $(TARGET_PLATFORM)

RES2SRC := folder2c

SRC_DIRS := libs src assets
ASSETS := font levels images audio

ASSET_GEN_EXT := c h rel
ASSET_GEN_DIRS := $(addprefix assets/,$(ASSETS))

ifeq ($(OS),Windows_NT)
	RM = del /Q
	RMDIR = rmdir /S /Q
	PATHSEP := $(strip \)
	FIXPATH = $(subst /,\,$1)
	SHELL := cmd.exe
else
	RM = rm -f
	RMDIR = rm -rf
	PATHSEP := /
	FIXPATH = $1
	SHELL := /bin/sh
endif

.PHONY: assets clean all run


EXTRA = assets/font.rel assets/audio.rel assets/images.rel assets/levels.rel
OBJS := global.rel console.rel strings.rel engine.rel views.rel widgets.rel animation.rel \
	$(EXTRA) \
	$(MAINENTRY).rel

# Zusätzliche Bibliotheken und crt0
LIBS := crt0/crt0_sg.rel PSGlib/PSGlib.rel SGlib/SGlib.rel

# --------------------
# Default target
# --------------------
all: $(EXTRA) $(OUTPUT).ihx
	$(LINK) $(OUTPUT).ihx $(OUTPUT).$(ROMEXT)

# --------------------
# Asset conversion
# --------------------
ifeq ($(OS),Windows_NT)

assets:
	@for %%d in ($(ASSETS)) do ( \
	    $(RES2SRC) assets\%%d assets\%%d &&\
	    $(COMPILER) $(FLAGS) -c -mz80 assets\%%d.c && \
	    move %%d.rel assets \
	)

else

assets:
	@for dir in $(ASSETS); do $(RES2SRC) assets/$$dir; done

endif

# --------------------
# Compile rules
# --------------------
# root sources
%.rel: %.c
	$(COMPILER) $(FLAGS) -c -mz80 $<

# libs sources - erzeugt .rel im aktuellen Verzeichnis
console.rel: libs/console.c
	$(COMPILER) $(FLAGS) -c -mz80 $<

strings.rel: libs/strings.c
	$(COMPILER) $(FLAGS) -c -mz80 $<



# --------------------
# Linking
# --------------------
$(OUTPUT).ihx: assets $(OBJS)
	$(COMPILER) $(FLAGS) -o $@ -mz80 --no-std-crt0 --data-loc 0xC000 $(LIBS) $(OBJS)

# --------------------
# Cleanup
# --------------------
ifeq ($(OS),Windows_NT)

clean:
	-@for %%f in ($(call FIXPATH,$(OBJS))) do if exist %%f $(RM) %%f 2>NUL
	-@if exist $(OUTPUT).ihx $(RM) $(OUTPUT).ihx 2>NUL
	-@if exist $(OUTPUT).$(ROMEXT) $(RM) $(OUTPUT).$(ROMEXT) 2>NUL
	-@for %%d in ($(ASSET_GEN_DIRS)) do if exist %%d\*.c $(RM) %%d\*.c 2>NUL
	-@for %%d in ($(ASSET_GEN_DIRS)) do if exist %%d\*.h $(RM) %%d\*.h 2>NUL
	-@for %%d in ($(ASSET_GEN_DIRS)) do if exist %%d\*.rel $(RM) %%d\*.rel 2>NUL
	$(RM) *.sym *.lst *.map *.adb *.cdb *.noi *.lk *.asm 2>NUL
else

clean:
	-$(RM) $(OBJS) bank2.c bank2.h *.ihx *.$(ROMEXT)
	@for d in $(ASSET_GEN_DIRS); do \
		$(RM) $$d/*.c $$d/*.h $$d/*.rel 2>/dev/null || true; \
	done

endif

run: 
	emulicious $(OUTPUT).$(ROMEXT)
