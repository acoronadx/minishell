#!/usr/bin/env bash
# Tester robusto para minishell (compatible con “JapoTester”)
# - Fuerza entorno estable (LC_ALL/LANG/PATH)
# - No imprime prompts
# - Cierra minishell por EOF (sin 'exit') y recorta cualquier 'exit' impreso
# - Compara contra bash con la MISMA orden
# - Aísla pruebas con redirecciones en tmpdirs
# - Evita "cp: cannot stat" comprobando existencia antes

set -euo pipefail

# =========================
# ENTORNO / CONFIG
# =========================
export LC_ALL=C
export LANG=C
MINISHELL="${MINISHELL:-./minishell}"
_MARK="__MSH_END__"
TIMEOUT_SECS="${TIMEOUT_SECS:-4}"

# Entorno mínimo idéntico para ambos
SAME_ENV='env -i LC_ALL=C LANG=C PATH="/usr/bin:/bin" HOME="$HOME"'

# Colores
RED='\033[0;31m'; GREEN='\033[0;32m'; RESET='\033[0m'
ok()   { echo -e "${GREEN}✔️${RESET}  $1"; }
nok()  { echo -e "${RED}❌${RESET}  $1"; }
showdiff(){ echo "     Diferencias:"; diff -u "$1" "$2" || true; }

_have_timeout(){ command -v timeout >/dev/null 2>&1; }

# =========================
# FIXTURES
# =========================
mkdir -p redirs
[ -f redirs/a ] || printf "ejemplo\nrollon\n" > redirs/a
[ -f redirs/b ] || printf "otra\ncosa\n"      > redirs/b

# =========================
# CAPTURA SALIDAS
# =========================
# Minishell: alimentamos CMD + echo MARK. Cerramos por EOF (sin 'exit').
# Recortamos todo a la izquierda del marcador y si la ÚLTIMA línea es 'exit', la eliminamos.
capture_ms() {
  local cmd="$1" out
  if _have_timeout; then
    out="$(
      { printf "%s\n" "$cmd"; printf "echo %s\n" "$_MARK"; } \
      | timeout -k 1 "${TIMEOUT_SECS}s" bash -c "$SAME_ENV \"$MINISHELL\"" 2>&1
    )" || true
  else
    out="$(
      { printf "%s\n" "$cmd"; printf "echo %s\n" "$_MARK"; } \
      | bash -c "$SAME_ENV \"$MINISHELL\"" 2>&1
    )" || true
  fi
  # Cortamos en el marcador
  out="$(printf "%s\n" "$out" | awk -v m="$_MARK" '$0==m{exit} {print}')"
  # Si la última línea es exactamente 'exit', elimínala (muchos minishell la imprimen indebidamente en no interactivo)
  if [ -n "$out" ] && [ "$(printf "%s\n" "$out" | tail -n1)" = "exit" ]; then
    out="$(printf "%s\n" "$out" | sed '$d')"
  fi
  printf "%s\n" "$out"
}

# Bash: ejecutamos con -lc, añadimos el mismo marcador y recortamos igual
capture_bash() {
  local cmd="$1"
  bash -lc "$cmd; echo $_MARK" 2>&1 | awk -v m="$_MARK" '$0==m{exit} {print}'
}

# =========================
# RUNNERS
# =========================
run() {
  local INPUT="$1" EXPECTED="$2"
  local t1 t2; t1="$(mktemp)"; t2="$(mktemp)"
  capture_ms "$INPUT"   > "$t1"
  capture_bash "$EXPECTED" > "$t2"
  if diff -q "$t1" "$t2" >/dev/null; then
    ok "$INPUT"
  else
    nok "$INPUT"; showdiff "$t1" "$t2"
  fi
  rm -f "$t1" "$t2"
}

run_status() { # compara $? tras ejecutar INPUT
  local INPUT="$1" EXP="$2" code=0
  if _have_timeout; then
    { printf "%s\n" "$INPUT"; } | timeout -k 1 "${TIMEOUT_SECS}s" bash -c "$SAME_ENV \"$MINISHELL\"" >/dev/null 2>&1 || true
  else
    { printf "%s\n" "$INPUT"; } | bash -c "$SAME_ENV \"$MINISHELL\"" >/dev/null 2>&1 || true
  fi
  code=$?
  if [ "$code" -eq "$EXP" ]; then ok "$INPUT (exit $code)"; else nok "$INPUT (got $code, expected $EXP)"; fi
}

