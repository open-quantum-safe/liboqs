// SPDX-License-Identifier: MIT
// Test cases for queries/malloc-null-check.ql.
// Self-contained: liboqs is stubbed so no headers are needed.

#define NULL ((void *)0)

typedef unsigned long size_t;

void *OQS_MEM_malloc(size_t size);
void *OQS_MEM_calloc(size_t num_elements, size_t element_size);
char *OQS_MEM_strdup(const char *str);
void *OQS_MEM_aligned_alloc(size_t alignment, size_t size);
void OQS_MEM_insecure_free(void *ptr);
void use(void *ptr);
void exit(int status);

#define OQS_EXIT_IF_NULLPTR(x, loc) \
	do {                            \
		if ((x) == (void *)0) {     \
			exit(1);                \
		}                           \
	} while (0)

struct buffers {
	char *a;
	char *b[2];
};

void bad_unchecked(void) {
	char *p = OQS_MEM_malloc(16); // BAD
	use(p);
}

void bad_assigned_unchecked(void) {
	char *p;
	p = OQS_MEM_calloc(4, 4); // BAD
	use(p);
}

void bad_other_pointer_checked(void) {
	char *p = OQS_MEM_strdup("x"); // BAD
	char *q = OQS_MEM_malloc(16);
	if (q == NULL) {
		return;
	}
	use(p);
	use(q);
}

void bad_one_of_batch_unchecked(void) {
	char *a = OQS_MEM_malloc(16);
	char *b = OQS_MEM_malloc(16); // BAD
	char *c = OQS_MEM_malloc(16);
	if ((a == NULL) || (c == NULL)) {
		return;
	}
	use(b);
}

void bad_used_before_check(void) {
	char *p = OQS_MEM_malloc(16); // BAD
	use(p);
	if (p == NULL) {
		return;
	}
}

void bad_checked_on_one_path(int c) {
	char *p = OQS_MEM_malloc(16); // BAD
	if (c) {
		OQS_EXIT_IF_NULLPTR(p, "test");
	}
	use(p);
}

void bad_member_used_before_check(struct buffers *s) {
	s->a = OQS_MEM_malloc(16); // BAD
	use(s->a);
	if (s->a == NULL) {
		return;
	}
}

void good_exit_if_nullptr(void) {
	char *p = OQS_MEM_malloc(16);
	OQS_EXIT_IF_NULLPTR(p, "test");
	use(p);
}

void good_batch(void) {
	char *a = OQS_MEM_malloc(16);
	char *b = OQS_MEM_aligned_alloc(32, 64);
	if (!a || !b) {
		return;
	}
	use(a);
	use(b);
}

void good_batch_freed_on_error(void) {
	char *a = OQS_MEM_malloc(16);
	char *b = OQS_MEM_malloc(16);
	if ((a == NULL) || (b == NULL)) {
		goto err;
	}
	use(a);
	use(b);
err:
	OQS_MEM_insecure_free(a);
	OQS_MEM_insecure_free(b);
}

void good_assignment_in_condition(void) {
	char *p;
	if ((p = OQS_MEM_malloc(16)) == NULL) {
		return;
	}
	use(p);
}

void good_conditional(size_t n) {
	char *p = (n > 0) ? OQS_MEM_malloc(n) : NULL;
	if (n && p == NULL) {
		return;
	}
	use(p);
}

void good_struct_members(struct buffers *s) {
	s->a = OQS_MEM_malloc(16);
	for (int i = 0; i < 2; i++) {
		s->b[i] = OQS_MEM_malloc(16);
		if (s->b[i] == NULL) {
			return;
		}
	}
	if (s->a != NULL) {
		use(s->a);
	}
}

void good_condition(void) {
	char *p = OQS_MEM_malloc(16);
	if (p) {
		use(p);
	}
}

void *good_returned(void) {
	return OQS_MEM_malloc(16);
}

void good_ignored(void) {
	char *p = OQS_MEM_malloc(16); // IGNORE malloc-check
	OQS_MEM_insecure_free(p);
}
