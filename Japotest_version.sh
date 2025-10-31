#!/usr/bin/env bash
# Tester robusto para minishell (compatible con “JapoTester”)
# - Estabiliza locale
# - Cierra siempre la shell con 'exit'
# - Recorta cualquier prompt usando un marcador de fin
# - Compara contra bash -lc con la MISMA orden
# - Crea fixtures si faltan

# =========================
# ENTORNO / CONFIGURACIÓN
# =========================
export LC_ALL=C
export LANG=C
MINISHELL="${MINISHELL:-./minishell}"   # Ajusta si lo tienes en otra ruta
_MARK="__MSH_END__"

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'

ok()   { echo -e "${GREEN}✔️${RESET}  $1"; }
fail() {
  echo -e "${RED}❌${RESET}  $1"
  echo "     Diferencias:"
  cat "$2"
  cat "$3"
}

# =========================
# FIXTURES
# =========================
mkdir -p redirs
[ -f redirs/a ] || printf "ejemplo\nrollon\n" > redirs/a
[ -f redirs/b ] || printf "otra\ncosa\n"      > redirs/b

# =========================
# HELPERS DE CAPTURA
# =========================
# Ejecuta en minishell y recorta todo después del marcador
capture_ms() {
  local cmd="$1"
  # Alimentamos minishell con la orden, luego un marcador y por último exit
  {
    printf "%s\n" "$cmd"
    printf "echo %s\n" "$_MARK"
    printf "exit\n"
  } | "$MINISHELL" 2>&1 | awk -v m="$_MARK" '$0==m{exit} {print}'
}

# Ejecuta en bash -lc y recorta todo después del marcador
capture_bash() {
  local cmd="$1"
  bash -lc "$cmd; echo $_MARK" 2>&1 | awk -v m="$_MARK" '$0==m{exit} {print}'
}

# =========================
# RUNNERS DE COMPARACIÓN
# =========================
run() {
  local INPUT="$1" EXPECTED="$2"
  local t1 t2; t1="$(mktemp)"; t2="$(mktemp)"
  capture_ms "$INPUT"   > "$t1"
  capture_bash "$EXPECTED" > "$t2"
  if diff -q "$t1" "$t2" >/dev/null; then
    ok "$INPUT"
  else
    fail "$INPUT" "$t1" "$t2"
  fi
  rm -f "$t1" "$t2"
}

# Entorno mínimo idéntico para ambos
SAME_ENV='env -i LC_ALL=C LANG=C PATH="/usr/bin:/bin" HOME="$HOME"'

run_simple() { # sin ficheros, solo stdout/err
  INPUT="$1"; EXPECTED="$2"
  # caso comando vacío: compara contra ":" (no-op)
  if printf '%s' "$EXPECTED" | grep -q '^[[:space:]]*$'; then EXPECTED=":"; fi

  printf "%s\nexit\n" "$INPUT" | $SAME_ENV "$MINISHELL" > ms.out 2>&1
  $SAME_ENV bash -c "$EXPECTED" > sh.out 2>&1

  if diff -u ms.out sh.out >/dev/null; then
    echo -e "${GREEN}✔️${RESET}  $INPUT"
  else
    echo -e "${RED}❌${RESET}  $INPUT"; echo "Diferencias:"; diff -u ms.out sh.out || true
  fi
  rm -f ms.out sh.out
}

run_status() { # compara $? de minishell tras ejecutar INPUT
  INPUT="$1"; EXP="$2"
  printf "%s\n" "$INPUT" | $SAME_ENV "$MINISHELL" >/dev/null 2>&1; code=$?
  if [ "$code" -eq "$EXP" ]; then
    echo -e "${GREEN}✔️${RESET}  $INPUT (exit $code)"
  else
    echo -e "${RED}❌${RESET}  $INPUT (got $code, expected $EXP)"
  fi
}

run_isolated() { # para redirecciones: cada test en un tmpdir limpio
  INPUT="$1"; EXPECTED="$2"
  TMP="$(mktemp -d)"; cp -r redirs "$TMP/" 2>/dev/null || true
  (
    cd "$TMP"
    printf "%s\nexit\n" "$INPUT" | $SAME_ENV "$MINISHELL" > ms.out 2>&1
    $SAME_ENV bash -c "$EXPECTED" > sh.out 2>&1
    # compara ficheros resultantes si aplica; por defecto comparamos stdout
    if diff -u ms.out sh.out >/dev/null; then
      echo -e "${GREEN}✔️${RESET}  $INPUT"
    else
      echo -e "${RED}❌${RESET}  $INPUT"; echo "Diferencias:"; diff -u ms.out sh.out || true
    fi
  )
  rm -rf "$TMP"
}

