SABER
=====

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: module learning with rounding
- **Scheme authors**: Jan-Pieter D'Anvers, Angshuman Karmakar, Sujoy Sinha Roy, Frederic Vercauteren
- **Authors' website**: https://www.esat.kuleuven.be/cosic/pqcrypto/saber/
- **Version**: NIST Round 3 submission

Implementation
--------------

- **Source of implementation**: https://github.com/KULeuven-COSIC/SABER/tree/509cc5ec3a7e12a751ccdd2ef5bd6e54e00bd350 via https://github.com/jschanck/package-pqclean/tree/1ae84c3c/saber
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
