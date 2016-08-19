# $1=cmd $2=numslaves $3=slave_prefix $4=master
function ssh_cmd () {
  (( $# < 3 )) && echo "usage: ssh_cmd <command> <num_slaves> <host_prefix> [master_name]" && return 1
  local mhost="localhost"
  (( $# == 4 )) && mhost=$4
  local set host
  for (( i=0; i<=$2; i++ )); do
    case $i in
      0)
        host=$mhost
        ;;
      *)
        (( $i >= 10 )) && host="${3}${i}" || host="${3}0${i}"
        ;;
    esac
    echo "ssh ${host} '${1/'$host'/${host}}'"
    ssh $host ${1/'$host'/$host}
  done
}
function ssh_cmd_async () {
  (( $# < 3 )) && echo "usage: ssh_cmd <command> <num_slaves> <host_prefix> [master_name]" && return 1
  local mhost="localhost"
  (( $# == 4 )) && mhost=$4
  local set host
  for (( i=0; i<=$2; i++ )); do
    case $i in
      0)
        host=$mhost
        ;;
      *)
        (( $i >= 10 )) && host="${3}${i}" || host="${3}0${i}"
        ;;
    esac
    # ssh $host ${1/'$host'/$host} &
    ssh $host $(echo $1 | sed 's/\${host}\|\$host/'$host'/g') &
  done
}
if [[ ${BASH_SOURCE[0]} == $0 ]]; then
  ssh_cmd_async "$@"
  # ssh_cmd "$@"
  unset ssh_cmd
fi