run_env_sorted() { # compara /usr/bin/env normalizado
  printf "env\nexit\n" | $SAME_ENV "$MINISHELL" | sort > ms_env
  $SAME_ENV bash -c "env" | sort > sh_env
  if diff -u ms_env sh_env >/dev/null; then
    echo -e "${GREEN}✔️${RESET}  /usr/bin/env (normalizado)"
  else
    echo -e "${RED}❌${RESET}  /usr/bin/env (normalizado)"; diff -u ms_env sh_env || true
  fi
  rm -f ms_env sh_env
}

# Para casos “echo …” (pero usa el mismo núcleo de run)
run_echo() {
  local INPUT="$1" EXPECTED="$2"
  run "$INPUT" "$EXPECTED"
}

# Redirecciones: el INPUT incluye “> archivo”. Comparamos el archivo generado por minishell
# con la salida que genera bash al ejecutar EXPECTED (sin redirección) en un tmp.
run_echo2() {  # mantiene tu nombre original
  local INPUT="$1" EXPECTED="$2"
  # Extrae el archivo tras el primer '>'
  local OUTFILE
  OUTFILE="$(echo "$INPUT" | sed -n 's/.*>\s*\([^ ]*\).*/\1/p')"
  [ -n "$OUTFILE" ] || OUTFILE="out.$$"

  # Ejecuta en minishell (creará OUTFILE). Cerramos con exit para que no quede colgado.
  { printf "%s\n" "$INPUT"; printf "exit\n"; } | "$MINISHELL" >/dev/null 2>&1

  # Genera referencia con bash
  local ref; ref="$(mktemp)"
  bash -lc "$EXPECTED" > "$ref" 2>&1

  if diff -q "$OUTFILE" "$ref" >/dev/null; then
    # Muestra el comando “limpio” (sin la parte de redirección), para legibilidad
    local DISPLAY; DISPLAY="$(echo "$INPUT" | sed 's/ *>\s*[^ ]*//')"
    ok "$DISPLAY"
  else
    local DISPLAY; DISPLAY="$(echo "$INPUT" | sed 's/ *>\s*[^ ]*//')"
    echo -e "${RED}❌${RESET}  $DISPLAY"
    echo "     Diferencias:"
    cat "$OUTFILE"
    cat "$ref"
  fi
  rm -f "$OUTFILE" "$ref"
}

# Para redirecciones genéricas (alias del anterior por compatibilidad)
run_echo_redir() { run_echo2 "$@"; }

# Comprueba código de salida haciendo que minishell termine con “exit $?”
run_exit() {
  local INPUT="$1" EXPECTED="$2"
  { printf "%s\n" "$INPUT"; printf "exit \$?\n"; } | "$MINISHELL" >/dev/null 2>&1
  local code=$?
  if [ "$code" -eq "$EXPECTED" ]; then
    echo -e "${GREEN}✔️${RESET}  $INPUT (exit $code)"
  else
    echo -e "${RED}❌${RESET}  $INPUT (got $code, expected $EXPECTED)"
  fi
}

# Mensajes genéricos: comparamos “contiene ‘command not found’” en ambos
run_command_invalid() {
  local INPUT="$1" EXPECTED="$2"
  local t1 t2; t1="$(mktemp)"; t2="$(mktemp)"
  { printf "%s\n" "$INPUT"; printf "exit\n"; } | "$MINISHELL" > "$t1" 2>&1
  bash -lc "$EXPECTED" > "$t2" 2>&1

  if grep -qi "command not found" "$t1" && grep -qi "command not found" "$t2"; then
    ok "$INPUT"
  else
    fail "$INPUT" "$t1" "$t2"
  fi
  rm -f "$t1" "$t2"
}

# “No such file or directory”
run_directory_invalid() {
  local INPUT="$1" EXPECTED="$2"
  local t1 t2; t1="$(mktemp)"; t2="$(mktemp)"
  { printf "%s\n" "$INPUT"; printf "exit\n"; } | "$MINISHELL" > "$t1" 2>&1
  bash -lc "$EXPECTED" > "$t2" 2>&1

  if grep -qi "No such file or directory" "$t1" && grep -qi "No such file or directory" "$t2"; then
    ok "$INPUT"
  else
    fail "$INPUT" "$t1" "$t2"
  fi
  rm -f "$t1" "$t2"
}

# Para pruebas que verifican el exit de un proceso ejecutado (tu misma idea)
run_command_return_value() {
  local INPUT="$1" EXPECTED="$2"
  { printf "%s\n" "$INPUT"; printf "exit \$?\n"; } | "$MINISHELL" >/dev/null 2>&1
  local code=$?
  if [ "$code" -eq "$EXPECTED" ]; then
    echo -e "${GREEN}✔️${RESET}  $INPUT (exit $code)"
  else
    echo -e "${RED}❌${RESET}  $INPUT (got $code, expected $EXPECTED)"
  fi
}

