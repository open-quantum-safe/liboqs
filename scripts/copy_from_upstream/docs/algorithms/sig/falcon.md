Falcon
======

- **Algorithm type**: signature
- **Main cryptographic assumption**: hardness of NTRU lattice problems
- **Scheme authors**: Thomas Prest, Pierre-Alain Fouque, Jeffrey Hoffstein, Paul Kirchner, Vadim Lyubashevsky, Thomas Pornin, Thomas Ricosset, Gregor Seiler, William Whyte, Zhenfei Zhang
- **Authors' website**: https://falcon-sign.info
- **Version**: 20201018

Implementation
--------------

- **Source of implementation**: supercop-20201018 via https://github.com/jschanck/package-pqclean/tree/cea1fa5a/falcon
- **Implementation version**: {{ schemes[0]['git_url'] }}, {{ schemes[0]['git_branch'] }}, {{ schemes[0]['git_commit'] }}
- **License**: CC0 1.0 Universal
- **Constant-time**: Yes
- **Optimizations**: Portable C {%- if all_required_flags|length > 0 %} with {% for flag in all_required_flags|sort -%}{{ flag|upper }}{%- if not loop.last %}, {% endif -%}{%- endfor %} instructions (if available at run-time){%- endif %}

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------------|----------------|-----------------------------|-------------------------|-------------------------|------------------------|
{% for scheme in schemes -%}
| {{ scheme['pretty_name_full'] }} | EUF-CMA | {{ scheme['metadata']['claimed-nist-level'] }} | {{ scheme['metadata']['length-public-key'] }} | {{ scheme['metadata']['length-secret-key'] }} | {{ scheme['metadata']['length-signature'] }} |
{% endfor -%}
