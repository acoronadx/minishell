#!/usr/bin/env bash
# ============================================================
#  Minishell Test Suite v3.1 — Bash vs Minishell (no interactivo)
#  - Autodetecta minishell en ./minishell, ./build/minishell, ./bin/minishell
#    o usa la ruta pasada como 1er argumento.
#  - Verifica que es archivo regular (-f) y ejecutable (-x); evita directorios.
#  - Evita here-string (<<<). Usa printf | cat (POSIX).
#  - Registra y COMPARA exit status.
#  - Compara logs (diff salidas).
#  - Compara filesystem: “solo en bash/minishell” y diffs de contenido.
# ============================================================

set -u

# -------- Resolver ruta del minishell --------
MINISHELL_BIN="${1:-}"
if [[ -z "$MINISHELL_BIN" ]]; then
  for c in ./minishell ./build/minishell ./bin/minishell; do
    if [[ -f "$c" && -x "$c" ]]; then
      MINISHELL_BIN="$c"
      break
    fi
  done
fi

BASH_BIN="bash"
STAMP="$(date +%Y%m%d_%H%M%S)"
SUITE_ROOT="mshell_suite_${STAMP}"
TESTFILE="test_commands.in"

mkdir -p "$SUITE_ROOT"/{work_bash,work_minishell}
cd "$SUITE_ROOT" || exit 1

WORK_BS="work_bash"
WORK_MS="work_minishell"

# -------- Generar batería (sin <<<) --------
cat > "$TESTFILE" <<'CMDS'
echo "==== SECCIÓN: BUILTINS Y EXTERNOS ===="
echo hola mundo
ls -l /bin
/bin/pwd
/bin/env

echo
echo "==== SECCIÓN: REDIRECCIONES BÁSICAS ===="
/bin/echo hola > file.txt
echo "-- ls tras crear file.txt --"
ls -l
echo "-- cat file.txt --"
cat -n file.txt

cat < file.txt
/bin/echo otra >> file.txt
echo "-- cat file.txt (tras append) --"
cat -n file.txt

echo
echo "==== SECCIÓN: STDERR y DUPs ===="
cat noexiste 2> err.txt
echo "-- ls para ver err.txt --"
ls -l err.txt
echo "-- cat err.txt --"
cat -n err.txt

ls 1> out.txt 2> err.txt
echo "-- ls de out/err separados --"
ls -l out.txt err.txt

ls &> todo.txt
echo "-- ls de todo.txt --"
ls -l todo.txt
echo "-- head de todo.txt --"
head -n 5 todo.txt

/bin/echo hola >| file.txt
echo "-- cat file.txt (>|) --"
cat -n file.txt

/bin/echo hola >&1
printf "linea_por_stdin_dup0\n" | cat -

echo
echo "==== SECCIÓN: ERRORES CLÁSICOS ===="
/bin                      # EISDIR
./noexistente             # ENOENT
ls > /no_perm             # EACCES probable

echo
echo "==== SECCIÓN: EXPANSIONES ===="
/bin/echo $HOME
/bin/echo "$PATH"
/bin/echo $HOME $PATH

echo
echo "==== SECCIÓN: OPERADORES LÓGICOS y SECUENCIACIÓN ===="
/bin/echo uno && /bin/echo dos
/bin/false || /bin/echo se ejecuta
false && /bin/echo no se ejecuta
/bin/echo uno; /bin/echo dos

echo
echo "==== SECCIÓN: SUBSHELLS (si no soporta, ver diffs) ===="
(/bin/echo subshell)
(/bin/echo uno && /bin/echo dos)
/bin/echo hola > out_logic.txt && cat < out_logic.txt
/bin/echo err > /tmp/test 2>> /tmp/test
echo "-- tail /tmp/test --"
tail -n 5 /tmp/test
(/bin/echo hola > hola.txt; cat hola.txt)
echo "-- ls hola.txt && cat --"
ls -l hola.txt
cat -n hola.txt

echo
echo "==== SECCIÓN: COMILLADO y ESCAPES ===="
""                         # Vacío válido
/bin/echo ""
/bin/echo exportando; /usr/bin/env | /usr/bin/grep TEST
'/bin/echo hola'
/bin/echo hola\ mundo
echo uno\ dos\ tres
echo signo\? interrogacion
echo barra\|vertical
echo barra\\invertida
/bin/echo comilla\'simple
/bin/echo comilla\"doble
/bin/echo "comilla\"doble"
/bin/echo 'comilla\'simple'
/bin/echo expansion\$USER
"/bin/echo hola"
ls -z

