
# /bin/bash

printf $'\x1b[1mLeyenda de colores\x1b[0m \x1b[31m[RED] \x1b[0m Punto de entrada / main \x1b[34m[BLUE] \x1b[0m Creación / inserción de nodos \x1b[32m[GREEN] \x1b[0m Consulta / conversión / lectura \x1b[33m[ORANGE]\x1b[0m Liberación / cleanup \x1b[35m[PURPLE]\x1b[0m Helpers / static (visibilidad local) \x1b[90m[GRAY] \x1b[0m Dependencias externas (libft/stdlib)

\x1b[1mDiagrama de funciones (árbol de llamadas)\x1b[0m

\x1b[31mmain(argc, argv, envp) [RED]\x1b[0m
├─ calls → \x1b[90msetup_signals()\x1b[0m
├─ calls → \x1b[90minit_shell_name(&shell, argv)\x1b[0m
├─ calls → \x1b[90mrun_shell_modes(argc, argv, envp, &shell)\x1b[0m
└─ calls → \x1b[90mcleanup_shell(&shell)\x1b[0m

\x1b[34menv_create(key, value, exported) [BLUE]\x1b[0m 
└─ uses → \x1b[90mft_strdup(key)\x1b[0m uses → \x1b[90mft_strdup(value) (si value != NULL)\x1b[0m

\x1b[34menv_add_back(t_env **env, t_env *new) [BLUE]\x1b[0m 
└─ acción: recorre lista con while (tmp->next) tmp = tmp->next;

\x1b[35mcreate_exported_env_var(str, &env) [PURPLE] (static)\x1b[0m 
├─ uses → \x1b[90mft_strchr(str, '=')\x1b[0m 
├─ uses → \x1b[90mft_substr(...)\x1b[0m 
├─ calls → \x1b[34menv_create(key, sep + 1, 1)\x1b[0m 
├─ calls → \x1b[34menv_add_back(env, new)\x1b[0m 
└─ on error → calls \x1b[33mfree_env_list(*env)\x1b[0m

\x1b[34minit_env_list(envp) [BLUE]\x1b[0m 
├─ recorre envp[]; si strchr('=') -> llama a create_exported_env_var 
└─ devuelve -> t_env * (lista creada)

\x1b[35menv_count(t_env *env) [PURPLE] (static)\x1b[0m 
└─ cuenta nodos con env->exported == 1

\x1b[35mjoin_key_value(key, value) [PURPLE] (static)\x1b[0m 
├─ uses → \x1b[90mft_strjoin(key, "=")\x1b[0m 
├─ uses → \x1b[90mft_strjoin(tmp, value)\x1b[0m 
└─ libera intermedio (free(tmp))

\x1b[32menv_to_array(t_env *env) [GREEN]\x1b[0m 
├─ calls → \x1b[35menv_count(env)\x1b[0m 
├─ uses → \x1b[90mft_calloc(size + 1, sizeof(char *))\x1b[0m 
├─ for each exported node → calls \x1b[35mjoin_key_value(key, value)\x1b[0m 
└─ on error → uses \x1b[90mft_free_strtab(env_array)\x1b[0m

\x1b[33mfree_env_list(t_env *env) [ORANGE]\x1b[0m 
└─ acción: while (env) { tmp = env->next; free(env->key); free(env->value); free(env); env = tmp; }

\x1b[32mget_env_value(t_shell *shell, const char *key) [GREEN]\x1b[0m 
├─ valida parámetros 
├─ recorre shell->env 
└─ compara con \x1b[90mft_strcmp(env->key, key)\x1b[0m → devuelve env->value si coincide

\x1b[90mDependencias externas (usadas)\x1b[0m ft_strdup, ft_substr, ft_strchr, ft_strjoin, ft_calloc, ft_free_strtab, ft_strcmp, malloc, free

\x1b[1mFunciones que llamadas en la ejecución\x1b[0m

    \x1b[32menv_to_array\x1b[0m — llamado desde execute_command.
    \x1b[32mget_env_value\x1b[0m — llamado desde find_executable.

\x1b[1mNotas rápidas\x1b[0m

    create_exported_env_var, env_count y join_key_value son static (alcance local).

    free_env_list sí se usa desde create_exported_env_var en caso de error.

    Es probable que run_shell_modes, init_shell_name o cleanup_shell (no incluidos) integren init_env_list, env_to_array, get_env_value en el flujo real.

    Si quieres que busque en todo el repo las referencias a env_to_array y get_env_value, lo hago ahora y te doy la lista completa.
	'