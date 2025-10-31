V - Tester + Leaks et errors utils
Tester

Avoir au préalable mis la possibilité de faire ./minishell -c (comme dans bash -c)

cd tester
./start.sh

fait par @yviavant, en reprenant ce testeur
Errors

Avec valgrind "invalid read of size 1"

str = malloc(sizeof(char) * ft_strlen(whole_cmd));
i = 0;
if (str[i] && str[i] == '$' && str[i - 1] != '\\') // if i == 0 invalid read of size 1 car str[i - 1]

str = malloc(sizeof(char) * 8);
i = 0;
while (str[i] && str[i] == '"') // avec str="coucou\" invalid read of size 1 possible si on fait i++ et qu'on revient a la condition avec i > 8)
{
	if (str[i] = '\\')
		i++;
	i++;
}

Avec valgrind "invalid write of size 1" : idem que read mais on a voulu écrire

str = malloc(sizeof(char) * 8);
i = 0;
while (str[i] && str[i] == '"')
{
	if (str[i] = '\\')
		i++;
	i++;
	copy->cmd[i] = str[i]; // invalid write of size 1 possible si ici i > 8)
}

Avec valgrind "Conditional jump or move depends on uninitialised value(s)"

if (str) // où str n'est pas initialisé

Leaks

    valgrind : valgrind --leak-check=full --show-leak-kinds=all ./minishell (sachant que les still reachable sont considérés comme des leaks à 42)
    https://github.com/bibhas2/Memd
    Garbage collector : mettre dans une liste chaînée pour pouvoir tout free après
    Imprimer le pointeur au moment du malloc et au moment du free, pour voir quels pointeurs n'ont pas été free

str = malloc(sizeof(char) * ft_strlen(whole_cmd));
printf("str malloc : %p", str);

printf("str free : %p", str);
free(str);
