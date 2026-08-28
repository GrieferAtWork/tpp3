/* Copyright (c) 2017-2026 Griefer@Work                                       *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgement (see the following) in the product     *
 *    documentation is required:                                              *
 *    Portions Copyright (c) 2017-2026 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef GUARD_TPP_LEXER_PARSEEMBED_C
#define GUARD_TPP_LEXER_PARSEEMBED_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "error.h"
#include "lexer.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_PARSEEMBED

#if TPP_HAVE_FILE_ENCODING_EMBED
#ifndef TPP_IO_READFILE_BLOCKING_STACK_BUFSIZE
#define TPP_IO_READFILE_BLOCKING_STACK_BUFSIZE 1024
#endif /* !TPP_IO_READFILE_BLOCKING_STACK_BUFSIZE */

static TPP_WUNUSED TPP_NONNULL((4)) tpp_ssize TPPCALL
tpp_io_readfile_blocking_with_buffer(tpp_io_handle file, tpp_formatprinter printer, void *arg,
                                     tpp_uintmax *tpp_restrict p_read_bytes, tpp_uintmax max_bytes,
                                     tpp_char *buffer, tpp_size bufsize) {
	tpp_ssize temp, result = 0;
	tpp_uintmax read_bytes = 0;
	while (read_bytes < max_bytes) {
		tpp_ssize read_status;
		tpp_uintmax remaining = max_bytes - read_bytes;
		tpp_size block_size = bufsize;
		if ((tpp_uintmax)block_size > remaining)
			block_size = (tpp_size)remaining;
		read_status = tpp_io_read_blocking(file, buffer, block_size);
		if (TPP_SSIZE_ISERR(read_status))
			return read_status;
		if (read_status == 0)
			break; /* EOF */
		read_bytes += (tpp_size)read_status;
		temp = tpp_formatprinter_print(printer, arg, buffer, (tpp_size)read_status);
		if (temp < 0) {
			result = temp;
			break;
		}
		result += temp;
	}
	*p_read_bytes = read_bytes;
	return result;
}

#ifndef tpp_alloca
static TPP_WUNUSED TPP_NONNULL((4)) tpp_ssize TPPCALL
tpp_io_readfile_blocking_with_stack_buffer(tpp_io_handle file, tpp_formatprinter printer, void *arg,
                                           tpp_uintmax *tpp_restrict p_read_bytes, tpp_uintmax max_bytes) {
	tpp_char buffer[TPP_IO_READFILE_BLOCKING_STACK_BUFSIZE];
	return tpp_io_readfile_blocking_with_buffer(file, printer, arg, p_read_bytes,
	                                            max_bytes, buffer, sizeof(buffer));
}
#endif /* !tpp_alloca */

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((4)) tpp_ssize TPPCALL
tpp_io_readfile_blocking(tpp_io_handle file, tpp_formatprinter printer, void *arg,
                         tpp_uintmax *tpp_restrict p_read_bytes, tpp_uintmax max_bytes) {
	tpp_ssize result;
	tpp_char *buffer;
	tpp_size bufsize = 64 * 1024;
	if ((tpp_uintmax)bufsize > max_bytes)
		bufsize = (tpp_size)max_bytes;
	buffer = (tpp_char *)tpp_trymalloc(bufsize);
	if (!buffer) {
#ifdef tpp_alloca
		bufsize = TPP_IO_READFILE_BLOCKING_STACK_BUFSIZE;
		buffer = (tpp_char *)tpp_alloca(TPP_IO_READFILE_BLOCKING_STACK_BUFSIZE);
#else /* tpp_alloca */
		return tpp_io_readfile_blocking_with_stack_buffer(file, printer, arg,
		                                                  p_read_bytes, max_bytes);
#endif /* !tpp_alloca */
	}
	result = tpp_io_readfile_blocking_with_buffer(file, printer, arg, p_read_bytes,
	                                              max_bytes, buffer, bufsize);
	tpp_free(buffer);
	return result;
}
#endif /* TPP_HAVE_FILE_ENCODING_EMBED */

