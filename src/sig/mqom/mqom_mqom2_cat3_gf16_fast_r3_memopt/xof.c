#include "xof.h"

int xof_init(xof_context *ctx) {
	int ret = -1;

	if (ctx == NULL) {
		goto err;
	}
	ctx->xof_finalized = 0;
	ret = _XOF_Init(&(ctx->ctx));
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

int xof_update(xof_context *ctx, const uint8_t* data, size_t byte_len) {
	int ret = -1;

	if (ctx == NULL) {
		goto err;
	}
	ret = _XOF_Update(&(ctx->ctx), data, byte_len << 3);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

int xof_squeeze(xof_context *ctx, uint8_t *out, uint32_t byte_len) {
	int ret = -1;

	if (ctx == NULL) {
		goto err;
	}
	/* NOTE: we transparently finalize the XOF to avoid inapropriate
	 * squeezing
	 * */
	if (ctx->xof_finalized != 1) {
		ret = _XOF_Final(&(ctx->ctx), NULL);
		ERR(ret, err);
		ctx->xof_finalized = 1;
	}
	ret = _XOF_Squeeze(&(ctx->ctx), out, byte_len << 3);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

void xof_clean_ctx(xof_context *ctx) {
	_XOF_Release(&(ctx->ctx));
}

int xof_init_x4(xof_context_x4 *ctx) {
	int ret = -1;

	if (ctx == NULL) {
		goto err;
	}
	ctx->xof_finalized = 0;
	ret = _XOF_Init_x4(&(ctx->ctx));
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

int xof_update_x4(xof_context_x4 *ctx, const uint8_t *data[4], size_t byte_len) {
	int ret = -1;

	if (ctx == NULL) {
		goto err;
	}
	ret = _XOF_Update_x4(&(ctx->ctx), data, byte_len << 3);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

int xof_squeeze_x4(xof_context_x4 *ctx, uint8_t *out[4], uint32_t byte_len) {
	int ret = -1;

	if (ctx == NULL) {
		goto err;
	}
	/* NOTE: we transparently finalize the XOF to avoid inapropriate
	 * squeezing
	 * */
	if (ctx->xof_finalized != 1) {
		ret = _XOF_Final_x4(&(ctx->ctx), NULL);
		ERR(ret, err);
		ctx->xof_finalized = 1;
	}
	ret = _XOF_Squeeze_x4(&(ctx->ctx), out, byte_len << 3);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

void xof_clean_ctx_x4(xof_context_x4 *ctx) {
	_XOF_Release_x4(&(ctx->ctx));
}


