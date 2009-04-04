# Don't edit Makefile!  Use conf-* for configuration.

SHELL=/bin/sh

default: it


822headerok: load 822headerok.o getln.a buffer.a str.a stralloc.a alloc.a \
error.a mess822.a
	./load 822headerok getln.a buffer.a str.a stralloc.a alloc.a error.a \
	mess822.a 

load: warn-auto.sh conf-ld
	( cat warn-auto.sh; \
	echo 'main="$$1"; shift'; \
	echo exec "`head -1 conf-ld`" \
	'-o "$$main" "$$main".o $${1+"$$@"}' \
	) > load
	chmod 755 load

822body: load 822body.o strerr.a error.a buffer.a getln.a stralloc.a \
alloc.a str.a mess822.a
	./load 822body strerr.a error.a buffer.a getln.a stralloc.a alloc.a \
	str.a mess822.a 

it: prog install instcheck

compile: warn-auto.sh conf-cc
	( cat warn-auto.sh; \
	echo exec "`head -1 conf-cc`" '-c $${1+"$$@"}' \
	) > compile
	chmod 755 compile

822body.o: compile \
822body.c \
buffer.h strerr.h error.h fd.h getln.h stralloc.h gen_alloc.h mess822.h \
caltime.h caldate.h exit.h
	./compile 822body.c

makelib: warn-auto.sh systype
	( cat warn-auto.sh; \
	echo 'main="$$1"; shift'; \
	echo 'rm -f "$$main"'; \
	echo 'ar cr "$$main" $${1+"$$@"}'; \
	case "`cat systype`" in \
	sunos-5.*) ;; \
	unix_sv*) ;; \
	irix64-*) ;; \
	irix-*) ;; \
	dgux-*) ;; \
	hp-ux-*) ;; \
	sco*) ;; \
	*) echo 'ranlib "$$main"' ;; \
	esac \
	) > makelib
	chmod 755 makelib

