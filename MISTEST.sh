#!/bin/bash
bash
/bin/echo hola
cat << E
uno
dos
E
echo pasando E
exit
echo pasando exit
EOF
foo
/bin/ls
/bin/ls -laF
/bin/ls -l
/bin/ls -a
/bin/ls -F
/bin/ls -la
/bin/ls -lF
/bin/ls/file_does_not_exist
echo "BUILTINS"
bash
exit
echo "it works"
echo it works
echo 'it works'
echo -n
echo -n -n
echo -n -n -n
echo -n "it works"
echo -n it works
echo -n 'it works'
echo -n -n "it works"
echo -n -n -n it works
echo $?
echo $? + $?
echo "cat lol.c | cat lol.c"
$
echo $
echo ''
echo ""
echo '$?'
echo "$?"
echo "echo $?"
echo "echo '$?'"
echo "echo \"$?\""
echo "echo \$?"
echo "echo \$$"
echo ' hola >> file '
ls
echo " hola >> file "
ls
echo hola >> file
ls
cat file
rm file
echo $a""a
echo $a"" a
echo $""
echo $2""
echo $variablequenoexiste""
echo $USER""
echo '$USER'""
echo 1 $2 $3"" $4"" 5 $seis $siete"" 8
echo 1 $USER"" $USER"" $USER"" 4
echo 1 $2"" $3"" $4"" $5"" 6

