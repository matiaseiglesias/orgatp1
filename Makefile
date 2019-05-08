# Makefile de ejemplo para C/C++
# Creado: 15/04/2004 - Leandro Lucarella
# Modificado: 01/09/2016 - Pablo Roca
# Copyleft 2004 - Leandro Lucarella, Bajo licencia GPL [http://www.gnu.org/]

# CONFIGURACION
################

# Nombre del ejecutable.
target = tp.exe

# Archivos con el código fuente que componen el ejecutable. Si no se especifica,
# toma todos los archivos con la extensión mencionada. Para especificar hay que
# descomentar la línea (quitarle el '#' del principio).
# NOTA: No poner cabeceras (.h).
#fuentes = entrada.cpp

# Si usa funciones de math.h, descomentar (quitar el '#' a) la siguiente línea.
#math = si

# Descomentar si se quiere ver como se invoca al compilador
verbose = si

# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ para tratamiento de errores y warnings.
CFLAGS = -Wall -Werror 

# Para optimizar el binario resultante lo mejor posible
CFLAGS += -O3

# Para valgrind o debug
CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
#LDFLAGS =

# Estandar de C a usar
CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++11

# Si se quiere compilar estaticamente, descomentar la siguiente linea
#static = si


# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################

# Linkea con libm de ser necesario.
ifdef math
LDFLAGS += -lm
endif

ifdef static
LDFLAGS += -static
endif

CFLAGS += -std=$(CSTD)
LD = $(CC)


occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif


# REGLAS
#########

.PHONY: all clean

all: $(target)

$(target): artist_ant.o ant_engine.o 
	cc -Wall -Werror  -O3 -ggdb -DDEBUG -fno-inline -std=c99 -D ASSEMBLY -I../tp1 -o $(target) artist_ant.o ant_engine.o

ant_engine.o: ant_engine.c ant_engine.h ant_advance.S paint.S palette_size.S ant_constants.h
	cc -Wall -Werror  -O3 -ggdb -DDEBUG -fno-inline -std=c99 -D ASSEMBLY -I../tp1 -c ant_engine.c ant_advance.S paint.S palette_size.S

artist_ant.o: artist_ant.c artist_ant.h ant_engine.h ant_constants.h
	cc -Wall -Werror  -O3 -ggdb -DDEBUG -fno-inline -std=c99 -D ASSEMBLY -I../tp1 -c artist_ant.c

clean:
	$(RM) $(wildcard *.o) $(target)

format: .clang-files .clang-format
	xargs -r clang-format -i <$<