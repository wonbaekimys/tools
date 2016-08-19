host="ravenleader"
str='${host}'" aaa"
echo $str
# echo $(echo $str | sed 's/\$\{?host\}?/'"${host}")
echo $(echo $str | sed 's/\${host}\|\$host/'$host'/g')