# Aíslate a tmpdir cuando haya ficheros implicados
_run_in_tmp() {
  local body_fn="$1"
  local TMP; TMP="$(mktemp -d)"
  ( cd "$TMP" && eval "$body_fn" )
  rm -rf "$TMP"
}

# Comparar stdout/err → sin ficheros
run_simple() {
  local INPUT="$1" EXPECTED="$2" t1 t2
  t1="$(mktemp)"; t2="$(mktemp)"
  if _have_timeout; then
    { printf "%s\n" "$INPUT"; } | timeout -k 1 "${TIMEOUT_SECS}s" bash -c "$SAME_ENV \"$MINISHELL\"" >"$t1" 2>&1 || true
  else
    { printf "%s\n" "$INPUT"; } | bash -c "$SAME_ENV \"$MINISHELL\"" >"$t1" 2>&1 || true
  fi
  $SAME_ENV bash -lc "$EXPECTED" >"$t2" 2>&1
  if diff -u "$t1" "$t2" >/dev/null; then ok "$INPUT"; else nok "$INPUT"; showdiff "$t1" "$t2"; fi
  rm -f "$t1" "$t2"
}

# Comparar redirecciones: lista de ficheros a verificar
cmp_files_iso() {
  local INPUT="$1" EXPECTED="$2"; shift 2
  local FILES=("$@")
  _run_in_tmp "$(cat <<'EOS'
  set -euo pipefail
  # Carga “contexto” desde variables de la función exterior
  INPUT="${INPUT}"; EXPECTED="${EXPECTED}"
  FILES=("${FILES[@]}")

  # limpia restos
  for f in "${FILES[@]}"; do rm -f "$f" "ms_$(basename "$f")" "ref_$(basename "$f")"; done

  # Ejecuta minishell
  { printf "%s\n" "$INPUT"; printf "echo __MSH_END__\n"; } | bash -c "$SAME_ENV \"$MINISHELL\"" >/dev/null 2>&1 || true

  # Renombra salidas minishell → ms_*
  for f in "${FILES[@]}"; do [ -e "$f" ] && mv "$f" "ms_$(basename "$f")"; done

  # Ejecuta bash de referencia
  bash -lc "$EXPECTED" >/dev/null 2>&1

  # Copia ref_* solo si existen
  for f in "${FILES[@]}"; do [ -e "$f" ] && cp -f "$f" "ref_$(basename "$f")" || true; done

  ok_all=1
  for f in "${FILES[@]}"; do
    local MS="ms_$(basename "$f")" REF="ref_$(basename "$f")"
    if [ ! -e "$MS" ] || [ ! -e "$REF" ] || ! diff -q "$MS" "$REF" >/dev/null; then ok_all=0; break; fi
  done

  if [ $ok_all -eq 1 ]; then
    echo -e "${GREEN}✔️${RESET}  $INPUT"
  else
    echo -e "${RED}❌${RESET}  $INPUT"
    echo "     Diferencias (ficheros):"
    for f in "${FILES[@]}"; do
      local MS="ms_$(basename "$f")" REF="ref_$(basename "$f")"
      [ -e "$MS" ] && [ -e "$REF" ] && diff -u "$MS" "$REF" || true
    done
  fi
EOS
  )"
}

# /usr/bin/env comparado normalizando (quitamos variable _)
run_env_filtered() {
  local t1 t2; t1="$(mktemp)"; t2="$(mktemp)"
  if _have_timeout; then
    printf "env\n" | timeout -k 1 "${TIMEOUT_SECS}s" bash -c "$SAME_ENV \"$MINISHELL\"" | grep -v '^_=' | sort > "$t1" || true
  else
    printf "env\n" | bash -c "$SAME_ENV \"$MINISHELL\"" | grep -v '^_=' | sort > "$t1" || true
  fi
  $SAME_ENV bash -lc "env" | grep -v '^_=' | sort > "$t2"
  if diff -u "$t1" "$t2" >/dev/null; then ok "/usr/bin/env (normalizado)"; else nok "/usr/bin/env (normalizado)"; showdiff "$t1" "$t2"; fi
  rm -f "$t1" "$t2"
}

