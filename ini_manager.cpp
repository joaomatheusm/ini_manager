#include "ini_manager.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    map<string, string> 	ini_map;
    char          			directory[100];
    char          			filename[200];    
    char          			buff_filename[200];
    char  				    formatted_datetime[20];
    char      				command[150];
    char         			ini_file[1000];
    char                  	key_buff[50];
    char                  	value_buff[50];
    FILE         			*p_txt_file = NULL;
    
    printf("---------------------------------------------\n");
    
    printf("Informe o diretorio (caminho completo): ");  
    read_input_trimmed(directory, sizeof(directory));
    
    if (check_directory(directory) != 0) 
    {
        printf("O diretorio [ %s ] nao existe.\n", directory);
        return 1;
    }
    
    printf("\nListando arquivos .ini no diretorio [ %s ]...\n", directory);
	list_ini_files(directory);
    
    printf("\nInforme o nome do arquivo: ");    
    read_input_trimmed(buff_filename, sizeof(buff_filename));

	printf("\n--------------------------------------------------\n\n");
    
    get_date(formatted_datetime);
    printf("Data/Hora do Inicio do Processo: %s\n", formatted_datetime);

    printf("Iniciando a leitura do arquivo...\n");
    
    sprintf(filename, "%s/%s", directory, buff_filename);
    
	p_txt_file = fopen(filename, "r");
    if(!p_txt_file)
    {
		printf("Erro na abertura do arquivo [ %s ]\n", filename);
		return 1;
    }
    
    while (fgets(ini_file, sizeof(ini_file), p_txt_file))
		extract_key_value_from_ini(ini_file, key_buff, value_buff, ini_map);
		
    printf("--------------------------------------------------\n");
	print_key_value(ini_map);
    	
    printf("--------------------------------------------------\n");
		
	int option = 0;
	while (option != 5)
	{
		clear_buffers(key_buff, value_buff);
     		
     	center_text("Exibindo o menu...", 50);
     	printf("\n--------------------------------------------------\n");
		printf("[ 1 ] Alterar o valor de uma chave\n[ 2 ] Adicionar uma nova chave-valor\n[ 3 ] Excluir uma chave\n[ 4 ] Exibir chaves e valores\n[ 5 ] Sair do programa\nEscolha uma opcao: ");
		scanf("%d", &option);
		getchar();
		printf("--------------------------------------------------\n");
				
		switch (option) 
		{
			case 1:
				change_key_value(ini_map);
				break;
			case 2:
				add_key_value(ini_map);
				break;
			case 3:
				delete_key(ini_map);
				break;
			case 4:
				print_key_value(ini_map);
				break;
			case 5:
				get_date(formatted_datetime);
    			printf("\nData/Hora do Fim do Processo: %s\n", formatted_datetime);
				break;
			default:
				printf("Opcao invalida\n");
		}
		printf("\n--------------------------------------------------\n");
	}

    if (fclose(p_txt_file) != 0)
    	printf("Erro ao fechar o arquivo.\n");
    
    p_txt_file = fopen(filename, "w");
    if(!p_txt_file)
	{
		printf("Erro na abertura do arquivo [ %s ]\n", filename);
		return 1;
    }
    
    write_ini_file(p_txt_file, ini_map);

	if (fclose(p_txt_file) != 0)
    	printf("Erro ao fechar o arquivo.\n");

	return 0;
}

// Retorna a data e hora formatadas
void get_date(char *date)
{
    time_t now;
    struct tm ts;

    time(&now);
    ts = *localtime(&now);
    strftime(date, 20, "%d/%m/%Y %H:%M:%S", &ts);
}

// Verifica se o diretório existe
int check_directory(const char *directory)
{
	DIR *dir = opendir(directory);
    if (dir)
    {
        closedir(dir);
        return 0;  
    }
    else
        return 1;
}

// Centraliza o texto para imprimir
int center_text(const char* text, int width)
{
    int len = strlen(text);
    
    if (len >= width)
    {
    	printf("%s", text);
    	return 1;
    }

    int spaces = (width - len) / 2;
    
    for (int i = 0; i < spaces; i++)
    	printf(" ");

    printf("%s", text);
    
    return 0;
}

// Limpa os buffers de chave e valor
void clear_buffers(char* key_buff, char* value_buff)
{
    key_buff[0] = '\0';
    value_buff[0] = '\0';
}

// Adiciona o caractere da iteração atual no final do buffer
void add_char_to_buffer(char c, char* buffer) 
{
    char temp[2] = { c, '\0' };
    strcat(buffer, temp);
}

