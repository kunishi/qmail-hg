DAEMONTOOLS = daemontools-0.76
DJBDNS = djbdns-1.05
EZMLM = ezmlm-0.53
EZMLMIDX = ezmlm-idx-5.1.2
QMAIL = netqmail-1.06
QGREY = qgrey-0.1-0.3
QGREYLIST = qgreylist-0.3
QMAILCONF = qmail-conf-0.60
QTOOLS = qtools-0.56
UCSPITCP = ucspi-tcp-0.88

WRKDIR = work
SRCDIR = src

MKDIR = mkdir -p
CPR = cp -rp
RMRF = rm -rf

QMAILDIR = /var/qmail
PSEUDO_SHELL = /bin/true
ADMINPKGDIR = /package/admin

BUILD_DONE = .build_done

${WRKDIR}:
	${MKDIR} ${WRKDIR}

## daemontools
install-daemontools:
	${MKDIR} ${ADMINPKGDIR}
	${CPR} ${SRCDIR}/${DAEMONTOOLS} ${ADMINPKGDIR}
	(cd ${ADMINPKGDIR}/${DAEMONTOOLS}; package/install)

## djbdns
configure-djbdns:
	useradd -g dnscache -d ${DNSCACHEDIR} -s ${PSEUDO_SHELL} dnscache
	useradd -g dnslog -d ${DNSCACHEDIR} -s ${PSEUDO_SHELL} dnslog
	useradd -g tinydns -d ${TINYDNSDIR} -s ${PSEUDO_SHELL} tinydns
	useradd -g axfrdns -d ${AXFRDNSDIR} -s ${PSEUDO_SHELL} axfrdns
	useradd -g walldns -d ${WALLDNSDIR} -s ${PSEUDO_SHELL} walldns

build-djbdns: ${WRKDIR}
	@${MAKE} clean-djbdns
	${CPR} ${SRCDIR}/${DJBDNS} ${WRKDIR}
	(cd ${WRKDIR}/${DJBDNS}; ${MAKE})

${WRKDIR}/${DJBDNS}/${BUILD_DONE}: build-djbdns
	touch $@

install-djbdns: ${WRKDIR}/${DJBDNS}/${BUILD_DONE}
	@${MAKE} pre-install-djbdns
	(cd ${WRKDIR}/${DJBDNS}; ${MAKE} setup check)
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
	${RMRF} ${WRKDIR}/${DJBDNS}

## qmail
# this is for Linux
configure-qmail:
	${MKDIR} ${QMAILDIR}
	groupadd nofiles
	useradd -g nofiles -d ${QMAILDIR}/alias -s ${PSEUDO_SHELL} alias
	useradd -g nofiles -d ${QMAILDIR} -s ${PSEUDO_SHELL} qmaild
	useradd -g nofiles -d ${QMAILDIR} -s ${PSEUDO_SHELL} qmaill
	useradd -g nofiles -d ${QMAILDIR} -s ${PSEUDO_SHELL} qmailp
	groupadd qmail
	useradd -g qmail -d ${QMAILDIR} -s ${PSEUDO_SHELL} qmailq
	useradd -g qmail -d ${QMAILDIR} -s ${PSEUDO_SHELL} qmailr
	useradd -g qmail -d ${QMAILDIR} -s ${PSEUDO_SHELL} qmails

build-qmail: ${WRKDIR}
	@${MAKE} clean-qmail
	${CPR} ${SRCDIR}/${QMAIL} ${WRKDIR}
	(cd ${WRKDIR}/${QMAIL}; ${MAKE})

install-qmail: build-qmail
	(cd ${WRKDIR}/${QMAIL}; ${MAKE} setup check)

clean-qmail:
	${RMRF} ${WRKDIR}/${QMAIL}

## ucspi-tcp
build-ucspi-tcp: ${WRKDIR}
	@${MAKE} clean-ucspi-tcp
	${CPR} ${SRCDIR}/${UCSPITCP} ${WRKDIR}
	(cd ${WRKDIR}/${UCSPITCP}; ${MAKE})

install-ucspi-tcp:
	(cd ${WRKDIR}/${UCSPITCP}; ${MAKE} setup check)

clean-ucspi-tcp:
	${RMRF} ${WRKDIR}/${UCSPITCP}

## qmail-conf
build-qmail-conf: ${WRKDIR}
	@${MAKE} clean-qmail-conf
	${CPR} ${SRCDIR}/${QMAILCONF} ${WRKDIR}
	${CPR} ${SRCDIR}/${DJBDNS} ${WRKDIR}
	(cd ${WRKDIR}/${QMAILCONF}; \
	  ${MAKE} -f Makefile.ini djbdns=../${DJBDNS})
	(cd ${WRKDIR}/${QMAILCONF}; ${MAKE})

install-qmail-conf:
	(cd ${WRKDIR}/${QMAILCONF}; ${MAKE} setup check)

clean-qmail-conf:
	${RMRF} ${WRKDIR}/${QMAILCONF} ${WRKDIR}/${DJBDNS}

## ezmlm-idx
build-ezmlm-idx: ${WRKDIR}
	@${MAKE} clean-ezmlm-idx
	${CPR} ${SRCDIR}/${EZMLM} ${WRKDIR}
	${CPR} ${SRCDIR}/${EZMLMIDX}/* ${WRKDIR}/${EZMLM}
	(cd ${WRKDIR}/${EZMLM}; patch < idx.patch)
	(cd ${WRKDIR}/${EZMLM}; ${MAKE}; ${MAKE} man)

install-ezmlm-idx:
	(cd ${WRKDIR}/${EZMLM}; ${MAKE} setup)

clean-ezmlm-idx:
	${RMRF} ${WRKDIR}/${EZMLM}

## qgrey
build-qgrey: ${WRKDIR}
	@${MAKE} clean-qgrey
	${CPR} ${SRCDIR}/${QGREYLIST} ${WRKDIR}
	${CPR} ${SRCDIR}/${QGREY}/* ${WRKDIR}/${QGREYLIST}
	(cd ${WRKDIR}/${QGREYLIST}; patch < qgrey.patch)

install-qgrey:
	${MKDIR} ${QMAILDIR}/greylist ${QMAILDIR}/whitelist
	chown qmaild ${QMAILDIR}/greylist
	(cd ${WRKDIR}/${QGREYLIST}; \
	  cp greylist ${QMAILDIR}/bin; \
	  cp s25r ${QMAILDIR})

clean-qgrey:
	${RMRF} ${WRKDIR}/${QGREYLIST}

## qtools
build-qtools: ${WRKDIR}
	@${MAKE} clean-qtools
	${CPR} ${SRCDIR}/${QTOOLS} ${WRKDIR}
	(cd ${WRKDIR}/${QTOOLS}; ${MAKE} prog)

install-qtools:
	(cd ${WRKDIR}/${QTOOLS}; ${MAKE} setup check)

clean-qtools:
	${RMRF} ${WRKDIR}/${QTOOLS}

clean:
	${RMRF} ${WRKDIR}