# Alias de comodidad
run_echo()  { run "$1" "$2"; }
run_echo2() { # INPUT con > fichero; EXPECTED sin redir. Deduce fichero automáticamente
  local INPUT="$1" EXPECTED="$2" OUTFILE
  OUTFILE="$(echo "$INPUT" | sed -n 's/.*>\s*\([^ ]*\).*/\1/p')"
  [ -n "${OUTFILE:-}" ] || OUTFILE="temp.out"
  cmp_files_iso "$INPUT" "$EXPECTED" "$OUTFILE"
}

# =========================
# TESTS
# =========================
clear
echo "##########################"
echo "# COMANDS con ruta       #"
echo "##########################"
echo

run '/bin/ls' '/bin/ls'
run '/bin/pwd' '/bin/pwd'
run '/bin/echo run_echo' '/bin/echo run_echo'
run_echo2 '/bin/echo -n run_echo > temp1' '/bin/echo -n run_echo'
run_echo2 '/bin/printf run_echo > temp1' '/bin/printf run_echo'
run '/bin/cat redirs/a' '/bin/cat redirs/a'
run 'whoami' 'whoami'
run '' ':'
run '       ' ':'
run '/bin/ls -la' '/bin/ls -la'
run '/bin/ls -l'  '/bin/ls -l'
run 'echo uno dos tres cuatro' 'echo uno dos tres cuatro'
run '/bin/ls -l -a -h' '/bin/ls -l -a -h'
run '/bin/ls -lhS'     '/bin/ls -lhS'
run '/bin/cat redirs/a redirs/b' '/bin/cat redirs/a redirs/b'
run 'head -n 5 redirs/a' 'head -n 5 redirs/a'

echo
echo "##############"
echo "# ECHO       #"
echo "##############"
echo

# NOTA: Algunos casos muy extremos de quoting se encapsulan en HEREDOC para evitar escapes locos.
run_echo 'echo patata' 'echo patata'
run_echo 'echo $a""a' 'echo $a""a'
run_echo 'echo $a"" a' 'echo $a"" a'
run_echo 'echo $a" " a' 'echo $a" " a'
run_echo 'echo $a "" a' 'echo $a "" a'
run_echo 'echo $a" $USER"' 'echo $a" $USER"'
run_echo 'echo $a"$USER"'  'echo $a"$USER"'
run_echo 'echo $USERa$HOME' 'echo $USERa$HOME'
run_echo 'echo "$  a"' 'echo "$  a"'
run_echo 'echo "$  a "' 'echo "$  a "'
run_echo 'echo "$?u "'  'echo "$?u "'
run_echo 'echo "$? us "' 'echo "$? us "'

kk=-n; export kk;               run_echo2 'echo $kk run_echo > temp1' 'echo $kk run_echo'
kk="-nnn -na run_echo"; export kk; run_echo2 'echo $kk tomate > temp1' 'echo $kk tomate'
run_echo 'echo $a' 'echo $a'
run_echo 'echo $"mgs"' 'echo $"mgs"'
run_echo "echo \$'msg'" "echo \$'msg'"

kk="echo -n msg1"; export kk;  run_echo '$kk -n msg2' '$kk -n msg2'

run_echo2 'echo ""-n run_echo > temp1' 'echo ""-n run_echo'
run_echo  'echo "" -n run_echo'        'echo "" -n run_echo'
run_echo2 'echo "-n" run_echo > temp1'  'echo "-n" run_echo'
run_echo2 "echo '-n' run_echo > temp1"  "echo '-n' run_echo"
run_echo  'echo "-n run_echo"'          'echo "-n run_echo"'
run_echo  "echo '-n run_echo'"          "echo '-n run_echo'"
run_echo  'echo "-n -na" run_echo'      'echo "-n -na" run_echo'
run_echo2 'echo "-nnnnnn" run_echo > temp1' 'echo "-nnnnnn" run_echo'
run_echo  'echo "-nnnnn " run_echo'     'echo "-nnnnn " run_echo'
run_echo  'echo " " -n run_echo'        'echo " " -n run_echo'
run_echo  'echo " " run_echo'           'echo " " run_echo'

