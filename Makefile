include .conf

ifeq ($(OS),Windows_NT)
LINK_FLAGS:=$(LINK_FLAGS) -lcomdlg32
TARGET_OBJFOLDER=$(OBJFOLDER)/windows
else
TARGET_OBJFOLDER=$(OBJFOLDER)/linux
endif

ifeq ($(SDL2), true)
ifeq ($(OS),Windows_NT)
COMPILATION_FLAGS:=$(COMPILATION_FLAGS) -IC:/MinGW/include/SDL2
LINK_FLAGS:=$(LINK_FLAGS) -LC:/MinGW/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
else
COMPILATION_FLAGS:=$(COMPILATION_FLAGS) -I/usr/include/SDL2
LINK_FLAGS:=$(LINK_FLAGS) -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
endif
endif

ifeq ($(filter debug,$(MAKECMDGOALS)),debug)
TARGET_OBJFOLDER := $(TARGET_OBJFOLDER)/debug
else
TARGET_OBJFOLDER := $(TARGET_OBJFOLDER)/release
endif

SRCFILES:=$(wildcard ./$(SRCFOLDER)/*$(SOURCE_EXT)) $(wildcard ./$(SRCFOLDER)/**/*$(SOURCE_EXT))
HEADERS:=$(foreach EXT,$(HEADER_EXT),$(wildcard ./$(INCFOLDER)/*$(EXT))) $(foreach EXT,$(HEADER_EXT),$(wildcard ./$(INCFOLDER)/**/*$(EXT)))
OBJECTS=$(subst $(SOURCE_EXT),.o,$(subst ./$(SRCFOLDER)/,./$(TARGET_OBJFOLDER)/,$(SRCFILES)))

PERCENT=0
CURCOUNT=0

ifeq ($(OS),Windows_NT)
OBJCOUNT=$(shell echo|set /a $(words $(OBJECTS))+1)
else
OBJCOUNT=$$(($(words $(OBJECTS))+1))
endif

debug: all

run: all
ifeq ($(OS),Windows_NT)
	@ echo [100%%] Built target $(FILE)
	@ echo.
	-@ $(FILE) || echo Process returned %ERRORLEVEL%
else
	@ printf "[100%%] Built target %s\n\n" "$(FILE)"
	@ ./$(FILE) || echo Process returned $$?
endif

all: objdir $(FILE)
ifeq ($(OS),Windows_NT)
	@ echo [100%%] Built target $(FILE)
else
	@ printf "[100%%] Built target %s\n" "$(FILE)"
endif


$(FILE): $(OBJECTS)
ifeq ($(OS),Windows_NT)
	@ $(eval PERCENT=$(shell echo|set /a $(CURCOUNT)*100/$(OBJCOUNT)))
	@ if $(PERCENT) LSS 10 (echo [  $(PERCENT)%%] [92mLinking executable $(FILE)[0m) else (echo [ $(PERCENT)%%] [92mLinking executable $(FILE)[0m)
	@ $(CC) $^ $(COMPILATION_FLAGS) $(OS_SPECIFIC_LINK_FLAGS) $(LINK_FLAGS) $(SDL_LINK_FLAGS) -o $(FILE) $(foreach I,./$(INCFOLDER)/,$(shell echo -I$(I)))
else
	@ printf "[%3i%%] \e[92mLinking executable %s\e[0m\n" "$$(($(CURCOUNT)*100/$(OBJCOUNT)))" "$(FILE)"
	@ $(CC) $^ $(COMPILATION_FLAGS) $(OS_SPECIFIC_LINK_FLAGS) $(LINK_FLAGS) $(SDL_LINK_FLAGS) -o $(FILE) $(foreach I,$(INCFOLDER)/,$(shell echo -I$(I)))
endif

./$(TARGET_OBJFOLDER)/%.o: ./$(SRCFOLDER)/%$(SOURCE_EXT) ./$(INCFOLDER)/%$(HEADER_EXT)
ifeq ($(OS),Windows_NT)
	@ $(eval PERCENT=$(shell echo|set /a $(CURCOUNT)*100/$(OBJCOUNT)))
	@ if $(PERCENT) LSS 10 (echo [  $(PERCENT)%%] [32mBuilding $(CC) object $@[0m) else (echo [ $(PERCENT)%%] [32mBuilding $(CC) object $@[0m)
	@ $(CC) $< -c $(COMPILATION_FLAGS) $(SDL_COMP_FLAGS) -o $@ $(foreach I,./$(INCFOLDER)/,$(shell echo -I$(I))) $(foreach d,$(DEFINES),$(shell echo -D$(d)))
	@ $(eval CURCOUNT=$(shell echo|set /a $(CURCOUNT)+1))
else
	@ printf "[%3i%%] \e[32mBuilding %s object %s\e[0m\n" "$$(($(CURCOUNT)*100/$(OBJCOUNT)))" "$(CC)" "$@"
	@ $(CC) $< -c $(COMPILATION_FLAGS) $(SDL_COMP_FLAGS) -o $@ $(foreach I,$(INCFOLDER)/,$(shell echo -I$(I))) $(foreach d,$(DEFINES),$(shell echo -D$(d)))
	@ $(eval CURCOUNT=$(shell echo $$(($(CURCOUNT)+1))))
endif

./$(TARGET_OBJFOLDER)/$(MAINFILE).o: ./$(SRCFOLDER)/$(MAINFILE)$(SOURCE_EXT) $(HEADERS)
ifeq ($(OS),Windows_NT)
	@ $(eval PERCENT=$(shell echo|set /a $(CURCOUNT)*100/$(OBJCOUNT)))
	@ if $(PERCENT) LSS 10 (echo [  $(PERCENT)%%] [32mBuilding $(CC) object $@[0m) else (echo [ $(PERCENT)%%] [32mBuilding $(CC) object $@[0m)
	@ $(CC) $< -c $(COMPILATION_FLAGS) -o $@ $(foreach I,./$(INCFOLDER)/,$(shell echo -I$(I))) $(foreach d,$(DEFINES),$(shell echo -D$(d)))
	@ $(eval CURCOUNT=$(shell echo|set /a $(CURCOUNT)+1))
else
	@ printf "[%3i%%] \e[32mBuilding %s object %s\e[0m\n" "$$(($(CURCOUNT)*100/$(OBJCOUNT)))" "$(CC)" "$@"
	@ $(CC) $< -c $(COMPILATION_FLAGS) -o $@ $(foreach I,$(INCFOLDER)/,$(shell echo -I$(I))) $(foreach d,$(DEFINES),$(shell echo -D$(d)))
	@ $(eval CURCOUNT=$(shell echo $$(($(CURCOUNT)+1))))
endif

objdir:
ifeq ($(OS),Windows_NT)
	-@ $(foreach folder,$(OBJECTS),$(shell mkdir "$(dir $(folder))" >nul 2>nul))
else
	-@ $(foreach folder,$(OBJECTS),$(shell mkdir -p "$(dir $(folder))"))
endif

clean:
ifeq ($(OS),Windows_NT)
	del /f /s /q $(foreach obj,$(subst /,\,$(OBJECTS)),"$(obj)") "$(FILE).exe" *~
else
	rm -f $(foreach obj,$(OBJECTS),"$(obj)") "$(FILE)" *~
endif

.PHONY: all run clean objdir