/* Quickly parse a `,`-separated sequence of `TPP_TOK_ISINT()`-like
 * tokens in range of `[0,0xff]` into a sequence of bytes, which are
 * then fed to `printer`.
 *
 * If enabled and used, this function has special optimizations for
 * input files pushed by `#embed` and `TPP_HAVE_FILE_ENCODING_EMBED`,
 * such that in this case, anything not already read from the file
 * will be streamed directly into `printer` (without the need of
 * `tpp_file` converting file bytes into decimals first).
 *
 * NOTES:
 * - This function must be called when the current token is an int
 *   This is asserted: `tpp_assert(TPP_TOK_ISINT(tpp_lexer_gettok(self)))`
 * - This function returns in the following situations:
 *   - After skipping whitespace and comments, the token that follows
 *     a `TPP_TOK_ISINT()`-like token isn't `TPP_TOK_COMMA`. In this
 *     case, the return value is the sum of calls to `printer`, and
 *     the currently loaded token is whatever that non-`TPP_TOK_ISINT()`
 *     turned out to be.
 *     In this case `*p_final_state = TPP_LEXER_PARSEEMBED_STATE_COMMA`
 *   - After skipping a `TPP_TOK_COMMA` (and any whitespace+comments
 *     thereafter), what follows wasn't a `TPP_TOK_ISINT()`-like token.
 *     Like with the prior case, the return value is the sum of calls
 *     to `printer`, and the currently loaded token is whatever that
 *     non-`TPP_TOK_ISINT()` turned out to be.
 *     In this case `*p_final_state = TPP_LEXER_PARSEEMBED_STATE_INTEGER`
 *   - A `TPP_TOK_ISINT()`-like token is encountered whose decoded value
 *     falls outside the range of `[0,0xff]`.
 *     Once again, the return value is the sum of calls to `printer`,
 *     and the currently loaded token is whatever that `TPP_TOK_ISINT()`
 *     token whose value (as per `tpp_lexer_decodeint()`) is bad.
 *     In this case `*p_final_state = TPP_LEXER_PARSEEMBED_STATE_INTEGER`
 *   - A call to `printer` returned a negative value, which is propagated
 *     immediately. In this case `*p_final_state` is undefined, and the
 *     currently loaded token is weakly undefined. It may point to some
 *     arbitrary, or unrelated token.
 *   - A call to `tpp_lexer_yield_blocking()` returned an error, which is
 *     propagated by being wrapped as `TPP_SSIZE_OFERR()`. In this case
 *     `*p_final_state` is undefined, and the currently loaded token is
 *     left in whatever state `tpp_lexer_yield_blocking()` left it in.
 *
 * @param: p_final_state: [0..1] Set to a description of the final parser state
 * @return: * : Success:  return value is sum of return value of `printer`
 *                        current token is whatever caused parsing to stop
 *                        parsing stop position is described by `*p_final_state`
 * @return: < 0: Failure: Either `printer` returned this value, or trying to
 *                        yield to the next token resulted in an error. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_lexer_parseembed(tpp_lexer *tpp_restrict self,
                     tpp_formatprinter printer, void *arg,
                     unsigned int *p_final_state) {
	tpp_ssize temp, result = 0;
	tpp_assert(TPP_TOK_ISINT(tpp_lexer_gettok(self)));
	for (;;) {
		tpp_char byte;
		tpp_token_id tok;
		tpp_errno error;
#if TPP_HAVE_FILE_ENCODING_EMBED
		if (tpp_file_getkind(tpp_lexer_getfile(self)) == TPP_FILE_KIND_IO &&
		    tpp_lexer_getfile(self)->tf_enc == TPP_FILE_ENCODING_EMBED) {
			/* Optimization: parse remainder of file as decimals, then
			 * proceed to directly stream data from underlying file into
			 * the supplied `printer` */
			tpp_file *const file = tpp_lexer_getfile(self);
			tpp_uintmax read_bytes;
			unsigned int inner_final_state;

			/* Parse remainder of file that has already been loaded into memory */
			tpp_file_autopopfile_pushoff(file);
			file->tf_kind = TPP_FILE_KIND_TEXT; /* Prevent more data from being loaded (and also recursion above) */
			temp = tpp_lexer_parseembed(self, printer, arg, &inner_final_state);
			file->tf_kind = TPP_FILE_KIND_IO;
			tpp_file_autopopfile_pop(file);
			if (temp < 0) {
				if (p_final_state)
					*p_final_state = TPP_LEXER_PARSEEMBED_STATE_INTEGER;
				return temp;
			}
			result += temp;

			/* Make sure that everything was parsed and we're at EOF now.
			 * Since it's an #embed-file, there really shouldn't be any
			 * way that it wouldn't be, but better be safe than sorry */
			if (inner_final_state != TPP_LEXER_PARSEEMBED_STATE_COMMA ||
			    tpp_lexer_gettok(self) != TPP_TOK_EOF) {
				if (p_final_state)
					*p_final_state = inner_final_state;
				if (tpp_lexer_gettok(self) == TPP_TOK_EOF)
					goto yield_at_eof_when_expecting_int;
				return result;
			}
			tpp_assert(file->tf_pos >= file->tf_end && "Then why are we at EOF?");

			temp = tpp_io_readfile_blocking(file->tf_data.td_io.tff_file, printer, arg, &read_bytes,
			                                file->tf_data.td_io.tff_encdat.tffed_embedlimit);
			file->tf_data.td_io.tff_encdat.tffed_embedlimit -= read_bytes;
			if (temp < 0) {
				if (p_final_state)
					*p_final_state = TPP_LEXER_PARSEEMBED_STATE_COMMA;
				return temp;
			}
			result += temp;