kk="  hola         que     tal      "; export kk
run_echo 'echo $kk'   'echo $kk'
run_echo 'echo "$kk"' 'echo "$kk"'
run_echo "echo '\$kk'" "echo '\$kk'"

kk="echo tomate"; export kk
run_echo '$kk' '$kk'

# Tríos difíciles de quoting → HEREDOC con pares INPUT<TAB>EXPECTED
while IFS=$'\t' read -r INPUT EXPECTED; do
  [ -z "$INPUT" ] && continue
  run_echo "$INPUT" "$EXPECTED"
done <<'PAIRS'
echo "$ a"	echo "$ a"
echo "$a5e#tomate 'p'"	echo "$a5e#tomate 'p'"
echo "$a 'p'"	echo "$a 'p'"
echo "$%run_echo"	echo "$%run_echo"
echo "$!run_echo"	echo "$!run_echo"
echo "$a664464562ssertetr)'p'"	echo "$a664464562ssertetr)'p'"
echo $2run_echo	echo run_echo
echo $2 run_echo	echo run_echo
echo $* run_echo	echo run_echo
echo 'run_echo"$USER"'	echo 'run_echo"$USER"'
echo $.	echo $.
echo $	echo $
PAIRS

run_echo2 'echo -n hola > temp1'         'echo -n hola'
run_echo2 'echo -n -n hola > temp1'      'echo -n -n hola'
run_echo2 'echo -n -n -n hola > temp1'   'echo -n -n -n hola'
run_echo2 'echo -n -n -n -n hola > temp1' 'echo -n -n -n -n hola'
run_echo2 'echo -n -nnnn hola > temp1'   'echo -n -nnnn hola'
run_echo2 'echo -nnnn hola > temp1'      'echo -nnnn hola'
run_echo  'echo hola "\"n mundo'          "echo 'hola \n mundo'"

run_echo 'echo "hola mundo"'  'echo "hola mundo"'
run_echo "echo 'hola mundo'"  "echo 'hola mundo'"
run_echo 'echo hola" mundo"'  'echo hola" mundo"'
run_echo "echo 'hola'\" mundo\"" "echo hola mundo"
run_echo 'echo "saludo: $HOME"' 'echo "saludo: $HOME"'
kk=32; export kk;         run_echo 'echo $kk' 'echo $kk'
kk=hola; export kk;       run_echo 'echo "$kk"' 'echo "$kk"'
run_echo 'echo $NOEXISTE' 'echo $NOEXISTE'
run_echo 'echo $PATH'     'echo $PATH'
run_echo 'echo $LS_COLORS' 'echo $LS_COLORS'
run_echo 'echo "$USER$HOME"' 'echo "$USER$HOME"'
run_echo 'echo $LS_COLORS$PATH$LS_COLORS$PATH$LS_COLORS$PATH$LS_COLORS$PATH$LS_COLORS$PATH' \
         'echo $LS_COLORS$PATH$LS_COLORS$PATH$LS_COLORS$PATH$LS_COLORS$PATH$LS_COLORS$PATH'
run_echo 'echo "$USER"' 'echo "$USER"'
run_echo 'echo $USER'   'echo $USER'
run_echo "echo '\$USER'" "echo '\$USER'"
run_echo 'echo "$?"' 'echo "$?"'
run_echo 'echo "$USER literal $HOME"' 'echo "$USER literal $HOME"'
unset UNSETVAR; run_echo 'echo $UNSETVAR' 'echo $UNSETVAR'
run_echo 'echo ""' 'echo ""'
run_echo "echo ''" "echo ''"
run_echo 'echo "hola     mundo"' 'echo "hola     mundo"'
run_echo 'echo hola     mundo'   'echo hola     mundo'
run_echo2 'echo -n "hola\nmundo" > temp1' 'echo -n "hola\nmundo"'
run_echo 'echo "a'$USER'"' 'echo "a'$USER'"'
run_echo 'echo "'$USER'"'  'echo "'$USER'"'
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