echo "$USER''z"
echo "$USER""z"
echo "$USER''"
echo 1 $2 $3 $4 $5 6
echo 1 $2 $3"" $4 $5"" 6
echo a'bc'd
echo 'foo'bar''
echo -e "\n"
'/bin/echo hola' # una sola palabra → ruta con espacio (debe fallar)
"/bin/echo hola" # una sola palabra (ruta+arg) → debe fallar
/bin/echo ""
ls -z # opción inválida → mismo error
EMPTYVAR=
echo $EMPTYVAR
export EMPTY_VAR
echo $EMPTY_VAR
EMPTY_VAR=ahora no estoy vacia
unset EMPTY_VAR
echo $EMPTY_VAR
echo $SHLVL
bash
echo $SHLVL
exit
echo $?
bash
echo $SHLVL
bash
echo $SHLVL
bash
echo $SHLVL
exit 1
echo $?
echo $SHLVL
exit 123
echo $?
echo $SHLVL
exit -n
echo $?
echo $SHLVL
pwd
VARPWD=$(pwd)
VARUSER=$(whoami)
echo $VARPWD
cd ./
cd .
cd ..
pwd
cd ../..
pwd
cd $VARPWD
pwd
cd /home/
pwd
/bin/pwd
cd $VARPWD
cd home
cd home/
cd ./home/
cd ../home
cd ../../home
cd /home/$VARUSER/../$VARUSER/./$VARUSER/
cd /home/$VARUSER/../$VARUSER/./$VARUSER
cd ~
cd $HOME
cd ~/../home/$VARUSER
cd ~/../home/$VARUSER/./$VARUSER
cd ~/../home/$VARUSER/../$VARUSER
cd /home/$VARUSER/
cd /home/$VARUSER
cd ~/home/$VARUSER/
cd ~/home/$VARUSER
cd /absulute/path/that/does/not/exist/
pwd
/bin/pwd
echo "ENV MANAGEMENT"
env
/bin/env
setenv FOO=bar
setenv HELLO="hello world"
setenv EMPTY=""
echo $FOO
echo $HELLO
echo $EMPTY
env
/usr/bin/env
unsetenv FOO
unsetenv HELLO
unsetenv EMPTY
echo "PATH MANAGEMENT"
VARPAHTH=$PATH
unsetenv PATH
echo $VARPAHTH
echo $PATH
env
/usr/bin/env
setenv PATH=/bin:/usr/bin
ls
/bin/ls
emacs
/bin/emacs
/usr/bin/emacs
./bin/ls
unsetenv PATH
ls
emacs
/bin/ls
/usr/bin/emacs
setenv PATH=$VARPAHTH
echo $PATH
env
/usr/bin/env
echo "COMMAND LINE MANAGEMENT"
NOTHING=""
SINGLESPACE=" "
SPACESANDTAB="    "
SPACES&TABBF&AFT="   /ls -l -A    "
SPACES&TABINSIDE="ls     -l    -A"
SPACES&TABINBDAFT="	ls -l   -A    "
$NOTHING
$SINGLESPACE
$SPACESANDTAB
$SPACES&TABBF&AFT
$SPACES&TABINSIDE
$SPACES&TABINBDAFT
# create folder
mkdir -p /tmp/bin
# create the program that prints "KO"
cat > /tmp/bin/test_exec_rights <<'EOF'
#!/usr/bin/env bash
echo "RIGHTS"
EOF
# set permissions to 644 (rw-r--r--)
chmod 644 /tmp/bin/test_exec_rights
# add /tmp/bin to PATH for this session (prepended so it takes precedence)
export PATH="/tmp/bin:$PATH"
./test_exec_rights
chmod 755 ./test_exec_rights
echo TOP; ls; echo MIDDLE; ls; echo BOTTOM;
cd $VARPWD
cat Makefile | grep minishell | wc -l
cat Makefile | grep noexist | wc -l
cat noexist | wc -l
asdfsdfgfdgdhghhg
echo $PATH
echo $HOME
echo $USER
echo $SHLVL
echo $?
echo $SHLVL + $?
USER=false_user
echo $USER
echo "$USER"
USER=acoronad
echo $USER
echo "$USER"
/bin/echo hola > file.txt                    # Redirección de salida
ls
cat file.txt
cat < file.txt                          # Redirección de entrada
/bin/echo otra >> file.txt                   # Append
cat file.txt
cat noexiste 2> err.txt                 # Redirigir stderr
ls
cat err.txt
cat noexiste
ls 1> out.txt 2> err.txt                # stdout y stderr separados
ls
cat out.txt
cat err.txt
ls &> todo.txt                          # stdout y stderr juntos
ls
cat todo.txt
/bin/echo hola >| file.txt                   # Redirección forzada
cat file.txt
/bin/echo hola >&1                           # Duplicar salida
/bin                                   # Ejecutar un directorio → EISDIR
./noexistente                          # Comando no encontrado → ENOENT    ¡¡¡¡¡ESTO DEBERÍA DEVOLVER NO SUCH FILE OR DIRECTORY!!!
ls > /no_perm                         # Sin permisos → EACCES
/bin/echo hola > /root/test.txt            # EACCES por falta de permisos
/bin/echo uno && /bin/echo dos
/bin/false || /bin/echo se ejecuta
false && /bin/echo no se ejecuta
/bin/echo uno; /bin/echo dos
(/bin/echo subshell)
(/bin/echo uno && /bin/echo dos)
/bin/echo hola > out.txt && cat < out.txt
lh
cat out.txt
rm out.txt
/bin/echo err > /tmp/test 2>> /tmp/test						#Como es evidente, hay que comprobar el documento test en la carpeta tmp
cat /tmp/test
rm /tmp/test
(/bin/echo hola > hola.txt; cat hola.txt)
rm hola.txt
/bin/echo exportando; /usr/bin/env | /usr/bin/grep TEST
/bin/echo exportando; /usr/bin/env | /usr/bin/grep HOME
'/bin/echo hola'               # Comillas simples sin expansión
/bin/echo hola\ mundo          # Escape
echo uno\ dos\ tres				# Esperado: uno dos tres
echo signo\? interrogacion
echo barra\|vertical
echo barra\\invertida
/bin/echo comilla\'simple
/bin/echo comilla\"doble
/bin/echo "comilla\"doble"
/bin/echo 'comilla\'simple'
/bin/echo 'comilla\'simple''
/bin/echo expansion\$USER
"/bin/echo hola"               # Comillas dobles
/bin/echo uno | tr a-z A-Z > upper.txt
cat upper.txt
cat < upper.txt | grep UNO >> grep_result.txt
cat grep_result.txt
rm upper.txt grep_result.txt
(/bin/echo uno && /bin/echo dos) | grep uno > pipe_output.txt
cat pipe_output.txt
rm pipe_output.txt
(/bin/echo uno; /bin/echo dos) && /bin/echo tres || /bin/echo cuatro
/bin/echo uno; /bin/echo dos | grep dos
#  /bin/echo $HOME "$PATH" hola\ mundo  uno\ dos\ tres signo\? interrogacion barra\|vertical barra\\invertida  comilla\'simple comilla\"doble "comilla\"doble" 'comilla\'simple'' expansion\$USER 
cat << EOF
linea 1
linea 2
EOF
cat << 'EOF'
$HOME no debe expandirse
EOF
/bin/echo heredoc >> prueba.txt << EOF     ¡¡¡¡¡¡¡¡¡ TODO TIPO DE PROBLEMAS CON EOF !!!!!!!
esto
es
una
prueba
EOF
cat prueba.txt
rm prueba.txt
 /usr/bin/env | /usr/bin/grep TEST'
/usr/bin/env | /usr/bin/grep HOME'



#autocompletar con tab
#probar señales
# && || para el bonus
# wildcards  *

# MANUAL TESTS FOR DEBUGGING PURPOSES
# cat <&0                                # Duplicar entrada
# echo 'foo'bar'

#autocompletar con tab
#probar señales
# gestionar background con & o eliminarlo