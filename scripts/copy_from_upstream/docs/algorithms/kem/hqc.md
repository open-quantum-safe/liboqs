HQC
===

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: Syndrome decoding of structure codes (Hamming Quasi-Cyclic)
- **Scheme authors**: Carlos Aguilar Melchor, Nicolas Aragon, Slim Bettaieb, Loïc Bidoux, Olivier Blazy, Jurjen Bos, Jean-Christophe Deneuville, Arnaud Dion, Philippe Gaborit, Jérôme Lacan, Edoardo Persichetti, Jean-Marc Robert, Rascal Véron, Gilles Zémor
- **Authors' website**: http://pqc-hqc.org
- **Version**: 2020/10/01

Implementation
--------------

- **Source of implementation**: hqc-submission_2020-10-01 via https://github.com/jschanck/package-pqclean/tree/c9181076/hqc
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