# Redirecciones prefijadas
run_echo2 '>kk echo run_echo' 'echo run_echo'
run_echo2 '> kk echo run_echo' 'echo run_echo'
run_echo2 '>kk echo -n run_echo' 'echo -n run_echo'
run_echo2 '>kk echo -nnnnn -na run_echo' 'echo -nnnnn -na run_echo'
run_echo2 '>kk echo -nnnnn' 'echo -nnnnn'
run_echo2 '>kk echo' 'echo'

echo
echo "############"
echo "# EXIT     #"
echo "############"
echo

run_status "exit" 0
run_status "exit 42" 42
run_status "exit 255" 255
run_status "exit 256" 0
run_status "exit -1" 255
run_status "exit -255" 1
run_status "exit -256" 0
run_status "exit 2147483647" 255
run_status "exit 2147483648" 0
run_status "exit 9223372036854775807" 255
run_status "exit -9223372036854775808" 0
run_status "exit 9223372036854775808" 2
run_status "exit -9223372036854775809" 2
run_status "exit -922337203685477580834" 2
run_status "exit run_echo" 2

echo
echo "###################################"
echo "# VALOR RETORNO DE UN PROCESO     #"
echo "###################################"
echo

run_status "/bin/ls" 0
run_status "/bin/run_echo" 127
run_status '/bin/grep "ejemplo" redirs/a' 0
run_status '/bin/grep "rollon" redirs/a' 1
run_status '/bin/grep "rollon" redirs/c' 2
run_status '/bin/diff redirs/a redirs/b' 1
run_status '/bin/diff redirs/a redirs/a' 0
run_status '/bin/diff redirs/a redirs/c' 2
run_status '/bin/test -f redirs/a' 0
run_status '/bin/test -f redirs/c' 1
run_status '/bin/test 5 -gt 3' 0
run_status '/bin/test 1 -gt 3' 1
run_status '/bin/test 3 -eq 3' 0

# expr: usa SHLVL si existe, si no, finge 1 (coincidir ambos lados)
SHLVL_VAL=${SHLVL:-1}
run "expr $SHLVL_VAL + $SHLVL_VAL" "expr $SHLVL_VAL + $SHLVL_VAL"
run 'expr $SHLVL + $SHLVL' 'expr $SHLVL + $SHLVL - 2'

echo
echo "######################"
echo "# COMILLAS DOBLES    #"
echo "######################"
echo

run '"/bin/ls"' '"/bin/ls"'
run '"/bin/pwd"' '"/bin/pwd"'
run '"/bin/echo" run_echo' '"/bin/echo" run_echo'
run_echo2 '"/bin/echo" -n run_echo > temp1' '"/bin/echo" -n run_echo'
run_echo2 '"/bin/printf" run_echo > temp1'   '"/bin/printf" run_echo'
run '"/bin/cat" redirs/a' '"/bin/cat" redirs/a'
run '"whoami"' '"whoami"'
run_simple '""' '""'
run_simple '   ""    ' '   ""    '
run '"/bin/ls" -la' '"/bin/ls" -la'
run '"/bin/ls" -l'  '"/bin/ls" -l'
run '"echo" uno dos tres cuatro' '"echo" uno dos tres cuatro'
run '"/bin/ls" -l -a -h' '"/bin/ls" -l -a -h'
run '"/bin/ls" -lhS'     '"/bin/ls" -lhS'
run '"/bin/cat" redirs/a redirs/b' '"/bin/cat" redirs/a redirs/b'
run '"head" -n 5 redirs/a' '"head" -n 5 redirs/a'
run '"""p"w"d"""' '"""p"w"d"""'
run_simple '"pwd "' '"pwd "'
run_simple '"run_echo"' '"run_echo"'
run 'echo "cat lol.c | cat > lol.c"' 'echo "cat lol.c | cat > lol.c"'
run_simple '"ls "' '"ls "'
run_simple '"abc def"' '"abc def"'
run_simple '"echoo"' '"echoo"'
run_simple '"/noexiste"' '"/noexiste"'

