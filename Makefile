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

${WRKDIR}:
	${MKDIR} ${WRKDIR}

## djbdns
build-djbdns: ${WRKDIR}
	@${MAKE} clean-djbdns
	${CPR} ${SRCDIR}/${DJBDNS} ${WRKDIR}
	(cd ${WRKDIR}/${DJBDNS}; ${MAKE})

install-djbdns: build-djbdns
	(cd ${WRKDIR}/${DJBDNS}; ${MAKE} setup check)

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

clean:
	${RMRF} ${WRKDIR}