systype: find-systype.sh conf-cc conf-ld trycpp.c x86cpuid.c
	( cat warn-auto.sh; \
	echo CC=\'`head -1 conf-cc`\'; \
	echo LD=\'`head -1 conf-ld`\'; \
	cat find-systype.sh; \
	) | sh > systype

strerr.o: compile \
strerr.c \
stralloc.h gen_alloc.h strerr.h
	./compile strerr.c

strerr.a: makelib strerr.o strerr_sys.o strerr_die.o
	./makelib strerr.a strerr.o strerr_sys.o strerr_die.o

strerr_sys.o: compile \
strerr_sys.c \
error.h strerr.h
	./compile strerr_sys.c

strerr_die.o: compile \
strerr_die.c \
buffer.h exit.h strerr.h
	./compile strerr_die.c

error.o: compile \
error.c \
error.h
	./compile error.c

error_str.o: compile \
error_str.c \
error.h
	./compile error_str.c

error.a: makelib error.o error_str.o error_temp.o
	./makelib error.a error.o error_str.o error_temp.o

error_temp.o: compile \
error_temp.c \
error.h
	./compile error_temp.c

buffer.o: compile \
buffer.c \
buffer.h
	./compile buffer.c

buffer.a: makelib buffer.o buffer_get.o buffer_put.o buffer_copy.o \
buffer_2.o buffer_1.o buffer_0.o
	./makelib buffer.a buffer.o buffer_get.o buffer_put.o buffer_copy.o \
	buffer_2.o buffer_1.o buffer_0.o

buffer_get.o: compile \
buffer_get.c \
buffer.h byte.h error.h
	./compile buffer_get.c

buffer_put.o: compile \
buffer_put.c \
buffer.h str.h byte.h error.h
	./compile buffer_put.c

buffer_copy.o: compile \
buffer_copy.c \
buffer.h
	./compile buffer_copy.c

buffer_2.o: compile \
buffer_2.c \
readwrite.h buffer.h
	./compile buffer_2.c

buffer_1.o: compile \
buffer_1.c \
readwrite.h buffer.h
	./compile buffer_1.c

buffer_0.o: compile \
buffer_0.c \
readwrite.h buffer.h
	./compile buffer_0.c

getln.o: compile \
getln.c \
byte.h getln.h buffer.h stralloc.h gen_alloc.h
	./compile getln.c

getln.a: makelib getln.o getln2.o
	./makelib getln.a getln.o getln2.o

getln2.o: compile \
getln2.c \
byte.h getln.h buffer.h stralloc.h gen_alloc.h
	./compile getln2.c

stralloc.a: makelib stralloc_eady.o stralloc_pend.o stralloc_copy.o \
stralloc_opys.o stralloc_opyb.o stralloc_cat.o stralloc_cats.o \
stralloc_catb.o stralloc_arts.o
	./makelib stralloc.a stralloc_eady.o stralloc_pend.o stralloc_copy.o \
	stralloc_opys.o stralloc_opyb.o stralloc_cat.o stralloc_cats.o \
	stralloc_catb.o stralloc_arts.o

stralloc_eady.o: compile \
stralloc_eady.c \
alloc.h stralloc.h gen_alloc.h gen_allocdefs.h
	./compile stralloc_eady.c

stralloc_pend.o: compile \
stralloc_pend.c \
alloc.h stralloc.h gen_alloc.h gen_allocdefs.h
	./compile stralloc_pend.c

stralloc_copy.o: compile \
stralloc_copy.c \
byte.h stralloc.h gen_alloc.h
	./compile stralloc_copy.c

stralloc_opys.o: compile \
stralloc_opys.c \
byte.h str.h stralloc.h gen_alloc.h
	./compile stralloc_opys.c

stralloc_opyb.o: compile \
stralloc_opyb.c \
stralloc.h gen_alloc.h byte.h
	./compile stralloc_opyb.c

stralloc_cat.o: compile \
stralloc_cat.c \
byte.h stralloc.h gen_alloc.h
	./compile stralloc_cat.c

stralloc_cats.o: compile \
stralloc_cats.c \
byte.h str.h stralloc.h gen_alloc.h
	./compile stralloc_cats.c

stralloc_catb.o: compile \
stralloc_catb.c \
stralloc.h gen_alloc.h byte.h
	./compile stralloc_catb.c

stralloc_arts.o: compile \
stralloc_arts.c \
byte.h str.h stralloc.h gen_alloc.h
	./compile stralloc_arts.c

alloc.o: compile \
alloc.c \
alloc.h error.h
	./compile alloc.c

alloc.a: makelib alloc.o alloc_re.o
	./makelib alloc.a alloc.o alloc_re.o

alloc_re.o: compile \
alloc_re.c \
alloc.h byte.h
	./compile alloc_re.c

str.a: makelib str_chr.o str_rchr.o str_cpy.o str_len.o str_diff.o \
str_diffn.o str_start.o byte_chr.o byte_diff.o byte_rchr.o byte_copy.o \
byte_cr.o
	./makelib str.a str_chr.o str_rchr.o str_cpy.o str_len.o str_diff.o \
	str_diffn.o str_start.o byte_chr.o byte_diff.o byte_rchr.o byte_copy.o \
	byte_cr.o

str_chr.o: compile \
str_chr.c \
str.h
	./compile str_chr.c

str_rchr.o: compile \
str_rchr.c \
str.h
	./compile str_rchr.c

str_cpy.o: compile \
str_cpy.c \
str.h
	./compile str_cpy.c

mess822_line.o: compile \
mess822_line.c \
mess822.h stralloc.h gen_alloc.h caltime.h caldate.h
	./compile mess822_line.c

str_len.o: compile \
str_len.c \
str.h
	./compile str_len.c

str_diff.o: compile \
str_diff.c \
str.h
	./compile str_diff.c

str_diffn.o: compile \
str_diffn.c \
str.h
	./compile str_diffn.c

str_start.o: compile \
str_start.c \
str.h
	./compile str_start.c

byte_chr.o: compile \
byte_chr.c \
byte.h
	./compile byte_chr.c

byte_diff.o: compile \
byte_diff.c \
byte.h
	./compile byte_diff.c

byte_rchr.o: compile \
byte_rchr.c \
byte.h
	./compile byte_rchr.c

byte_copy.o: compile \
byte_copy.c \
byte.h
	./compile byte_copy.c

byte_cr.o: compile \
byte_cr.c \
byte.h
	./compile byte_cr.c

mess822.a: makelib mess822_addr.o mess822_ok.o mess822_line.o \
mess822_token.o mess822_when.o
	./makelib mess822.a mess822_addr.o mess822_ok.o mess822_line.o \
	mess822_token.o mess822_when.o

mess822_addr.o: compile \
mess822_addr.c \
mess822.h stralloc.h gen_alloc.h caltime.h caldate.h str.h
	./compile mess822_addr.c

mess822_ok.o: compile \
mess822_ok.c \
mess822.h stralloc.h gen_alloc.h caltime.h caldate.h byte.h
	./compile mess822_ok.c

mess822_token.o: compile \
mess822_token.c \
mess822.h stralloc.h gen_alloc.h caltime.h caldate.h
	./compile mess822_token.c

mess822_when.o: compile \
mess822_when.c \
mess822.h stralloc.h gen_alloc.h caltime.h caldate.h scan.h
	./compile mess822_when.c

822headerok.o: compile \
822headerok.c \
buffer.h error.h getln.h stralloc.h gen_alloc.h mess822.h caltime.h \
caldate.h exit.h
	./compile 822headerok.c

822bodyfilter.o: compile \
822bodyfilter.c \
readwrite.h exit.h error.h strerr.h buffer.h getln.h stralloc.h \
gen_alloc.h mess822.h caltime.h caldate.h fd.h seek.h pathexec.h
	./compile 822bodyfilter.c

pathexec.a: makelib pathexec_run.o pathexec_env.o
	./makelib pathexec.a pathexec_run.o pathexec_env.o

pathexec_run.o: compile \
pathexec_run.c \
error.h stralloc.h gen_alloc.h str.h env.h pathexec.h
	./compile pathexec_run.c

pathexec_env.o: compile \
pathexec_env.c \
stralloc.h gen_alloc.h alloc.h str.h byte.h env.h pathexec.h
	./compile pathexec_env.c

env.a: makelib env.o
	./makelib env.a env.o

env.o: compile \
env.c \
str.h env.h
	./compile env.c

fd.a: makelib fd_copy.o fd_move.o
	./makelib fd.a fd_copy.o fd_move.o

seek.a: makelib seek_set.o seek_cur.o
	./makelib seek.a seek_set.o seek_cur.o

fd_copy.o: compile \
fd_copy.c \
fd.h
	./compile fd_copy.c

fd_move.o: compile \
fd_move.c \
fd.h
	./compile fd_move.c

seek_set.o: compile \
seek_set.c \
seek.h
	./compile seek_set.c

seek_cur.o: compile \
seek_cur.c \
seek.h
	./compile seek_cur.c

822bodyfilter: load 822bodyfilter.o strerr.a getln.a buffer.a pathexec.a \
env.a stralloc.a alloc.a str.a mess822.a fd.a error.a seek.a
	./load 822bodyfilter strerr.a getln.a buffer.a pathexec.a env.a \
	stralloc.a alloc.a str.a mess822.a fd.a error.a seek.a 

choose: warn-auto.sh choose.sh conf-home
	cat warn-auto.sh choose.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> choose
	chmod 755 choose

fork.h: choose compile load tryvfork.c fork.h1 fork.h2
	./choose cl tryvfork fork.h1 fork.h2 >fork.h

822headerfilter.o: compile \
822headerfilter.c \
readwrite.h exit.h error.h fork.h wait.h strerr.h buffer.h getln.h \
stralloc.h gen_alloc.h mess822.h caltime.h caldate.h fd.h pathexec.h
	./compile 822headerfilter.c

wait.a: makelib wait_pid.o
	./makelib wait.a wait_pid.o

wait_pid.o: compile \
wait_pid.c \
error.h haswaitp.h
	./compile wait_pid.c

haswaitp.h: choose trywaitp.c haswaitp.h1 haswaitp.h2 compile load
	./choose cl trywaitp haswaitp.h1 haswaitp.h2 >haswaitp.h

822headerfilter: load 822headerfilter.o wait.a strerr.a buffer.a \
pathexec.a env.a getln.a stralloc.a alloc.a str.a mess822.a fd.a error.a
	./load 822headerfilter wait.a strerr.a buffer.a pathexec.a env.a getln.a \
	stralloc.a alloc.a str.a mess822.a fd.a error.a 

822addr: load 822addr.o strerr.a error.a mess822.a fs.a getln.a \
stralloc.a buffer.a alloc.a str.a stralloc.a case.a
	./load 822addr strerr.a error.a mess822.a fs.a getln.a stralloc.a \
	buffer.a alloc.a str.a stralloc.a case.a 

822addr.o: compile \
822addr.c \
buffer.h strerr.h getln.h stralloc.h gen_alloc.h mess822.h caltime.h \
caldate.h case.h exit.h
	./compile 822addr.c

fmt_ulong.o: compile \
fmt_ulong.c \
fmt.h
	./compile fmt_ulong.c

fmt_str.o: compile \
fmt_str.c \
fmt.h
	./compile fmt_str.c

fmt_strn.o: compile \
fmt_strn.c \
fmt.h
	./compile fmt_strn.c

scan_long.o: compile \
scan_long.c \
scan.h
	./compile scan_long.c

scan_sign.o: compile \
scan_sign.c \
scan.h
	./compile scan_sign.c

scan_uint.o: compile \
scan_uint.c \
scan.h
	./compile scan_uint.c

fs.a: makelib fmt_ulong.o fmt_str.o fmt_strn.o scan_long.o scan_sign.o \
scan_uint.o scan_ulong.o
	./makelib fs.a fmt_ulong.o fmt_str.o fmt_strn.o scan_long.o scan_sign.o \
	scan_uint.o scan_ulong.o

scan_ulong.o: compile \
scan_ulong.c \
scan.h
	./compile scan_ulong.c

case.a: makelib case_diffb.o case_diffs.o case_startb.o
	./makelib case.a case_diffb.o case_diffs.o case_startb.o

auto_qmail.o: compile \
auto_qmail.c
	./compile auto_qmail.c

case_diffb.o: compile \
case_diffb.c \
case.h
	./compile case_diffb.c

case_diffs.o: compile \
case_diffs.c \
case.h
	./compile case_diffs.c

case_startb.o: compile \
case_startb.c \
case.h
	./compile case_startb.c

822headers: load 822headers.o strerr.a error.a mess822.a fs.a getln.a \
stralloc.a buffer.a alloc.a str.a stralloc.a case.a
	./load 822headers strerr.a error.a mess822.a fs.a getln.a stralloc.a \
	buffer.a alloc.a str.a stralloc.a case.a 

822headers.o: compile \
822headers.c \
buffer.h strerr.h getln.h stralloc.h gen_alloc.h mess822.h caltime.h \
caldate.h case.h exit.h
	./compile 822headers.c

822fields: load 822fields.o strerr.a error.a mess822.a fs.a getln.a \
stralloc.a buffer.a alloc.a str.a stralloc.a case.a
	./load 822fields strerr.a error.a mess822.a fs.a getln.a stralloc.a \
	buffer.a alloc.a str.a stralloc.a case.a 

822fields.o: compile \
822fields.c \
buffer.h strerr.h getln.h stralloc.h gen_alloc.h mess822.h caltime.h \
caldate.h case.h exit.h
	./compile 822fields.c

checkaddr: load checkaddr.o mess822.a getln.a strerr.a buffer.a \
stralloc.a alloc.a error.a env.a str.a case.a fs.a
	./load checkaddr mess822.a getln.a strerr.a buffer.a stralloc.a alloc.a \
	error.a env.a str.a case.a fs.a 

checkaddr.o: compile \
checkaddr.c \
buffer.h strerr.h getln.h stralloc.h gen_alloc.h mess822.h caltime.h \
caldate.h case.h env.h exit.h
	./compile checkaddr.c

checkdomain: load checkdomain.o mess822.a getln.a strerr.a buffer.a \
stralloc.a alloc.a error.a env.a str.a case.a fs.a
	./load checkdomain mess822.a getln.a strerr.a buffer.a stralloc.a \
	alloc.a error.a env.a str.a case.a fs.a 

auto-str: load auto-str.o buffer.a error.a str.a
	./load auto-str buffer.a error.a str.a 

checkdomain.o: compile \
checkdomain.c \
buffer.h strerr.h getln.h stralloc.h gen_alloc.h mess822.h caltime.h \
caldate.h case.h env.h exit.h
	./compile checkdomain.c

auto_qmail.c: auto-str conf-qmail
	./auto-str auto_qmail `head -1 conf-qmail` > auto_qmail.c

filterto.o: compile \
filterto.c \
sig.h readwrite.h exit.h env.h error.h fork.h wait.h qmail.h buffer.h \
strerr.h fmt.h getln.h stralloc.h gen_alloc.h mess822.h caltime.h \
caldate.h fd.h pathexec.h
	./compile filterto.c

filterto: load filterto.o auto_qmail.o qmail.o strerr.a fd.a sig.a wait.a \
env.a pathexec.a getln.a mess822.a buffer.a error.a str.a fs.a stralloc.a \
alloc.a
	./load filterto auto_qmail.o qmail.o strerr.a fd.a sig.a wait.a env.a \
	pathexec.a getln.a mess822.a buffer.a error.a str.a fs.a stralloc.a \
	alloc.a 

auto-str.o: compile \
auto-str.c \
buffer.h readwrite.h exit.h
	./compile auto-str.c

qmail.o: compile \
qmail.c \
buffer.h readwrite.h wait.h exit.h fork.h fd.h qmail.h auto_qmail.h \
pathexec.h
	./compile qmail.c

sig.o: compile \
sig.c \
sig.h
	./compile sig.c

sig.a: makelib sig.o sig_catch.o
	./makelib sig.a sig.o sig_catch.o

sig_catch.o: compile \
sig_catch.c \
sig.h hassgact.h
	./compile sig_catch.c

hassgact.h: choose compile trysgact.c hassgact.h1 hassgact.h2
	./choose cl trysgact hassgact.h1 hassgact.h2 > hassgact.h

tomaildir: load tomaildir.o sig.a open.a strerr.a buffer.a env.a str.a \
error.a fs.a now.o
	./load tomaildir sig.a open.a strerr.a buffer.a env.a str.a error.a fs.a \
	now.o 

tomaildir.o: compile \
tomaildir.c \
readwrite.h sig.h byte.h exit.h open.h buffer.h strerr.h error.h fmt.h \
str.h now.h datetime.h env.h
	./compile tomaildir.c

open_read.o: compile \
open_read.c \
open.h
	./compile open_read.c

open.a: makelib open_read.o open_trunc.o open_excl.o
	./makelib open.a open_read.o open_trunc.o open_excl.o

open_trunc.o: compile \
open_trunc.c \
open.h
	./compile open_trunc.c

open_excl.o: compile \
open_excl.c \
open.h
	./compile open_excl.c

now.o: compile \
now.c \
datetime.h now.h
	./compile now.c

condtomaildir: load condtomaildir.o sig.a open.a strerr.a buffer.a env.a \
pathexec.a stralloc.a alloc.a str.a error.a fs.a seek.a wait.a now.o
	./load condtomaildir sig.a open.a strerr.a buffer.a env.a pathexec.a \
	stralloc.a alloc.a str.a error.a fs.a seek.a wait.a now.o 

condtomaildir.o: compile \
condtomaildir.c \
sig.h readwrite.h byte.h exit.h open.h buffer.h strerr.h error.h fmt.h \
env.h str.h fork.h wait.h seek.h now.h datetime.h pathexec.h
	./compile condtomaildir.c

iftoccfrom: load iftoccfrom.o mess822.a getln.a strerr.a buffer.a \
stralloc.a alloc.a error.a env.a str.a case.a fs.a open.a
	./load iftoccfrom mess822.a getln.a strerr.a buffer.a stralloc.a alloc.a \
	error.a env.a str.a case.a fs.a open.a 

iftoccfrom.o: compile \
iftoccfrom.c \
buffer.h strerr.h getln.h stralloc.h gen_alloc.h mess822.h caltime.h \
caldate.h case.h env.h exit.h
	./compile iftoccfrom.c

ifaddr: load ifaddr.o mess822.a getln.a strerr.a buffer.a stralloc.a \
alloc.a error.a env.a str.a fs.a case.a
	./load ifaddr mess822.a getln.a strerr.a buffer.a stralloc.a alloc.a \
	error.a env.a str.a fs.a case.a 

getconf.o: compile \
getconf.c \
stralloc.h gen_alloc.h openreadclose.h strerr.h getconf.h
	./compile getconf.c

ifaddr.o: compile \
ifaddr.c \
buffer.h strerr.h getln.h stralloc.h gen_alloc.h mess822.h caltime.h \
caldate.h case.h env.h exit.h
	./compile ifaddr.c

replier.o: compile \
replier.c \
stralloc.h gen_alloc.h strerr.h error.h qmail.h buffer.h env.h sig.h \
open.h getln.h case.h scan.h str.h fmt.h readwrite.h exit.h getconf.h \
constmap.h fd.h wait.h pathexec.h
	./compile replier.c

replier: load replier.o qmail.o stralloc.a alloc.a strerr.a buffer.a \
env.a pathexec.a str.a error.a sig.a openreadclose.o readclose.o open.a \
getln.a case.a fs.a fd.a wait.a getconf.o constmap.o auto_qmail.o
	./load replier qmail.o stralloc.a alloc.a strerr.a buffer.a env.a \
	pathexec.a str.a error.a sig.a openreadclose.o readclose.o open.a \
	getln.a case.a fs.a fd.a wait.a getconf.o constmap.o auto_qmail.o 

rts: warn-auto.sh rts.sh conf-home
	cat warn-auto.sh rts.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> rts
	chmod 755 rts

openreadclose.o: compile \
openreadclose.c \
error.h open.h readclose.h stralloc.h gen_alloc.h openreadclose.h
	./compile openreadclose.c

readclose.o: compile \
readclose.c \
readwrite.h error.h readclose.h stralloc.h gen_alloc.h
	./compile readclose.c

constmap.o: compile \
constmap.c \
constmap.h alloc.h case.h
	./compile constmap.c

prog: 822body 822headerok 822bodyfilter 822headerfilter 822addr \
822headers 822fields checkaddr checkdomain filterto tomaildir \
condtomaildir iftoccfrom ifaddr replier replier-config

replier-config: load replier-config.o strerr.a buffer.a stralloc.a str.a \
error.a alloc.a open.a auto_ezmlm.o auto_home.o auto_qmail.o
	./load replier-config strerr.a buffer.a stralloc.a str.a error.a alloc.a \
	open.a auto_ezmlm.o auto_home.o auto_qmail.o 

instcheck: load instcheck.o hier.o auto_home.o strerr.a buffer.a error.a \
str.a
	./load instcheck hier.o auto_home.o strerr.a buffer.a error.a str.a 

make-text: load make-text.o getln.a buffer.a stralloc.a alloc.a error.a \
str.a
	./load make-text getln.a buffer.a stralloc.a alloc.a error.a str.a 

make-text.o: compile \
make-text.c \
buffer.h stralloc.h gen_alloc.h getln.h str.h exit.h
	./compile make-text.c

help.t: make-text help.txt
	./make-text <help.txt >help.t

replier-config.o: compile \
replier-config.c \
strerr.h buffer.h stralloc.h gen_alloc.h open.h readwrite.h exit.h \
auto_ezmlm.h auto_home.h auto_qmail.h str.h help.t
	./compile replier-config.c

auto_ezmlm.o: compile \
auto_ezmlm.c
	./compile auto_ezmlm.c

auto_ezmlm.c: auto-str conf-ezmlm
	./auto-str auto_ezmlm `head -1 conf-ezmlm` > auto_ezmlm.c

auto_home.o: compile \
auto_home.c
	./compile auto_home.c

auto_home.c: auto-str conf-home
	./auto-str auto_home `head -1 conf-home` > auto_home.c

install: load install.o hier.o auto_home.o stralloc.a alloc.a strerr.a \
buffer.a open.a error.a str.a
	./load install hier.o auto_home.o stralloc.a alloc.a strerr.a buffer.a \
	open.a error.a str.a 

install.o: compile \
install.c \
buffer.h strerr.h error.h open.h readwrite.h exit.h
	./compile install.c

hier.o: compile \
hier.c \
auto_home.h
	./compile hier.c

test: prog rts rts.tests rts.exp
	./rts | cmp - rts.exp >&2

instcheck.o: compile \
instcheck.c \
strerr.h error.h readwrite.h exit.h
	./compile instcheck.c

setup: it install
	./install

check: it instcheck
	./instcheck
