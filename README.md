# tcpinfo

This tool is used for getting tcp connection infomation from system. Previously we do 'ss' or 'netstat' command to check. 'ss' by alexander is admired by us which take fully advantage of netlink of tcp_diag kernel module. 'netstat' is an old famous tool which read the proc interface /proc/net/tcp.

Neither proc file system nor netlink from tcp_diag can specify a check rule by which we can limit our data transfer quatity. 

Tcp_info is the tool for frequently tcp connection extraction. With which you can specify the four tuple (exclude interface) in any combination, which means ip:port ip:port no need to be transfered from kernel to user space but only the matches you specify. This is done in v3 with a kernel module.

V1 and V2 use the tcp_diag module the same, but this module is not mantained for quite a while. Using this module you also can specify the contents you want. The performance of v1 and v2 is the same as ss, but with more flexible. You can change the code to fullfill your desire. I will be glad you can push your changes.
