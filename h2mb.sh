function h2mb() {
  lh=$1
  if [[ $lh =~ [0-9]+[mM][bB] ]]; then
    lh=${lh/[mM][bB]/}
  elif [[ $lh =~ [0-9]+[gG][bB] ]]; then
    lh=${lh/[gG][bB]/}
    ((lh*=1024))
  fi
  echo $lh
}
if [[ ${BASH_SOURCE[0]} == $0 ]]; then
  h2mb $1
  unset h2mb
fi
