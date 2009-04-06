#!/bin/sh

[ $# != 2 ] && echo "usage: $0 user name" && exit 1

user=$1
name=$2

useradd -c ${name} -d /home/${user} -m -k /etc/skel ${user}
echo ${user} | passwd --stdin ${user}

cd /var/qmail/users
echo ${user} >> include
/var/qmail/bin/qmail-pw2u < /etc/passwd > assign
/var/qmail/bin/qmail-newu
