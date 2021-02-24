CRYSTALS-Kyber
==============

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: module learning with errors (MLWE)
- **Scheme authors**: Roberto Avanzi, Joppe Bos, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Vadim Lyubashevsky, John M. Schanck, Peter Schwabe, Gregor Seiler, Damien Stehle
- **Authors' website**: https://pq-crystals.org/kyber
- **Version**: NIST Round 3 submission

Implementation
--------------

- **Source of implementation**: https://github.com/pq-crystals/kyber
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