# =========================
# TESTS (tu batería)
# =========================
clear
echo "##########################"
echo "# COMANDS con ruta       #"
echo "##########################"
echo -e "\n"

run '/bin/ls' '/bin/ls'
run '/bin/pwd' '/bin/pwd'
run '/bin/echo run_echo' '/bin/echo run_echo'
run_echo2 '/bin/echo -n run_echo > temp1' '/bin/echo -n run_echo'
run_echo2 '/bin/printf run_echo > temp1' '/bin/printf run_echo'
run '/bin/cat redirs/a' '/bin/cat redirs/a'
run 'whoami' 'whoami'
run '' ''
run '       ' '       '
run '/bin/ls -la' '/bin/ls -la'
run '/bin/ls -l' '/bin/ls -l'
run 'echo uno dos tres cuatro' 'echo uno dos tres cuatro'
run '/bin/ls -l -a -h' '/bin/ls -l -a -h'
run '/bin/ls -lhS' '/bin/ls -lhS'
run '/bin/cat redirs/a redirs/b' '/bin/cat redirs/a redirs/b'
run 'head -n 5 redirs/a' 'head -n 5 redirs/a'

echo -e "\n"
echo "##############"
echo "# ECHO       #"
echo "##############"
echo -e "\n"

run_echo 'echo patata' 'echo patata'
run_echo 'echo $a""a' 'echo $a""a'
run_echo 'echo $a"" a' 'echo $a"" a'
run_echo 'echo $a" " a' 'echo $a" " a'
run_echo 'echo $a "" a' 'echo $a "" a'
run_echo 'echo $a" $USER"' 'echo $a" $USER"'
run_echo 'echo $a"$USER"' 'echo $a"$USER"'
run_echo 'echo $USERa$HOME' 'echo $USERa$HOME'
run_echo 'echo "$  a"' 'echo "$  a"'
run_echo 'echo "$  a "' 'echo "$  a "'
run_echo 'echo "$?u "' 'echo "$?u "'
run_echo 'echo "$? us "' 'echo "$? us "'
export kk=-n
run_echo2 'echo $kk run_echo > temp1' 'echo $kk run_echo'
export kk="-nnn -na run_echo"
run_echo2 'echo $kk tomate > temp1' 'echo $kk tomate'
run_echo 'echo $a' 'echo $a'
run_echo 'echo $"mgs"' 'echo $"mgs"'
run_echo "echo $'msg'" "echo $'msg'"
export kk="echo -n msg1"
run_echo2 '$kk msg2 > temp1' '$kk msg2'
export kk="echo -nnnnn -n -nnnann msg1"
run_echo2 '$kk msg2 > temp1' '$kk msg2'
export kk="echo msg1"
run_echo '$kk -n msg2' '$kk -n msg2'
run_echo2 'echo ""-n run_echo > temp1' 'echo ""-n run_echo'
run_echo 'echo "" -n run_echo' 'echo "" -n run_echo'
run_echo2 'echo "-n" run_echo > temp1' 'echo "-n" run_echo'
run_echo2 "echo '-n' run_echo > temp1" "echo '-n' run_echo"
run_echo 'echo "-n run_echo"' 'echo "-n run_echo"'
run_echo "echo '-n run_echo'" "echo '-n run_echo'"
run_echo 'echo "-n -na" run_echo' 'echo "-n -na" run_echo'
run_echo2 'echo "-nnnnnn" run_echo > temp1' 'echo "-nnnnnn" run_echo'
run_echo 'echo "-nnnnn " run_echo' 'echo "-nnnnn " run_echo'
run_echo 'echo " " -n run_echo' 'echo " " -n run_echo'
run_echo 'echo " " run_echo' 'echo " " run_echo'
export kk="  hola         que     tal      "
run_echo 'echo $kk' 'echo $kk'
run_echo 'echo "$kk"' 'echo "$kk"'
run_echo "echo '\$kk'" "echo '\$kk'"
export kk="echo tomate"
run_echo '$kk' '$kk'
run_echo 'echo "$ a"' 'echo "$ a"'
run_echo 'echo "$a5e#tomate '"'"'p'"'"'"' 'echo "$a5e#tomate '"'"'p'"'"'"'
run_echo 'echo "$a '"'"'p'"'"'"' 'echo "$a '"'"'p'"'"'"'
run_echo 'echo "$%run_echo"' 'echo "$%run_echo"'
run_echo 'echo "$!run_echo"' 'echo "$!run_echo"'
run_echo 'echo "$a664464562ssertetr)'"'"'p'"'"'"' 'echo "$a664464562ssertetr)'"'"'p'"'"'"'
run_echo 'echo $2run_echo' 'echo run_echo'
run_echo 'echo $2 run_echo' 'echo run_echo'
run_echo 'echo $* run_echo' 'echo run_echo'
run_echo "echo 'run_echo\"\$USER\"'" "echo 'run_echo\"\$USER\"'"
run_echo 'echo $.' 'echo $.'
run_echo 'echo $' 'echo $'
run_echo 'echo hola' 'echo hola'
run_echo 'echo '"'"'hola'"'"'' 'echo '"'"'hola'"'"''
run_echo "echo '\"hola\"'" "echo '\"hola\"'"
run_echo2 'echo -n hola > temp1' 'echo -n hola'
run_echo2 'echo -n -n hola > temp1' 'echo -n -n hola'
run_echo2 'echo -n -n -n hola > temp1' 'echo -n -n -n hola'
run_echo2 'echo -n -n -n -n hola > temp1' 'echo -n -n -n -n hola'
run_echo2 'echo -n -nnnn hola > temp1' 'echo -n -nnnn hola'
run_echo2 'echo -nnnn hola > temp1' 'echo -nnnn hola'
run_echo 'echo hola "\"n mundo' "echo 'hola \n mundo'"
run_echo 'echo "hola mundo"' 'echo "hola mundo"'
run_echo "echo 'hola mundo'" "echo 'hola mundo'"
run_echo 'echo hola" mundo"' 'echo hola" mundo"'
run_echo 'echo "hola mundo"' 'echo "hola mundo"'
run_echo "echo 'hola mundo'" "echo 'hola mundo'"
run_echo "echo 'hola'\" mundo\"" "echo hola mundo"
run_echo 'echo "saludo: $HOME"' 'echo "saludo: $HOME"'
export kk=32
run_echo 'echo $kk' 'echo $kk'
export kk=hola
run_echo 'echo "$kk"' 'echo "$kk"'
run_echo 'echo $NOEXISTE' 'echo $NOEXISTE'
run_echo 'echo $PATH' 'echo $PATH'
run_echo 'echo $LS_COLORS' 'echo $LS_COLORS'
run_echo 'echo "$USER$HOME"' 'echo "$USER$HOME"'
run_echo 'echo $LS_COLORS$PATH$LS_COLORS$PATH$LS_COLORS$PATH$LS_COLORS$PATH$LS_COLORS$PATH' 'echo $LS_COLORS$PATH$LS_COLORS$PATH$LS_COLORS$PATH$LS_COLORS$PATH$LS_COLORS$PATH'
run_echo 'echo "$USER"' 'echo "$USER"'
run_echo 'echo $USER' 'echo $USER'
run_echo "echo '\$USER'" "echo '\$USER'"
run_echo 'echo "$?"' 'echo "$?"'
run_echo 'echo "$USER literal $HOME"' 'echo "$USER literal $HOME"'
export UNSETVAR=run_echo; unset UNSETVAR
run_echo 'echo $UNSETVAR' 'echo $UNSETVAR'
run_echo 'echo ""' 'echo ""'
run_echo "echo ''" "echo ''"
run_echo 'echo "hola     mundo"' 'echo "hola     mundo"'
run_echo 'echo hola     mundo' 'echo hola     mundo'
run_echo2 'echo -n "hola\nmundo" > temp1' 'echo -n "hola\nmundo"'
run_echo 'echo "a'\$USER'"' 'echo "a'\$USER'"'
run_echo 'echo "'\$USER'"' 'echo "'\$USER'"'
run_echo "echo \"\$USER's folder\"" "echo \"\$USER's folder\""
run_echo 'echo "$UNDEFINED"' 'echo "$UNDEFINED"'
run_echo 'echo "$UNDEFINED texto"' 'echo "$UNDEFINED texto"'
run_echo "echo '\$UNDEFINED texto'" "echo '\$UNDEFINED texto'"
run_echo 'echo "$USER''$HOME"' 'echo "$USER''$HOME"'
run_echo 'echo "$USER '' $HOME"' 'echo "$USER '' $HOME"'
run_echo 'echo "  $USER  "' 'echo "  $USER  "'
run_echo "echo \"'single' \"double\"\"" "echo \"'single' double\""
run_echo 'echo "$? status"' 'echo "$? status"'
run_echo 'echo $?' 'echo $?'
run_echo "echo \"'\$?'\"" "echo \"'\$?'\""
run_echo "echo '\"hola\"'" "echo '\"hola\"'"
run_echo "echo \" \" \$USER \" \"" "echo \" \" \$USER \" \""
run_echo 'echo $a " " a' 'echo $a " " a'
run_echo 'echo "$ a "' 'echo "$ a "'
run_echo 'echo run_echo "" "" "" tomate' 'echo run_echo "" "" "" tomate'
run_echo 'echo $a run_echo $a "" "" "" "" tomate' 'echo $a run_echo "" "" "" "" tomate'
run_echo 'echo run_echo $a "" "" "" "" tomate' 'echo run_echo $a "" "" "" "" tomate'
run_echo 'echo run_echo $a "" "" "" "" $a tomate' 'echo run_echo $a "" "" "" "" $a tomate'
run_echo 'echo run_echo $a """""""" $a tomate' 'echo run_echo $a """""""" $a tomate'
run_echo 'echo ""$a""t' 'echo ""$a""t'
run_echo 'echo ""$a"" t' 'echo ""$a"" t'
run_echo 'echo ""$a" " t' 'echo ""$a" " t'
run_echo 'echo ""$a "  " t ""$a "  " t ' 'echo ""$a "  " t ""$a "  " t '
run_echo_redir '>kk echo run_echo' 'echo run_echo'
run_echo_redir '> kk echo run_echo' 'echo run_echo'
run_echo_redir '>kk echo -n run_echo' 'echo -n run_echo'
run_echo_redir '>kk echo -nnnnn -na run_echo' 'echo -nnnnn -na run_echo'
run_echo_redir '>kk echo -nnnnn' 'echo -nnnnn'
run_echo_redir '>kk echo' 'echo'

