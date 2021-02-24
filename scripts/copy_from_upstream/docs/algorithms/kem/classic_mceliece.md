Classic McEliece
================

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: Niederreiter's dual version of McEliece's public key encryption using binary Goppa codes
- **Scheme authors**: Daniel J. Bernstein, Tung Chou, Tanja Lange, Ingo von Maurich, Rafael Misoczki, Ruben Niederhagen, Edoardo Persichetti, Christiane Peters, Peter Schwabe, Nicolas Sendrier, Jakub Szefer, Wen Wang
- **Authors' website**: https://classic.mceliece.org
- **Version**: SUPERCOP-20191221

Implementation
--------------

- **Source of implementation**: SUPERCOP-20191221, "vec" implementation ("avx" implementation if OQS_USE_CPU_EXTENSIONS set)
- **Implementation version**: {{ schemes[0]['git_url'] }}, {{ schemes[0]['git_branch'] }}, {{ schemes[0]['git_commit'] }}
- **License**: Public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C {%- if all_required_flags|length > 0 %} with {% for flag in all_required_flags|sort -%}{{ flag|upper }}{%- if not loop.last %}, {% endif -%}{%- endfor %} instructions (if available at run-time){%- endif %}

Parameter sets
--------------

| Parameter set             | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------------------|----------------|-----------------------------|-------------------------|-------------------------|-------------------------|----------------------------|
{% for scheme in schemes -%}
| {{ scheme['pretty_name_full'] }} | {{ scheme['metadata']['claimed-security'] }} | {{ scheme['metadata']['claimed-nist-level'] }} | {{ scheme['metadata']['length-public-key'] }} | {{ scheme['metadata']['length-secret-key'] }} | {{ scheme['metadata']['length-ciphertext'] }} | {{ scheme['metadata']['length-shared-secret'] }} |
{% endfor -%}