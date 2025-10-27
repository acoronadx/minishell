#!/usr/bin/env bash
# Extrae src/exec y compila minishell para todos los commits.
# Uso:
#   ./extract_all_commits.sh         # todos los commits de la rama actual (HEAD)
#   ./extract_all_commits.sh --all   # todos los commits del repositorio (git rev-list --all)
#   ./extract_all_commits.sh --limit N   # detener después de N commits
#   ./extract_all_commits.sh --since <commit>  # empezar desde <commit> (incluido)
#
set -uo pipefail

# defaults
mode="head"   # "head" o "all"
limit=0
since_commit=""

# parse args
while [ $# -gt 0 ]; do
  case "$1" in
    --all) mode="all"; shift;;
    --limit) shift; limit="$1"; shift;;
    --since) shift; since_commit="$1"; shift;;
    -h|--help)
      cat <<EOF
Usage: $0 [--all] [--limit N] [--since <commit>]
  --all        process git rev-list --all instead of current branch
  --limit N    process only first N commits
  --since X    start at commit X (inclusive). X must be a commit sha.
EOF
      exit 0;;
    *) echo "Unknown arg: $1" >&2; exit 1;;
  esac
done

repo_root="$(git rev-parse --show-toplevel 2>/dev/null || true)"
if [ -z "$repo_root" ]; then
  echo "ERROR: ejecuta este script dentro de la raíz del repo git." >&2
  exit 1
fi

out_dir="$repo_root/outputs_all_commits"
out_src_dir="$out_dir/exec_sources"
out_bin_dir="$out_dir/exe"
out_log_dir="$out_dir/logs"

mkdir -p "$out_src_dir" "$out_bin_dir" "$out_log_dir"

# build commit list
if [ "$mode" = "all" ]; then
  commits_cmd="git rev-list --reverse --all"
else
  commits_cmd="git rev-list --reverse HEAD"
fi

# if since_commit specified, cut list starting from that commit
commits_list="$(eval $commits_cmd)"
if [ -n "$since_commit" ]; then
  # keep only lines from since_commit onward
  commits_list="$(echo "$commits_list" | awk -v s="$since_commit" 'BEGIN{found=0} { if($0==s){found=1} if(found) print $0 }')"
fi

# iterate
count=0
echo "Procesando commits... salida en: $out_dir"
while read -r commit; do
  [ -z "$commit" ] && continue
  count=$((count+1))
  if [ "$limit" -gt 0 ] && [ "$count" -gt "$limit" ]; then
    break
  fi

  short="$(git rev-parse --short=7 "$commit" 2>/dev/null || echo "$commit")"
  echo "[$count] commit: $commit (short: $short)"

  out_file="$out_src_dir/$commit"
  bin_dest="$out_bin_dir/minishell-$short"
  log_file="$out_log_dir/$short.log"

  # skip if already processed (both source file and bin exist)
  if [ -f "$out_file" ] && [ -f "$bin_dest" ]; then
    echo "  ya procesado: fuentes y binario existen, salto."
    continue
  fi

  tmpdir="$(mktemp -d)"
  trap 'rm -rf "$tmpdir"' EXIT

  # 1) extraer solo src/exec y concatenar
  if git archive --format=tar "$commit" src/exec 2>/dev/null | tar -x -C "$tmpdir" 2>/dev/null; then
    src_dir="$tmpdir/src/exec"
    if [ -d "$src_dir" ]; then
      rm -f "$out_file"
      # concatenar en orden alfabético y añadir separadores de archivo
      (cd "$src_dir" && for f in $(ls -1); do
         echo "/* ---- file: $f ---- */" >> "$out_file"
         cat "$f" >> "$out_file"
         echo "" >> "$out_file"
       done)
      echo "  fuentes concatenadas -> $out_file"
    else
      echo "  no existe src/exec en $commit"
    fi
  else
    echo "  git archive src/exec falló (posible que no exista src/exec en este commit)."
  fi

  # 2) extraer todo para compilar
  if git archive --format=tar "$commit" 2>/dev/null | tar -x -C "$tmpdir" 2>/dev/null; then
    if [ -f "$tmpdir/Makefile" ] || [ -f "$tmpdir/makefile" ]; then
      echo "  Makefile encontrado: compilando (logs -> $log_file)..."
      # compilar y guardar stdout+stderr en log_file (no abortar todo si falla)
      (cd "$tmpdir" && make -j) > "$log_file" 2>&1 || echo "  compilación devolvió error (ver $log_file)"
      # buscar ejecutable
      bin_found="$(find "$tmpdir" -type f -name minishell -perm /111 2>/dev/null | head -n 1 || true)"
      if [ -z "$bin_found" ]; then
        bin_found="$(find "$tmpdir" -type f -name minishell 2>/dev/null | head -n 1 || true)"
      fi
      if [ -n "$bin_found" ]; then
        cp "$bin_found" "$bin_dest" 2>/dev/null || cp "$bin_found" "$bin_dest"
        chmod +x "$bin_dest" 2>/dev/null || true
        echo "  ejecutable copiado -> $bin_dest"
      else
        echo "  no se encontró minishell tras compilar (revisa $log_file)"
      fi
    else
      echo "  no hay Makefile en $commit: no se compila"
    fi
  else
    echo "  extraer snapshot entero falló para $commit"
  fi

  # cleanup tmpdir for this iteration
  rm -rf "$tmpdir"
  trap - EXIT

done <<EOF
$commits_list
EOF

echo "Hecho. Resultados:"
echo "  fuentes concat: $out_src_dir"
echo "  ejecutables:   $out_bin_dir"
echo "  logs:          $out_log_dir"