CRYSTALS-Dilithium
==================

- **Algorithm type**: signature
- **Main cryptographic assumption**: hardness of lattice problems over module lattices.
- **Scheme authors**: Vadim Lyubashevsky, Leo Ducas, Eike Kiltz, Tancrede Lepoint, Peter Schwabe, Gregor Seiler, Damien Stehle
- **Authors' website**: https://pq-crystals.org/dilithium/
- **Version**: 3.1

Implementation
--------------

- **Source of implementation**: https://github.com/pq-crystals/dilithium
- **Implementation version**: {{ schemes[0]['git_url'] }}, {{ schemes[0]['git_branch'] }}, {{ schemes[0]['git_commit'] }}
- **License**: public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C {%- if all_required_flags|length > 0 %} with {% for flag in all_required_flags|sort -%}{{ flag|upper }}{%- if not loop.last %}, {% endif -%}{%- endfor %} instructions (if available at run-time){%- endif %}
  
Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------------|----------------|-----------------------------|-------------------------|-------------------------|------------------------|
{% for scheme in schemes -%}
| {{ scheme['pretty_name_full'] }} | EUF-CMA | {{ scheme['metadata']['claimed-nist-level'] }} | {{ scheme['metadata']['length-public-key'] }} | {{ scheme['metadata']['length-secret-key'] }} | {{ scheme['metadata']['length-signature'] }} | 
{% endfor -%}
