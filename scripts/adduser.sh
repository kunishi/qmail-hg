#!/bin/sh

[ $# != 2 ] && echo "usage: $0 user name" && exit 1

user=$1
name=$2

useradd -c ${name} -d /home/${user} -m -k /etc/skel ${user}
echo ${user} | passwd --stdin ${user}

echo ${user} >> /var/qmail/users/include
/var/qmail/bin/qmail-pw2u < /etc/passwd > /var/qmail/users/assign
/var/qmail/bin/qmail-newu
