/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:12:04 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/22 13:40:41 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "ast.h"

// --- Funciones Auxiliares ---

// Redirige 'fd_to_redirect_from' a 'target_std_fd' (ej., 1 para stdout).
// Cierra 'fd_to_redirect_from' después de dup2.
static int  redirect_to(int fd_to_redirect_from, int target_std_fd)
{
    if (dup2(fd_to_redirect_from, target_std_fd) < 0)
    {
        perror("minishell: dup2");
        close(fd_to_redirect_from); // Asegurarse de cerrar el FD si dup2 falla
        return (1); // Indica fallo
    }
    close(fd_to_redirect_from); // ¡IMPORTANTE! Cierra el FD original después de duplicarlo
    return (0); // Indica éxito
}

// Maneja la redirección de heredoc (<<).
// Asume que heredoc_read crea un archivo temporal o pipe y devuelve un FD.
// ... (resto del código igual) ...
static int  handle_heredoc(t_ast *redir_node)
{
    int fd;
    // CORRECCIÓN: Acceder directamente a 'delimiter' dentro de 'redir'
    fd = heredoc_read(redir_node->redir.delimiter);
    if (fd < 0)
        return (1);
    return (redirect_to(fd, STDIN_FILENO));
}

int open_redirection(t_ast *redir_node)
{
    int fd;
    int flags;
    mode_t mode;

    fd = -1;
    if (!redir_node)
        return (-1);

    flags = 0;
    mode = 0644;

    // CORRECCIÓN: Acceder directamente a 'redir_type' y 'filename' dentro de 'redir'
    if (redir_node->redir.redir_type == REDIR_IN)
        flags = O_RDONLY;
    else if (redir_node->redir.redir_type == REDIR_OUT || redir_node->redir.redir_type == REDIR_FORCE)
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    else if (redir_node->redir.redir_type == REDIR_APPEND)
        flags = O_WRONLY | O_CREAT | O_APPEND;
    else if (redir_node->redir.redir_type == REDIR_ERR || redir_node->redir.redir_type == REDIR_ALL)
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    else if (redir_node->redir.redir_type == REDIR_APPEND_ERR || redir_node->redir.redir_type == REDIR_APPEND_ALL)
        flags = O_WRONLY | O_CREAT | O_APPEND;
    else
    {
        ft_dprintf(2, "minishell: internal error: unexpected redirection type in open_redirection\n");
        return (-1);
    }
    
    if (flags & O_CREAT)
        fd = open(redir_node->redir.filename, flags, mode);
    else
        fd = open(redir_node->redir.filename, flags);

    return (fd);
}

static int  handle_dup_redir(t_ast *redir_node)
{
    int src_fd;
    int dest_fd;

    // CORRECCIÓN: Acceder directamente a 'redir_fd' y 'filename' dentro de 'redir'
    dest_fd = redir_node->redir.redir_fd;

    if (ft_strcmp(redir_node->redir.filename, "-") == 0)
        src_fd = -1;
    else
    {
        src_fd = ft_atoi(redir_node->redir.filename);
    }

    if (src_fd == -1)
    {
        if (close(dest_fd) < 0)
        {
            perror("minishell: close");
            return (1);
        }
        return (0);
    }
    
    if (dest_fd == src_fd)
        return (0);

    if (dup2(src_fd, dest_fd) < 0)
    {
        perror("minishell: dup2");
        return (1);
    }
    return (0);
}

int handle_simple_redir(t_ast *redir_node)
{
    int fd;
    int target_std_fd;

    fd = open_redirection(redir_node);
    if (fd < 0)
    {
        perror(redir_node->redir.filename);
        return (1);
    }

    // AÑADIMOS LA LÓGICA PARA REDIR_ERR y REDIR_APPEND_ERR AQUÍ
    if (redir_node->redir.redir_fd != -1)
        target_std_fd = redir_node->redir.redir_fd;
    else if (redir_node->redir.redir_type == REDIR_IN || redir_node->redir.redir_type == REDIR_HEREDOC)
        target_std_fd = STDIN_FILENO;
    else if (redir_node->redir.redir_type == REDIR_OUT || redir_node->redir.redir_type == REDIR_APPEND || redir_node->redir.redir_type == REDIR_FORCE)
        target_std_fd = STDOUT_FILENO;
    // ---> INSERTA ESTE BLOQUE AQUÍ <---
    else if (redir_node->redir.redir_type == REDIR_ERR || redir_node->redir.redir_type == REDIR_APPEND_ERR)
        target_std_fd = STDERR_FILENO; // <--- ¡Esta es la clave para 2> y 2>>!
    // ------------------------------------
    else // Este 'else' debería capturar REDIR_ALL y REDIR_APPEND_ALL, u otros casos no manejados.
    {
        // Esto solo se ejecuta si el tipo de redirección es REDIR_ALL o REDIR_APPEND_ALL
        // O si hay un tipo inesperado no cubierto en los 'if' anteriores.
        // La lógica para REDIR_ALL/APPEND_ALL la manejaremos aquí mismo, en lugar de un 'if' separado fuera.
        if (redir_node->redir.redir_type == REDIR_ALL || redir_node->redir.redir_type == REDIR_APPEND_ALL)
        {
            if (redirect_to(fd, STDOUT_FILENO) != 0)
            {
                return (1);
            }
            if (dup2(STDOUT_FILENO, STDERR_FILENO) < 0)
            {
                perror("minishell: dup2 for stderr");
                return (1);
            }
            return (0);
        }
        else
        {
            // Si llegamos aquí con otro tipo de redirección, es un error de lógica.
            ft_dprintf(2, "minishell: internal error: unhandled redirection type in handle_simple_redir\n");
            close(fd); // Asegúrate de cerrar el FD abierto si no se va a usar
            return (1);
        }
    }
    
    // Si no fue REDIR_ALL/APPEND_ALL, entonces se usa el target_std_fd determinado arriba
    return (redirect_to(fd, target_std_fd));
}

int apply_redirections(t_ast *redir_list)
{
    t_ast   *curr;
    int     fail;

    fail = 0;
    curr = redir_list;

    while (curr)
    {
        if (curr->type != N_REDIR)
        {
            ft_dprintf(2, "minishell: internal error: expected N_REDIR node in redirection list\n");
            return (1);
        }

        // CORRECCIÓN: Acceder directamente a 'redir_type' dentro de 'redir'
        if (curr->redir.redir_type == REDIR_HEREDOC)
        {
            if (handle_heredoc(curr))
                fail = 1;
        }
        else if (curr->redir.redir_type == REDIR_DUP_IN || curr->redir.redir_type == REDIR_DUP_OUT)
        {
            if (handle_dup_redir(curr))
                fail = 1;
        }
        else
        {
            if (handle_simple_redir(curr))
                fail = 1;
        }

        if (fail)
        {
            return (1);
        }

        // Se mantiene `curr = curr->bin.right;` asumiendo que tu AST organiza las redirecciones así.
        curr = curr->bin.right;
    }
    return (0);
}
