# /bin/bash
# name=format_all.sh

for file in $(find . -type f -name '*.c'); do
	c_formatter_42 "$file"
	# Eliminar archivos temporales
	rm -f "$file.bak"
done
echo "Formateo completado."
exit 0