yield_at_eof_when_expecting_int:;
		} else
#endif /* TPP_HAVE_FILE_ENCODING_EMBED */
		{
			tpp_expr_intvalue intval_expr;
			tpp_intmax intval;
			error = tpp_lexer_decodeint(self, &intval_expr);
			if (TPP_ISERR(error)) {
handle_integer_decode_error:
				if (p_final_state)
					*p_final_state = TPP_LEXER_PARSEEMBED_STATE_INTEGER;
				return TPP_SSIZE_OFERR(error);
			}
			error = tpp_expr_intvalue_asintmax(&intval_expr, &intval);
			if (TPP_ISERR(error)) {
#if TPP_EXPR_INTVALUE_ASINTMAX_CANOVERFLOW
				if (error == TPP_ENOENT) {
					intval = -1;
				} else
#endif /* TPP_EXPR_INTVALUE_ASINTMAX_CANOVERFLOW */
				{
					goto handle_integer_decode_error;
				}
			}

			if (intval < 0 || intval > 0xff) {
				if (p_final_state)
					*p_final_state = TPP_LEXER_PARSEEMBED_STATE_INTEGER;
				break;
			}
			byte = (tpp_char)intval;
			temp = tpp_formatprinter_print(printer, arg, &byte, 1);
			if (temp < 0) {
				if (p_final_state)
					*p_final_state = TPP_LEXER_PARSEEMBED_STATE_INTEGER;
				return temp;
			}
			result += temp;
		}
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok)) {
			if (p_final_state)
				*p_final_state = TPP_LEXER_PARSEEMBED_STATE_COMMA;
			return TPP_SSIZE_OFERR(TPP_TOK_ASERR(tok));
		}
		if (tok != ',') {
			if (p_final_state)
				*p_final_state = TPP_LEXER_PARSEEMBED_STATE_COMMA;
			break;
		}
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok)) {
			if (p_final_state)
				*p_final_state = TPP_LEXER_PARSEEMBED_STATE_INTEGER;
			return TPP_SSIZE_OFERR(TPP_TOK_ASERR(tok));
		}
		if (!TPP_TOK_ISINT(tok)) {
			if (p_final_state)
				*p_final_state = TPP_LEXER_PARSEEMBED_STATE_INTEGER;
			break;
		}
	}
	return result;
}
#endif /* TPP_HAVE_LEXER_PARSEEMBED */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_PARSEEMBED_C */