echo
echo "#######################"
echo "# COMILLAS SIMPLES    #"
echo "#######################"
echo

run "'/bin/ls'" "'/bin/ls'"
run "'/bin/pwd'" "'/bin/pwd'"
run "'/bin/echo' run_echo" "'/bin/echo' run_echo"
run_echo2 "'/bin/echo' -n run_echo > temp1" "'/bin/echo' -n run_echo"
run "'/bin/cat' redirs/a" "'/bin/cat' redirs/a"
run "'whoami'" "'whoami'"
run_simple "''" "''"
run_simple "   ''    " "   ''    "
run "'/bin/ls' -la" "'/bin/ls' -la"
run "'/bin/ls' -l"  "'/bin/ls' -l"
run "'echo' uno dos tres cuatro" "'echo' uno dos tres cuatro"
run "'/bin/ls' -l -a -h" "'/bin/ls' -l -a -h"
run "'/bin/ls' -lhS"     "'/bin/ls' -lhS"
run "'/bin/cat' redirs/a redirs/b" "'/bin/cat' redirs/a redirs/b"
run "'head' -n 5 redirs/a" "'head' -n 5 redirs/a"
run "'''p'w'd'''" "'''p'w'd'''"
run_simple "'pwd '" "'pwd '"
run_simple "'run_echo'" "'run_echo'"
run "echo 'cat lol.c | cat > lol.c'" "echo 'cat lol.c | cat > lol.c'"
run_simple "'ls '" "'ls '"
run_simple "'abc def'" "'abc def'"
run_simple "'echoo'" "'echoo'"
run_simple "'/noexiste'" "'/noexiste'"
run "'echo' '\$HOME'" "'echo' '\$HOME'"
run "'/bin/echo' '\$PATH'" "'/bin/echo' '\$PATH'"
run "'printenv' 'USER'" "'printenv' 'USER'"
run "'echo' '   uno   dos   tres   '" "'echo' '   uno   dos   tres   '"
run "'/bin/echo' '   '" "'/bin/echo' '   '"
run "'cat' '   redirs/a   '" "'cat' '   redirs/a   '"
run "'/bin/echo' run_echo '|' 'cat'" "'/bin/echo' run_echo '|' 'cat'"
run "'/bin/ls' '-l' '|' 'wc' '-l'" "'/bin/ls' '-l' '|' 'wc' '-l'"
run_simple "'cat' redirs/a '|' 'grep' hola" "'cat' redirs/a '|' 'grep' hola"
run "'echo' hola '>' redirs/salida.txt" "'echo' hola '>' redirs/salida.txt"
run "'cat' '<' redirs/a" "'cat' '<' redirs/a"
run_simple "'grep' hola '<' redirs/a '>' redirs/out" "'grep' hola '<' redirs/a '>' redirs/out"
run 'echo '\''$USER'\''' 'echo '\''$USER'\'''
run_simple 'cd '\''~'\''' 'cd '\''~'\'''

echo
echo "########################"
echo "# EXTRA: COMANDOS BÁSICOS"
echo "########################"
echo

run 'echo hola mundo' 'echo hola mundo'
run 'ls -l /bin' 'ls -l /bin'
run '/bin/pwd' '/bin/pwd'
run_env_filtered

echo
echo "########################"
echo "# EXTRA: REDIRECCIONES"
echo "########################"
echo

printf "inicio\n" > file.txt
cmp_files_iso '/bin/echo hola > file.txt' '/bin/echo hola > file.txt' file.txt
run 'cat < file.txt' 'cat < file.txt'
cmp_files_iso '/bin/echo otra >> file.txt' '/bin/echo otra >> file.txt' file.txt
cmp_files_iso 'cat noexiste 2> err.txt' 'cat noexiste 2> err.txt' err.txt
cmp_files_iso 'ls 1> out.txt 2> err.txt' 'ls 1> out.txt 2> err.txt' out.txt err.txt
cmp_files_iso 'ls &> todo.txt' 'ls &> todo.txt' todo.txt
printf "old\n" > file.txt
cmp_files_iso '/bin/echo hola >| file.txt' '/bin/echo hola >| file.txt' file.txt
run '/bin/echo hola >&1' '/bin/echo hola >&1'