// Extrai a chave e o valor do arquivo ini com base no separador "=", e armazena no map
void extract_key_value_from_ini(const char* ini_file, char* key_buff, char* value_buff, map<string, string>& ini_map)
{
    clear_buffers(key_buff, value_buff);
    
    int is_key = TRUE;
    int ini_len = strlen(ini_file);

    for (int j = 0; j < ini_len; j++)
    {
        if (ini_file[j] != ' ' && ini_file[j] != '=' && ini_file[j] != '\n')
        {   
            if (is_key)
				add_char_to_buffer(ini_file[j], key_buff);
            else
				add_char_to_buffer(ini_file[j], value_buff);
		}
        else if (ini_file[j] == '=')
            is_key = FALSE;
    }

    ini_map[key_buff] = value_buff;
}

// Troca o valor de uma chave
int change_key_value(map<string, string>& ini_map) 
{
    char key_buff[50];
    char value_buff[50];
    
    printf("Informe a chave: ");
    read_input_trimmed(key_buff ,sizeof(key_buff));
        
    if (ini_map.count(key_buff) > 0)
    {
        printf("Informe o valor: ");
        read_input_trimmed(value_buff ,sizeof(value_buff));
        
        ini_map[key_buff] = value_buff;
        printf("Valor alterado com sucesso!\n");
        
        return 0;
    }
    else
    {
        printf("Chave inexistente\n");
        return 1;
    }
}

// Adiciona uma chave ao arquivo ini
int add_key_value(map<string, string>& ini_map) 
{
    char key_buff[50];
    char value_buff[50];

    printf("Informe a chave: ");
    read_input_trimmed(key_buff ,sizeof(key_buff));

    if (ini_map.count(key_buff) <= 0)
    {
        printf("Informe o valor: ");
    	read_input_trimmed(value_buff ,sizeof(value_buff));

        ini_map[key_buff] = value_buff;
        printf("Chave-valor adicionado com sucesso!\n");
        
        return 0;
    }
    else
    {
        printf("A chave inserida já existe.\n");
        return 1;
    }
}

// Apaga uma chave do arquivo ini
int delete_key(map<string, string>& ini_map)
{
	char key_buff[50];
    char value_buff[50];
	
	printf("Informe a chave: ");
	read_input_trimmed(key_buff ,sizeof(key_buff));

   	if (ini_map.count(key_buff) > 0)
    {
    	ini_map.erase(key_buff);
    	printf("Chave excluida com sucesso!\n");
    	
    	return 0;
    }
    else
    {
    	printf("Chave inexistente\n");
    	return 1;
	}
}

// Imprime chave e valor
void print_key_value(map<string, string>& ini_map)
{
	center_text("Exibindo chaves e valores...", 50);
	printf("\n--------------------------------------------------\n");
						
	for (map <string, string>::iterator it = ini_map.begin(); it != ini_map.end(); ++it)
    	printf("%s = %s\n", it->first.c_str(), it->second.c_str());
}

// Remove a quebra de linha do input
void read_input_trimmed(char* buffer, size_t size)
{
    if (fgets(buffer, size, stdin)) 
        buffer[strcspn(buffer, "\n")] = '\0';
}

// Percorre o map e grava cada chave-valor atualizado no arquivo ini
void write_ini_file(FILE *p_txt_file, map<string, string>& ini_map)
{
    for (map<string, string>::iterator it = ini_map.begin(); it != ini_map.end(); ++it)
        fprintf(p_txt_file, "%s=%s\n", it->first.c_str(), it->second.c_str());
}

// Lista arquivos com extensão ".ini" no diretório
void list_ini_files(const char* directory)
{
    DIR *dir = opendir(directory);
    if (!dir)
    {
        printf("Erro ao abrir o diretório: %s\n", directory);
        return;
    }

    struct dirent *entry;
    struct stat file_stat;
    char file_path[1024];

    printf("Arquivos .ini no diretório [%s]:\n", directory);

    while ((entry = readdir(dir)) != NULL)
    {
        snprintf(file_path, sizeof(file_path), "%s/%s", directory, entry->d_name);

        // Verifica se o caminho aponta para um arquivo regular
        if (stat(file_path, &file_stat) == 0 && S_ISREG(file_stat.st_mode))
        {
            if (strstr(entry->d_name, ".ini") != NULL)
                printf("- %s\n", entry->d_name);
        }
    }

    closedir(dir);
}

