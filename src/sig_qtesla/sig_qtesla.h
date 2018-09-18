/**
 * \file sig_qtesla.h
 * \brief Header for the qTesla library
 */
#ifndef __OQS_SIG_QTESLA_H
#define __OQS_SIG_QTESLA_H

#include <oqs/sig.h>

#ifdef ENABLE_SIG_QTESLA
#include <stddef.h>
#include <stdint.h>
#include <oqs/common.h>

OQS_STATUS OQS_SIG_qTESLA_get(OQS_SIG *sig, enum OQS_SIG_algid algid);
OQS_STATUS OQS_SIG_qTESLA_keygen(const OQS_SIG *s, uint8_t *priv, uint8_t *pub);
OQS_STATUS OQS_SIG_qTESLA_sign(const OQS_SIG *s, const uint8_t *priv, const uint8_t *msg, const size_t msg_len, uint8_t *sig, size_t *sig_len);
OQS_STATUS OQS_SIG_qTESLA_verify(const OQS_SIG *s, const uint8_t *pub, const uint8_t *msg, const size_t msg_len, const uint8_t *sig, const size_t sig_len);
void OQS_SIG_qTESLA_free(OQS_SIG *s);
#endif
#endif