echo -e "\n"
echo "############"
echo "# EXIT     #"
echo "############"
echo -e "\n"

run_exit "exit" 0
run_exit "exit 42" 42
run_exit "exit 255" 255
run_exit "exit 255" 255
run_exit "exit 256" 0
run_exit "exit -1" 255
run_exit "exit -255" 1
run_exit "exit -256" 0
run_exit "exit 2147483647" 255
run_exit "exit 2147483648" 0
run_exit "exit 9223372036854775807" 255
run_exit "exit -9223372036854775808" 0
run_exit "exit 9223372036854775808" 2
run_exit "exit -9223372036854775809" 2
run_exit "exit -922337203685477580834" 2
run_exit "exit run_echo" 2

echo -e "\n"
echo "###################################"
echo "# VALOR RETORNO DE UN PROCESO     #"
echo "###################################"
echo -e "\n"

run_command_return_value "/bin/ls" 0
run_command_return_value "/bin/run_echo" 127
run_command_return_value '/bin/grep "ejemplo" redirs/a' 0
run_command_return_value '/bin/grep "rollon" redirs/a' 1
run_command_return_value '/bin/grep "rollon" redirs/c' 2
run_command_return_value '/bin/diff redirs/a redirs/b' 1
run_command_return_value '/bin/diff redirs/a redirs/a' 0
run_command_return_value '/bin/diff redirs/a redirs/c' 2
run_command_return_value '/bin/test -f redirs/a' 0
run_command_return_value '/bin/test -f redirs/c' 1
run_command_return_value '/bin/test 5 -gt 3' 0
run_command_return_value '/bin/test 1 -gt 3' 1
run_command_return_value '/bin/test 3 -eq 3' 0

