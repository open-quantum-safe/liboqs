AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libsig{{ family }}.la
noinst_LTLIBRARIES += {% for scheme in schemes %}libsig{{ scheme['pqclean_scheme'] }}.la {%endfor %}

libsig{{ family }}_la_LIBADD  = {% for scheme in schemes %}libsig{{ scheme['pqclean_scheme'] }}.la {%endfor %}
libsig{{ family }}_la_SOURCES = 

{% for scheme in schemes -%}
libsig{{ scheme['pqclean_scheme'] }}_la_SOURCES = sig_{{ scheme['pqclean_scheme'] }}.c {% for source in scheme['sources'] %}pqclean_{{ scheme['pqclean_scheme'] }}_clean/{{ source }} {% endfor %}
libsig{{ scheme['pqclean_scheme'] }}_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

{% endfor -%}
