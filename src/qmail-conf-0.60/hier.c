#include "auto_qmail.h"

void hier (void)
{
    h(auto_qmail, -1, -1, 0755);
    d(auto_qmail, "bin", -1, -1, 0755);

    c(auto_qmail, "bin", "qmail-delivery-conf", -1, -1, 0755);
    c(auto_qmail, "bin", "qmail-pop3d-conf", -1, -1, 0755);
    c(auto_qmail, "bin", "qmail-qmqpd-conf", -1, -1, 0755);
    c(auto_qmail, "bin", "qmail-qmtpd-conf", -1, -1, 0755);
    c(auto_qmail, "bin", "qmail-smtpd-conf", -1, -1, 0755);

    d(auto_qmail, "service", -1, -1, 0755);
}
