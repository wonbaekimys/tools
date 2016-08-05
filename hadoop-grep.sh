# (export GREP_COLOR='2;34'; grep --color=always -ri -m1 --include '*.java' $1 $HADOOP_SRC) | (export GREP_COLOR='1;31'; grep --color=always '([^\/]*?)(?=\.java)')
(export GREP_COLOR='2;34'; grep --color=always -ri -m1 --include '*.java' $1 $HADOOP_SRC) | (export GREP_COLOR='1;31'; grep --color=always '[^\/]*\.java')
# | (export GREP_COLOR='5'; grep --color=always '(^|(?<=\\x1B\[\dm)|(?<=\\x1B\[\d\dm))(.*?)($|(?=\\x1B\[\d\dm)|(?=\\x1B\[\dm))')
