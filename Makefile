daemontools = daemontools-0.76
djbdns = djbdns-1.05
ezmlm = ezmlm-0.53
ezmlmidx = ezmlm-idx-7.0.2
mess822 = mess822-0.58
qmail = netqmail-1.06
qgrey = qgrey-0.1-0.3
qgreylist = qgreylist-0.3
qmailconf = qmail-conf-0.60
qtools = qtools-0.56
ucspitcp = ucspi-tcp-0.88

topdir = .
wrkdir = ${topdir}/work
srcdir = ${topdir}/src
docdir = ${topdir}/doc

mkdir = mkdir -p
cpr = cp -rp
rmrf = rm -rf

qmaildir = /var/qmail
pseudo_shell = /bin/true
adminpkgdir = /package/admin
dnscachedir = /var/dnscache
tinydnsdir = /var/tinydns
axfrdnsdir = /var/axfrdns
walldnsdir = /var/walldns

build_done = .build_done

${wrkdir}:
	${mkdir} ${wrkdir}

clean:
	${rmrf} ${wrkdir}

## daemontools
install-daemontools:
	${mkdir} ${adminpkgdir}
	${cpr} ${srcdir}/${daemontools} ${adminpkgdir}
	(cd ${adminpkgdir}/${daemontools}; package/install)

## djbdns
configure-djbdns:
	useradd -g dnscache -d ${dnscachedir} -s ${pseudo_shell} dnscache
	useradd -g dnslog -d ${dnscachedir} -s ${pseudo_shell} dnslog
	useradd -g tinydns -d ${tinydnsdir} -s ${pseudo_shell} tinydns
	useradd -g axfrdns -d ${axfrdnsdir} -s ${pseudo_shell} axfrdns
	useradd -g walldns -d ${walldnsdir} -s ${pseudo_shell} walldns

build-djbdns: ${wrkdir}
	@${MAKE} clean-djbdns
	${cpr} ${srcdir}/${djbdns} ${wrkdir}
	(cd ${wrkdir}/${djbdns}; ${MAKE})

${wrkdir}/${djbdns}/${build_done}: build-djbdns
	touch $@

install-djbdns: ${wrkdir}/${djbdns}/${build_done}
	@${MAKE} pre-install-djbdns
	(cd ${wrkdir}/${djbdns}; ${MAKE} setup check)
	@${MAKE} post-install-djbdns

pre-install-djbdns:
	[ -L /service/dnscache ] && \
	  svc -d /service/dnscache /service/dnscache/log
	[ -L /service/tinydns ] && svc -d /service/tinydns /service/tinydns/log

post-install-djbdns:
	[ -L /service/dnscache ] && \
	  svc -u /service/dnscache /service/dnscache/log
	[ -L /service/tinydns ] && svc -u /service/tinydns /service/tinydns/log

clean-djbdns:
	${rmrf} ${wrkdir}/${djbdns}

## qmail
# this is for Linux
configure-qmail:
	${mkdir} ${qmaildir}
	groupadd nofiles
	useradd -g nofiles -d ${qmaildir}/alias -s ${pseudo_shell} alias
	useradd -g nofiles -d ${qmaildir} -s ${pseudo_shell} qmaild
	useradd -g nofiles -d ${qmaildir} -s ${pseudo_shell} qmaill
	useradd -g nofiles -d ${qmaildir} -s ${pseudo_shell} qmailp
	groupadd qmail
	useradd -g qmail -d ${qmaildir} -s ${pseudo_shell} qmailq
	useradd -g qmail -d ${qmaildir} -s ${pseudo_shell} qmailr
	useradd -g qmail -d ${qmaildir} -s ${pseudo_shell} qmails

build-qmail: ${wrkdir}
	@${MAKE} clean-qmail
	${cpr} ${srcdir}/${qmail} ${wrkdir}
	${cpr} ${docdir}/qmail-badremotehost.patch ${srcdir}/${qmail}
	(cd ${wrkdir}/${qmail}; patch < qmail-badremotehost.patch)
	(cd ${wrkdir}/${qmail}; ${MAKE})

${wrkdir}/${qmail}/${build_done}: build-qmail
	touch $@

install-qmail: ${wrkdir}/${qmail}/${build_done}
	@${MAKE} pre-install-qmail
	(cd ${wrkdir}/${qmail}; ${MAKE} setup check)
	@${MAKE} post-install-qmail

pre-install-qmail:
	[ -L /service/qmail ] && svc -d /service/qmail /service/qmail/log
	[ -L /service/smtpd ] && svc -d /service/smtpd /service/smtpd/log