echo
echo "########################"
echo "# EXTRA: ERRORES/RETORNOS"
echo "########################"
echo
run 'false; echo $?' 'false; echo $?'
run 'true;  echo $?' 'true; echo $?'
run '/bin' '/bin'
run './noexistente' './noexistente'
run '/bin/echo hola > /root/test.txt' '/bin/echo hola > /root/test.txt'

echo
echo "########################"
echo "# EXTRA: EXPANSIONES"
echo "########################"
echo
run '/bin/echo $HOME' '/bin/echo $HOME'
run '/bin/echo "$PATH"' '/bin/echo "$PATH"'
run '/bin/echo $HOME $PATH' '/bin/echo $HOME $PATH'

echo
echo "########################"
echo "# EXTRA: LÓGICOS Y SECUENCIAS"
echo "########################"
echo
run '/bin/echo uno && /bin/echo dos' '/bin/echo uno && /bin/echo dos'
run '/bin/false || /bin/echo se ejecuta' '/bin/false || /bin/echo se ejecuta'
run 'false && /bin/echo no se ejecuta'   'false && /bin/echo no se ejecuta'
run '/bin/echo uno; /bin/echo dos'       '/bin/echo uno; /bin/echo dos'

echo
echo "########################"
echo "# EXTRA: ESCAPES/COMILLAS"
echo "########################"
echo
run "'/bin/echo hola'" "'/bin/echo hola'"
run '/bin/echo hola\ mundo' '/bin/echo hola\ mundo'
run 'echo uno\ dos\ tres' 'echo uno\ dos\ tres'
run 'echo signo\? interrogacion' 'echo signo\? interrogacion'
run 'echo barra\|vertical' 'echo barra\|vertical'
run 'echo barra\\invertida' 'echo barra\\invertida'
run '/bin/echo comilla\'simple' '/bin/echo comilla\'simple'
run '/bin/echo comilla\"doble' '/bin/echo comilla\"doble'
run '/bin/echo "comilla\"doble"' '/bin/echo "comilla\"doble"'
run '/bin/echo expansion\$USER' '/bin/echo expansion\$USER'
run '"/bin/echo hola"' '"/bin/echo hola"'
run '/bin/echo ""' '/bin/echo ""'
run 'ls -z' 'ls -z'

echo
echo "########################"
echo "# EXTRA: PIPES+FICHEROS"
echo "########################"
echo
cmp_files_iso '/bin/echo uno | tr a-z A-Z > upper.txt' '/bin/echo uno | tr a-z A-Z > upper.txt' upper.txt
cmp_files_iso 'cat < upper.txt | grep UNO >> grep_result.txt' 'cat < upper.txt | grep UNO >> grep_result.txt' grep_result.txt
run '/bin/echo uno; /bin/echo dos | grep dos' '/bin/echo uno; /bin/echo dos | grep dos'
run '/bin/echo hola > out.txt && cat < out.txt' '/bin/echo hola > out.txt && cat < out.txt'

echo
echo "########################"
echo "# EXTRA: HERE-DOCS"
echo "########################"
echo
run 'cat << EOF
linea 1
linea 2
EOF' 'cat << EOF
linea 1
linea 2
EOF'

cmp_files_iso '/bin/echo heredoc >> prueba.txt << EOF
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

run "cat << 'EOF'
\$HOME no debe expandirse
EOF" "cat << 'EOF'
\$HOME no debe expandirse
EOF"

run '/bin/false; /bin/echo $?' '/bin/false; /bin/echo $?'

echo
echo "########################"
echo "# EXTRA: ENV en pipeline"
echo "########################"
echo
run '/bin/echo exportando; /usr/bin/env | /usr/bin/grep TEST' '/bin/echo exportando; /usr/bin/env | /usr/bin/grep TEST'
run '/bin/echo exportando; /usr/bin/env | /usr/bin/grep HOME' '/bin/echo exportando; /usr/bin/env | /usr/bin/grep HOME'
