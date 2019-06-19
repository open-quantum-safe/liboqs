AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES  = libkem{{ family }}.la
noinst_LTLIBRARIES += {% for scheme in schemes %}libkem{{ scheme['pqclean_scheme'] }}.la {%endfor %}

libkem{{ family }}_la_LIBADD  = {% for scheme in schemes %}libkem{{ scheme['pqclean_scheme'] }}.la {%endfor %}
libkem{{ family }}_la_SOURCES = 

{% for scheme in schemes -%}
libkem{{ scheme['pqclean_scheme'] }}_la_SOURCES = kem_{{ scheme['pqclean_scheme'] }}.c {% for source in scheme['sources'] %}pqclean_{{ scheme['pqclean_scheme'] }}_{{ scheme['implementation'] }}/{{ source }} {% endfor %}
libkem{{ scheme['pqclean_scheme'] }}_la_CFLAGS  = $(AM_CFLAGS) -I../../common/pqclean_shims

{% endfor -%}