post-install-qmail:
	[ -L /service/qmail ] && svc -u /service/qmail /service/qmail/log
	[ -L /service/smtpd ] && svc -u /service/smtpd /service/smtpd/log

clean-qmail:
	${rmrf} ${wrkdir}/${qmail}

## ucspi-tcp
build-ucspi-tcp: ${wrkdir}
	@${MAKE} clean-ucspi-tcp
	${cpr} ${srcdir}/${ucspitcp} ${wrkdir}
	(cd ${wrkdir}/${ucspitcp}; ${MAKE})

${wrkdir}/${ucspitcp}/${build_done}: build-ucspi-tcp
	touch $@

install-ucspi-tcp: ${wrkdir}/${ucspitcp}/${build_done}
	(cd ${wrkdir}/${ucspitcp}; ${MAKE} setup check)

clean-ucspi-tcp:
	${rmrf} ${wrkdir}/${ucspitcp}

## qmail-conf
build-qmail-conf: ${wrkdir}
	@${MAKE} clean-qmail-conf
	${cpr} ${srcdir}/${qmailconf} ${wrkdir}
	${cpr} ${srcdir}/${djbdns} ${wrkdir}
	(cd ${wrkdir}/${qmailconf}; \
	  ${MAKE} -f Makefile.ini djbdns=../${djbdns})
	(cd ${wrkdir}/${qmailconf}; ${MAKE})

${wrkdir}/${qmailconf}/${build_done}: build-qmail-conf
	touch ${wrkdir}/${qmailconf}/${build_done}

install-qmail-conf: ${wrkdir}/${qmailconf}/${build_done}
	(cd ${wrkdir}/${qmailconf}; ${MAKE} setup check)

clean-qmail-conf:
	${rmrf} ${wrkdir}/${qmailconf} ${wrkdir}/${djbdns}

## ezmlm-idx
build-ezmlm-idx: ${wrkdir}
	@${MAKE} clean-ezmlm-idx
	${cpr} ${srcdir}/${ezmlm} ${wrkdir}
	${cpr} ${srcdir}/${ezmlmidx}/* ${wrkdir}/${ezmlm}
	# (cd ${wrkdir}/${ezmlm}; patch < idx.patch)
	(cd ${wrkdir}/${ezmlm}; ${MAKE}; ${MAKE} man)

${wrkdir}/${ezmlm}/${build_done}: build-ezmlm-idx
	touch $@

install-ezmlm-idx: ${wrkdir}/${ezmlm}/${build_done}
	(cd ${wrkdir}/${ezmlm}; ${MAKE} setup)

clean-ezmlm-idx:
	${rmrf} ${wrkdir}/${ezmlm}

## qgrey
build-qgrey: ${wrkdir}
	@${MAKE} clean-qgrey
	${cpr} ${srcdir}/${qgreylist} ${wrkdir}
	${cpr} ${srcdir}/${qgrey}/* ${wrkdir}/${qgreylist}
	(cd ${wrkdir}/${qgreylist}; patch < qgrey.patch)

${wrkdir}/${qgrey}/${build_done}: build-qgrey
	touch $@

install-qgrey: ${wrkdir}/${qgrey}/${build_done}
	${mkdir} ${qmaildir}/greylist ${qmaildir}/whitelist
	chown qmaild ${qmaildir}/greylist
	(cd ${wrkdir}/${qgreylist}; \
	  cp greylist ${qmaildir}/bin; \
	  cp s25r ${qmaildir})

clean-qgrey:
	${rmrf} ${wrkdir}/${qgreylist}

## qtools
build-qtools: ${wrkdir}
	@${MAKE} clean-qtools
	${cpr} ${srcdir}/${qtools} ${wrkdir}
	(cd ${wrkdir}/${qtools}; ${MAKE} prog)

${wrkdir}/${qtools}/${build_done}: build-qtools
	touch $@

install-qtools: ${wrkdir}/${qtools}/${build_done}
	(cd ${wrkdir}/${qtools}; ${MAKE} setup check)

clean-qtools:
	${rmrf} ${wrkdir}/${qtools}

## mess822
build-mess822: ${wrkdir}
	@${MAKE} clean-mess822
	${cpr} ${srcdir}/${mess822} ${wrkdir}
	(cd ${wrkdir}/${mess822}; ${MAKE})

${wrkdir}/${mess822}/${build_done}: build-mess822
	touch $@

install-mess822: ${wrkdir}/${mess822}/${build_done}
	(cd ${wrkdir}/${mess822}; ${MAKE} setup check)

clean-mess822:
	${rmrf} ${wrkdir}/${mess822}
