#Output name
TARGET = Bonekeep.exe

#Resource file
RES = info.rc

#Project directories
INCDIR = inc
SRCDIR = src
OBJDIR = obj
BINDIR = bin

#Search directories
IDIR = -I./extlibs/glfw/include -I./extlibs/gl3w/include -I./extlibs/openal-soft/include -I./extlibs/libogg/include -I./extlibs/libvorbis/include -I./extlibs/zlib/include
LDIR = -L./extlibs/glfw/lib-mingw -L./extlibs/gl3w/lib -L./extlibs/openal-soft/libs/Win32 -L./extlibs/libogg/libs/Win32 -L./extlibs/libvorbis/libs/Win32 -L./extlibs/zlib/lib

#Project Files
SRC = $(call rwildcard, $(SRCDIR)/,*.cpp,)
INC = $(call rwildcard, $(INCDIR)/,*.hpp,)

#Flags
CC = g++ -static-libgcc -static-libstdc++
CFLAGS = -Wall
#-mwindows
LFLAGS = -lgl3w -lglfw3 -lopengl32 -lgdi32 -lOpenAL32 -logg -lvorbisfile -lzdll

#Commands
MKD = @if not exist $(subst $(SRCDIR),$(OBJDIR),$(subst /,\,$(@D))) mkdir $(subst $(SRCDIR),$(OBJDIR),$(subst /,\,$(@D)))
CMD = @echo Compiling: $@ && @$(CC) $(CFLAGS) -I./$(INCDIR) $(IDIR) -c $< -o $@

#Recursive wildcard
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

#Dependencies
DEPS = depend

#Done
all: $(BINDIR)/$(TARGET)
	@echo Done!

#Directories
$(BINDIR):
	@mkdir $(BINDIR)

#Linking
$(BINDIR)/$(TARGET): $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SRC:.cpp=.obj)) | $(BINDIR)
	@echo Linking: $@
	@windres $(RES) -O coff -o $(RES:.rc=.res)
	@$(CC) $(CFLAGS) $^ $(LDIR) $(LFLAGS) -o $@ $(RES:.rc=.res)

#Building dependencies
$(DEPS): $(SRC) $(INC)
	@echo.> $(DEPS) && $(foreach x,$(SRC),$(CC) -I./$(INCDIR) $(IDIR) -MM $(x) -MT $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(x:.cpp=.obj)) >> $(DEPS) && @echo 	$$(MKD) >> $(DEPS) && @echo 	$$(CMD) >> $(DEPS) && ) @echo.>> $(DEPS)

#Compiling
-include $(DEPS)

clean:
	@if  exist $(BINDIR)\$(TARGET) del $(BINDIR)\$(TARGET)
	@if  exist $(OBJDIR) rmdir /Q /S $(OBJDIR)
	@if  exist $(DEPS) del $(DEPS)
	@if  exist $(RES:.rc=.res) del $(RES:.rc=.res)
	@echo Done!


