# $1=cmd $2=numslaves $3=slave_prefix $4=master
(( $# == 4 )) && mhost=$4 || mhost="localhost"
for (( i=0; i<=$2; i++ )); do
  case $i in
    0)
      host=$mhost
      ;;
    *)
      (( $i >= 10 )) && host="${3}${i}" || host="${3}0${i}"
      ;;
    esac
    echo "ssh ${host} ${1}"
	ssh $host $1
done
