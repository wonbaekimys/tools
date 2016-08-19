# usage: avg_dstat job_name data_size block_size [num_runs] [num_slaves]
avg_dstat=/home/wb/tools/examples/avg_dstat
# $avg_dstat sort 250gb 4mb 1 32
# $avg_dstat sort 250gb 8mb 1 32
# $avg_dstat sort 250gb 16mb 1 32
# $avg_dstat sort 250gb 32mb 1 32
# $avg_dstat sort 250gb 64mb 1 32
# $avg_dstat sort 250gb 128mb 1 32
# $avg_dstat sort 250gb 256mb 1 32
# $avg_dstat sort 250gb 512mb 1 32
# $avg_dstat sort 250gb 1gb 1 32
$avg_dstat grep 250gb 2gb 1 32
$avg_dstat wc 250gb 2gb 1 32
