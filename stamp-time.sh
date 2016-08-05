# Time Stamped STDOUT

while read line ; do
  echo "$(date '+%F %H:%M:%S,%3N') ${line}"
done