# Ejemplos expr con y sin SHLVL (si quieres mantenerlos)
SHLVL_VAL=${SHLVL:-1}
run "expr $SHLVL_VAL + $SHLVL_VAL" "expr $SHLVL_VAL + $SHLVL_VAL"
run 'expr $SHLVL + $SHLVL' 'expr $SHLVL + $SHLVL - 2'

echo -e "\n"
echo "######################"
echo "# COMILLAS DOBLES    #"
echo "######################"
echo -e "\n"

run '"/bin/ls"' '"/bin/ls"'
run '"/bin/pwd"' '"/bin/pwd"'
run '"/bin/echo" run_echo' '"/bin/echo" run_echo'
run_echo2 '"/bin/echo" -n run_echo > temp1' '"/bin/echo" -n run_echo'
run_echo2 '"/bin/printf" run_echo > temp1' '"/bin/printf" run_echo'
run '"/bin/cat" redirs/a' '"/bin/cat" redirs/a'
run '"whoami"' '"whoami"'
run_command_invalid '""' '""'
run_command_invalid '   ""    ' '   ""    '
run '"/bin/ls" -la' '"/bin/ls" -la'
run '"/bin/ls" -l' '"/bin/ls" -l'
run '"echo" uno dos tres cuatro' '"echo" uno dos tres cuatro'
run '"/bin/ls" -l -a -h' '"/bin/ls" -l -a -h'
run '"/bin/ls" -lhS' '"/bin/ls" -lhS'
run '"/bin/cat" redirs/a redirs/b' '"/bin/cat" redirs/a redirs/b'
run '"head" -n 5 redirs/a' '"head" -n 5 redirs/a'
run '"""p"w"d"""' '"""p"w"d"""'
run_command_invalid '"pwd "' '"pwd "'
run_command_invalid '"run_echo"' '"run_echo"'
run 'echo "cat lol.c | cat > lol.c"' 'echo "cat lol.c | cat > lol.c"'
run_command_invalid '"ls "' '"ls "'
run_command_invalid '"abc def"' '"abc def"'
run_command_invalid '"echoo"' '"echoo"'
run_directory_invalid '"/noexiste"' '"/noexiste"'