echo
echo "==== SECCIÓN: PIPES y REDIRECCIONES COMBINADAS ===="
/bin/echo uno | tr a-z A-Z > upper.txt
echo "-- cat upper.txt --"
cat -n upper.txt

cat < upper.txt | grep UNO >> grep_result.txt
echo "-- cat grep_result.txt --"
cat -n grep_result.txt

(/bin/echo uno && /bin/echo dos) | grep uno > pipe_output.txt
echo "-- cat pipe_output.txt --"
cat -n pipe_output.txt

(/bin/echo uno; /bin/echo dos) && /bin/echo tres || /bin/echo cuatro
/bin/echo uno; /bin/echo dos | grep dos

echo
echo "==== SECCIÓN: HEREDOC BÁSICO ===="
cat << EOF
linea 1
linea 2
EOF

/bin/echo heredoc >> prueba.txt << EOF
esto
es
una
prueba
EOF
echo "-- ls y cat de prueba.txt --"
ls -l prueba.txt
cat -n prueba.txt

echo
echo "==== SECCIÓN: HEREDOC SIN EXPANSIÓN ===="
cat << 'EOF'
$HOME no debe expandirse
EOF

echo
echo "==== SECCIÓN: CÓDIGO DE SALIDA ($?) ===="
/bin/echo $?
false
/bin/echo "status false -> $?"
true
/bin/echo "status true -> $?"

echo
echo "==== SECCIÓN: BACKGROUND (&) ===="
/bin/echo fondo &
ls > out_bg.txt 2>&1 &
(/bin/echo uno > uno.txt) & /bin/echo dos >> dos.txt
sleep 1 &
/bin/echo terminado
(/bin/echo uno && sleep 1) &
sleep 2
echo "-- ls de uno.txt y dos.txt --"
ls -l uno.txt dos.txt
echo "-- cat uno.txt; cat dos.txt --"
cat -n uno.txt 2>/dev/null || true
cat -n dos.txt 2>/dev/null || true

echo
echo "==== SECCIÓN: PIPES EN BACKGROUND ===="
/bin/echo inicio && cat << EOF | grep hola > resultado.txt
hola mundo
chao
EOF
/bin/echo uno > uno.txt & /bin/echo dos >> dos.txt
/bin/echo uno | /bin/echo dos > file.txt &
sleep 1
echo "-- ls y cat de resultado/uno/dos/file --"
ls -l resultado.txt uno.txt dos.txt file.txt 2>/dev/null || true
cat -n resultado.txt 2>/dev/null || true
cat -n file.txt 2>/dev/null || true

echo
echo "==== SECCIÓN: CASOS DE EXPANSIÓN ADYACENTE ===="
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
echo 'foo'bar'

echo
echo "==== FIN DE LA BATERÍA ===="
CMDS

# -------- helpers de snapshot y run --------
snapshot_fs() {
  # $1 = workdir, $2 = label
  ( cd "$1" || exit 1
    find . -type f -printf "%P\n" | sort > "fs_list_${2}.txt"
    if command -v md5sum >/dev/null 2>&1; then
      find . -type f -print0 | sort -z | xargs -0 md5sum > "fs_hash_${2}.txt"
    else
      find . -type f -print0 | sort -z | xargs -0 sha1sum > "fs_hash_${2}.txt"
    fi
  )
}

run_suite() {
  local label="$1" bin="$2" workdir="$3"
  local outlog="${label}_output_results.txt"
  local status_file="exit_status_${label}.txt"

  echo "[*] Ejecutando en ${label} -> ${bin}"
  ( cd "$workdir" || exit 1
    cp ../"$TESTFILE" ./

    if [[ "$label" == "bash" ]]; then
      "$bin" --noprofile --norc < "$TESTFILE" > "$outlog" 2>&1
    else
      "$bin" < "$TESTFILE" > "$outlog" 2>&1
    fi

    echo $? > "$status_file"
    snapshot_fs "." "$label"
    echo "[*] ${label} terminó (exit=$(<"$status_file")). Log: $(pwd)/$outlog"
  )
}

# -------- Ejecutar en bash --------
run_suite "bash" "$BASH_BIN" "$WORK_BS"

