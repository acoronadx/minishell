/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:26:37 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/01 16:52:26 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_valid_name(const char *s)
{
    if (!s || !*s) return 0;
    if (!(isalpha((unsigned char)*s) || *s == '_')) return 0;
    for (const char *p = s+1; *p; ++p)
        if (!(isalnum((unsigned char)*p) || *p == '_')) return 0;
    return 1;
}

static t_env *env_find(t_env *e, const char *key)
{
    while (e) { if (e->key && key && strcmp(e->key, key)==0) return e; e = e->next; }
    return NULL;
}

/* set/mark exported en tu lista enlazada */
static void env_set_exported(t_shell *sh, const char *key, const char *val_opt, int mark_only)
{
    t_env *n = env_find(sh->env, key);
    if (!n) {
        /* crea nuevo */
        n = env_create(strdup(key), val_opt ? strdup(val_opt) : NULL, 1);
        env_add_back(&sh->env, n);
        return;
    }
    n->exported = 1;
    if (!mark_only && val_opt) {
        free(n->value);
        n->value = strdup(val_opt);
    }
}

int run_export(char **argv, t_shell *shell)
{
    int status = 0;

    /* Sin argumentos: Bash imprime el entorno exportado (declare -x).
       Aquí devolvemos 0 para tus tests actuales. */
    if (!argv[1]) return 0;

    for (int i = 1; argv[i]; ++i) {
        char *arg = argv[i];
        char *eq  = strchr(arg, '=');

        if (!eq) {
            /* solo marcar como exportada */
            if (!is_valid_name(arg)) {
                fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
                status = 1;
                continue;
            }
            env_set_exported(shell, arg, NULL, 1);
            continue;
        }

        /* NAME=VALUE (sin espacios) */
        *eq = '\0';
        const char *name  = arg;
        const char *value = eq + 1;

        if (!is_valid_name(name)) {
            fprintf(stderr, "minishell: export: `%s=': not a valid identifier\n", name);
            status = 1;
            *eq = '=';
            continue;
        }

        env_set_exported(shell, name, value, 0);
        *eq = '=';
    }
    return status;
}