echo -e "\n"
echo "#######################"
echo "# COMILLAS SIMPLES    #"
echo "#######################"
echo -e "\n"

run "'/bin/ls'" "'/bin/ls'"
run "'/bin/pwd'" "'/bin/pwd'"
run "'/bin/echo' run_echo" "'/bin/echo' run_echo"
run_echo2 "'/bin/echo' -n run_echo > temp1" "'/bin/echo' -n run_echo"
run "'/bin/cat' redirs/a" "'/bin/cat' redirs/a"
run "'whoami'" "'whoami'"
run_command_invalid "''" "''"
run_command_invalid "   ''    " "   ''    "
run "'/bin/ls' -la" "'/bin/ls' -la"
run "'/bin/ls' -l" "'/bin/ls' -l"
run "'echo' uno dos tres cuatro" "'echo' uno dos tres cuatro"
run "'/bin/ls' -l -a -h" "'/bin/ls' -l -a -h"
run "'/bin/ls' -lhS" "'/bin/ls' -lhS"
run "'/bin/cat' redirs/a redirs/b" "'/bin/cat' redirs/a redirs/b"
run "'head' -n 5 redirs/a" "'head' -n 5 redirs/a"
run "'''p'w'd'''" "'''p'w'd'''"
run_command_invalid "'pwd '" "'pwd '"
run_command_invalid "'run_echo'" "'run_echo'"
run "echo 'cat lol.c | cat > lol.c'" "echo 'cat lol.c | cat > lol.c'"
run_command_invalid "'ls '" "'ls '"
run_command_invalid "'abc def'" "'abc def'"
run_command_invalid "'echoo'" "'echoo'"
run_directory_invalid "'/noexiste'" "'/noexiste'"
run "'echo' '\$HOME'" "'echo' '\$HOME'"
run "'/bin/echo' '\$PATH'" "'/bin/echo' '\$PATH'"
run "'printenv' 'USER'" "'printenv' 'USER'"
run "'echo' '   uno   dos   tres   '" "'echo' '   uno   dos   tres   '"
run "'/bin/echo' '   '" "'/bin/echo' '   '"
run "'cat' '   redirs/a   '" "'cat' '   redirs/a   '"
run "'/bin/echo' run_echo '|' 'cat'" "'/bin/echo' run_echo '|' 'cat'"
run "'/bin/ls' '-l' '|' 'wc' '-l'" "'/bin/ls' '-l' '|' 'wc' '-l'"
run_directory_invalid "'cat' redirs/a '|' 'grep' hola" "'cat' redirs/a '|' 'grep' hola"
run "'echo' hola '>' redirs/salida.txt" "'echo' hola '>' redirs/salida.txt"
run "'cat' '<' redirs/a" "'cat' '<' redirs/a"
run_directory_invalid "'grep' hola '<' redirs/a '>' redirs/out" "'grep' hola '<' redirs/a '>' redirs/out"
run 'echo '\''$USER'\''' 'echo '\''$USER'\'''
run_directory_invalid 'cd '\''~'\''' 'cd '\''~'\'''

#############################
# === EXTRA TESTS (ADD-ON) ===
# Pega todo este bloque al final de tu script actual
#############################

# Helpers locales (no colisionan con los tuyos)
run_env_filtered() {
    local INPUT="$1" EXPECTED="$2"
    printf "%s\nexit\n" "$INPUT" | "$MINISHELL" 2>&1 | sed '1d;$d' | grep -v '^_=' > .ms_env_out
    bash -lc "$EXPECTED" 2>&1 | grep -v '^_=' > .sh_env_out
    if diff -q .ms_env_out .sh_env_out >/dev/null; then
        echo -e "${GREEN}✔️${RESET}  $INPUT"
    else
        echo -e "${RED}❌${RESET}  $INPUT"
        echo "     Diferencias:"; diff -u .ms_env_out .sh_env_out || true
    fi
    rm -f .ms_env_out .sh_env_out
}

