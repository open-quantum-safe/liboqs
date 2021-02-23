Rainbow
=======

- **Algorithm type**: signature
- **Main cryptographic assumption**: multivariable polynomials, unbalanced oil and vinegar
- **Scheme authors**: Jintai Ding, Ming-Shing Chen, Albrecht Petzoldt, Dieter Schmidt, Bo-Yin Yang
- **Version**: NIST Round 3 submission

Implementation
--------------

- **Source of implementation**: https://github.com/fast-crypto-lab/rainbow-submission-round2/commit/173ada0e077e1b9dbd8e4a78994f87acc0c92263
- **Implementation version**: {{ schemes[0]['git_url'] }}, {{ schemes[0]['git_branch'] }}, {{ schemes[0]['git_commit'] }}
- **License**: CC0 1.0
- **Constant-time**: Yes
- **Optimizations**: Portable C {%- if all_required_flags|length > 0 %} with {% for flag in all_required_flags|sort -%}{{ flag|upper }}{%- if not loop.last %}, {% endif -%}{%- endfor %} instructions (if available at run-time){%- endif %}

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------------|----------------|-----------------------------|-------------------------|-------------------------|------------------------|
{% for scheme in schemes -%}
| {{ scheme['pretty_name_full'] }} | EUF-CMA | {{ scheme['metadata']['claimed-nist-level'] }} | {{ scheme['metadata']['length-public-key'] }} | {{ scheme['metadata']['length-secret-key'] }} | {{ scheme['metadata']['length-signature'] }} |
{% endfor -%}

Security considerations
-----------------------

In October 2020, Beullens announced [improved cryptanalysis of Rainbow](https://eprint.iacr.org/2020/1343.pdf) that somewhat reduces the security of the Round 2 and Round 3 parameters.  [As of October 28, 2020](https://groups.google.com/a/list.nist.gov/g/pqc-forum/c/70We3SNi7Ss), the scheme authors have acknowledged the attack and are preparing a response.