# -------- Ejecutar en minishell (si existe y es ejecutable de verdad) --------
HAVE_MS=0
if [[ -n "${MINISHELL_BIN:-}" ]]; then
  if [[ -d "${MINISHELL_BIN}" ]]; then
    echo "[!] La ruta dada es un DIRECTORIO, no un ejecutable: ${MINISHELL_BIN}"
    echo "    Pásame el binario, p.ej.: ./run_minishell_tests.sh ./minishell"
  elif [[ -f "${MINISHELL_BIN}" && -x "${MINISHELL_BIN}" ]]; then
    HAVE_MS=1
    run_suite "minishell" "$MINISHELL_BIN" "$WORK_MS"
  else
    echo "[!] Ruta no ejecutable o no es archivo regular: ${MINISHELL_BIN}"
    echo "    Comprueba: existe (-f) y permisos (+x)."
  fi
else
  echo "[!] No encuentro ejecutable minishell en rutas comunes."
  echo "    Pásame la ruta: ./run_minishell_tests.sh /ruta/a/minishell"
fi

# -------- Diffs de salida --------
if [[ $HAVE_MS -eq 1 ]]; then
  diff -u "$WORK_BS/bash_output_results.txt" "$WORK_MS/minishell_output_results.txt" > diff_outputs.txt || true
fi

# -------- Comparación de filesystem --------
{
  echo "[FS] Comparación de archivos"
  if [[ $HAVE_MS -eq 1 ]]; then
    echo "== Listas =="
    echo "-- Solo en bash --"
    comm -23 <(cut -d' ' -f1 "$WORK_BS/fs_list_bash.txt" | sort) <(cut -d' ' -f1 "$WORK_MS/fs_list_minishell.txt" | sort) || true
    echo "-- Solo en minishell --"
    comm -13 <(cut -d' ' -f1 "$WORK_BS/fs_list_bash.txt" | sort) <(cut -d' ' -f1 "$WORK_MS/fs_list_minishell.txt" | sort) || true

    echo
    echo "== Contenido distinto (comunes) =="
    join -j1 <(awk '{print $1}' "$WORK_BS/fs_list_bash.txt" | sort) <(awk '{print $1}' "$WORK_MS/fs_list_minishell.txt" | sort) | while read -r f; do
      if [[ -f "$WORK_BS/$f" && -f "$WORK_MS/$f" ]]; then
        if command -v md5sum >/dev/null 2>&1; then
          HB=$(md5sum "$WORK_BS/$f" | awk '{print $1}')
          HM=$(md5sum "$WORK_MS/$f" | awk '{print $1}')
        else
          HB=$(sha1sum "$WORK_BS/$f" | awk '{print $1}')
          HM=$(sha1sum "$WORK_MS/$f" | awk '{print $1}')
        fi
        if [[ "$HB" != "$HM" ]]; then
          echo "----- DIFF ${f} -----"
          diff -u "$WORK_BS/$f" "$WORK_MS/$f" || true
        fi
      fi
    done
  else
    echo "(Saltado: no hay ejecución de minishell)"
  fi
} > diff_files.txt

# -------- Resumen final --------
echo
echo "============================================================"
printf "Exit bash:      %s\n" "$(cat "$WORK_BS/exit_status_bash.txt")"
if [[ $HAVE_MS -eq 1 ]]; then
  printf "Exit minishell: %s\n" "$(cat "$WORK_MS/exit_status_minishell.txt")"
  if ! diff -q "$WORK_BS/exit_status_bash.txt" "$WORK_MS/exit_status_minishell.txt" >/dev/null 2>&1; then
    echo "⚠️  Diferencia en exit status."
  else
    echo "✓ Exit status coinciden."
  fi
fi

echo "Logs:"
echo "  Bash:      $SUITE_ROOT/$WORK_BS/bash_output_results.txt"
if [[ $HAVE_MS -eq 1 ]]; then
  echo "  Minishell: $SUITE_ROOT/$WORK_MS/minishell_output_results.txt"
  echo "Diffs:"
  echo "  Salidas:   $SUITE_ROOT/diff_outputs.txt"
  echo "  Archivos:  $SUITE_ROOT/diff_files.txt"
else
  echo "  Minishell: (pendiente, no se ejecutó)"
  echo "  Pásame el binario: ./run_minishell_tests.sh /ruta/a/minishell"
fi
echo "Workdirs:"
echo "  $SUITE_ROOT/$WORK_BS"
echo "  $SUITE_ROOT/$WORK_MS"
echo "============================================================"