# Ejecuta un comando que crea/modifica ficheros y compara el/los archivos resultantes
_cmp_files() {
    local INPUT="$1" EXPECTED="$2"; shift 2
    local FILES=("$@")

    # limpiamos cualquier resto previo
    for f in "${FILES[@]}"; do rm -f "$f" "ms_$(basename "$f")"; done

    # minishell → mover salidas a ms_*
    printf "%s\nexit\n" "$INPUT" | "$MINISHELL" >/dev/null 2>&1
    for f in "${FILES[@]}"; do
        [ -e "$f" ] && mv "$f" "ms_$(basename "$f")"
    done

    # bash → crea salidas "normales"
    bash -lc "$EXPECTED" >/dev/null 2>&1

    # diff de cada fichero
    local ok=1
    for f in "${FILES[@]}"; do
        local base="ms_$(basename "$f")"
        if ! diff -q "$base" "$f" >/dev/null; then ok=0; break; fi
    done

    if [ $ok -eq 1 ]; then
        echo -e "${GREEN}✔️${RESET}  $INPUT"
    else
        echo -e "${RED}❌${RESET}  $INPUT"
        echo "     Diferencias (ficheros):"
        for f in "${FILES[@]}"; do
            local base="ms_$(basename "$f")"
            [ -e "$base" ] && [ -e "$f" ] && diff -u "$base" "$f" || true
        done
    fi

    rm -f "${FILES[@]}" ms_*
}

echo -e "\n"
echo "########################"
echo "# EXTRA: COMANDOS BÁSICOS"
echo "########################"
echo -e "\n"

run 'echo hola mundo' 'echo hola mundo'                # builtin
run 'ls -l /bin' 'ls -l /bin'                          # externo simple
run '/bin/pwd' '/bin/pwd'                              # pwd (ruta absoluta)

# /bin/env (filtramos _= para que no cambie entre shells)
run_env_filtered '/bin/env' '/bin/env'

echo -e "\n"
echo "########################"
echo "# EXTRA: REDIRECCIONES"
echo "########################"
echo -e "\n"

# Prepara file.txt
printf "inicio\n" > file.txt

# > y >>
_cmp_files '/bin/echo hola > file.txt' '/bin/echo hola > file.txt' file.txt
run 'cat < file.txt' 'cat < file.txt'
_cmp_files '/bin/echo otra >> file.txt' '/bin/echo otra >> file.txt' file.txt

# stderr a fichero
_cmp_files 'cat noexiste 2> err.txt' 'cat noexiste 2> err.txt' err.txt

# stdout y stderr separados
_cmp_files 'ls 1> out.txt 2> err.txt' 'ls 1> out.txt 2> err.txt' out.txt err.txt

# stdout+stderr juntos
_cmp_files 'ls &> todo.txt' 'ls &> todo.txt' todo.txt

# >| forzar sobreescritura
printf "old\n" > file.txt
_cmp_files '/bin/echo hola >| file.txt' '/bin/echo hola >| file.txt' file.txt

# Duplicación FDs
run 'cat <&0' 'cat <&0'
run '/bin/echo hola >&1' '/bin/echo hola >&1'

echo -e "\n"
echo "########################"
echo "# EXTRA: ERRORES/RETORNOS"
echo "########################"
echo -e "\n"

# Código de salida
run 'false; echo $?' 'false; echo $?'
run 'true; echo $?'  'true; echo $?'

# Ejecutar directorio → EISDIR
run '/bin' '/bin'

# No existe → ENOENT
run './noexistente' './noexistente'

# Falta de permisos → EACCES (no uses /no_perm: suele ser ENOENT)
run '/bin/echo hola > /root/test.txt' '/bin/echo hola > /root/test.txt'

echo -e "\n"
echo "########################"
echo "# EXTRA: EXPANSIONES"
echo "########################"
echo -e "\n"

run '/bin/echo $HOME' '/bin/echo $HOME'
run '/bin/echo "$PATH"' '/bin/echo "$PATH"'
run '/bin/echo $HOME $PATH' '/bin/echo $HOME $PATH'

echo -e "\n"
echo "########################"
echo "# EXTRA: LÓGICOS Y SECUENCIAS"
echo "########################"
echo -e "\n"

run '/bin/echo uno && /bin/echo dos' '/bin/echo uno && /bin/echo dos'
run '/bin/false || /bin/echo se ejecuta' '/bin/false || /bin/echo se ejecuta'
run 'false && /bin/echo no se ejecuta' 'false && /bin/echo no se ejecuta'
run '/bin/echo uno; /bin/echo dos' '/bin/echo uno; /bin/echo dos'

echo -e "\n"
echo "########################"
echo "# EXTRA: ESCAPES/COMILLAS"
echo "########################"
echo -e "\n"

