SPHINCS+
========

- **Algorithm type**: signature
- **Main cryptographic assumption**: hash-based signatures
- **Scheme authors**: Andreas Hulsing, Daniel J. Bernstein, Christoph Dobraunig, Maria Eichlseder, Scott Fluhrer, Stefan-Lukas Gazdag, Panos Kampanakis, Stefan Kolbl, Tanja Lange, Martin M Lauridsen, Florian Mendel, Ruben Niederhagen, Christian Rechberger, Joost Rijneveld, Peter Schwabe, Jean-Philippe Aumasson
- **Authors' website**: https://sphincs.org/
- **Version**: NIST Round 2 submission

Implementation
--------------

- **Source of implementation**: https://github.com/sphincs/sphincsplus
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
