#include "xof.h"
#include <string.h>

int xof_init(xof_context *ctx) {
	int ret = -1;

	if (ctx == NULL) {
		goto err;
	}
	ctx->state = XOF_STATE_UNINITIALIZED;
	ret = _XOF_Init(&(ctx->ctx));
	ERR(ret, err);
	ctx->state = XOF_STATE_ABSORBING;

	ret = 0;
err:
	return ret;
}

int xof_update(xof_context *ctx, const uint8_t* data, size_t byte_len) {
	int ret = -1;

	if ((ctx == NULL) || (ctx->state != XOF_STATE_ABSORBING) ||
	    ((data == NULL) && (byte_len != 0))) {
		goto err;
	}
	ret = _XOF_Update(&(ctx->ctx), data, byte_len);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

int xof_squeeze(xof_context *ctx, uint8_t *out, size_t byte_len) {
	int ret = -1;

	if ((ctx == NULL) ||
	    ((ctx->state != XOF_STATE_ABSORBING) && (ctx->state != XOF_STATE_SQUEEZING)) ||
	    ((out == NULL) && (byte_len != 0))) {
		goto err;
	}
	/* NOTE: we transparently finalize the XOF to avoid inapropriate
	 * squeezing
	 * */
	if (ctx->state == XOF_STATE_ABSORBING) {
		ret = _XOF_Final(&(ctx->ctx), NULL);
		ERR(ret, err);
		ctx->state = XOF_STATE_SQUEEZING;
	}
	ret = _XOF_Squeeze(&(ctx->ctx), out, byte_len);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

void xof_clean_ctx(xof_context *ctx) {
	if (ctx == NULL) {
		return;
	}
	if ((ctx->state == XOF_STATE_ABSORBING) || (ctx->state == XOF_STATE_SQUEEZING)) {
		_XOF_Release(&(ctx->ctx));
	}
	mqom_cleanse(ctx, sizeof(*ctx));
}

int xof_init_x4(xof_context_x4 *ctx) {
	int ret = -1;

	if (ctx == NULL) {
		goto err;
	}
	ctx->state = XOF_STATE_UNINITIALIZED;
	ret = _XOF_Init_x4(&(ctx->ctx));
	ERR(ret, err);
	ctx->state = XOF_STATE_ABSORBING;

	ret = 0;
err:
	return ret;
}

int xof_update_x4(xof_context_x4 *ctx, const uint8_t *data[4], size_t byte_len) {
	int ret = -1;
	unsigned int i;

	if ((ctx == NULL) || (ctx->state != XOF_STATE_ABSORBING) ||
	    (data == NULL)) {
		goto err;
	}
	for (i = 0; i < 4; i++) {
		if ((data[i] == NULL) && (byte_len != 0)) {
			goto err;
		}
	}
	ret = _XOF_Update_x4(&(ctx->ctx), data, byte_len);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

int xof_squeeze_x4(xof_context_x4 *ctx, uint8_t *out[4], size_t byte_len) {
	int ret = -1;
	unsigned int i;

	if ((ctx == NULL) ||
	    ((ctx->state != XOF_STATE_ABSORBING) && (ctx->state != XOF_STATE_SQUEEZING)) ||
	    (out == NULL)) {
		goto err;
	}
	for (i = 0; i < 4; i++) {
		if ((out[i] == NULL) && (byte_len != 0)) {
			goto err;
		}
	}
	/* NOTE: we transparently finalize the XOF to avoid inapropriate
	 * squeezing
	 * */
	if (ctx->state == XOF_STATE_ABSORBING) {
		ret = _XOF_Final_x4(&(ctx->ctx), NULL);
		ERR(ret, err);
		ctx->state = XOF_STATE_SQUEEZING;
	}
	ret = _XOF_Squeeze_x4(&(ctx->ctx), out, byte_len);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

void xof_clean_ctx_x4(xof_context_x4 *ctx) {
	if (ctx == NULL) {
		return;
	}
	if ((ctx->state == XOF_STATE_ABSORBING) || (ctx->state == XOF_STATE_SQUEEZING)) {
		_XOF_Release_x4(&(ctx->ctx));
	}
	mqom_cleanse(ctx, sizeof(*ctx));
}

int xof_init_x8(xof_context_x8 *ctx) {
	int ret = -1;

	if (ctx == NULL) {
		goto err;
	}
	ctx->state = XOF_STATE_UNINITIALIZED;
	ret = _XOF_Init_x8(&(ctx->ctx));
	ERR(ret, err);
	ctx->state = XOF_STATE_ABSORBING;

	ret = 0;
err:
	return ret;
}

int xof_update_x8(xof_context_x8 *ctx, const uint8_t *data[8], size_t byte_len) {
	int ret = -1;
	unsigned int i;

	if ((ctx == NULL) || (ctx->state != XOF_STATE_ABSORBING) ||
	    (data == NULL)) {
		goto err;
	}
	for (i = 0; i < 8; i++) {
		if ((data[i] == NULL) && (byte_len != 0)) {
			goto err;
		}
	}
	ret = _XOF_Update_x8(&(ctx->ctx), data, byte_len);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

int xof_squeeze_x8(xof_context_x8 *ctx, uint8_t *out[8], size_t byte_len) {
	int ret = -1;
	unsigned int i;

	if ((ctx == NULL) ||
	    ((ctx->state != XOF_STATE_ABSORBING) && (ctx->state != XOF_STATE_SQUEEZING)) ||
	    (out == NULL)) {
		goto err;
	}
	for (i = 0; i < 8; i++) {
		if ((out[i] == NULL) && (byte_len != 0)) {
			goto err;
		}
	}
	if (ctx->state == XOF_STATE_ABSORBING) {
		ret = _XOF_Final_x8(&(ctx->ctx), NULL);
		ERR(ret, err);
		ctx->state = XOF_STATE_SQUEEZING;
	}
	ret = _XOF_Squeeze_x8(&(ctx->ctx), out, byte_len);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

void xof_clean_ctx_x8(xof_context_x8 *ctx) {
	if (ctx == NULL) {
		return;
	}
	if ((ctx->state == XOF_STATE_ABSORBING) || (ctx->state == XOF_STATE_SQUEEZING)) {
		_XOF_Release_x8(&(ctx->ctx));
	}
	mqom_cleanse(ctx, sizeof(*ctx));
}