run "'/bin/echo hola'" "'/bin/echo hola'"              # una sola palabra → ruta con espacio (debe fallar)
run '/bin/echo hola\ mundo' '/bin/echo hola\ mundo'
run 'echo uno\ dos\ tres' 'echo uno\ dos\ tres'
run 'echo signo\? interrogacion' 'echo signo\? interrogacion'
run 'echo barra\|vertical' 'echo barra\|vertical'
run 'echo barra\\invertida' 'echo barra\\invertida'
run '/bin/echo comilla\'simple' '/bin/echo comilla\'simple'
run '/bin/echo comilla\"doble' '/bin/echo comilla\"doble'
run '/bin/echo "comilla\"doble"' '/bin/echo "comilla\"doble"'
run '/bin/echo expansion\$USER' '/bin/echo expansion\$USER'
run '"/bin/echo hola"' '"/bin/echo hola"'              # una sola palabra (ruta+arg) → debe fallar
run '/bin/echo ""' '/bin/echo ""'
run 'ls -z' 'ls -z'                                    # opción inválida → mismo error

echo -e "\n"
echo "########################"
echo "# EXTRA: PIPES+FICHEROS"
echo "########################"
echo -e "\n"

# upper.txt con mayúsculas
_cmp_files '/bin/echo uno | tr a-z A-Z > upper.txt' '/bin/echo uno | tr a-z A-Z > upper.txt' upper.txt

# append de coincidencias
_cmp_files 'cat < upper.txt | grep UNO >> grep_result.txt' 'cat < upper.txt | grep UNO >> grep_result.txt' grep_result.txt

# Precedencia ; vs |
run '/bin/echo uno; /bin/echo dos | grep dos' '/bin/echo uno; /bin/echo dos | grep dos'

# Combinada: escribe y luego lee
run '/bin/echo hola > out.txt && cat < out.txt' '/bin/echo hola > out.txt && cat < out.txt'

echo -e "\n"
echo "########################"
echo "# EXTRA: HERE-DOCS"
echo "########################"
echo -e "\n"

# Básico
run 'cat << EOF
linea 1
linea 2
EOF' 'cat << EOF
linea 1
linea 2
EOF'

# Heredoc + append en el mismo comando
_cmp_files '/bin/echo heredoc >> prueba.txt << EOF
esto
es
una
prueba
EOF' '/bin/echo heredoc >> prueba.txt << EOF
esto
es
una
prueba
EOF' prueba.txt

# Delimitador con comillas: NO expande
run "cat << 'EOF'
\$HOME no debe expandirse
EOF" "cat << 'EOF'
\$HOME no debe expandirse
EOF"

# Imprimir último status (después de algo que falle)
run '/bin/false; /bin/echo $?' '/bin/false; /bin/echo $?'

echo -e "\n"
echo "########################"
echo "# EXTRA: ENV en pipeline"
echo "########################"
echo -e "\n"

run '/bin/echo exportando; /usr/bin/env | /usr/bin/grep TEST' '/bin/echo exportando; /usr/bin/env | /usr/bin/grep TEST'
run '/bin/echo exportando; /usr/bin/env | /usr/bin/grep HOME' '/bin/echo exportando; /usr/bin/env | /usr/bin/grep HOME'

echo -e "\n"
echo "########################"
echo "# BONUS (desactivado por defecto)"
echo "########################"
echo -e "\n"
# Para activar: export BONUS=1
if [ "${BONUS:-0}" -eq 1 ]; then
  echo "(BONUS) Se lanzan pruebas con background/subshell; el orden puede variar."
  # OJO: estas pueden ser frágiles por asincronía; se muestran para exploración manual
  run '(/bin/echo subshell)' '(/bin/echo subshell)'
  run '(/bin/echo uno && /bin/echo dos)' '(/bin/echo uno && /bin/echo dos)'
  run '(/bin/echo uno; /bin/echo dos) && /bin/echo tres || /bin/echo cuatro' '(/bin/echo uno; /bin/echo dos) && /bin/echo tres || /bin/echo cuatro'

  _cmp_files '/bin/echo err > /tmp/test 2>> /tmp/test' '/bin/echo err > /tmp/test 2>> /tmp/test' /tmp/test

  run '/bin/echo fondo &' '/bin/echo fondo &'
  run 'ls > out.txt 2>&1 &' 'ls > out.txt 2>&1 &'
  run '(/bin/echo uno > uno.txt) & /bin/echo dos >> dos.txt' '(/bin/echo uno > uno.txt) & /bin/echo dos >> dos.txt'
  run 'sleep 1 &; /bin/echo terminado' 'sleep 1 &; /bin/echo terminado'
  run '(/bin/echo uno && sleep 1) &' '(/bin/echo uno && sleep 1) &'
  run '/bin/echo inicio && cat << EOF | grep hola > resultado.txt
hola mundo
chao
EOF' '/bin/echo inicio && cat << EOF | grep hola > resultado.txt
hola mundo
chao
EOF'
  run '/bin/echo uno > uno.txt & /bin/echo dos >> dos.txt' '/bin/echo uno > uno.txt & /bin/echo dos >> dos.txt'
  run '/bin/echo uno | /bin/echo dos > file.txt &' '/bin/echo uno | /bin/echo dos > file.txt &'
fi
