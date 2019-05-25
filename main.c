#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define ERROR -1
#define BUF_LEN 100
#define N_ARG 2

char *readln(FILE* file){
	char *line = malloc(BUF_LEN*sizeof(char));
	if (!line) return NULL;
	unsigned int chars_read = 0;
	char letter = fgetc(file);
	if (letter == EOF){
		free(line);
		return NULL;
	}
	//while (fgets(line + chars_read, sizeof(char), file)){
	while ((letter != EOF) && (letter != '\n')){
		line[chars_read] = letter;
		chars_read = chars_read + sizeof(char);
		//printf("la cuanta chars_read  BUF_LENda: %i\n", chars_read % BUF_LEN );
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

int
main(int argc, char **argv)
{
	if (argc != N_ARG){
		printf("%s\n","La cantidad de parametros no es la correcta" );
		return ERROR;
	}
	FILE *fp = fopen (argv[1], "r");        
	if (!fp) {
		printf("Error: %s al abir la ruta: %s\n", strerror(errno), argv[1]);
		return ERROR;
	}
	char *line = readln(fp);
	if (!line){
		printf("%s\n", "ERROR al leer" );
		return ERROR;
	}
	printf("%s\n", line );
	while (line){
		if (strncmp(line, "FLUSH", 6) == 0){
			printf("%s\n", "flush action");
		} else if (strncmp(line, "R ", 2) == 0){
			if (strlen(&line[2]) != 5){
				free (line);
				fclose(fp);
				return ERROR;
			}
			printf("%s\n", "r action");
		} else if (strncmp(line, "W ", 2) == 0){
			if (line[7]!= ',' || line[8] != ' ' || strlen(line) != 12){
				free (line);
				fclose(fp);
				return ERROR;
			}
			printf("%s\n", "w action");
		} else if (strncmp(line, "MR", 3) == 0){
			printf("%s\n", "mr action");
		} else {
			printf("Eror al leer comando: %s\n", line );
			free (line);
			fclose(fp);
			return ERROR;
		}
		free (line);
		line = readln(fp);
	}
	free (line);
	fclose(fp);
	return 0;
} 
/*
El comando “FLUSH” se ejecuta llamando a la función init(). Repre-
senta el vaciado del caché.
Los comandos de la forma “R ddddd” se ejecutan llamando a la función
read byte(ddddd) e imprimiendo el resultado.
Los comandos de la forma “W ddddd, vvv” se ejecutan llamando a la
función write byte(unsigned int ddddd, char vvv) e imprimien-
do el resultado.
El comando “MR” se ejecuta llamando a la función get miss rate() e
imprimiendo el resultado.
*/