#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#include "cache.h"

#define ERROR -1
#define BUF_LEN 100
#define N_ARG 2
#define BASE 10 
#define ADDRESS 0
#define VALUE 1

char *readln(FILE* file){
	char *line = malloc(BUF_LEN*sizeof(char));
	if (!line) return NULL;
	unsigned int chars_read = 0;
	char letter = fgetc(file);
	if (letter == EOF){
		free(line);
		return NULL;
	}
	while ((letter != EOF) && (letter != '\n')){
		line[chars_read] = letter;
		chars_read = chars_read + sizeof(char);
		if ((chars_read % BUF_LEN) == 0){
			if (!realloc(line, BUF_LEN * ((chars_read/BUF_LEN)+1) * sizeof(char))){
				free(line);
				return NULL;
			} 
		}
		letter = fgetc(file);
	}
	return line;
}

int operation_error(FILE* file, char *line){
	printf("Eror al leer comando: %s\n", line );
	free (line);
	fclose(file);
	return ERROR;
}

bool get_parameters(char *line, long int *parameters){
	errno = 0;
	char **last_c = &line;
	char *tmp_line = line;
	long int addr = strtol(tmp_line, last_c, BASE);
	if (errno != 0) return false;
	if (**last_c == ','){
		tmp_line = *last_c;
		long int value = strtol(tmp_line + 2, last_c, BASE);
		parameters[VALUE] = value;
	}
	if (errno != 0) return false;
	parameters[ADDRESS] = addr;
	return true;
}

int
main(int argc, char **argv)
{
	
	//if (argc != N_ARG){
	//	printf("%s\n","La cantidad de parametros no es la correcta" );
	//	return ERROR;
	//}
	//FILE *fp = fopen (argv[1], "r"); 
	FILE *fp = fopen ("Pruebas_tp2/prueba1.mem", "r");        
	if (!fp) {
		printf("Error: %s al abir la ruta: %s\n", strerror(errno), argv[1]);
		return ERROR;
	}
	char *line = readln(fp);
	if (!line){
		printf("%s\n", "ERROR al leer" );
		return ERROR;
	}
	long int parameters[2];
	
	cache = malloc(sizeof(cache_t));
	printf("%d\n", init());

	while (line){
		if (strncmp(line, "FLUSH", 6) == 0){
			printf("%s\n", "flush action");
			init();

		} else if (strncmp(line, "R ", 2) == 0){
			if (! get_parameters(line, parameters)){
				return operation_error(fp, line);
			}
			printf("%s\n", "r action");
			read_byte(parameters[ADDRESS]);

		} else if (strncmp(line, "W ", 2) == 0){
			if (line[7]!= ',' || line[8] != ' '){
				if (! get_parameters(line, parameters)){
					return operation_error(fp, line);
				}
			}
			printf("%s\n", "w action");
			write_byte(parameters[ADDRESS], parameters[VALUE]);

		} else if (strncmp(line, "MR", 2) == 0){
			printf("%s\n", "mr action");
			get_miss_rate();

		} else {
			return operation_error(fp, line);
		}

		free (line);
		line = readln(fp);
	}

	free (line);
	fclose(fp);
	delete_cache();
	free(cache);
	return 0;
} 
