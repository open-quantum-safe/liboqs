AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libsig{{ family }}.la
noinst_LTLIBRARIES += {% for scheme in schemes %}libsig{{family}}_{{ scheme['scheme_c'] }}.la {%endfor %}

libsig{{ family }}_la_LIBADD  = {% for scheme in schemes %}libsig{{family}}_{{ scheme['scheme_c'] }}.la {%endfor %}
libsig{{ family }}_la_SOURCES = 

{% for scheme in schemes -%}
libsig{{family}}_{{ scheme['scheme_c'] }}_la_SOURCES = sig_{{family}}_{{ scheme['scheme_c'] }}.c {% for source in scheme['sources'] %}pqclean_{{family}}_{{ scheme['scheme_c'] }}_clean/{{ source }} {% endfor %}
libsig{{family}}_{{ scheme['scheme_c'] }}_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

{% endfor -%}
